#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h> 

#include "bitmap.h"
#include "manipulations.h"
#include "auxiliary_functions.h"

#define INFILE_INDEX 0
#define OUTFILE_INDEX 1
#define XMIN_INDEX 2
#define YMIN_INDEX 3
#define XMAX_INDEX 4
#define YMAX_INDEX 5

int main(int argc, char *argv[]){

    /* Parse commandline arguments */

    int opt; /* For getopt */

    /* Used to locate the position of the annotation in the commandline arguments,
        incremented whenever a flag or a flag argument is found */
    int flagAndArgCount = false; 

    int invertFlag = false, greyscaleFlag = false, blurFlag = false;
    int hueFlag = false, saturationFlag = false, brightnessFlag = false, annotationFlag = false;
    
    char *blurRadius, *hueDegrees, *saturationFactor, *brightnessFactor, *annotation;

    /* Initialise flags and their associated arguments */
    while((opt = getopt(argc, argv, ":h::i::g::b:H:s:v:a")) != -1){  
        switch(opt){ 
            case 'h':
                printHelp();
                exit(EXIT_SUCCESS);
                break;
            case 'i': 
                invertFlag = true;
                flagAndArgCount++;
                break; 
            case 'g': 
                greyscaleFlag = true;
                flagAndArgCount++;
                break; 
            case 'b':
                blurFlag = true;
                flagAndArgCount++;
                blurRadius = optarg;
                flagAndArgCount++;
                break;
            case 'H':
                hueFlag = true;
                flagAndArgCount++;
                hueDegrees = optarg;
                flagAndArgCount++;
                break;
            case 's':
                saturationFlag = true;
                flagAndArgCount++;
                saturationFactor = optarg;
                flagAndArgCount++;
                break;
            case 'v':  
                brightnessFlag = true;
                flagAndArgCount++;
                brightnessFactor = optarg;
                flagAndArgCount++;
                break;
            case 'a':  
                annotationFlag = true;

                /* Offset the extra arguments parameter by one,
                    so we skip the annotation when we are going through the extra arguments */
                optind++; 

                flagAndArgCount++;
                flagAndArgCount++;
                annotation = argv[flagAndArgCount];
                break;  
            case ':':  
                break;  
            case '?':  
                printf("Invalid flag: -%c.\n", optopt); 
                exit(EXIT_FAILURE);
                break;  
        }  
    }

    char *infilename = NULL, *outfilename = NULL;
    int xmin, ymin, xmax, ymax;
    Bmp img;

    /* Going through the extra arguments (input filenmae, output filename, boundaries) */
    for(int j = 0; optind < argc; optind++, j++){

        /* Initialise input bitmap image and set default manipulation boundaries */
        if(j == INFILE_INDEX){
            infilename = argv[optind];
            img = read_bmp(infilename);

            /* Default boundaries */
            xmin = 0;
            ymin = 0;
            xmax = img.width;
            ymax = img.height;
        } 
        /* Initialise the output filename if it is given */
        if(j == OUTFILE_INDEX){
            outfilename = argv[optind]; 
        } 
        /* Initialise the custom boundaries if they are given */
        if(j == XMIN_INDEX){
            xmin = atoi(argv[optind]);
        }
        if(j == YMIN_INDEX){
            ymin = atoi(argv[optind]);
        }
        if(j == XMAX_INDEX){
            xmax = atoi(argv[optind]);
        }
        if(j == YMAX_INDEX){
            ymax = atoi(argv[optind]);
        }
    }


    /* Manipulate bitmap image and output result */

    /* Check if no flags are given */
    if(!flagAndArgCount){
        
        /* When the only argument is a filename,
            output the width and height of the image */
        if(outfilename == NULL){
            printf("Width: %u\nHeight: %u\n", img.width, img.height);
        }
        /* When the only arguments are input and output filenames,
            the output image is identical to the input image */
        else{
            write_bmp(img, outfilename);
        }

        free_bmp(img);
        exit(EXIT_SUCCESS);    
    }

    /* Perform the required manipulations on the input bitmap image,
        in the order shown in the help prompt */
    if(invertFlag){
        invertImage(&img, ymin, ymax, xmin, xmax);
    }
    if(greyscaleFlag){
        greyscaleImage(&img, ymin, ymax, xmin, xmax);
    }
    if(blurFlag){
        blurImage(blurRadius, &img, ymin, ymax, xmin, xmax);
    }
    if(hueFlag || saturationFlag || brightnessFlag){
        HSVAdjust(hueFlag, saturationFlag, brightnessFlag, \
            hueDegrees, saturationFactor, brightnessFactor, &img, ymin, ymax, xmin, xmax);
    }
    if(annotationFlag){
        annotateImage(&img, annotation);
    }

    /* Write the manipulated bitmap image to the output file */
    write_bmp(img, outfilename);
    free_bmp(img);

    exit(EXIT_SUCCESS);
}


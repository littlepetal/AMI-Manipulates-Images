#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "bitmap.h"
#include "manipulations.h"
#include "auxiliary_functions.h"

#define NUM_OF_CHAR 27
#define SPACE_INDEX 26
#define MAX_PIXEL_RGB 255
#define MAX_DEGREES 360
#define CONVERT_DEGREES 60

/* Inverts the bitmap image at the pointer (within the desired boundaries) */
void invertImage(Bmp* img, int ymin, int ymax, int xmin, int xmax){
    for(int i = ymin; i < ymax; i++){
        for(int j = xmin; j < xmax; j++){

            img->pixels[i][j][RED] = abs(img->pixels[i][j][RED] - MAX_PIXEL_RGB); 
            img->pixels[i][j][GREEN] = abs(img->pixels[i][j][GREEN] - MAX_PIXEL_RGB);
            img->pixels[i][j][BLUE] = abs(img->pixels[i][j][BLUE] - MAX_PIXEL_RGB);
        }
    }
}

/* Greyscales the bitmap image at the pointer (within the desired boundaries) */
void greyscaleImage(Bmp *img, int ymin, int ymax, int xmin, int xmax){
    for(int i = ymin; i < ymax; i++){
        for(int j = xmin; j < xmax; j++){

            int average = (img->pixels[i][j][RED] + img->pixels[i][j][GREEN] + img->pixels[i][j][BLUE])/3;
            
            img->pixels[i][j][RED] = average; 
            img->pixels[i][j][GREEN] = average;
            img->pixels[i][j][BLUE] = average;
        }
    }
}

/* Blurs the bitmap image at the pointer (within the desired boundaries) */
void blurImage(char *blurArg, Bmp *img, int ymin, int ymax, int xmin, int xmax){
    int blurRadius = atoi(blurArg);
    Bmp imgCopy = copy_bmp(*img);

    /* Loops through all pixels within the boundaries */
    for(int i = ymin; i < ymax; i++){
        for(int j = xmin; j < xmax; j++){

            int redTotal = 0, greenTotal = 0, blueTotal = 0, pixelCount = 0;
            
            /* Loop through all pixels that are within the given radius of the current pixel */
            for(int k = (i - blurRadius); k <= (i + blurRadius); k++){
                for(int l = (j - blurRadius); l <= (j + blurRadius); l++){

                    /* Ignore the pixel if it is outside of the boundaries */
                    if(k < ymin || l < xmin || k >= ymax || l >= xmax){
                        continue;
                    }

                    redTotal += imgCopy.pixels[k][l][RED];
                    greenTotal += imgCopy.pixels[k][l][GREEN];
                    blueTotal += imgCopy.pixels[k][l][BLUE];

                    pixelCount++;
                }    
            }

            img->pixels[i][j][RED] = redTotal/pixelCount; 
            img->pixels[i][j][GREEN] = greenTotal/pixelCount;
            img->pixels[i][j][BLUE] = blueTotal/pixelCount;
        }
    }

    free_bmp(imgCopy);
}

/* Changes the RGB pixel values of the bitmap image at the pointer 
    according to the required HSV adjustments (within the desired boundaries) */
void HSVAdjust(int hueFlag, int saturationFlag, int brightnessFlag, \
    char *hueArg, char *saturationArg, char *brightnessArg, Bmp *img, int ymin, int ymax, int xmin, int xmax){
    
    /* Default changes in HSV values set to zero */
    double hueDegrees = 0;
    double satFactor = 0;
    double brightFactor = 0;

    /* Initialse the HSV adjustment values that are given */
    if(hueFlag){
        hueDegrees = strtod(hueArg, NULL);
    }
    if(saturationFlag){
        satFactor = strtod(saturationArg, NULL);
    }
    if(brightnessFlag){
        brightFactor = strtod(brightnessArg, NULL);
    }

    Bmp imgCopy = copy_bmp(*img);

    for(int i = ymin; i < ymax; i++){
        for(int j = xmin; j < xmax; j++){

            Bmp *imgPointer = &imgCopy;

            double max = maxRGB(imgPointer, i, j);
            double min = minRGB(imgPointer, i, j);

            /* Map RGB values to between 0 and 1 inclusive */
            double R = (double)imgCopy.pixels[i][j][RED]/MAX_PIXEL_RGB;
            double G = (double)imgCopy.pixels[i][j][GREEN]/MAX_PIXEL_RGB;
            double B = (double)imgCopy.pixels[i][j][BLUE]/MAX_PIXEL_RGB;

            double chroma = 0, value = 0, saturation = 0;
            double piecewiseHue = 0, hue = 0;

            int invalid = false;

            /* Find the original chroma, brightness, saturation and hue of the current pixel */
            chroma = max - min;
            value = max;
            if(value == 0){
                saturation = 0;
            }
            else{
                saturation = chroma/value;
            }

            if(chroma == 0){
                invalid = true;
            }
            else{
                piecewiseHue = getPiecewiseHue(max, R, G, B, chroma);
            }

            hue = fmod(CONVERT_DEGREES*piecewiseHue, MAX_DEGREES);
            if(hue < 0){
                hue = MAX_DEGREES + hue;
            }

            /* Apply adjustments for hue, saturation and brightness */
            hue = newHue(hue, hueDegrees, invalid);
            saturation = newSaturation(saturation, satFactor);
            value = newBrightnessValue(value, brightFactor);

            /* Convert the new HSV values to RGB values */

            chroma = value*saturation;
            piecewiseHue = hue/CONVERT_DEGREES;

            double intermediateNum = chroma*(1 - fabs((fmod(piecewiseHue, 2)) - 1));
            double tempR = 0, tempG = 0, tempB = 0;

            /* Calculate R1, G1, B1 as described in the given equations */
            if(invalid){
                tempR = 0; 
                tempG = 0;
                tempB = 0;
            }
            else if(piecewiseHue >= 0 && piecewiseHue <= 1){
                tempR = chroma; 
                tempG = intermediateNum;
                tempB = 0;
            }
            else if(piecewiseHue > 1 && piecewiseHue <= 2){
                tempR = intermediateNum; 
                tempG = chroma;
                tempB = 0;
            }
            else if(piecewiseHue > 2 && piecewiseHue <= 3){
                tempR = 0; 
                tempG = chroma;
                tempB = intermediateNum;
            }
            else if(piecewiseHue > 3 && piecewiseHue <= 4){
                tempR = 0; 
                tempG = intermediateNum;
                tempB = chroma;
            }
            else if(piecewiseHue > 4 && piecewiseHue <= 5){
                tempR = intermediateNum; 
                tempG = 0;
                tempB = chroma;
            }
            else if(piecewiseHue > 5 && piecewiseHue <= 6){
                tempR = chroma; 
                tempG = 0;
                tempB = intermediateNum;
            }

            min = value - chroma;

            img->pixels[i][j][RED] = (int)((tempR + min)*MAX_PIXEL_RGB); 
            img->pixels[i][j][GREEN] = (int)((tempG + min)*MAX_PIXEL_RGB);
            img->pixels[i][j][BLUE] = (int)((tempB + min)*MAX_PIXEL_RGB);
        }
    }
    
    free_bmp(imgCopy);
}

/* Annotes the bitmap image at the pointer */
void annotateImage(Bmp *img, char *annotation){
    Bmp alphabet = read_bmp("alphabet.bmp");

    /* Initialise the width of each character in alphabet.bmp */
    int charWidth = alphabet.width/NUM_OF_CHAR;

    /* Loop through each letter in the annotation string 
        and find its equivalent in the alphabet image */
    for(int n = 0; n < strlen(annotation); n++){

        /* Map the ASCII code of each letter to a number between 0 and 25 inclusive,
            we will handle the special case of the whitespace character later */
        int key = abs('a' - (int)annotation[n]);

        int charIndex = 0;

        /* Initialise the horizontal component of the starting pixel of the character 
            in the output image */
        int ouputCharStartX = n*charWidth;

        /* If the character is a whitespcae, we will match it to the last character
            in the alphabet image */
        if(annotation[n] == ' '){
            charIndex = SPACE_INDEX;
        }
        else{
            charIndex = key;
        }

        for(int i = 0; i < alphabet.height; i++){
            for(int j = 0; j < charWidth; j++){

                img->pixels[i][ouputCharStartX + j][RED] \
                    = alphabet.pixels[i][charIndex*charWidth + j][RED];
                img->pixels[i][ouputCharStartX + j][GREEN] \
                    = alphabet.pixels[i][charIndex*charWidth + j][GREEN];
                img->pixels[i][ouputCharStartX + j][BLUE] \
                    = alphabet.pixels[i][charIndex*charWidth + j][BLUE];
            }
        }
    }

    free_bmp(alphabet);  
} 

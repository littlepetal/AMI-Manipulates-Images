#ifndef _MANIPULATIONS_H
#define _MANIPULATIONS_H

/* Inverts the bitmap image at the pointer (within the desired boundaries) */
void invertImage(Bmp* img, int ymin, int ymax, int xmin, int xmax);

/* Greyscales the bitmap image at the pointer (within the desired boundaries) */
void greyscaleImage(Bmp *img, int ymin, int ymax, int xmin, int xmax);

/* Blurs the bitmap image at the pointer (within the desired boundaries) */
void blurImage(char *blurArg, Bmp *img, int ymin, int ymax, int xmin, int xmax);

/* Changes the RGB pixel values of the bitmap image at the pointer 
    according to the required HSV adjustments (within the desired boundaries) */
void HSVAdjust(int hueFlag, int saturationFlag, int brightnessFlag, \
    char *hueArg, char *saturationArg, char *brightnessArg, Bmp *img, int ymin, int ymax, int xmin, int xmax);

/* Annotes the bitmap image at the pointer */
void annotateImage(Bmp *img, char *annotation);

#endif

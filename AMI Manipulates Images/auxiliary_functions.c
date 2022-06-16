#include <stdio.h>
#include <math.h>

#include "bitmap.h"
#include "auxiliary_functions.h"

#define MAX_PIXEL_RGB 255
#define MAX_DEGREES 360
#define CONVERT_DEGREES 60
#define ONE_HUNDRED_PERCENT 1
#define MIN_SATURATION 0
#define MAX_SATURATION 1
#define MIN_BRIGHTNESS_VALUE 0
#define MAX_BRIGHTNESS_VALUE 1

/* Outputs the help prompt */
void printHelp(void){
    printf("Usage: ./ami [options ...] <input-file> <output-file> [<xmin> <ymin> <xmax> <ymax>]\n\n"
            "  -i       Invert the image\n"
            "  -g       Apply a grayscale filter to the image\n"
            "  -b <r>   Apply a box blur on the image with radius <r>\n"
            "  -H <d>   Rotate the hue of the image by <d> degrees\n"
            "  -s <p>   Modify the saturation of the image by a factor of <p>\n"
            "  -v <b>   Modify the brightness of the image by a factor of <b>\n"
            "  -a \"<s>\" Annotate the image with a 6 letter string <s>\n");
}

/* Returns the maximum of (R, G, B) where R, G and B are defined between 0 and 1 inclusive */
double maxRGB(Bmp *imgPointer, int i, int j){
    double R = (double)imgPointer->pixels[i][j][RED]/MAX_PIXEL_RGB;
    double G = (double)imgPointer->pixels[i][j][GREEN]/MAX_PIXEL_RGB;
    double B = (double)imgPointer->pixels[i][j][BLUE]/MAX_PIXEL_RGB;

    if(R >= G && R >= B){
        return R;
    }
    else if(G >= R && G >= B){
        return G;
    }
    else{
        return B;
    }
}

/* Returns the minimum of (R, G, B) where R, G and B are defined between 0 and 1 inclusive */
double minRGB(Bmp *imgPointer, int i, int j){
    double R = (double)imgPointer->pixels[i][j][RED]/MAX_PIXEL_RGB;
    double G = (double)imgPointer->pixels[i][j][GREEN]/MAX_PIXEL_RGB;
    double B = (double)imgPointer->pixels[i][j][BLUE]/MAX_PIXEL_RGB;

    if(R <= G && R <= B){
        return R;
    }
    else if(G <= R && G <= B){
        return G;
    }
    else{
        return B;
    }
}

/* Return the piecewise hue value according to the given piecewise equations */
double getPiecewiseHue(double max, double R, double G, double B, double chroma){
    double piecewiseHue;
    if(max == R){
        piecewiseHue = fmod((G - B)/chroma, 6);
    }
    else if(max == G){
        piecewiseHue = ((((B - R)/chroma)) + 2);
    }
    else if(max == B){
        piecewiseHue = ((((R - G)/chroma)) + 4);
    }

    return piecewiseHue;
}

/* Return the adjusted hue which is defined between 0 and 360 inclusive */
double newHue(double hue, double hueDegrees, int invalid){
    double newHue = hue;

    if(!invalid){
        newHue = fmod(hue + hueDegrees, MAX_DEGREES);
    }

    return newHue;
}

/* Return the adjusted saturation which is defined between 0 and 1 inclusive */
double newSaturation(double saturation, double satFactor){
    saturation *= (ONE_HUNDRED_PERCENT + satFactor);

    if(saturation < MIN_SATURATION){
        saturation = MIN_SATURATION;
    }
    if(saturation > MAX_SATURATION){
        saturation = MAX_SATURATION;
    }

    return saturation;
}

/* Return the adjusted brightness value which is defined between 0 and 1 inclusive */
double newBrightnessValue(double value, double brightFactor){
    value *= (ONE_HUNDRED_PERCENT + brightFactor);

    if(value < MIN_BRIGHTNESS_VALUE){
        value = MIN_BRIGHTNESS_VALUE;
    }
    if(value > MAX_BRIGHTNESS_VALUE){
        value = MAX_BRIGHTNESS_VALUE;
    }

    return value;
}

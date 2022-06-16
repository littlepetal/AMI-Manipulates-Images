#ifndef _AUXILIARY_FUNCTIONS_H
#define _AUXILIARY_FUNCTIONS_H

/* Outputs the help prompt */
void printHelp(void);

/* Returns the maximum of (R, G, B) where R, G and B are defined between 0 and 1 inclusive */
double maxRGB(Bmp *imgPointer, int i, int j);

/* Returns the minimum of (R, G, B) where R, G and B are defined between 0 and 1 inclusive */
double minRGB(Bmp *imgPointer, int i, int j);

/* Return the piecewise hue value according to the given piecewise equations */
double getPiecewiseHue(double max, double R, double G, double B, double chroma);

/* Return the adjusted hue which is defined between 0 and 360 inclusive */
double newHue(double hue, double hueDegrees, int invalid);

/* Return the adjusted saturation which is defined between 0 and 1 inclusive */
double newSaturation(double saturation, double satFactor);

/* Return the adjusted brightness value which is defined between 0 and 1 inclusive */
double newBrightnessValue(double value, double brightFactor);

#endif

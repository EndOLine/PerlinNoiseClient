#pragma once
/*
Program: clsCLR
Description: define colours and structures. Alpha channel defaults to 255.
Configuration:

Log:
	20211011: Created for RGBa 
	20221201: included operator !=

*/
#ifndef CLSCLR_H
#define CLSCLR_H

#define CLR_BLACK  0x000000
#define CLR_BLUE   0x0000FF
#define CLR_BRASS  0xb5a642
#define CLR_BRONZE 0xb08d57
#define CLR_BROWN  0xA52A2A
#define CLR_CYAN   0x00FFFF
#define CLR_GOLD   0xFFD700
#define CLR_GRAY   0x808080
#define CLR_GREEN  0x008000
#define CLR_LIME   0x00FF00
#define CLR_MAGENTA 0xFF00FF
#define CLR_MAROON 0x800000
#define CLR_NAVY   0x000080
#define CLR_OLIVE  0x808000
#define CLR_ORANGE 0xFF2200
#define CLR_PURPLE 0x800080
#define CLR_RED    0xFF0000
#define CLR_SADDLEBROWN 0x8B4513
#define CLR_SILVER 0xC0C0C0
#define CLR_TEAL   0x008080
#define CLR_WHITE  0xFFFFFF
#define CLR_YELLOW 0xFFFF00



class clsRGBa{
public:
	unsigned char r, g, b, alpha;
public:
	clsRGBa() {	r = 0; g = 0; b = 0; alpha = 255; };
	clsRGBa(const unsigned char red, const unsigned char green, const unsigned char blue) { r = red; g = green; b = blue; alpha = 255;};
	clsRGBa(const int red, const int green, const int blue) { r = red; g = green; b = blue; alpha = 255; };
	clsRGBa(const unsigned int Colour) { r = (0xFF0000 & Colour)>>16; g = (0xFF00 & Colour) >> 8; b = (0xFF & Colour); alpha = 255; };
	bool operator==(const clsRGBa& rhs) const;
	bool operator!=(const clsRGBa& rhs) const;
};

#endif
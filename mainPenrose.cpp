#define _USE_MATH_DEFINES
#include "pixel.h"
#include <iostream>
#include <fstream>
#include <math.h>

int
main (int argc, char * argv[]) {
  const scale = 255;

  ofstream infile;
  int width, height, scale;
  height = 1000;
  width = height * (3.0 / 2);

  // Creates new image file with specified width and height
  // Magic number (P3) so image file is '.ppm'
  infile.open("penrose.ppm");
  infile << "P3" << endl;
  infile << width << " " << height << endl;
  infile << scale << endl << endl;

  // Initialize constants for use with base-region (six triangles)
  const double shortLeg = height * (1.0 / 4)*(sqrt(5) - 1);
  const double longLeg = height * sqrt((0.625) + sqrt(5)/8);
  const int matH = height + 2*ceil(shortLeg);
  const int matW = 2*ceil(longLeg);
  
  // Initialize Pixel matrix & assign intial values based on...
  // the pixel's encompassing triangle on the base-region of six triangles
  Pixel  pixelMap[matH][matW];
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      // IF statement deals with left side of Block
      if (i < longLeg) {
	// Eliminating "out of bounds" region above and below
	if ( j > ((0 - shortLeg)/longLeg) * i + shortLeg ) {
	  if ( j < (shortLeg/longLeg) * i + shortLeg + height) {
	    // Setting up bottom-left triangle
	    if ( j <= (height + shortLeg)/(longLeg) * (longLeg - i) ) {
	      pixelMap[i][j] = new Pixel(i, j, [0, 1, 0]);
	    } // Setting up middle triangle 
	    else if ( j <= (shortLeg/longLeg) * (longLeg - i) + height) {
	      pixelMap[i][j] = new Pixel(i, j, [0, 0, 0]);
	    } // Setting up top triangle
	    else {
	      pixelMap[i][j] = new Pixel(i, j, [1, 0, 0]);
	    }
	  }
	}
      } // ELSE statement deals with right side
      else {
	// Eliminating "out of bounds" region above and below
	if ( j > (shortLeg/longLeg) * (i - longLeg) ) {
	  if ( j < ((0 - shortLeg)/longLeg) * (i - longLeg) + matH ) {
	    // Setting up bottom-right triangle
	    if ( j <= ((shortLeg + height) / longLeg) * (i - longLeg)) {
	      pixelMap[i][j] = new Pixel(i, j, [0, 0, 2]);
	    } // Setting up middle triangle
	    else if ( j <= (shortLeg/longLeg) * (i - longLeg) + height) {
	      pixelMap[i][j] = new Pixel(i, j, [0, 1, 2]);
	    } // Setting up top triangle
	    else {
	      pixelMap[i][j] = new Pixel(i, j, [1, 1, 0]);
	    }
	  }
	}
      }
    }
  }

  // Iterating through the Penrose tiling algorithm...
  for (int i = 0; i < matW; i++) {
    for (int j = 0; j < matH; j++) {
      // IF statement dealing with the 108-36-36 triangles
      if ( pixelMap[i][j].getType() == 0 ) {
	// IF statement dealing with the top-rotation triangle
	// ELSE IF statement dealing with the bottom-rotation triangle
	// See Ianiv Schweber's table for specification
	if ( pixelMap[i][j].getRot() == 0 ) {

	} else if ( pixelMap[i][j].getRot() == 1 ) {

	}
      } // ELSE IF statement dealing with the 54-72-72 triangles
      else if ( pixelMap[i][j].getType() == 1 ) {
	// IF statement dealing with the top-rotation triangle
	// ELSE IF statment dealing with the bottom-rotation triangle
	// See Ianiv Schweber's table for specification
	if ( pixelMap[i][j].getRot() == 0 ) {

	} else if ( pixelMap[i][j].getRot() == 1 ) {

	}
      }
    }
  }
}

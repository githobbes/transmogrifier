#include "pixel.cpp"
#include <iostream>
#include <fstream>

using namespace std;

int
main (int argc, char * argv[]) {
  const int scale = 255;

  ofstream infile;
  int width, height;
  height = 100;

  // Initialize constants for use with base-region (six triangles)
  const float shortLeg = 30.9; // height * (1.0 / 4)*(sqrt(5) - 1);
  const float longLeg = 95.11; // height * sqrt((0.625) + sqrt(5)/8);
  const int matH = 100 + 2*31; // height + 2*ceil(shortLeg);
  const int matW = 2 * 96; //2*ceil(longLeg);
  width = 2 * longLeg;
  // Initialize Pixel matrix & assign intial values based on...
  // the pixel's encompassing triangle on the base-region of six triangles

  //  Pixel **pixelMap;
  //  pixelMap = new Pixel*[matW];
  //  for(int i = 0; i < matW; ++i)
  //    pixelMap[i] = new Pixel[matH];

  Pixel pixelMap[matW][matH];

  int botLeft[3] = {0, 1, 0};
  int midLeft[3] = {0, 0, 0};
  int topLeft[3] = {1, 0, 0};
  int botRight[3] = {0, 0, 2};
  int midRight[3] = {0, 1, 2};
  int topRight[3] = {1, 1, 0};
  int black[3] = {-1, -1, -1};

  for (int j = 0; j < matH; j++) {
    for (int i = 0; i < matW; i++) {
      pixelMap[i][j].setX(i);
      pixelMap[i][j].setY(j);
      if (i < longLeg) {
	// Eliminating "out of bounds" region above and below
	if ( j > ((shortLeg)/longLeg) * (0 - i) + shortLeg && j < (shortLeg/longLeg) * i + shortLeg + height ) {
	  if ( j <= (shortLeg/longLeg) * (i) + shortLeg) {
	    pixelMap[i][j].setTypes(topLeft);
	    pixelMap[i][j].setVertices(0, shortLeg, longLeg, 0, longLeg, 2*shortLeg);
	  }
	  else if ( j <= ((shortLeg + height)/longLeg) * (i) + shortLeg) {
	    pixelMap[i][j].setTypes(midLeft);
	    pixelMap[i][j].setVertices(longLeg, matH, longLeg, 2*shortLeg, 0, shortLeg);
	  } // Setting up top triangle
	  else {
	    pixelMap[i][j].setTypes(botLeft);
	    pixelMap[i][j].setVertices(0, shortLeg + height, longLeg, matH, 0, shortLeg);
	  }
	} else { 
	  pixelMap[i][j].setTypes(black);
	}
      } // ELSE statement deals with right side
      else {
	// Eliminating "out of bounds" region above and below
	if ( j > (shortLeg/longLeg) * (i - longLeg) && j < ((0 - shortLeg)/longLeg) * (i - longLeg) + matH ) {
	  if ( j <= (shortLeg/longLeg) * (longLeg - i) + 2*shortLeg) {
	    pixelMap[i][j].setTypes(topRight);
	    pixelMap[i][j].setVertices(2*longLeg, shortLeg, longLeg, 2*shortLeg, longLeg, 0);
	  }
	  else if ( j <= ((shortLeg + height)/longLeg) * (longLeg - i) + matH) {
	    pixelMap[i][j].setTypes(midRight);
	    pixelMap[i][j].setVertices(longLeg, 2*shortLeg, longLeg, matH, 2*longLeg, shortLeg);
	  } // Setting up top triangle
	  else {
	    pixelMap[i][j].setTypes(botRight);
	    pixelMap[i][j].setVertices(longLeg, matH, 2*longLeg, shortLeg + height, 2*longLeg, shortLeg);
	  }
	} else { 
	  pixelMap[i][j].setTypes(black);
	}
      }
    }
  }
  
  // Iterating through the Penrose tiling algorithm...
  for(int k = 0; k < 0; k++) {
    for (int j = 0; j < matH; j++) {
      for (int i = 0; i < matW; i++) {
	if ((j > ((shortLeg)/longLeg) * (0 - i) + shortLeg && 
	     j < (shortLeg/longLeg) * i + shortLeg + height ) && 
	    (j > (shortLeg/longLeg) * (i - longLeg) && 
	     j < ((0 - shortLeg)/longLeg) * (i - longLeg) + matH)) {
	  // IF statement dealing with the 108-36-36 triangles
	  if ( pixelMap[i][j].getType() == 0 ) {
	    // IF statement dealing with the top-rotation triangle
	    // ELSE IF statement dealing with the bottom-rotation triangle
	    // See Ianiv Schweber's table for specification
	    if ( pixelMap[i][j].getRot() == 0 ) {
	      pixelMap[i][j].changeLt();
	    } 
	    else if ( pixelMap[i][j].getRot() == 1 ) {
	      pixelMap[i][j].changeLT();
	    }
	    else
	      cout << "Error1" << endl;
	  } // ELSE IF statement dealing with the 54-72-72 triangles
	  else if ( pixelMap[i][j].getType() == 1 ) {
	    // IF statement dealing with the top-rotation triangle
	    // ELSE IF statment dealing with the bottom-rotation triangle
	    // See Ianiv Schweber's table for specification
	    if ( pixelMap[i][j].getRot() == 0 ) {
	      pixelMap[i][j].changelt();
	    } 
	    else if ( pixelMap[i][j].getRot() == 1 ) {
	      pixelMap[i][j].changelT();
	    }
	    else
	      cout << "Error2" << endl;
	  }
	}
      }
    }
  }



  // Creates new image file with specified width and height
  // Magic number (P3) so image file is '.ppm'
  infile.open("penrose.ppm");
  infile << "P3" << endl;
  infile << matW << " " << matH << endl;
  infile << scale << endl << endl;
  for (int j = 0; j < matH; j++) {
    for(int i = 0; i < matW; i++) {
      if (pixelMap[i][j].getType() == 0) {
	if (pixelMap[i][j].getRot() == 0)
	  infile << 255 << " " << 0 << " " << 0 << " ";	
	else if (pixelMap[i][j].getRot() == 1)
	  infile << 0 << " " << 255 << " " << 0 << " ";
      } 
      else if (pixelMap[i][j].getType() == 1) {
	if (pixelMap[i][j].getRot() == 0)
	  infile << 0 << " " << 0 << " " << 255 << " ";
	else if (pixelMap[i][j].getRot() == 1) {
	  infile << 255 << " " << 0 << " " << 255 << " ";
	  if (i == ceil(longLeg) - 1 && j == ceil(shortLeg + height) + 3)
	    cout << pixelMap[i][j].getSpin() << endl;
	} 
      }
      else {
	infile << 0 << " " << 0 << " " << 0 << " ";
      }
    }
    infile << endl;
  }
  
  infile.close();
  return 0;
}

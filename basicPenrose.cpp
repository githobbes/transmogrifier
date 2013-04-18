#include "pixel.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

using namespace std;
using namespace transmogrifier;

int
main (int argc, char * argv[]) {

	const int scale = 255;

	ofstream infile;
	ifstream picfile;
	char pre[2];
	int width, height, sGarbage;

	height = 500;
	width = 500;

	/*	picfile.open(argv[1]);
	picfile >> pre;
	picfile >> width;
	picfile >> height;
	picfile >> sGarbage;

	int **pic;
	pic = new int*[width*3];
	for (int i = 0; i < width*3; ++i) {
		pic[i] = new int[height];
	}

	cout << "Reading pigments into 'pic'" << endl;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width * 3; i++) {
			picfile >> pic[i][j];
		}
	}
	picfile.close(); */

	// Initialize constants for use with base-region (six triangles)
	const float shortLeg = height * (1.0 / 4)*(sqrt(5) - 1);
	const float longLeg = height * sqrt((0.625) + sqrt(5)/8);
	const int matH = height + ceil(2*shortLeg);
	const int matW = ceil(2*longLeg);

	cout << shortLeg << ", " << longLeg << ", " << matH << ", " << matW << std::endl;
	cout << height << ", " << width << std::endl;

	int pic[3*matW][matH];

	Pixel **pixelMap;
	pixelMap = new Pixel*[matW];
	for (int i = 0; i < matW; ++i) {
		pixelMap[i] = new Pixel[matH];
	}

	int botLeft[3] = {0, 1, 0};
	int midLeft[3] = {0, 0, 0};
	int topLeft[3] = {1, 0, 0};
	int botRight[3] = {0, 0, 2};
	int midRight[3] = {0, 1, 2};
	int topRight[3] = {1, 1, 0};
	int black[3] = {-1, -1, -1};

	cout << "Initializing pixelMap" << endl;
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

	cout << "Iterating through the Penrose tiling algorithm" << endl;
	// Iterating through the Penrose tiling algorithm...
	for(int k = 0; k < 4; k++) {
	    cout << "Here." << std::endl;
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

	for (int j = 0; j < matH; j++)
	    for (int i = 0; i < matW; i++) {
		if (pixelMap[i][j].getType() == 0 && pixelMap[i][j].getRot() == 0) {
		    pic[3*i][j] = 255;
		    pic[3*i + 1][j] = 0;
		    pic[3*i + 2][j] = 0;
		}
		else if (pixelMap[i][j].getType() == 0 && pixelMap[i][j].getRot() == 1) {
		    pic[3*i][j] = 0;
		    pic[3*i + 1][j] = 255;
		    pic[3*i + 2][j] = 0;
		}
		else if (pixelMap[i][j].getType() == 1 && pixelMap[i][j].getRot() == 0) {
		    pic[3*i][j] = 0;
		    pic[3*i + 1][j] = 0;
		    pic[3*i + 2][j] = 255;
		}
		else if (pixelMap[i][j].getType() == 1 && pixelMap[i][j].getRot() == 1){
		    pic[3*i][j] = 255;
		    pic[3*i + 1][j] = 0;
		    pic[3*i + 2][j] = 255;
		}
	    }

	int top, bot, left, right;
	int red, gre, blu, rin, gin, bin, rout, gout, bout, red0, red1, red2, gre0, gre1, gre2, blu0, blu1, blu2;
	double layer0, layer1, layer2, weight1, weight2;
	cout << "Coloring" << std::endl;
	for (int j = 0; j < matH; j++) {
	    cout << "J: " << j << std::endl;
		for (int i = 0; i < matW; i++) {
			if (!pixelMap[i][j].coloredYet() && (pixelMap[i][j].getType() == 0 || pixelMap[i][j].getType() == 1)) {
				red0 = gre0 = blu0 = red1 = gre1 = blu1 = red2 = gre2 = blu2 = 0;
				layer0 = layer1 = layer2 = 0;
				top = std::min((int)pixelMap[i][j].getY1(), (int)pixelMap[i][j].getY2());
				top = std::min(top, (int)pixelMap[i][j].getY3());
				bot = std::max((int)pixelMap[i][j].getY1(), (int)pixelMap[i][j].getY2());
				bot = std::max(bot, (int)pixelMap[i][j].getY3());
				bot = std::min(bot, (int)(matH));
				left = std::min((int)pixelMap[i][j].getX1(), (int)pixelMap[i][j].getX2());
				left = std::min(left, (int)pixelMap[i][j].getX3());
				left = std::max(0, left);
				right = std::max((int)pixelMap[i][j].getX1(), (int)pixelMap[i][j].getX2());
				right = std::max(right, (int)pixelMap[i][j].getX3());
				right = std::min(right, (int)(matW));
				
				for (int y = top; y <= bot && y < matH; y++) {
					for (int x = left; x <= right && x < matW; x++) {
					    if (!pixelMap[x][y].coloredYet()) {
						if (pixelMap[i][j].getType() == pixelMap[x][y].getType()
						        && pixelMap[i][j].getRot() == pixelMap[x][y].getRot()
						        && pixelMap[i][j].getSpin() == pixelMap[x][y].getSpin()
						        && !pixelMap[x][y].coloredYet()) {
							// Determine location within ellipses
							pixelMap[x][y].ellipticize();
							// Set up for color determination
							if (x < matW && y < matH) {
								if (pixelMap[x][y].getLayer() == 0) {
									red0 += pic[3*x][y];
									gre0 += pic[3*x + 1][y];
									blu0 += pic[3*x + 2][y];
									layer0++;
								}
								else if (pixelMap[x][y].getLayer() == 1) {
									red1 += pic[3*x][y];
									gre1 += pic[3*x + 1][y];
									blu1 += pic[3*x + 2][y];
									layer1++;
								}
								else {
									red2 += pic[3*x][y];
									gre2 += pic[3*x + 1][y];
									blu2 += pic[3*x + 2][y];
									layer2++;
								}
							}
						}
					    }
					}
				}

				if (layer0 != 0 ) {

					red = (int)((red0 + red1) / (layer0 + layer1));
					gre = (int)((gre0 + gre1) / (layer0 + layer1));
					blu = (int)((blu0 + blu1) / (layer0 + layer1));

					if (red0 + red1 + red2 > 0) {
					    red0 = 255;
					    red1 = 127;
					    red2 = 0;
					}
					if (gre0 + gre1 + gre2 > 0) {
					    gre0 = 255;
					    gre1 = 127;
					    gre2 = 0;
					}
					if (blu0 + blu1 + blu2 > 0) {
					    blu0 = 255;
					    blu1 = 127;
					    blu2 = 0;
					}

					// Reset pixels in "pic" to clustered color
					for (int y = top; y <= bot && y < matH; y++) {
						for (int x = left; x <= right && x < matW; x++) {
						    if (!pixelMap[x][y].coloredYet())
							if (pixelMap[i][j].getType() == pixelMap[x][y].getType()
							        && pixelMap[i][j].getRot() == pixelMap[x][y].getRot()
							        && pixelMap[i][j].getSpin() == pixelMap[x][y].getSpin()
							        && !pixelMap[x][y].coloredYet()) {
								if (x < matW && y < matH) {
									if (pixelMap[x][y].getLayer() == 0) {
										pic[3*x][y] = red0;
										pic[3*x + 1][y] = gre0;
										pic[3*x + 2][y] = blu0;
										pixelMap[x][y].color();
									}
									else if (pixelMap[x][y].getLayer() == 1) {
										pic[3*x][y] = red1;
										pic[3*x + 1][y] = gre1;
										pic[3*x + 2][y] = blu1;
										pixelMap[x][y].color();
									}
									else if (pixelMap[x][y].getLayer() == 2) {
										pic[3*x][y] = red2;
										pic[3*x + 1][y] = gre2;
										pic[3*x + 2][y] = blu2;
										pixelMap[x][y].color();
									}
								}
							}
						}
					}
				}
			}
		}
	}

	cout << width << " is the width." << std::endl;

	cout << "Writing penpic.ppm" << endl;
	infile.open("penpic.ppm");
	infile << "P3" << endl;
	infile << matW << " " << matH << endl;
	infile << scale << endl << endl;
	for (int j = 0; j < matH; j++) {
		for (int i = 0; i < matW*3; i++) {
		    infile << pic[i][j] << " ";
		}
		infile << endl;
	}
	infile.close();


	// Creates new image file with specified width and height
	// Magic number (P3) so image file is '.ppm'
	cout << "Writing penrose.ppm" << endl;
	infile.open("penrose.ppm");
	infile << "P3" << endl;
	infile << matW << " " << matH << endl;
	infile << scale << endl << endl;
	for (int j = 0; j < matH; j++) {
		for(int i = 0; i < matW; i++) {
			if (pixelMap[i][j].getType() == 0) {
				if (pixelMap[i][j].getRot() == 0)
					infile << 255 << " " << 0 << " " << 0 << " ";
				else
					infile << 0 << " " << 255 << " " << 0 << " ";
			}
			else if (pixelMap[i][j].getType() == 1) {
				if (pixelMap[i][j].getRot() == 0)
					infile << 0 << " " << 0 << " " << 255 << " ";
				else
					infile << 255 << " " << 0 << " " << 255 << " ";
			}
			else {
				infile << 0 << " " << 0 << " " << 0 << " ";
			}
		}
		infile << endl;
	}
}

#include "algorithms.hpp"
#include "logging.hpp"
#include "pixel.hpp"
#include <cmath>
#include <cstdlib>

using transmogrifier::log;

void
transmogrifier::penroseChuck(std::istream& inputPPMStream, std::ostream& outputPPMStream, unsigned iterations) {
	char pre[2];
	int width, height, scale;

	inputPPMStream >> pre;
	inputPPMStream >> width;
	inputPPMStream >> height;
	inputPPMStream >> scale;

	int **pic;
	pic = new int*[width*3];
	for (int i = 0; i < width*3; ++i) {
		pic[i] = new int[height];
	}

	log() << "Reading pigments into matrix" << std::endl;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width * 3; i++) {
			inputPPMStream >> pic[i][j];
		}
	}

	// Initialize constants for use with base-region (six triangles)
	const float shortLeg = height * (1.0 / 4)*(std::sqrt(5) - 1);
	const float longLeg = height * std::sqrt((0.625) + std::sqrt(5)/8);
	const int matH = height + 2*std::ceil(shortLeg);
	const int matW = std::min(width, (int)(2*std::ceil(longLeg)));

	transmogrifier::Pixel **pixelMap;
	pixelMap = new transmogrifier::Pixel*[matW];
	for (int i = 0; i < matW; ++i) {
		pixelMap[i] = new transmogrifier::Pixel[matH];
	}

	int botLeft[3] = {0, 1, 0};
	int midLeft[3] = {0, 0, 0};
	int topLeft[3] = {1, 0, 0};
	int botRight[3] = {0, 0, 2};
	int midRight[3] = {0, 1, 2};
	int topRight[3] = {1, 1, 0};
	int black[3] = {-1, -1, -1};

	log() << "Initializing base Penrose tiling" << std::endl;
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

	log() << "Iterating through the Penrose tiling algorithm" << std::endl;
	// Iterating through the Penrose tiling algorithm...
	for(int k = 0; k < iterations; k++) {
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
					}
				}
			}
		}
	}



	int top, bot, left, right;
	int red, gre, blu, rin, gin, bin, rout, gout, bout, red0, red1, red2, gre0, gre1, gre2, blu0, blu1, blu2;
	double layer0, layer1, layer2, weight1, weight2;
	log() << "Coloring" << std::endl;
	for (int j = shortLeg; j < shortLeg + height; j++) {
		for (int i = 0; i < matW; i++) {
			if (!pixelMap[i][j].coloredYet() && (pixelMap[i][j].getType() == 0 || pixelMap[i][j].getType() == 1)) {
				red0 = gre0 = blu0 = red1 = gre1 = blu1 = red2 = gre2 = blu2 = 0;
				layer0 = layer1 = layer2 = 0;
				top = std::min((int)pixelMap[i][j].getY1(), (int)pixelMap[i][j].getY2());
				top = std::min(top, (int)pixelMap[i][j].getY3());
				top = std::max(0, (int)shortLeg);
				bot = std::max((int)pixelMap[i][j].getY1(), (int)pixelMap[i][j].getY2());
				bot = std::max(bot, (int)pixelMap[i][j].getY3());
				bot = std::min(bot, (int)(shortLeg + height));
				left = std::min((int)pixelMap[i][j].getX1(), (int)pixelMap[i][j].getX2());
				left = std::min(left, (int)pixelMap[i][j].getX3());
				left = std::max(0, left);
				right = std::max((int)pixelMap[i][j].getX1(), (int)pixelMap[i][j].getX2());
				right = std::max(right, (int)pixelMap[i][j].getX3());
				right = std::min(right, (int)(width));

				for (int y = top; y <= bot && y < shortLeg + height; y++) {
					for (int x = left; x <= right && x < matW; x++) {
						if (pixelMap[i][j].getType() == pixelMap[x][y].getType()
						        && pixelMap[i][j].getRot() == pixelMap[x][y].getRot()
						        && pixelMap[i][j].getSpin() == pixelMap[x][y].getSpin()
						        && !pixelMap[x][y].coloredYet()) {
							// Determine location within ellipses
							pixelMap[x][y].ellipticize();
							// Set up for color determination
							if (x < width && y - (int) shortLeg < height) {
								if (pixelMap[x][y].getLayer() == 0) {
									red0 += pic[3*x][y - (int) shortLeg];
									gre0 += pic[3*x + 1][y - (int) shortLeg];
									blu0 += pic[3*x + 2][y - (int) shortLeg];
									layer0++;
								}
								else if (pixelMap[x][y].getLayer() == 1) {
									red1 += pic[3*x][y - (int) shortLeg];
									gre1 += pic[3*x + 1][y - (int) shortLeg];
									blu1 += pic[3*x + 2][y - (int) shortLeg];
									layer1++;
								}
								else {
									red2 += pic[3*x][y - (int) shortLeg];
									gre2 += pic[3*x + 1][y - (int) shortLeg];
									blu2 += pic[3*x + 2][y - (int) shortLeg];
									layer2++;
								}
							}
						}
					}
				}

				if (layer0 != 0 ) {

					red = (int)((red0 + red1) / (layer0 + layer1));
					gre = (int)((gre0 + gre1) / (layer0 + layer1));
					blu = (int)((blu0 + blu1) / (layer0 + layer1));

					rin = (int)((red2) / (layer2));
					gin = (int)((gre2) / (layer2));
					bin = (int)((blu2) / (layer2));

					weight1 = layer0 / (layer0 + layer1);
					weight2 = layer1 / (layer0 + layer1);

					if (weight2 != 0) {
						red0 = std::rand() % (1 + std::abs(std::max(0, std::max(red - 40, (int)((red - 255 * weight2) / weight1)))
						                                   - std::min(255, std::min(red + 40, (int)(red / weight2)))));
						red0 += std::min(std::max(0, std::max(red - 40, (int)((red - 255 * weight2) / weight1))),
						                 std::min(255, std::min(red + 40, (int)(red / weight2))));
						gre0 = std::rand() % (1 + std::abs(std::max(0, std::max(gre - 40, (int)((gre - 255 * weight2) / weight1)))
						                                   - std::min(255, std::min(gre + 40, (int)(gre / weight2)))));
						gre0 += std::min(std::max(0, std::max(gre - 40, (int)((gre - 255 * weight2) / weight1))),
						                 std::min(255, std::min(gre + 40, (int)(gre / weight2))));
						blu0 = std::rand() % (1 + std::abs(std::max(0, std::max(blu - 40, (int)((blu - 255 * weight2) / weight1)))
						                                   - std::min(255, std::min(blu + 40, (int)(blu / weight2)))));
						blu0 += std::min(std::max(0, std::max(blu - 40, (int)((blu - 255 * weight2) / weight1))),
						                 std::min(255, std::min(blu + 40, (int)(blu / weight2))));
						red1 = std::floor((red-red0*weight1) / weight2);
						gre1 = std::floor((gre-gre0*weight1) / weight2);
						blu1 = std::floor((blu-blu0*weight1) / weight2);
					}

					if (weight2 == 0) {
						red0 = std::rand() % (1 + std::abs(std::max(0, std::max(red - 40, (int)((red - 255 * weight2) / weight1)))
						                                   - std::min(255, red + 40)));
						red0 += std::min(std::max(0, std::max(red - 40, (int)((red - 255 * weight2) / weight1))),
						                 std::min(255, red + 40));
						gre0 = std::rand() % (1 + std::abs(std::max(0, std::max(gre - 40, (int)((gre - 255 * weight2) / weight1)))
						                                   - std::min(255, gre + 40)));
						gre0 += std::min(std::max(0, std::max(gre - 40, (int)((gre - 255 * weight2) / weight1))),
						                 std::min(255, gre + 40));
						blu0 = std::rand() % (1 + std::abs(std::max(0, std::max(blu - 40, (int)((blu - 255 * weight2) / weight1)))
						                                   - std::min(255, blu + 40)));
						blu0 += std::min(std::max(0, std::max(blu - 40, (int)((blu - 255 * weight2) / weight1))),
						                 std::min(255, blu + 40));
						red1 = 0;
						gre1 = 0;
						blu1 = 0;
					}

					red2 = std::rand() % (std::abs(rin - std::min(255, rin + 40))) + rin + 1;
					gre2 = std::rand() % (std::abs(gin - std::min(255, gin + 40))) + gin + 1;
					blu2 = std::rand() % (std::abs(bin - std::min(255, bin + 40))) + bin + 1;

					// Reset pixels in "pic" to clustered color
					for (int y = top; y <= bot && y < shortLeg + height; y++) {
						for (int x = left; x <= right && x < matW; x++) {
							if (pixelMap[i][j].getType() == pixelMap[x][y].getType()
							        && pixelMap[i][j].getRot() == pixelMap[x][y].getRot()
							        && pixelMap[i][j].getSpin() == pixelMap[x][y].getSpin()
							        && !pixelMap[x][y].coloredYet()) {
								if (3*x < width * 3 && y - (int) shortLeg < height) {
									if (pixelMap[x][y].getLayer() == 0) {
										pic[3*x][y - (int) shortLeg] = red0;
										pic[3*x + 1][y - (int) shortLeg] = gre0;
										pic[3*x + 2][y - (int) shortLeg] = blu0;
										pixelMap[x][y].color();
									}
									else if (pixelMap[x][y].getLayer() == 1) {
										pic[3*x][y - (int) shortLeg] = red1;
										pic[3*x + 1][y - (int) shortLeg] = gre1;
										pic[3*x + 2][y - (int) shortLeg] = blu1;
										pixelMap[x][y].color();
									}
									else if (pixelMap[x][y].getLayer() == 2) {
										pic[3*x][y - (int) shortLeg] = red2;
										pic[3*x + 1][y - (int) shortLeg] = gre2;
										pic[3*x + 2][y - (int) shortLeg] = blu2;
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

	log() << "Writing new image" << std::endl;
	outputPPMStream << "P3" << std::endl;
	outputPPMStream << width << " " << height << std::endl;
	outputPPMStream << scale << std::endl;

	int count0, count1;
	count0 = count1 = 0;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width*3; i++) {
			if (pic[i][j] < 0) {
				//	log() << "I: " << i << " J: " << j << " - Val:" << pic[i][j] << std::endl;
				pic[i][j] = 0;
				count0++;
			}
			else if (pic[i][j] > 255) {
				//				log() << "I: " << i << " J: " << j << " - Val: " << pic[i][j] << std::endl;
				pic[i][j] = 255;
				count0++;
			}
			outputPPMStream << pic[i][j] << " ";
			count1++;
		}
		outputPPMStream << std::endl;
	}

	log() << "Corrupt data: " << float(count0) / count1 << "%" << std::endl;

	log() << "Clearing memory" << std::endl;
	delete pic;
	delete pixelMap;
}

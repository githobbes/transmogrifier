#define _USE_MATH_DEFINES

#include "pixel.hpp"
#include <cmath>
#include <iostream>

// Class definition for Pixels

int
Pixel::getX()
{
	return X;
}

void
Pixel::setX(int x) {
	X = x;
}

void
Pixel::setY(int y) {
	Y = y;
}

void
Pixel::color() {
	isColored = true;
}

bool
Pixel::coloredYet() {
	return isColored;
}

double
Pixel::getX1() {
	return X1;
}

double
Pixel::getY1() {
	return Y1;
}

double
Pixel::getX2() {
	return X2;
}

double
Pixel::getY2() {
	return Y2;
}

double
Pixel::getX3() {
	return X3;
}

double
Pixel::getY3() {
	return Y3;
}

void
Pixel::setTypes(int* type) {
	isColored = false;
	layer = 0;
	triType[0] = type[0];
	triType[1] = type[1];
	triType[2] = type[2];
}

// Returns 0 if containing triangle is a 108/36/36 triangle.
// Returns 1 if containing triangle is a 36/72/72 triangle.
int
Pixel::getType() {
	return triType[0];
}

// Returns 0 if containing triangle is a top-rotation triangle
// Returns 1 if containing triangle is a bottom-rotation triangle
// See Ianiv Schweber's table for specification.
int
Pixel::getRot() {
	return triType[1];
}

int
Pixel::getSpin() {
	return triType[2];
}

int
Pixel::getLayer() {
	return layer;
}

void
Pixel::setVertices(double x1, double y1, double x2, double y2, double x3, double y3)
{
	X1 = x1;
	Y1 = y1;
	X2 = x2;
	Y2 = y2;
	X3 = x3;
	Y3 = y3;

	// Evaluate the P and (possibly) Q point
	if ( triType[0] == 0 ) {
		if ( triType[1] == 0 ) {
			// First, we determine the values for P1 and P2.
			if ( triType[2] % 5 == 4 )
				P1 = X2;
			else if ( triType[2] % 5 == 1 )
				P1 = X1;
			else {
				P1 = ((Y1 - Y3)/(X1 - X3))*X1 - std::tan(((36 * triType[2]) - 54) / (180 / M_PI))*X2 + Y2 - Y1;
				P1 = P1 / ((Y1 - Y3)/(X1 - X3) - std::tan(((36 * triType[2]) - 54) / (180 / M_PI)));
			}
			if ( triType[2] % 5 == 1 )
				P2 = std::tan((0 - 18)/(180 / M_PI))*(P1 - X2) + Y2;
			else
				P2 = ((Y1 - Y3)/(X1 - X3))*(P1 - X1) + Y1;

			// Then, we find the (Q1, Q2).
			if ( triType[2] % 5 == 0 )
				Q1 = P1;
			else if ( triType[2] % 5 == 2 )
				Q1 = X2;
			else {
				Q1 = ((Y2 - Y3)/(X2 - X3))*X2 - std::tan(((36 * triType[2]) - 90) / (180 / M_PI))*P1 + P2 - Y2;
				Q1 = Q1 / (((Y2 - Y3)/(X2 - X3)) - std::tan(((36 * triType[2]) - 90) / (180 / M_PI)));
			}
			if ( triType[2] % 5 == 2 )
				Q2 = std::tan(((36 * (triType[2] - 2)) - 18)/(180 / M_PI))*(Q1 - P1) + P2;
			else
				Q2 = ((Y2 - Y3)/(X2 - X3))*(Q1 - X2) + Y2;

		}
		else {
			if ( triType[2] % 5 == 3 )
				P1 = X1;
			else if ( triType[2] % 5 == 1 )
				P1 = X2;
			else {
				P1 = ((Y3 - Y2)/(X3 - X2))*X3 - std::tan(((36 * triType[2]) - 18) / (180 / M_PI))*X1 + Y1 - Y3;
				P1 = P1 / (((Y3 - Y2)/(X3 - X2)) - std::tan(((36 * triType[2]) - 18) / (180 / M_PI)));
			}
			if ( triType[2] % 5 == 1 )
				P2 = std::tan((18)/(180 / M_PI))*(P1 - X1) + Y1;
			else
				P2 = ((Y3 - Y2)/(X3 - X2))*(P1 - X3) + Y3;

			if ( triType[2] % 5 == 0 )
				Q1 = X1;
			else if ( triType[2] % 5 == 2 )
				Q1 = P1;
			else {
				Q1 = ((Y3 - Y1)/(X3 - X1))*X3 - std::tan((18 + (36 * triType[2])) / (180 / M_PI))*P1 + P2 - Y3;
				Q1 = Q1 / (((Y3 - Y1)/(X3 - X1)) - std::tan((18 + (36 * triType[2])) / (180 / M_PI)));
			}
			if ( triType[2] % 5 == 0 )
				Q2 = std::tan((18 + (36 * triType[2]))/(180 / M_PI))*(Q1 - P1) + P2;
			else
				Q2 = ((Y3 - Y1)/(X3 - X1))*(Q1 - X3) + Y3;
		}
	}
	else {
		if ( triType[1] == 0 ) {
			if (triType[2] == 4 || triType[2] == 9) {
				P1 = X2;
			}
			else if (triType[2] == 2 || triType[2] == 7) {
				P1 = X1;
			}
			else {
				P1 = ((Y1 - Y3)/(X1 - X3))*X3 - std::tan(((36 * triType[2]) - 54)/(180 / M_PI))*X2 + Y2 - Y3;
				P1 = P1 / (((Y1 - Y3)/(X1 - X3)) - std::tan(((36 * triType[2]) - 54)/(180 / M_PI)));
			}
			if (triType[2] == 4 || triType[2] == 9) {
				P2 = ((Y3 - Y1)/(X3 - X1))*(P1 - X1) + Y1;
			}
			else {
				P2 = std::tan(((36 * triType[2]) - 54)/(180 / M_PI))*(P1 - X2) + Y2;
			}
		}
		else {
			if (triType[2] == 3 || triType[2] == 8) {
				P1 = X1;
			}
			else if (triType[2] == 1 || triType[2] == 6) {
				P1 = X3;
			}
			else {
				P1 = ((Y1 - Y2)/(X1 - X2))*X2 - std::tan((54 + (36 * triType[2]))/(180 / M_PI))*X3 + Y3 - Y2;
				P1 = P1 / (((Y1 - Y2)/(X1 - X2)) - std::tan((54 + (36 * triType[2]))/(180 / M_PI)));
			}
			if (triType[2] == 1 || triType[2] == 6) {
				P2 = ((Y2 - Y1)/(X2 - X1))*(P1 - X2) + Y2;
			}
			else {
				P2 = std::tan(((36 * triType[2]) + 54)/(180 / M_PI))*(P1 - X3) + Y3;
			}
		}
	}
}

void
Pixel::changeLt() {
	if ( triType[2] == 0 ) {
		if ( X < P1 ) {
			triType[1] = 1;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(Q1, Q2, X3, Y3, P1, P2);
		}
		else if ( Y > std::tan((0 - 54 + (36 * triType[2]))/(180 / M_PI))*(X - X2) + Y2 ) {
			triType[2] = (triType[2] + 6) % 10;
			setVertices(X2, Y2, P1, P2, X1, Y1);
		}
		else {
			triType[0] = 1;
			triType[1] = 1;
			triType[2] = (triType[2] + 3) % 10;
			setVertices(P1, P2, Q1, Q2, X2, Y2);
		}
	}
	else if ((triType[2] == 1) || (triType[2] == 2) || (triType[2] == 3)) {
		if ( Y < std::tan((0 - 90 + (36 * triType[2]))/(180 / M_PI))*(X - P1) + P2 ) {
			triType[1] = 1;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(Q1, Q2, X3, Y3, P1, P2);
		}
		else if ( Y > std::tan((0 - 54 + (36 * triType[2]))/(180 / M_PI))*(X - X2) + Y2 ) {
			triType[2] = (triType[2] + 6) % 10;
			setVertices(X2, Y2, P1, P2, X1, Y1);
		}
		else {
			triType[0] = 1;
			triType[1] = 1;
			triType[2] = (triType[2] + 3) % 10;
			setVertices(P1, P2, Q1, Q2, X2, Y2);
		}
	}
	else if ( triType [2] == 4 ) {
		if ( Y < std::tan((0 - 90 + (36 * triType[2]))/(180 / M_PI))*(X - P1) + P2 ) {
			triType[1] = 1;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(Q1, Q2, X3, Y3, P1, P2);
		}
		else if ( X < X2 ) {
			triType[2] = (triType[2] + 6) % 10;
			setVertices(X2, Y2, P1, P2, X1, Y1);
		}
		else {
			triType[0] = 1;
			triType[1] = 1;
			triType[2] = (triType[2] + 3) % 10;
			setVertices(P1, P2, Q1, Q2, X2, Y2);
		}
	}
	else if ( triType[2] == 5 ) {
		if ( X > P1 ) {
			triType[1] = 1;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(Q1, Q2, X3, Y3, P1, P2);
		}
		else if ( Y < std::tan((0 - 54 + (36 * triType[2]))/(180 / M_PI))*(X - X2) + Y2 ) {
			triType[2] = (triType[2] + 6) % 10;
			setVertices(X2, Y2, P1, P2, X1, Y1);
		}
		else {
			triType[0] = 1;
			triType[1] = 1;
			triType[2] = (triType[2] + 3) % 10;
			setVertices(P1, P2, Q1, Q2, X2, Y2);
		}
	}
	else if ( (triType[2] == 6) || (triType[2] == 7) || (triType[2] == 8) ) {
		if ( Y > std::tan((0 - 90 + (36 * triType[2]))/(180 / M_PI))*(X - P1) + P2 ) {
			triType[1] = 1;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(Q1, Q2, X3, Y3, P1, P2);
		}
		else if ( Y < std::tan((0 - 54 + (36 * triType[2]))/(180 / M_PI))*(X - X2) + Y2 ) {
			triType[2] = (triType[2] + 6) % 10;
			setVertices(X2, Y2, P1, P2, X1, Y1);
		}
		else {
			triType[0] = 1;
			triType[1] = 1;
			triType[2] = (triType[2] + 3) % 10;
			setVertices(P1, P2, Q1, Q2, X2, Y2);
		}
	}
	else {
		if ( Y > std::tan((0 - 90 + (36 * triType[2]))/(180 / M_PI))*(X - P1) + P2 ) {
			triType[1] = 1;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(Q1, Q2, X3, Y3, P1, P2);
		}
		else if ( X > P1 ) {
			triType[2] = (triType[2] + 6) % 10;
			setVertices(X2, Y2, P1, P2, X1, Y1);
		}
		else {
			triType[0] = 1;
			triType[1] = 1;
			triType[2] = (triType[2] + 3) % 10;
			setVertices(P1, P2, Q1, Q2, X2, Y2);
		}
	}
}

void
Pixel::changeLT() {
	if ( triType[2] == 2 ) {
		if ( X > P1 ) {
			triType[1] = 0;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(X3, Y3, Q1, Q2, P1, P2);
		}
		else if ( Y > std::tan((0 - 18 + (36 * triType[2]))/(180 / M_PI))*(X - X1) + Y1 ) {
			triType[2] = (triType[2] + 4) % 10;
			setVertices(P1, P2, X1, Y1, X2, Y2);
		}
		else {
			triType[0] = 1;
			triType[1] = 0;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(P1, P2, X1, Y1, Q1, Q2);
		}
	}
	else if ( triType[2] == 3 ) {
		if ( Y > std::tan((18 + (36 * triType[2]))/(180 / M_PI))*(X - P1) + P2 ) {
			triType[1] = 0;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(X3, Y3, Q1, Q2, P1, P2);
		}
		else if ( X < P1 ) {
			triType[2] = (triType[2] + 4) % 10;
			setVertices(P1, P2, X1, Y1, X2, Y2);
		}
		else {
			triType[0] = 1;
			triType[1] = 0;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(P1, P2, X1, Y1, Q1, Q2);
		}
	}
	else if ( triType[2] > 3 && triType[2] < 7 ) {
		if ( Y > std::tan((18 + (36 * triType[2]))/(180 / M_PI))*(X - P1) + P2 ) {
			triType[1] = 0;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(X3, Y3, Q1, Q2, P1, P2);
		}
		else if ( Y < std::tan((0 - 18 + (36 * triType[2]))/(180 / M_PI))*(X - X1) + Y1 ) {
			triType[2] = (triType[2] + 4) % 10;
			setVertices(P1, P2, X1, Y1, X2, Y2);
		}
		else {
			triType[0] = 1;
			triType[1] = 0;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(P1, P2, X1, Y1, Q1, Q2);
		}
	}
	else if ( triType[2] == 7 ) {
		if ( X < P1 ) {
			triType[1] = 0;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(X3, Y3, Q1, Q2, P1, P2);
		}
		else if ( Y < std::tan((0 - 18 + (36 * triType[2]))/(180 / M_PI))*(X - X1) + Y1 ) {
			triType[2] = (triType[2] + 4) % 10;
			setVertices(P1, P2, X1, Y1, X2, Y2);
		}
		else {
			triType[0] = 1;
			triType[1] = 0;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(P1, P2, X1, Y1, Q1, Q2);
		}
	}
	else if ( triType[2] == 8 ) {
		if ( Y < std::tan((18 + (36 * triType[2]))/(180 / M_PI))*(X - P1) + P2 ) {
			triType[1] = 0;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(X3, Y3, Q1, Q2, P1, P2);
		}
		else if ( X > P1 ) {
			triType[2] = (triType[2] + 4) % 10;
			setVertices(P1, P2, X1, Y1, X2, Y2);
		}
		else {
			triType[0] = 1;
			triType[1] = 0;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(P1, P2, X1, Y1, Q1, Q2);
		}
	}
	else {
		if ( Y < std::tan((18 + (36 * triType[2]))/(180 / M_PI))*(X - P1) + P2 ) {
			triType[1] = 0;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(X3, Y3, Q1, Q2, P1, P2);
		}
		else if ( Y > std::tan((0 - 18 + (36 * triType[2]))/(180 / M_PI))*(X - X1) + Y1 ) {
			triType[2] = (triType[2] + 4) % 10;
			setVertices(P1, P2, X1, Y1, X2, Y2);
		}
		else {
			triType[0] = 1;
			triType[1] = 0;
			triType[2] = (triType[2] + 5) % 10;
			setVertices(P1, P2, X1, Y1, Q1, Q2);
		}
	}
}

void
Pixel::changelt() {
	if ( triType[2] == 4 ) {
		if ( X > P1 ) {
			triType[0] = 0;
			triType[2] = (triType[2] + 3) % 10;
			setVertices(X1, Y1, P1, P2, X2, Y2);
		} else {
			triType[2] = (triType[2] + 3) % 10;
			setVertices(X2, Y2, X3, Y3, P1, P2);
		}
	}
	else if ( triType[2] == 9 ) {
		if ( X < P1 ) {
			triType[0] = 0;
			triType[2] = (triType[2] + 3) % 10;
			setVertices(X1, Y1, P1, P2, X2, Y2);
		} else {
			triType[2] = (triType[2] + 3) % 10;
			setVertices(X2, Y2, X3, Y3, P1, P2);
		}
	}
	else if ( triType[2] < 4 ) {
		if ( Y < std::tan((0 - 54 + (36 * triType[2]))/(180 / M_PI))*(X - X2) + Y2 ) {
			triType[0] = 0;
			triType[2] = (triType[2] + 3) % 10;
			setVertices(X1, Y1, P1, P2, X2, Y2);
		} else {
			triType[2] = (triType[2] + 3) % 10;
			setVertices(X2, Y2, X3, Y3, P1, P2);
		}
	}
	else {
		if ( Y > std::tan((0 - 54 + (36 * triType[2]))/(180 / M_PI))*(X - X2) + Y2 ) {
			triType[0] = 0;
			triType[2] = (triType[2] + 3) % 10;
			setVertices(X1, Y1, P1, P2, X2, Y2);
		} else {
			triType[2] = (triType[2] + 3) % 10;
			setVertices(X2, Y2, X3, Y3, P1, P2);
		}
	}
}

void
Pixel::changelT() {
	if ( triType[2] == 6 ) {
		if ( X < P1 ) {
			triType[0] = 0;
			triType[2] = (triType[2] + 9) % 10;
			setVertices(P1, P2, X1, Y1, X3, Y3);
		} else {
			triType[2] = (triType[2] + 7) % 10;
			setVertices(X3, Y3, P1, P2, X2, Y2);
		}
	}
	else if ( triType[2] == 1 ) {
		if ( X > P1 ) {
			triType[0] = 0;
			triType[2] = (triType[2] + 9) % 10;
			setVertices(P1, P2, X1, Y1, X3, Y3);
		} else {
			triType[2] = (triType[2] + 7) % 10;
			setVertices(X3, Y3, P1, P2, X2, Y2);
		}
	}
	else if (triType[2] < 6 && triType[2] > 1 ) {
		if ( Y > std::tan((54 + (36 * triType[2]))/(180 / M_PI))*(X - X3) + Y3 ) {
			triType[0] = 0;
			triType[2] = (triType[2] + 9) % 10;
			setVertices(P1, P2, X1, Y1, X3, Y3);
		} else {
			triType[2] = (triType[2] + 7) % 10;
			setVertices(X3, Y3, P1, P2, X2, Y2);
		}
	}
	else {
		if ( Y < std::tan((54 + (36 * triType[2]))/(180 / M_PI))*(X - X3) + Y3 ) {
			triType[0] = 0;
			triType[2] = (triType[2] + 9) % 10;
			setVertices(P1, P2, X1, Y1, X3, Y3);
		} else {
			triType[2] = (triType[2] + 7) % 10;
			setVertices(X3, Y3, P1, P2, X2, Y2);
		}
	}
}

void
Pixel::ellipticize() {
	int dist;
	if (triType[0] == 0) {
		if (triType[2] == 1) {
			if (triType[1] == 0) {
				P1 = Q1 = X1 + (1.0/3)*(X2 - X1);

				P2 = Y3 + (1.0/4)*(Y1 - Y3);
				Q2 = Y3 + (3.0/4)*(Y1 - Y3);

				dist = (17.0/32)*(Y1 - Y3);
			}
			else if (triType[1] == 1) {
				P1 = Q1 = X1 + (2.0/3)*(X2 - X1);

				P2 = Y3 + (1.0/4)*(Y2 - Y3);
				Q2 = Y3 + (3.0/4)*(Y2 - Y3);

				dist = (17.0/32)*(Y2 - Y3);
			}
		}
		else if (triType[2] == 6) {
			if (triType[1] == 0) {
				P1 = Q1 = X2 + (2.0/3)*(X1 - X2);

				P2 = Y1 + (1.0/4)*(Y3 - Y1);
				Q2 = Y1 + (3.0/4)*(Y3 - Y1);

				dist = (17.0/32)*(Y3 - Y1);
			}
			else if (triType[1] == 1) {
				P1 = Q1 = X2 + (1.0/3)*(X1 - X2);

				P2 = Y2 + (1.0/4)*(Y3 - Y2);
				Q2 = Y2 + (3.0/4)*(Y3 - Y2);

				dist = (17.0/32)*(Y3 - Y2);
			}
		}
		else {
			if (triType[1] == 0) {
				// Set midpoint of long edge
				P1 = (1.0/2)*(X1 + X3);
				P2 = (1.0/2)*(Y1 + Y3);

				// Find center of ellipse
				Q1 = P1 + (1.0/3)*(X2 - P1);
				Q2 = P2 + (1.0/3)*(Y2 - P2);

				// Set (P1, P2) onto line with Center - extended from X3
				dist = P2 - Q2;
				P2 = Y3 - dist;
				dist = P1 - Q1;
				P1 = X3 - dist;

				// Find first foci - (Q1, Q2)
				Q1 = (1.0/2)*(P1 + Q1);
				Q2 = (1.0/2)*(P2 + Q2);

				// Find second foci - (P1, P2)
				P1 = P1 + 3*(Q1 - P1);
				P2 = P2 + 3*(Q2 - P2);

				dist = (17.0/32)*sqrt(pow(X1 - X3, 2) + pow(Y1 - Y3, 2));
			}
			else if (triType[1] == 1) {
				// Set midpoint of long edge
				P1 = (1.0/2)*(X2 + X3);
				P2 = (1.0/2)*(Y2 + Y3);

				// Find center of ellipse
				Q1 = X1 + (2.0/3)*(P1 - X1);
				Q2 = Y1 + (2.0/3)*(P2 - Y1);

				// Set (P1, P2) onto line with Center - extended from X3
				dist = Q1 - P1;
				P1 = X3 + dist;
				dist = Q2 - P2;
				P2 = Y3 + dist;

				// Find first foci - (Q1, Q2)
				Q1 = (1.0/2)*(Q1 + P1);
				Q2 = (1.0/2)*(Q2 + P2);

				// Find second foci - (P1, P2)
				P1 = P1 + 3*(Q1 - P1);
				P2 = P2 + 3*(Q2 - P2);

				dist = (17.0/32)*sqrt(pow(X2 - X3, 2) + pow(Y2 - Y3, 2));
			}
		}
	}
	else if (triType[0] == 1) {
		P1 = (1.0/2)*(X2 + X3);
		P2 = (1.0/2)*(Y2 + Y3);

		Q1 = X1 + (10.0/12)*(P1 - X1);
		Q2 = Y1 + (10.0/12)*(P2 - Y1);

		P1 = (1.0/2)*(P1 + X1);
		P2 = (1.0/2)*(P2 + Y1);

		dist = (1.5)*sqrt(pow(P1 - Q1, 2) + pow(P2 - Q2, 2));
	}

	// Determine if pixel lies within ellipse
	if (triType[0] == 0) {
		if (sqrt(pow(X - P1,2) + pow(Y - P2,2)) + sqrt(pow(X - Q1,2) + pow(Y - Q2, 2)) <= (1.1)*dist) {
			layer = 1;
			if (sqrt(pow(X - P1,2) + pow(Y - P2,2)) + sqrt(pow(X - Q1,2) + pow(Y - Q2, 2)) <= dist)
				layer = 2;
		}
	}
	else if (triType[0] == 1) {
		if (sqrt(pow(X - P1,2) + pow(Y - P2,2)) + sqrt(pow(X - Q1,2) + pow(Y - Q2, 2)) <= (1.3)*dist) {
			layer = 1;
			if (sqrt(pow(X - P1,2) + pow(Y - P2,2)) + sqrt(pow(X - Q1,2) + pow(Y - Q2, 2)) <= dist)
				layer = 2;
		}
	}
}


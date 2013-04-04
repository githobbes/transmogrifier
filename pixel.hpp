#ifndef PIXEL_HPP
#define PIXEL_HPP

class Pixel {
	// The values X and Y correspond to the pixel's location.
	// The value for color
	int X;
	int Y;

	// The following two-dimensional array corresponds to the type of encompassing triangle.
	// 0 - Red, 1 - Blue
	// 0 - Top piece, 1 - Bottom piece - (T/B corresponds to location on Ianiv Schweber's table)
	// The third entry is the rotation.
	// The value 0 refers to the position in the downward pointing star in the top-left position.
	// From there, the subsequent value increments the angle clockwise by 36 degrees.
	int triType[3];

	// Keeps track of which Chuck Close layer this pixel lies on
	int layer;

	// The X's and Y's correspond to the encompassing triangle's vertices.
	// If Pixel is red, the P's correspond to the point on the long edge, and
	// the Q's correspond to the point on one of the shorter edges.
	// Otherwise, the P's correspond to the only other point and the Q's are null.
	double X1, X2, X3, Y1, Y2, Y3, P1, P2, Q1, Q2;

	bool isColored;
public:

	int
	getX();

	void
	setX(int x);

	void
	setY(int y);

	void
	color();

	bool
	coloredYet();

	double
	getX1();

	double
	getY1();

	double
	getX2();

	double
	getY2();

	double
	getX3();

	double
	getY3();

	void
	setTypes(int* type);

	int
	getType();

	int
	getRot();

	int
	getSpin();

	int
	getLayer();

	void
	setVertices(double x1, double y1, double x2, double y2, double x3, double y3);

	void
	changeLt();

	void
	changeLT();

	void
	changelt();

	void
	changelT();

	void
	ellipticize();
};
#endif /* PIXEL_HPP */

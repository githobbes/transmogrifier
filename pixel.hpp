#ifndef PIXEL_HPP
#define PIXEL_HPP

int
getX();

void
setX(int x);

void
setY(int y);

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

#endif /* PIXEL_HPP */

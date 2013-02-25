#ifndef PIXEL_HPP
#define PIXEL_HPP

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

using namespace std;

// Class definition for Pixels
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
  int* triType[3];

  // The X's and Y's correspond to the encompassing triangle's vertices.
  // If Pixel is red, the P's correspond to the point on the long edge, and
  // the Q's correspond to the point on one of the shorter edges.
  // Otherwise, the P's correspond to the only other point and the Q's are null.
  double X1, X2, X3, Y1, Y2, Y3, P1, P2, Q1, Q2;

  // The following will be used for comparing a Pixel's location to that of the
  // the rest of the Pixels (and thus, the triangle which encompasses it).
  double triClose = tan((54 - (36 * triType[2]))/(180 / M_PI))(X - X2) + Y2;
  double sqrClose = tan((90 - (36 * triType[2]))/(180 / M_PI))(X - P1) + P2;
  double triOpen = tan((-18 - (36 * triType[2]))/(180 / M_PI))(X - P1) + P2;
  double sqrOpen = tan((18 - (36 * triType[2]))/(180 / M_PI))(X - X1) + Y1;
  double circClose = tan((54 - (36 * triType[2]))/(180 / M_PI))(X - X2) + Y2;
  double circOpen = tan((- 54 - (36 * triType[2]))/(180 / M_PI))(X - X3) + Y3;

 public:
  Pixel();

  Pixel(int x, int y);

  Pixel(int x, int y, int* type) {
    X = x;
    Y = y;
    triType[0] = type[0];
    triType[1] = type[1];
    triType[2] = type[2];
  }

  void
    setVertices(double x1, double y1, double x2, double y2, double x3, double y3) 
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
	P1 = (X1 - X3)*(X2 * tan((54 - (72 * triType[2])) / (180 / M_PI)) + Y1 - Y2);
	P1 = P1 / ((X1 - X3) * (tan((54 - (72 * triType[2])) / (180 / M_PI))) - Y1 + Y3);
	P2 = ((Y1 - Y3)/(X1 - X3))*(P1 - X1) + Y1;
	
	// Then, we find the (Q1, Q2).
	if ( triType[2] == 0 )
	  Q1 = P1;
	else {
	  Q1 = ((Y2 - Y3)/(X2 - X3))*X2 - tan(90 - (72 * triType[2]) / (180 / M_PI))*P1 + P2 - Y2;
	  Q1 = Q1 / (((Y2 - Y3)/(X2 - X3)) - tan(90 - (72 * triType[2]) / (180 / M_PI))); 
	}
	Q2 = ((Y2 - Y3)/(X2 - X3))*(Q1 - X2) + Y2;
	
      } else {
	P1 = ((Y3 - Y2)/(X3 - X2))*X3 - tan(18 - (72 * triType[2]) / (180 / M_PI))*X1 + Y1 - Y3;
	P1 = P1 / (((Y3 - Y2)/(X3 - X2)) - tan(18 - (72 * triType[2]) / (180 / M_PI)));
	P2 = ((Y3 - Y2)/(X3 - X2))*(P1 - X3) + Y3;
	
	Q1 = ((Y3 - Y1)/(X3 - X1))*X3 - tan(-18 - (72 * triType[2]) / (180 / M_PI))*P1 + P2 - Y3;
	Q1 = Q1 / (((Y3 - Y1)/(X3 - X1)) - tan(-18 - (72 * triType[2]) / (180 / M_PI)));
	Q2 = ((Y3 - Y1)/(X3 - X1))*(Q1 - X3) + Y3;
      }
    } else {
      if ( triType[1] == 0 ) {
	P1 = ((Y1 - Y3)/(X1 - X3))*X3 - tan(54 - (72 * triType[2])/(180 / M_PI))*X2 + Y2 - Y3;
	P1 = P1 / ((Y1 - Y3)/(X1 - X3)) - tan(54 - (72 * triType[2])/(180 / M_PI));
	P2 = tan(54 - (72 * triType[2])/(180 / M_PI))(P1 - X2) + Y2;
      } else {
	P1 = ((Y1 - Y2)/(X1 - X2))*X2 - tan(0 - 54 - (72 * triType[2])/(180 / M_PI))*X3 + Y3 - Y2;
	P1 = P1 / ((Y1 - Y2)/(X1 - X2)) - tan(0 - 54 - (72 * triType[2])/(180 / M_PI));
	P2 = tan(0 - 54 - (72 * triType[2])/(180 / M_PI))(P1 - X3) + Y3;
      }
    }

    triClose = tan((54 - (36 * triType[2]))/(180 / M_PI))(X - X2) + Y2;
    sqrClose = tan((90 - (36 * triType[2]))/(180 / M_PI))(X - P1) + P2;
    triOpen = tan((-18 - (36 * triType[2]))/(180 / M_PI))(X - P1) + P2;
    sqrOpen = tan((18 - (36 * triType[2]))/(180 / M_PI))(X - X1) + Y1;
    circClose = tan((54 - (36 * triType[2]))/(180 / M_PI))(X - X2) + Y2;
    circOpen = tan((- 54 - (36 * triType[2]))/(180 / M_PI))(X - X3) + Y3;
  }
  
  void
    changeLt() {
    if ( triType[2] == 0 ) {
      if ( X < P1 ) {
	triType[1] = 1;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(Q1, Q2, X3, Y3, P1, P2);
      } else if ( Y < triClose ) {
	triType[2] = (triType[2] + 6) % 10;
	setVertices(X2, Y2, P1, P2, X1, Y1);
      } else {
	triType[0] = 1;
	triType[1] = 1;
	triType[2] = (triType[2] + 3) % 10;
	setVertices(P1, P2, Q1, Q2, X2, Y2);
      }
    } else if ((triType[2] == 1) || (triType[2] == 2) || (triType[2] == 3)) {
      if ( Y > sqrClose ) {
	triType[1] = 1;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(Q1, Q2, X3, Y3, P1, P2);
      } else if ( Y < triClose ) {
	triType[2] = (triType[2] + 6) % 10;
	setVertices(X2, Y2, P1, P2, X1, Y1);
      } else {
	triType[0] = 1;
	triType[1] = 1;
	triType[2] = (triType[2] + 3) % 10;
	setVertices(P1, P2, Q1, Q2, X2, Y2);
      }
    } else if ( triType [2] == 4 ) {
      if ( X < sqrBlock ) {
	triType[1] = 1;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(Q1, Q2, X3, Y3, P1, P2);
      } else if ( X < P1 ) {
	triType[2] = (triType[2] + 6) % 10;
	setVertices(X2, Y2, P1, P2, X1, Y1);
      } else {
	triType[0] = 1;
	triType[1] = 1;
	triType[2] = (triType[2] + 3) % 10;
	setVertices(P1, P2, Q1, Q2, X2, Y2);
      }
    } else if ( triType[2] == 5 ) {
      if ( X > P1 ) {
	triType[1] = 1;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(Q1, Q2, X3, Y3, P1, P2);
      } else if ( Y > triBlock ) {
	triType[2] = (triType[2] + 6) % 10;
	setVertices(X2, Y2, P1, P2, X1, Y1);
      } else {
	triType[0] = 1;
	triType[1] = 1;
	triType[2] = (triType[2] + 3) % 10;
	setVertices(P1, P2, Q1, Q2, X2, Y2);
      }
    } else if ( (triType[2] == 6) || (triType[2] == 7) || (triType[2] == 8) ) {
      if ( Y < sqrBlock ) {
	triType[1] = 1;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(Q1, Q2, X3, Y3, P1, P2);
      } else if ( Y > triBlock ) {
	triType[2] = (triType[2] + 6) % 10;
	setVertices(X2, Y2, P1, P2, X1, Y1);
      } else {
	triType[0] = 1;
	triType[1] = 1;
	triType[2] = (triType[2] + 3) % 10;
	setVertices(P1, P2, Q1, Q2, X2, Y2);
      }
    } else {
      if ( Y < sqrBlock ) {
	triType[1] = 1;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(Q1, Q2, X3, Y3, P1, P2);
      } else if ( X > P1 ) {
	triType[2] = (triType[2] + 6) % 10;
	setVertices(X2, Y2, P1, P2, X1, Y1);
      } else {
	triType[0] = 1;
	triType[1] = 1;
	triType[2] = (triType[2] + 3) % 10;
	setVertices(P1, P2, Q1, Q2, X2, Y2);
      }
    }
  }
  
  void
    changeLT() {
    if ( triType[2] == 2 ) {
      if ( X > P1 ) {
	triType[1] = 0;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(X3, Y3, Q1, Q2, P1, P2);
      } else if ( Y < sqrOpen ) {
	triType[2] = (triType[2] + 4) % 10;
	setVertices(P1, P2, X1, Y1, X2, Y2);
      } else {
	triType[0] = 1;
	triType[1] = 0;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(P1, P2, X1, Y1, Q1, Q2);
      }
    } else if ( triType[2] == 3 ) {
      if ( Y < triOpen ) {
	triType[1] = 0;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(X3, Y3, Q1, Q2, P1, P2);
      }
      else if ( X < P1 ) {
	triType[2] = (triType[2] + 4) % 10;
	setVertices(P1, P2, X1, Y1, X2, Y2);
      } else {
	triType[0] = 1;
	triType[1] = 0;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(P1, P2, X1, Y1, Q1, Q2);
      }
    } else if ( triType[2] > 3 && triType[2] < 7 ) {
      if ( Y < triOpen ) {
	triType[1] = 0;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(X3, Y3, Q1, Q2, P1, P2);
      }
      else if ( Y > sqrOpen ) {
	triType[2] = (triType[2] + 4) % 10;
	setVertices(P1, P2, X1, Y1, X2, Y2);
      } else {
	triType[0] = 1;
	triType[1] = 0;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(P1, P2, X1, Y1, Q1, Q2);
      }
    } else if ( triType[2] == 7 ) {
      if ( X < P1 ) {
	triType[1] = 0;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(X3, Y3, Q1, Q2, P1, P2);
      }
      else if ( Y > sqrOpen ) {
	triType[2] = (triType[2] + 4) % 10;
	setVertices(P1, P2, X1, Y1, X2, Y2);
      } else {
	triType[0] = 1;
	triType[1] = 0;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(P1, P2, X1, Y1, Q1, Q2);
      }
    } else if ( triType[2] == 8 ) {
      if ( Y > triOpen ) {
	triType[1] = 0;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(X3, Y3, Q1, Q2, P1, P2);
      }
      else if ( X > P1 ) {
	triType[2] = (triType[2] + 4) % 10;
	setVertices(P1, P2, X1, Y1, X2, Y2);
      } else {
	triType[0] = 1;
	triType[1] = 0;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(P1, P2, X1, Y1, Q1, Q2);
      }
    } else {
      if ( Y < triOpen ) {
	triType[1] = 0;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(X3, Y3, Q1, Q2, P1, P2);
      }
      else if ( X < P1 ) {
	triType[2] = (triType[2] + 4) % 10;
	setVertices(P1, P2, X1, Y1, X2, Y2);
      } else {
	triType[0] = 1;
	triType[1] = 0;
	triType[2] = (triType[2] + 5) % 10;
	setVertices(P1, P2, X1, Y1, Q1, Q2);
      }
    }
  }
  
  void
    changelt() {
    if ( (54 - (36 * triType[2])) % 360 == 270 ) {
      if ( X > P1 ) {
	triType[0] = 0;
	setVertices(X1, Y1, P1, P2, X2, Y2);
      } else
	setVertices(X2, Y2, X3, Y3, P1, P2);
    }
    else if ( (54 - (36 * triType[2])) % 360 == 90 ) {
      if ( X < P1 ) {
	triType[0] = 0;
	setVertices(X1, Y1, P1, P2, X2, Y2);
      } else
	setVertices(X2, Y2, X3, Y3, P1, P2);
    }
    else if ( triType[2] < 4 ) {
      if ( Y > circClose ) {
	triType[0] = 0;
	setVertices(X1, Y1, P1, P2, X2, Y2);
      } else
	setVertices(X2, Y2, X3, Y3, P1, P2);
    } 
    else
      if ( Y < circClose ) {
	triType[0] = 0;
	setVertices(X1, Y1, P1, P2, X2, Y2);
      } else 
	setVertices(X2, Y2, X3, Y3, P1, P2);
    triType[2] += 3;
  }
  
  void
    changelT() {
    if ( ( (36 * triType[2]) - 54 ) % 360 == 270 ) {
      if ( X < P1 ) {
	triType[0] = 0;
	triType[2] += 4;
	setVertices(P1, P2, X1, Y1, X3, Y3);
      } else {
	triType[2] += 2;
	setVertices(X3, Y3, X2, Y2, P1, P2);
      }
    }
    else if ( ( (36 * triType[2]) - 54 ) % 360 == 90 ) {
      if ( X > P1 ) {
	triType[0] = 0;
	triType[2] += 4;
	setVertices(P1, P2, X1, Y1, X3, Y3);
      } else {
	triType[2] += 2;
	setVertices(X3, Y3, X2, Y2, P1, P2);
      }
    }
    else if (triType[2] < 6 && triType[2] > 1 ) {
      if ( Y < circOpen ) {
	triType[0] = 0;
	triType[2] += 4;
	setVertices(P1, P2, X1, Y1, X3, Y3);
      } else {
	triType[2] += 2;
	setVertices(X3, Y3, X2, Y2, P1, P2);
      }
    }
    else
      if ( Y > circOpen ) {
	triType[0] = 0;
	triType[2] += 4;
	setVertices(P1, P2, X1, Y1, X3, Y3);
      } else {
	triType[2] += 2;
	setVertices(X3, Y3, X2, Y2, P1, P2);
      }
  }
}
#endif /* PIXEL.HPP */

/* |Close.cpp|: reads in PGM and PPM files, undergoes Chuck Close
 * transformation, outputs PPM file to generate altered image.
 */

/* Phillips, Michael
 * accounts: phillimj
 */

#include <iostream>
#include <sys/stat.h>
#include <cstdlib>
#include <fstream>
#include <string>
#include <math.h>
#include <Dense>
#include <Magick++.h>

using namespace std;
using namespace Eigen;

int
main(int argc, char * argv[])
{
    ifstream infile, picfile;
    char pre[2];
    float width, height, tscale;
    int scale, c, r, k;
    string name;
    int circle, lhor, lvert, lpos, lneg, hhor, hvert, hpos, hneg;
    circle = 1; lhor = 2; lvert = 3; lpos = 4;
    lneg = 5; hhor = 6; hvert = 7; hpos = 8; hneg = 9;

    /* Enter color thresholds */
    int rrange = 40;
    int grange = 40;
    int brange = 40;

    /* Enter minimum data points for regression (>=1) */
    int datamin = 3;

    /* Enter global high-pass thrshold */
    int threshold = 7;

    /* Enter r-squared threshold */
    int r2thresh = 0.1;

    /* Enter required pixel count within given neighborhood */
    int pixelthresh = 5;

    /* Enter neighborhood dimension */
    int nbhd_dim = 5;

    /* Import tile */
    infile.open("../data/tile_triangle1.pgm");
    infile >> pre;
    infile >> width;
    infile >> height;
    infile >> tscale;

    MatrixXi TILE = MatrixXi::Zero((int) height + 1, (int) width + 1);
    for (int i = 1; i <= height; i++)
	for (int j = 1; j <= width; j++) {
	    infile >> k;
	    TILE(i,j) = k;
        }
    infile.close();

    /* Create dictionary of replacement marks and determine areas
     * for weighting */
    int shape = 0;
    MatrixXd ain  = MatrixXd::Zero(9+1,4+1);
    MatrixXd aout = ain;

    /* MARKS */
    int mark[9+1][4+1][60+1][58+1];
    int layer = 1;

    for (int i = 1; i < 9+1; i++)
    for (int j = 1; j < 4+1; j++)
    for (int x = 1; x < 60+1; x++)
    for (int y = 1; y < 58+1; y++)
	mark[i][j][x][y] = 0;

    infile.open("../data/names");
    for (int x = 0; x < 36; x++) {
	getline(infile, name);

	picfile.open(name.c_str());
	picfile >> pre;
	picfile >> c;
	picfile >> r;
	picfile >> scale;
	
	for (int i = 1; i <= r; i++) {
	    for (int j = 1; j <= c; j++) {
		picfile >> k; mark[shape][layer][i][j] = k;
		if (mark[shape][layer][i][j] == 0)
		    aout(shape, layer)++;
		if (mark[shape][layer][i][j] == 255)
		    ain(shape, layer)++;
	    }
	}
	picfile.close();
	
	if (layer == 4) {
	    layer = 1;
	    shape++;
	}
	else { layer++; }
    }
    infile.close();

cout << "Importing original image name and dimensions..." << endl;
    /* Import original image name and dimensions */
    infile.open("list");
    getline (infile, name);

    picfile.open(name.c_str());
    picfile >> pre;
    picfile >> c;
    picfile >> r;
    picfile >> scale;

cout << "Determining expanded matrix dimensions..." << endl;
    /* Determine expanded matrix dimensions */
    int V, H, R, C;
    V = (int)(floor(r / (height/2)));
    H = (int)(ceil(c / width));
    R = (int)((height/2)*(V + 1));
    C = (int)(width * H);

cout << "Initializing color image matrix..." << endl;
    /* Initialize color image matrix */
    MatrixXi IMAGE = MatrixXi::Zero(R+1,C*3+1);

cout << "Importing color image data..." << endl;
    /* Import color image data with zeros padding top, bottom, and right */
    for (int i = 1; i <= r; i++)
	for (int j = 1; j <= c*3; j++) {
	    picfile >> k;
	    IMAGE(i,j) = k;
        }
    picfile.close();

cout << "Importing grayscale image name and dimensions, then data..." << endl;
    /* Import grayscale image name and dimensions, then data */
    getline(infile, name);
    infile.close();

    picfile.open(name.c_str());
    picfile >> pre;
    picfile >> c;
    picfile >> r;
    picfile >> scale;

    MatrixXi GRAY = MatrixXi::Zero(R+1,C+1);
    for (int i = 1; i <= r; i++)
    for (int j = 1; j <= c; j++) {
	picfile >> k;
	GRAY(i,j) = k;
    }
    picfile.close();

cout << "Initializing total high-pass filter matrix..." << endl;
    /* Initialize total high-pass filter matrix */
    int frow = floor(R/2);
    int fcol = floor(C/2);
    MatrixXi hpf = MatrixXi::Zero(frow+1, fcol+1);
cout << "Filter once using bivariate Haar high-pass filters..." << endl;
    /* Filter once using bivariate Haar high-pass filters, summing
     * absolute values for V/H/D */
    for (int i = 1; i <= frow; i++)
    for (int j = 1; j <= fcol; j++) {
	hpf(i,j) = (1/2)*abs(GRAY(2*i-1,2*j-1) - GRAY(2*i-1,2*j) + GRAY(2*i,2*j-1) - GRAY(2*i,2*j)) 
		+ (1/2)*abs(GRAY(2*i-1,2*j-1) + GRAY(2*i-1,2*j) - GRAY(2*i,2*j-1) - GRAY(2*i,2*j)) 
		+ (1/2)*abs(GRAY(2*i-1,2*j-1) - GRAY(2*i-1,2*j) - GRAY(2*i,2*j-1) + GRAY(2*i,2*j));
    }

cout << "Thresholding entire image and removing isolated data points..." << endl;
    /* Threshold entire image and remove isolated data points */
    for (int i = 1; i <= frow; i++)
    for (int j = 1; j <= fcol; j++)
	if (hpf(i,j) <= threshold)
	    hpf(i,j) = 0;

    int pixelcount, rdist, cdist, mxr, mxc, mnr, mnc;
    MatrixXi neighborhood(frow, fcol);
    for (int i = 1; i <= frow; i++)
    for (int j = 1; j <= fcol; j++) {
	if (hpf(i,j) > 0) {
	    pixelcount = -1;
	    mxr = max(1, i - (nbhd_dim-1)/2);
	    mnr = min(frow, i + (nbhd_dim-1)/2);
  	    mxc = max(1, j - (nbhd_dim-1)/2);
	    mnc = min(fcol, j + (nbhd_dim-1)/2);
            rdist = mnr - (mxr-1);
	    cdist = mnc - (mxc-1);
	    neighborhood = hpf.block(mxr, mxc, rdist, cdist);
	    for (int k = 0; k <= rdist; k++)
	        for (int m = 0; m<= cdist; m++)
	            if (neighborhood(k,1) > 0)
		        pixelcount++;
	    if (pixelcount < pixelthresh)
		hpf(i,j) = 0;
 	}
    }

cout << "Analyzing tiles and assigning new marks..." << endl;
    /* Analyze each tile and assign new mark */
    int datacount, orientation, hor, vert, pos, neg;
    hor = 1; vert = 2; pos = 3; neg = 4;
    double slope, intercept, rcoefficient, rsquare, ttl1, ttl2;
    MatrixXi S = MatrixXi::Zero(height + 1, 3*width + 1);
    MatrixXi fblock = MatrixXi::Zero(height/2 + 1, width/2 + 1);
    /* The length of the following array is determined by the height and width of
     * the "tile_triangle1.pgm" file. If that file is no longer being called above
     * to determine "height" and "width", the following numbers MUST change as well.
     */
    
    for (int n = 1; n <= V; n++)
    for (int m = 1; m <= H; m++) {
cout << "Index into image matrix" << endl;
	/* Index into image matrix */
	S.block(1, 1, height, 3*width) = IMAGE.block((n-1)*height/2 + 1, 3*(m-1)*width + 1, height, 3*width);

cout << "Index into filter output matrix" << endl;
	/* Index into filter output matrix */
	fblock.block(1, 1, height/2, width/2) = hpf.block((n-1)*height/4 + 1, (m-1)*width/2 + 1, height/2, width/2);
	datacount = 0;

cout << "Count data points in filter block..." << endl;
	/* Count data points in filter block */
	for (int i = 1; i <= height/2; i++)
	for (int j = 1; j <= width/2; j++) {
	    if (fblock(i,j) > 0)
		datacount++;
	}
	Vector2d XY[871];

	if (datacount <= datamin) {
cout << "No worries! It's a circle!!!" << endl;
	    shape = circle;
	} else {
	    int k = 0;
	    for (int i = 1; i <= height/2; i++)
	    for (int j = 1; j <= width/2; j++) {
		if (fblock(i,j) > 0) {
		    k++;
		    XY[k] = Vector2d(j, height/2 - i);
		}
	    }
 
cout << "Calculating regression line and coefficient..." << endl;
	    /* Calculate regression line and regression coefficient */
	    double x_sum, y_sum, x_avg, y_avg, sumxx, sumxy;
	    x_sum = y_sum = x_avg = y_avg = sumxx = sumxy = 0;
	    double prods[58 * 60 / 4]; // These numbers must change, too

	    for (int i = 1; i <= datacount; i++) {
	        x_sum += XY[i].x();
	        y_sum += XY[i].y();
		sumxx += XY[i].x() * XY[i].x();
		sumxy += XY[i].x() * XY[i].y();
            }
	    x_avg = x_sum / (58 * 60 / 4); // These numbers must change, too
	    y_avg = y_sum / (58 * 60 / 4); // These numbers must change, too
	    slope = (870 * sumxy - x_sum * y_sum) / (870 * sumxx - x_sum * x_sum);
	    intercept = y_avg - slope * x_avg;
	    rcoefficient = sumxy / sumxx;

	    /* Calculate R^2 value */
	    ttl1 = ttl2 = 0;
	    for (int i = 1; i <= datacount; i++)
		ttl1 += XY[i].y();
	    for (int i = 1; i <= datacount; i++)
		ttl2 += pow(XY[i].y() - (ttl1 / datacount), 2);

	    if (ttl2 == 0)
		rsquare = 0;
	    else {
		ttl1 = ttl2 = 0;
		for (int i = 1; i <= datacount; i++)
		    ttl1 += pow(XY[i].y() - intercept - slope * XY[i].x(), 2);
		rsquare = ttl1;
		ttl1 = 0;
		for (int i = 1; i <= datacount; i++)
		    ttl1 += XY[i].y();
		for (int i = 1; i <= datacount; i++)
		    ttl2 += pow(XY[i].y() - ttl1 / datacount, 2);
		rsquare = rsquare / ttl2;
	    }

cout << "Determining orientation based on slope..." << endl;
	    /* Determine orientation based on slope (+, -, hori, vert) */
	    if ( rsquare < r2thresh ) {
	        shape = circle;
	    } else {
		if ( slope <= 0.414 && slope > -2.414 )
		    orientation = hor;
		else if ( slope > 2.414 || slope < -2.414 )
		    orientation = vert;
		else if ( slope <= 2.414 && slope > 0.414 )
		    orientation = pos;
		else
		    orientation = neg;

cout << "Submatrix into pgm file and filter, retaining sign info to determine edge type..." << endl;
	        /* Submatrix into pgm file 'Gray' and filter, retaining sign info to determine edge type */
	        MatrixXi typefilter = GRAY.block((int)((n-1)*height/2 + 1), (int)((m-1)*width + 1), height, width);
	        VectorXd filterlist = VectorXd::Zero(int (width*height/4) + 1);

	        /* Use only the horizontal high-pass for horizontal orientation, otherwise use vertical */
	        if (orientation == hor) {
	            for (int i = 1; i <= height/2; i++)
	            for (int j = 1; j <= width/2; j++)
		        filterlist((int)((i-1)*width/2+j)) = (1/2)*(typefilter(2*i-1, 2*j-1) + typefilter(2*i-1, 2*j)
		                - typefilter(2*i, 2*j-1) - typefilter(2*i, 2*j));
	        } else {
	            for (int i = 1; i <= height/2; i++)
	            for (int j = 1; j <= width/2; j++)
	 	        filterlist((int)((i-1)*width/2+j)) = (1/2)*(typefilter(2*i-1, 2*j-1) - typefilter(2*i-1, 2*j)
			        + typefilter(2*i, 2*j-1) - typefilter(2*i, 2*j));
	        }

	        /* Calculate what percentage of nonzero filter data is negative */
	        int cneg, cnzero, percneg;
	        cneg = cnzero = 0;
	        for (int i = 0; i < width*height/4; i++) {
	            if (filterlist(i) != 0) {
		        cnzero++;
		        if (filterlist(i) < 0)
		            cneg++;
	        }
 	        percneg = cneg / cnzero;

	        /* Assign mark shape; Determine type and orientation */
	        if (percneg > 0.45 && percneg > 0.55)
	            shape = 1 + orientation;
	        else
	            shape = 5 + orientation;
	        }
	    }
        }
 
cout << "---Beginning color-picking algorithm---" << endl;
	/* Begin color-picking algorithm */
	int rout, gout, bout, rin, gin, bin, red, red1, red2, gre, gre1, gre2, blu, blu1, blu2;
	rout = gout = bout = rin = gin = bin = 0;
	double weight1, weight2;

	/* For tiles in an 'even' position (right-pointing triangles) */
	if ((n+m)%2 == 0) {
	    /* For each tile */
	    for (int i = 1; i <= height; i++)
	    for (int j = 1; j <= width; j++) {
		if (mark[shape][2][i][j] == 0) {
		    rout += S(i, 3*j-2);
		    gout += S(i, 3*j-1);
		    bout += S(i, 3*j-0);
		} else if (mark[shape][2][i][j] == 255 ) {
		    rin += S(i, 3*j-2);
		    gin += S(i, 3*j-1);
		    bin += S(i, 3*j-0); 
		}
	    }

	    rout = (int)(rout / aout(shape, 2));
	    gout = (int)(gout / aout(shape, 2));
	    bout = (int)(bout / aout(shape, 2));
	    rin = (int)(rin / ain(shape, 2));
	    gin = (int)(gin / ain(shape, 2));
	    bin = (int)(bin / ain(shape, 2));

cout << "Generate two colors for outer region..." << endl;
	    /* Generate two colors for outer region */
	    red = rout;
	    gre = gout;
	    blu = bout;
	    weight1 = (aout(shape, 1) / aout(shape, 2));
	    weight2 = ((aout(shape, 2) - aout(shape, 1)) / aout(shape, 2));
	    red1 = rand() % (1 + abs(max(0, max(red - rrange,(int)((red - 255 * weight2) / weight1)))
			  - min(255, min(red + rrange, (int)(red / weight2)))));
	    red1 += min(max(0, max(red - rrange, (int)((red - 255 * weight2) / weight1))),
			min(255, min(red + rrange, (int)(red / weight2))));
	    gre1 = rand() % (1 + abs(max(0, max(gre - grange, (int)((gre - 255 * weight2) / weight1)))
			  - min(255, min(gre + grange, (int)(gre / weight2)))));
	    gre1 += min(max(0, max(gre - grange, (int)((gre - 255 * weight2) / weight1))),
			min(255, min(gre + grange, (int)(gre / weight2))));
	    blu1 = rand() % (1 + abs(max(0, max(blu - brange, (int)((blu - 255 * weight2) / weight1)))
			  - min(255, min(blu + brange, (int)(blu / weight2)))));
	    blu1 += min(max(0, max(blu - brange, (int)((blu - 255 * weight2) / weight1))),
			min(255, min(blu + brange, (int)(blu / weight2))));
	    red2 = floor((red-red1*weight1) / weight2);
	    gre2 = floor((gre-gre1*weight1) / weight2);
	    blu2 = floor((blu-blu1*weight1) / weight2);

if (red1 > 255 || red2 > 255 || gre1 > 255 || gre2 > 255 || blu1 > 255 || blu2 > 255) {
cout << (int)((red-255*weight2)/weight1) << endl;
cout << (int)((gre-255*weight2)/weight1) << endl;
cout << (int)((blu-255*weight2)/weight1) << endl;
cout << "HERE1" << endl; 
}
cout << "Setting colors into S" << endl;
	    for (int i = 1; i <= height; i++)
	    for (int j = 1; j <= width; j++) {
		if (mark[shape][1][i][j] == 0) {
		    S(i, 3*j-2) = red1;
		    S(i, 3*j-1) = gre1;
		    S(i, 3*j-0) = blu1;
		} else if (mark[shape][2][i][j] == 0) {
		    S(i, 3*j-2) = red2;
		    S(i, 3*j-1) = gre2;
		    S(i, 3*j-0) = blu2;
		}
	    }

cout << "Generate two colors for inner region..." << endl;
	    /* Generate two colors for inner region */
	    red = rin;
	    gre = gin;
	    blu = bin; 
	    weight1 = ((ain(shape, 2) - ain(shape, 3)) / ain(shape, 2)); 
	    weight2 = (ain(shape, 3) / ain(shape, 2)); 
	    red1 = rand() % (1 + abs(max(0, max(red - rrange,(int)((red - 255 * weight2) / weight1)))
			  - min(255, min(red + rrange, (int)(red / weight2)))));
	    red1 += min(max(0, max(red - rrange, (int)((red - 255 * weight2) / weight1))),
			min(255, min(red + rrange, (int)(red / weight2))));
	    gre1 = rand() % (1 + abs(max(0, max(gre - grange, (int)((gre - 255 * weight2) / weight1)))
			  - min(255, min(gre + grange, (int)(gre / weight2)))));
	    gre1 += min(max(0, max(gre - grange, (int)((gre - 255 * weight2) / weight1))),
			min(255, min(gre + grange, (int)(gre / weight2))));
	    blu1 = rand() % (1 + abs(max(0, max(blu - brange, (int)((blu - 255 * weight2) / weight1)))
			  - min(255, min(blu + brange, (int)(blu / weight2)))));
	    blu1 += min(max(0, max(blu - brange, (int)((blu - 255 * weight2) / weight1))),
			min(255, min(blu + brange, (int)(blu / weight2))));
	    red2 = floor((red - red1*weight1)/weight2);
	    gre2 = floor((gre - gre1*weight1)/weight2);
	    blu2 = floor((blu - blu1*weight1)/weight2);

if (red1 > 255 || red2 > 255 || gre1 > 255 || gre2 > 255 || blu1 > 255 || blu2 > 255) {
cout << (int)((red-255*weight2)/weight1) << endl;
cout << (int)((gre-255*weight2)/weight1) << endl;
cout << (int)((blu-255*weight2)/weight1) << endl;
cout << "HERE2" << endl;
}
cout << "Setting colors into S" << endl;
	    for (int i = 1; i <= height; i++)
	    for (int j = 1; j <= width; j++) {
		if (mark[shape][2][i][j] == 255) {
		    S(i, 3*j-2) = red1;
		    S(i, 3*j-1) = gre1;
		    S(i, 3*j-0) = blu1;
		}		
    	    }		

cout << "Recursively assign average color and generate two colors for innermost layers..." << endl;
	    /* Recursively assign average color and generate two colors for innermost layers */
	    red = max(0, red2);
	    gre = max(0, gre2);
	    blu = max(0, blu2);
	    weight1 = ((ain(shape, 3) - ain(shape, 4)) / ain(shape, 3));
	    weight2 = (ain(shape, 4) / ain(shape, 3)); 
	    red2 = rand() % (1 + abs(max(0, max(red - rrange,(int)((red - 255 * weight2) / weight1)))
			  - min(255, min(red + rrange, (int)(red / weight2)))));
	    red2 += min(max(0, max(red - rrange, (int)((red - 255 * weight2) / weight1))),
			min(255, min(red + rrange, (int)(red / weight2))));
	    gre2 = rand() % (1 + abs(max(0, max(gre - grange, (int)((gre - 255 * weight2) / weight1)))
			  - min(255, min(gre + grange, (int)(gre / weight2)))));
	    gre2 += min(max(0, max(gre - grange, (int)((gre - 255 * weight2) / weight1))),
			min(255, min(gre + grange, (int)(gre / weight2))));
	    blu2 = rand() % (1 + abs(max(0, max(blu - brange, (int)((blu - 255 * weight2) / weight1)))
			  - min(255, min(blu + brange, (int)(blu / weight2)))));
	    blu2 += min(max(0, max(blu - brange, (int)((blu - 255 * weight2) / weight1))),
			min(255, min(blu + brange, (int)(blu / weight2))));
	    red1 = floor((red - red2*weight2)/weight1);
	    gre1 = floor((gre - gre2*weight2)/weight1);
	    blu1 = floor((blu - blu2*weight2)/weight1);

if (red1 > 255 || red2 > 255 || gre1 > 255 || gre2 > 255 || blu1 > 255 || blu2 > 255) {
cout << (int)((red-255*weight2)/weight1) << endl;
cout << (int)((gre-255*weight2)/weight1) << endl;
cout << (int)((blu-255*weight2)/weight1) << endl;
cout << "HERE3" << endl;
}
cout << "Setting colors into S" << endl;
cout << "N: " << n << endl;
cout << "M: " << m << endl;
	    for (int i = 1; i <= height; i++)
	    for (int j = 1; j <= width; j++) {
		if (mark[shape][4][i][j] == 255) {
		    S(i, 3*j-2) = red2;
		    S(i, 3*j-1) = gre2;
		    S(i, 3*j-0) = blu2;
		} else if (mark[shape][3][i][j] == 0) {
		    S(i, 3*j-2) = red1;
		    S(i, 3*j-1) = gre1;
		    S(i, 3*j-0) = blu1;
		}
		if (TILE(i, j) == 0) {
		    IMAGE((n-1)*height/2+i, 3*(m-1)*width+3*j-2) = S(i, 3*j-2);
		    IMAGE((n-1)*height/2+i, 3*(m-1)*width+3*j-1) = S(i, 3*j-1);
		    IMAGE((n-1)*height/2+i, 3*(m-1)*width+3*j-0) = S(i, 3*j-0);
		}
	    }

	} else {
cout << "In else - For tiles in an 'odd' position" << endl;
	    /* For tiles in an 'odd' position (left-pointing triangles) */
	    if (shape > 1) {
	        if (shape % 2 == 0)
	            shape++;
	        else
	            shape--;
	    }

 	    for (int i = 1; i <= height; i++)
	    for (int j = 1; j <= width; j++) {
	        if (mark[shape][2][i][(int)(width+1-j)] == 0) {
		    rout += S(i, 3*j-2);
		    gout += S(i, 3*j-1);
		    bout += S(i, 3*j-0);
	        } else if (mark[shape][2][i][(int)(width+1-j)] == 255) {
	            rin += S(i, 3*j-2);
		    gin += S(i, 3*j-1);
		    bin += S(i, 3*j-0);
	        }
	    }

	    rout = (int)(rout/aout(shape, 2));
	    gout = (int)(gout/aout(shape, 2));
	    bout = (int)(bout/aout(shape, 2));
	    rin = (int)(rin/ain(shape, 2));
	    gin = (int)(gin/ain(shape, 2));
	    bin = (int)(bin/ain(shape, 2));

cout << "Generating two colors for outer region..." << endl;
	    /* Generate two colors for outer region */
	    red = rout;
	    gre = gout;
	    blu = bout;
	    weight1 = (aout(shape, 1) / aout(shape, 2));
	    weight2 = (aout(shape, 2) - aout(shape, 1)) / aout(shape, 2);
	    red1 = rand() % (1 + abs(max(0, max(red - rrange,(int)((red - 255 * weight2) / weight1)))
			  - min(255, min(red + rrange, (int)(red / weight2)))));
	    red1 += min(max(0, max(red - rrange, (int)((red - 255 * weight2) / weight1))),
			min(255, min(red + rrange, (int)(red / weight2))));
	    gre1 = rand() % (1 + abs(max(0, max(gre - grange, (int)((gre - 255 * weight2) / weight1)))
			  - min(255, min(gre + grange, (int)(gre / weight2)))));
	    gre1 += min(max(0, max(gre - grange, (int)((gre - 255 * weight2) / weight1))),
			min(255, min(gre + grange, (int)(gre / weight2))));
	    blu1 = rand() % (1 + abs(max(0, max(blu - brange, (int)((blu - 255 * weight2) / weight1)))
			  - min(255, min(blu + brange, (int)(blu / weight2)))));
	    blu1 += min(max(0, max(blu - brange, (int)((blu - 255 * weight2) / weight1))),
			min(255, min(blu + brange, (int)(blu / weight2))));
	    red2 = floor((red - red1*weight1)/weight2);
	    gre2 = floor((gre - gre1*weight1)/weight2);
	    blu2 = floor((blu - blu1*weight1)/weight2); 

if (red1 > 255 || red2 > 255 || gre1 > 255 || gre2 > 255 || blu1 > 255 || blu2 > 255) {
cout << (int)((red-255*weight2)/weight1) << endl;
cout << (int)((gre-255*weight2)/weight1) << endl;
cout << (int)((blu-255*weight2)/weight1) << endl;
cout << "HERE4" << endl;
}
cout << "Inputting colors into S..." << endl;
	    for (int i = 1; i <= height; i++) 
	    for (int j = 1; j <= width; j++) {
	        if (mark[shape][1][i][j] == 0) {
		    S(i, 3*j-2) = red1;
		    S(i, 3*j-1) = gre1;
		    S(i, 3*j-0) = blu1;
	        } else if (mark[shape][2][i][j] == 0) {
	     	    S(i, 3*j-2) = red2;
		    S(i, 3*j-1) = gre2;
		    S(i, 3*j-0) = blu2;
	        }
	    }

cout << "Generating two colors for inner region..." << endl;
	    /* Generate two colors for inner region */
	    red = rin;
	    gre = gin;
	    blu = bin;
	    weight1 = (ain(shape, 2) - ain(shape, 3)) / ain(shape, 2);
	    weight2 = ain(shape, 3) / ain(shape, 2);
	    red1 = rand() % (1 + abs(max(0, max(red - rrange,(int)((red - 255 * weight2) / weight1)))
			  - min(255, min(red + rrange, (int)(red / weight2)))));
	    red1 += min(max(0, max(red - rrange, (int)((red - 255 * weight2) / weight1))),
			min(255, min(red + rrange, (int)(red / weight2))));
	    gre1 = rand() % (1 + abs(max(0, max(gre - grange, (int)((gre - 255 * weight2) / weight1)))
			  - min(255, min(gre + grange, (int)(gre / weight2)))));
	    gre1 += min(max(0, max(gre - grange, (int)((gre - 255 * weight2) / weight1))),
			min(255, min(gre + grange, (int)(gre / weight2))));
	    blu1 = rand() % (1 + abs(max(0, max(blu - brange, (int)((blu - 255 * weight2) / weight1)))
			  - min(255, min(blu + brange, (int)(blu / weight2)))));
	    blu1 += min(max(0, max(blu - brange, (int)((blu - 255 * weight2) / weight1))),
			min(255, min(blu + brange, (int)(blu / weight2))));
	    red2 = floor((red - red1*weight1)/weight2);
	    gre2 = floor((gre - gre1*weight1)/weight2);
	    blu2 = floor((blu - blu1*weight1)/weight2);

if (red1 > 255 || red2 > 255 || gre1 > 255 || gre2 > 255 || blu1 > 255 || blu2 > 255) {
cout << (int)((red-255*weight1)/weight2) << endl;
cout << (int)((gre-255*weight1)/weight2) << endl;
cout << (int)((blu-255*weight1)/weight2) << endl;
cout << "HERE5" << endl;
}
cout << "Inputting into S..." << endl;
	    for (int i = 1; i <= height; i++)
	    for (int j = 1; j <= width; j++) {
	        if (mark[shape][2][i][j] == 255) {
		    S(i, 3*j-2) = red1;
		    S(i, 3*j-1) = gre1;
		    S(i, 3*j-0) = blu1;
	        }
	    }

cout << "Recursively assign average color and generate two colors for innermost layers..." << endl;
	    /* Recursively assign average color and generate two colors for innermost layers */
	    red = max(0, red2);
	    gre = max(0, gre2);
	    blu = max(0, blu2);
	    weight1 = (ain(shape, 3) - ain(shape, 4)) / ain(shape, 3);
	    weight2 = ain(shape, 4) / ain(shape, 3);
	    red2 = rand() % (1 + abs(max(0, max(red - rrange,(int)((red - 255 * weight2) / weight1)))
			  - min(255, min(red + rrange, (int)(red / weight2)))));
	    red2 += min(max(0, max(red - rrange, (int)((red - 255 * weight2) / weight1))),
			min(255, min(red + rrange, (int)(red / weight2))));
	    gre2 = rand() % (1 + abs(max(0, max(gre - grange, (int)((gre - 255 * weight2) / weight1)))
			  - min(255, min(gre + grange, (int)(gre / weight2)))));
	    gre2 += min(max(0, max(gre - grange, (int)((gre - 255 * weight2) / weight1))),
			min(255, min(gre + grange, (int)(gre / weight2))));
	    blu2 = rand() % (1 + abs(max(0, max(blu - brange, (int)((blu - 255 * weight2) / weight1)))
			  - min(255, min(blu + brange, (int)(blu / weight2)))));
	    blu2 += min(max(0, max(blu - brange, (int)((blu - 255 * weight2) / weight1))),
			min(255, min(blu + brange, (int)(blu / weight2))));
	    red1 = floor((red - red2*weight2) / weight1);
 	    gre1 = floor((gre - gre2*weight2) / weight1);
	    blu1 = floor((blu - blu2*weight2) / weight1);

if (red1 > 255 || red2 > 255 || gre1 > 255 || gre2 > 255 || blu1 > 255 || blu2 > 255) {
cout << (int)((red-255*weight2)/weight1) << endl;
cout << (int)((gre-255*weight2)/weight1) << endl;
cout << (int)((blu-255*weight2)/weight1) << endl;
cout << "HERE6" << endl;
}
cout << "Inputting into S again..." << endl;
	    for (int i = 1; i <= height; i++)
	    for (int j = 1; j <= width; j++) {
	        if (mark[shape][4][i][j] == 255) {
		    S(i, 3*j-2) = red2;
		    S(i, 3*j-1) = gre2;
		    S(i, 3*j-0) = blu2;
	        } else if (mark[shape][3][i][j] == 0) {
		    S(i, 3*j-2) = red1;
		    S(i, 3*j-1) = gre1;
		    S(i, 3*j-0) = blu1;
	        }
	        if (TILE(i, j) == 0) {
		    IMAGE((n-1)*height/2+i, 3*(m)*width-3*(j-1)-2) = S(i, 3*j-2);
		    IMAGE((n-1)*height/2+i, 3*(m)*width-3*(j-1)-1) = S(i, 3*j-1);
		    IMAGE((n-1)*height/2+i, 3*(m)*width-3*(j-1)-0) = S(i, 3*j-0);
	        }
	    }
	}
    }

cout << "Determining dimensions for export..." << endl;
    /* Determine dimensions for export */
    int Rout = R-3*height/2;
    int Cout = C-width;

cout << "Exporting image..." << endl;
    /* Export image */
    ofstream mkfile;
    struct stat buf;
    if (stat("../data/pix/shellOutput.ppm", &buf) != -1)
	remove("../data/pix/shellOutput.ppm");
    mkfile.open ("../data/pix/shellOutput.ppm");
    mkfile << "P3" << endl;
    mkfile << Cout << ' ' << Rout << endl << scale << endl;
    for (int i = 1; i <= Rout; i++) {
	mkfile << endl;
	for (int j = 1; j <= 3*Cout; j++)
	    mkfile << IMAGE(i+height/2, j) << ' ';
    }
    mkfile.close();

cout << "DONE!!!" << endl;
}

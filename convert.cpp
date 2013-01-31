#include <iostream>
#include <sstream>
#include <Magick++.h>

using namespace std;
using namespace Magick;

int main (int argc, char *argv[])
{
	InitializeMagick(*argv);

	Image image;

	image.read(argv[1]);

	// Setting quality to 0 ensures that the formatting is not compressed to binary.
	// Instead, the file is converted to an ASCII formatt.
	// See PNM section: http://www.graphicsmagick.org/formats.html
	// Also, see: http://www.graphicsmagick.org/formats.html#quality
	image.quality(0);
	image.magick( "PPM" );

	Blob blob;
	image.write( &blob );

	stringstream ppmStream;

	ppmStream << (char*) blob.data();

	char pre[2];
	int c, r, scale;
	ppmStream >> pre;
	ppmStream >> c;
	ppmStream >> r;
	ppmStream >> scale;
	
	cout << pre[0] << pre[1] << endl;
	cout << c << ", " << r << ", " << scale << endl;

	return 0;
}

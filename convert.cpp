#include <Magick++.h>

using namespace Magick;

int main (int argc, char *argv[])
{
	InitializeMagick(*argv);

	Image image;

	image.read(argv[1]);
	image.quality(0);
	image.write(argv[2]);

	return 0;
}

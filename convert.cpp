#include "convert.hpp"
#include "logging.hpp"
#include <Magick++.h>
#include <iostream>
#include <sstream>

void
toPixelMap(const std::string& inputImgName, std::ostream& ppmStream, std::ostream& pgmStream)
{
	Magick::Image image;

	try {
		image.read(inputImgName);
	}
	catch( Magick::ErrorFileOpen  &error ) {
		log() << inputImgName << ": image file cannot be opened" << std::endl;
		std::exit(1);
	}
	catch( Magick::ErrorCorruptImage &error ) {
		log() << inputImgName << ": image file corrupt" << std::endl;
		std::exit(1);
	}

	// Setting quality to 0 ensures that the formatting is not compressed to binary.
	// Instead, the file is converted to an ASCII format.
	// See PNM section: http://www.graphicsmagick.org/formats.html
	// Also, see: http://www.graphicsmagick.org/formats.html#quality
	image.quality(0);

	Magick::Blob blob;

	// Writing PPM stream
	image.magick( "PPM" );
	image.write( &blob );
	ppmStream << (char*) blob.data();

	// Writing PGM stream
	image.magick( "PGM" );
	image.write( &blob );
	pgmStream << (char*) blob.data();
}

void
writeImage(const std::stringstream& ppmStream, const std::string& pngImgName)
{
	std::string str = ppmStream.str();
	Magick::Blob blob( (void*) str.c_str(), str.length());

	Magick::Image image( blob );

	image.write(pngImgName);
}

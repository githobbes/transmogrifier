#include "convert.hpp"
#include "logging.hpp"
#include <Magick++.h>
#include <iostream>
#include <sstream>

using transmogrifier::error;

void
transmogrifier::streamToPixelMap(const std::istream& inputStream, std::ostream& ppmStream)
{
	std::stringstream buffer;
	buffer << inputStream.rdbuf();
	std::string str = buffer.str();
	Magick::Blob inputBlob( (void*) str.c_str(), str.length() );

	Magick::Image image ( inputBlob );

	// Setting quality to 0 ensures that the formatting is not compressed to binary.
	// Instead, the file is converted to an ASCII format.
	// See PNM section: http://www.graphicsmagick.org/formats.html
	// Also, see: http://www.graphicsmagick.org/formats.html#quality
	image.quality(0);

	Magick::Blob outputBlob;

	// Writing PPM stream
	image.magick( "PPM" );
	image.write( &outputBlob );
	ppmStream << (char*) outputBlob.data();
}

void
transmogrifier::namedFileToPixelMap(const std::string& inputImgName, std::ostream& ppmStream)
{
	Magick::Image image;

	try {
		image.read(inputImgName);
	}
	catch( Magick::ErrorFileOpen& e) {
		error() << inputImgName << ": image file cannot be opened" << std::endl;
		std::exit(1);
	}
	catch( Magick::ErrorCorruptImage& e ) {
		error() << inputImgName << ": image file corrupt" << std::endl;
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
}

void
transmogrifier::pixelMapToStream(const std::stringstream& ppmStream, std::ostream& outStream, const std::string& outputFormat)
{
	std::string str = ppmStream.str();
	Magick::Blob blob( (void*) str.c_str(), str.length());

	Magick::Image image( blob );
	image.magick( outputFormat );
	try {
		image.write( &blob );
	}
	catch (Magick::ErrorMissingDelegate& e) {
		error() << "Output image format `" << outputFormat << "' is unsupported" << std::endl;
		std::exit(1);
	}

	outStream.write((char*) blob.data(), blob.length());
}

void
transmogrifier::pixelMapToNamedFile(const std::stringstream& ppmStream, const std::string& pngImgName)
{
	std::string str = ppmStream.str();
	Magick::Blob blob( (void*) str.c_str(), str.length());

	Magick::Image image( blob );

	image.write(pngImgName);
}

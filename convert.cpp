#include <iostream>
#include <sstream>
#include <Magick++.h>

void
toPixelMap(const std::string& inputImgName, std::ostream& ppmStream, std::ostream& pgmStream)
{
  Magick::Image image;
  
  image.read(inputImgName);
  
  // Setting quality to 0 ensures that the formatting is not compressed to binary.
  // Instead, the file is converted to an ASCII format.
  // See PNM section: http://www.graphicsmagick.org/formats.html
  // Also, see: http://www.graphicsmagick.org/formats.html#quality
  image.quality(0);

  Blob blob;

  // Writing PPM stream
  image.magick( "PPM" );  
  image.write( &blob );
  ppmStream << (char*) blob.data();

  // Writing PGM stream
  image.magick( "PGM" );
  image.write( &blob );
  pgmStream << (char*) blob.data();
}

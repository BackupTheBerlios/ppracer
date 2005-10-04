/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "image.h"

#include "../base/errors.h"

//Readers
#include "readPNG.h"

//Writers
#include "writePPM.h"


namespace ppogl {
	
Image::Image()
 : data(NULL),
   width(0),
   height(0),
   depth(0)
{
}	

Image::Image(unsigned short width, unsigned short height, unsigned char depth)
{
	PP_REQUIRE(depth==3 || depth==4,"Unsupoorted depth for image: " << depth);
	this->width=width;
	this->height=height;
	this->depth=depth;
	this->data=new unsigned char[width*height*depth];
	return;
}

Image::~Image()
{
	if(data!=NULL) delete [] data;
}

Image*
Image::readFile(const std::string& filename)
{
	int len = filename.length();
	
	// PNG Image?
	if(!strcmp(filename.c_str()+len-3,"png")){
		Image* image=new ReadPNG(filename);
		
		// check if the image contains valid data
		if(image->data!=NULL){
			return image;
		} else {
			delete image;
			PP_WARNING("Unable to load images: " << filename);
			return NULL;
		}
	}else{
		PP_WARNING("Unsuported image format: " << filename);
		return NULL;	
	}
}

bool 
Image::writeToFile(const std::string& filename) const
{
	WritePPM writer(filename,*this);
	
	return true;
}


} //namespace ppogl

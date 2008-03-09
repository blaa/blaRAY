/**********************************************************************
 * blaRAY -- photon mapper/raytracer
 * (C) 2008 by Tomasz bla Fortuna <bla@thera.be>, <bla@af.gliwice.pl>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * See Docs/LICENSE
 *********************/


#include "World/Color.hh"
#include "Graphics/Image.hh"

namespace Graphics {
	Image::Image(const Int Width, const Int Height)
		: Drawable(Width, Height)
	{
		Data = new World::Color[Width * Height];
	}

	Image::~Image()
	{
		delete[] Data;
	}

	void Image::Refresh()
	{
	}

	void Image::Save(const std::string Filename) const
	{
		
	}
}

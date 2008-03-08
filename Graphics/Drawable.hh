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

#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

#include <string.h>
#include "General/Types.hh"
#include "Scene/Color.hh"

namespace Graphics {

	/** \brief Drawable interface */
	class Drawable {
	public:
		virtual ~Drawable() {};
		virtual Int GetWidth() const = 0;
		virtual Int GetHeight() const = 0;
		virtual void PutPixel(Int x, Int y, const Scene::Color &C) = 0;

		virtual void Refresh() = 0;
		virtual void Save(const std::string Filename) const = 0;
	};
};

#endif

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
#include "World/Color.hh"

namespace Graphics {

	/** \brief Abstract drawable interface */
	class Drawable {
	protected:
		/**@{ Drawable size */
		const Int Width, Height;
		/*@}*/

	public:
		/** Interface constructor */
		inline Drawable(Int Width, Int Height)
			: Width(Width), Height(Height) {}

		/** Compulsory virtual destructor */
		virtual ~Drawable() {};

		/** Return width */
		virtual Int GetWidth() const {
			return this->Width;
		}

		/** Return height */
		virtual Int GetHeight() const {
			return this->Height;
		}

		/** Puts pixel of specified color at specified location */
		virtual void PutPixel(Int x, Int y, const World::Color &C) = 0;

		/** Refreshes drawable (stores for images */
		virtual void Refresh() = 0;

		/** Saves image/screen/whatever to the file */
		virtual void Save(const std::string Filename) const = 0;
	};
};

#endif

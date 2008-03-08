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

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdexcept>
#include "General/Types.hh"
#include "General/Debug.hh"
#include "Graphics/Drawable.hh"
#include "Scene/Color.hh"

namespace Graphics {
	/**
	 * \brief Holds image data with basic operations.
	 */
	class Image : public Drawable {
	private:
		const Int Width, Height;
		Scene::Color *Data;

	protected:

		inline const Scene::Color &Get(Int X, Int Y, const Scene::Color &C) const
		{
			return Data[Y * Width + X];
		}

	public:
		Image(const Int Width, const Int Height);
		virtual ~Image();

		inline void PutPixel(Int X, Int Y, const Scene::Color &C)
		{
			if (DEBUG) {
				if (X > Width || X < 0 || Y > Height || Y < 0)
					throw std::invalid_argument(
						"X or Y beyond range");
			}
			Data[Y * Width + X] = C;
		}

		virtual Int GetWidth() const;
		virtual Int GetHeight() const;

		virtual void Save(const std::string Filename) const;
		virtual void Refresh();
	};
};

#endif

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

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Graphics/Drawable.hh"

namespace Render {
	/** \brief Renderer interface */
	class Renderer {
	public:
		/** Compulsory virtual destructor */
		virtual ~Renderer() {}

		/** Function of all renderers */
		virtual void Render(Graphics::Drawable &Img) = 0;
	};
}



#endif

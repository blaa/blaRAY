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

#ifndef _SCREEN_H_
#define _SCREEN_H_

/* 
 * Since all it's identifiers start with SDL_
 * Don't place it in namespace
 */
#include <SDL/SDL.h>
#include "Graphics/Drawable.hh"
#include "Scene/Color.hh"


/**
 * \brief
 *	Input/output functions 

 * Provides classes for loading .png/.jpg files,
 * for storing such files and for displaying rendered
 * image on the screen.
 */
namespace Graphics {
	/** 
	 * \brief Abstracts a window on which we can directly 
	 * display rendered pixels. Might be platform-dependent.
	 */
	class Screen : public Drawable {
	protected:
		const Int Width, Height;

		struct SDL {
			SDL_Surface *S;
			SDL_Thread *RefreshThread;
		} SDL;
		static int Thread(void *Data);


	public:
		Screen(const Int Width, const Int Height);
		virtual ~Screen();
		virtual void PutPixel(Int x, Int y, const Scene::Color &C);
		virtual Int GetWidth() const;
		virtual Int GetHeight() const;
		virtual void Refresh();
		virtual void Save(const std::string Filename) const;
		void EventWait() const;
	};
};

#endif

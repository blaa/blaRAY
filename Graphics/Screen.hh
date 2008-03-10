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
#include "World/Color.hh"


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

		/** Holds SDL information */
		struct SDL {
			/** Surface representing screen */
			SDL_Surface *S;

			/** SDL Refresh thread data */
			SDL_Thread *RefreshThread;
		};
		SDL SDL; /**< SDL Information */

		/** Internal screen thread for refreshing screen
		 * \param Data	struct SDL 
		 */
		static int Thread(void *Data);

		/** Private copy-constructor */
		Screen(const Screen &G);

		/** Private operator= */
		void operator=(const Screen &G) const;
	public:
		/** Construct screen */
		Screen(const Int Width, const Int Height);

		virtual ~Screen();
		virtual void PutPixel(Int x, Int y, const World::Color &C);
		virtual void Refresh();
		virtual void Save(const std::string Filename) const;
		void EventWait() const;
	};
};

#endif

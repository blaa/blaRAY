/**********************************************************************
 * blaRAY -- photon mapper/raytracer
 * (C) 2008 by Tomasz bla Fortuna <bla@thera.be>, <bla@af.gliwice.pl>
 *
 * This program is free software: you can redistribute it and/or modify
d*sa  under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * See Docs/LICENSE
 *********************/

#include <stdexcept>
#include <string>

#include "Graphics/Screen.hh"

namespace Graphics {
	Screen::Screen(const Int Width, const Int Height)
		: Drawable(Width, Height)
	{
		if (VALGRIND) return;

		if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE) != 0) {
			throw std::runtime_error("Unable to open display");
		}
		this->SDL.S = SDL_SetVideoMode(Width, Height, 32, SDL_SWSURFACE);
		if (SDL.S == NULL) {
			std::string error("Unable to set video mode ");
			error += SDL_GetError();
			SDL_Quit();
			throw std::runtime_error(error);
		}

		/* Create refresh thread */
		SDL.RefreshThread = SDL_CreateThread(&Screen::Thread, &this->SDL);
	}

	Screen::~Screen()
	{
		if (VALGRIND) return;
		SDL_KillThread(SDL.RefreshThread);
		SDL_FreeSurface(this->SDL.S);
		SDL_Quit();
	}

	void Screen::PutPixel(Int x, Int y, const World::Color &C)
	{
		if (VALGRIND) return;

		Char R = static_cast<char>(C[0] * 255);
		Char G = static_cast<char>(C[1] * 255);
		Char B = static_cast<char>(C[2] * 255);
		Uint32 Pixel = SDL_MapRGB(SDL.S->format, R, G, B);



		Uint32 *Bits = ((Uint32 *)SDL.S->pixels) + y*Width + x;
		*(Uint32*)Bits = Pixel;
		if (x % 15 == 0 && y == 0) {
  			Refresh();
		}

	}

	int Screen::Thread(void *Data)
	{
		struct SDL *SDL = static_cast<struct SDL*>(Data);
		SDL_Event E;
		for (;;) {
			SDL_WaitEvent(NULL);
			while (SDL_PollEvent(&E)) {
				if (E.type == SDL_USEREVENT)
					break;
			}
			SDL_UpdateRect(SDL->S, 0, 0, 0, 0);
		}
		return 0;
	}

	void Screen::Refresh()
	{
		if (VALGRIND) return;
		static SDL_Event A = { SDL_USEREVENT };
		SDL_PushEvent(&A);
	}


	void Screen::Save(const std::string Filename) const
	{
		if (VALGRIND) return;
		SDL_SaveBMP(SDL.S, Filename.c_str());
	}


	void Screen::EventWait() const
	{
		if (VALGRIND) return;

		SDL_Event Event;
		/**
		 * \bug Check if such loop isn't CPU intensive
		 */
		while (1) {
			if (SDL_PollEvent(&Event)) {
				switch (Event.type) {
				case SDL_KEYUP:
				case SDL_KEYDOWN:
				case SDL_QUIT:
					return;
				}
			}
		}
	}
}

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

#ifndef _DEBUG_H_
#define _DEBUG_H_

#define DEBUG
// #define VALGRIND

#ifdef DEBUG
#	undef DEBUG
#	define DEBUG 1
#else
#	define DEBUG 0
#endif

/* For debugging memory usage */
#ifdef VALGRIND
#	undef VALGRIND
#	define VALGRIND 1
#else
#	define VALGRIND 0
#endif


#endif

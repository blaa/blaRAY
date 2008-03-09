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

#ifndef _ABS_H_
#define _ABS_H_

namespace Math {
	/** Return absolute value of arg */
	template<typename T>
	inline T Abs(T arg) {
		if (arg < 0)
			return -arg;
		return arg;
	}
}

#endif

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

#include <iostream>

#include "General/Debug.hh"
#include "Scene/Color.hh"

namespace Scene {

	std::ostream &operator<<(std::ostream &os, const Color &C)
	{
		os << "[Color " << C.Dump();
		os << "]";
		return os;
	}


	Color::Color(Double r, Double g, Double b)
	{
		if (DEBUG)
			if ( r<0.0 || g<0.0 || b<0.0 || r>1.0 || g>1.0 | b > 1.0) {
				std::cout << "Assert: Color value out of range" << std::endl;
				exit(-1);
			}
		this->D[R] = r;
		this->D[G] = g;
		this->D[B] = b;
	}
}

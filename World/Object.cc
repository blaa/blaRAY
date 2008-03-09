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
#include <string>
#include <sstream>

#include "Math/Matrix.hh"
#include "Math/Vector.hh"
#include "World/Material.hh"
#include "World/Object.hh"



namespace World {
	Object::~Object()
	{
	}

	std::string Object::Dump() const
	{
		std::stringstream s;
		s << "[Object Mat="
		  << M
		  << "]";
		return s.str();
	}

	std::ostream &operator<<(std::ostream &os, const Object &O)
	{
		os << O.Dump();
		return os;
	}

};

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


#include "World/Light.hh"

namespace World {

	std::ostream &operator<<(std::ostream &os, const Light &L)
	{
		os << L.Dump();
		return os;
	}


	std::string PointLight::Dump() const
	{
		std::stringstream s;
		s << "[PointLight Pos="
		  << Position
		  << " Color="
		  << C
		  << "]";
		return s.str();
	}

	std::string AmbientLight::Dump() const
	{
		std::stringstream s;
		s << "[AmbientLight Color="
		  << C << "]";
		return s.str();
	}

}

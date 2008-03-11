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

#include "World/Color.hh"
#include "World/Texture.hh"

namespace World {

	/*** Debug ***/
	std::string Texture::Dump() const
	{
		std::stringstream s;
		s << "[Tex U=" << SizeU
		  << " V=" << SizeV << " "
		  << (Tiled ? "Tiled" : "Untiled")
		  << "]";
		return s.str();
	}

	std::ostream &operator<<(std::ostream &os, const Texture &T)
	{
		os << T.Dump();
		return os;
	}

	namespace TexLib {

		std::string Plain::Dump() const
		{
			std::stringstream s;
			s << "[Plain Color="
			  << this->C
			  << " " << Texture::Dump()
			  << "]";
			return s.str();
		}

		std::string Checked::Dump() const
		{
			return std::string("[Checked ")
				+Texture::Dump()
				+ std::string("]");
		}
		const Texture &Red()
		{
			static const Plain Tex(ColLib::Red());
			return Tex;
		}

		const Texture &Green()
		{
			static const Plain T(ColLib::Green());
			return T;
		}

		const Texture &Blue()
		{
			static const Plain T(ColLib::Blue());
			return T;
		}

		const Texture &White()
		{
			static const Plain T(ColLib::White());
			return T;
		}

		const Texture &Black()
		{
			static const Plain T(ColLib::Black());
			return T;
		}

		const Texture &Gray()
		{
			static const Plain T(ColLib::Gray());
			return T;
		}
       }

};

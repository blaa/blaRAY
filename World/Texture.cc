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
			return std::string("[Plain ")
				+Texture::Dump()
				+ std::string("]");
		}

		std::string Checked::Dump() const
		{
			return std::string("[Checked ")
				+Texture::Dump()
				+ std::string("]");
		}

		/* Factory functions */
		Texture *NewPlain(const Color &C)
		{
			return new Plain(C);
		}

		const Texture &Red()
		{
			static Plain T(ColLib::Red());
			return T;
		}

		const Texture &Green()
		{
			static Plain T(ColLib::Green());
			return T;
		}

		const Texture &Blue()
		{
			static Plain T(ColLib::Red());
			return T;
		}

		const Texture &White()
		{
			static Plain T(ColLib::White());
			return T;
		}

		const Texture &Black()
		{
			static Plain T(ColLib::Black());
			return T;
		}

		const Texture &Gray()
		{
			static Plain T(ColLib::Gray());
			return T;
		}


		Texture *NewChecked(const Color &A, const Color &B,
				    const Double SizeU,
				    const Double SizeV,
				    const Bool Tiled)
		{
			return new Checked(A, B, SizeU, SizeV, Tiled);
		}
	}

};

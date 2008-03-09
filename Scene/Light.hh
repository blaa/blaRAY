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

#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <iostream>
#include <string>

#include "Math/Vector.hh"
#include "Scene/Color.hh"

namespace Scene {

	/**
	 * \brief Light interface.
	 *
	 * Class has to abstract access to any light source -
	 * point, spherical, plane used both in raytracer and
	 * photon mapper.
	 */
	class Light {
	protected:
		const Color C;

		/** Debug function */
		virtual std::string Dump() const = 0;
	public:
		Light(const Color &C) : C(C)
		{
		}

		virtual ~Light() {}
		inline const Color &GetColor() const {
			return C;
		}

		/** Debug function */
		friend std::ostream &operator<<(std::ostream &os, const Light &L);
	};

	class PointLight : public Light {
	protected:
		const Math::Vector Position;

		virtual std::string Dump() const;
	public:
		PointLight(const Math::Vector &Position,
			   const Color &C = ColLib::White())
			: Light(C), Position(Position)
		{
		}

		inline const Math::Vector &GetPosition() const {
			return Position;
		}
	};

	class AmbientLight : public Light {
	protected:

		virtual std::string Dump() const;
	public:
		AmbientLight(const Color &C = ColLib::Gray())
			: Light(C)
		{
		}
	};

}

#endif

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

#ifndef _PLANE_H_
#define _PLANE_H_

#include <iostream>
#include <string>
#include <sstream>

#include "Math/Vector.hh"
#include "Render/Ray.hh"
#include "World/Object.hh"

namespace World {
	/**
	 * \brief
	 *	Object abstracting infinite plane.
	 *
	 * Plane is described by it's normal and
	 * a distance along this normal from point (0,0,0).
	 */
	class Plane : public Object {
	protected:
		const Math::Vector Normal;
		const Double Distance;

		virtual std::string Dump() const;
	public:
		Plane(const Math::Vector &Normal,
		      Double Distance,
		      const Material &M = MatLib::Red(),
		      e_t(bool) Visible = true)
			: Object(M, Visible),
			  Normal(Normal), Distance(Distance)
		{
		}

		virtual Bool Collide(const Render::Ray &R, Double &RayPos) const;
		virtual Math::Vector NormalAt(const Math::Vector &Point) const;

		/** \bug Won't work on not horizontal plane. */
		virtual Math::Point UVAt(const Math::Vector &Point) const;
	};
}

#endif

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

#ifndef _SPHERE_H_
#define _SPHERE_H_

#include <iostream>
#include <string>

#include "World/Object.hh"

namespace World {
	/** \brief
	 *	Mathematical sphere (ball rather)
	 *
	 * Implements collisions, UV coordinates and normal
	 * calculations for spheres.
	 */
	class Sphere : public Object {
	protected:
		/** Sphere center */
		Math::Vector Center;

		/** Sphere radius */
		Double Radius;

       		virtual std::string Dump() const;
	public:
		/** Construct sphere object */
		Sphere(const Math::Vector &Center,
		       Double Radius,
		       const Material &M = MatLib::Red(), 
		       Bool Visible = true)
			: Object(M, Visible), 
			  Center(Center), Radius(Radius)
		{
		}

		virtual Bool Collide(const Render::Ray &R, Double &RayPos) const;
		virtual Math::Vector NormalAt(const Math::Vector &Point) const;
		virtual Math::Point UVAt(const Math::Vector &Point) const;

	};
};

#endif

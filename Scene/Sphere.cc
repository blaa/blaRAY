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
#include <sstream>
#include <string>
#include <cmath>

#include "Math/Constants.hh"
#include "Math/Vector.hh"
#include "Scene/Scene.hh"
#include "Scene/Sphere.hh"

namespace Scene {
	 
	Bool Sphere::Collide(const Render::Ray &R, Double &RayPos) const
	{
		/** Ray - Sphere collision 
		 * R - radius
		 * C[a,b,c] - sphere center
		 * P[x,y,z] - sphere point
		 *
		 * |P - C| = R
		 * Rs + t*Rd = P
		 * | Rs+t*Rd - C | = R
		 */
		const Math::Vector v = R.Start() - this->Center;

		const Double Denominator = 2.0 * R.Direction().SquareLength();
		const Double a = 2.0 * v.Dot( R.Direction() );
		const Double Delta =
			(a * a) - 
			(2 * Denominator) *
			(v.SquareLength() - (Radius * Radius));
		if (Delta > 0.0) {
			const Double b = std::sqrt(Delta);
			const Double First = (-a - b) / Denominator;
			const Double Second = (-a + b) / Denominator;
			/* Choose nearest collision bigger than NearestCollision */
			if (First < Second && First > NearestCollision)
				RayPos = First;
			else {
				if (Second > NearestCollision)
					RayPos = Second;
				else {
					if (First > NearestCollision) 
						RayPos = First;
					else return false;
				}
			}
			return true;
			
		} 
		else return false;
	}

	Math::Vector Sphere::NormalAt(const Math::Vector &Point) const
	{
		return (Point - this->Center).Normalize();
	}

	Math::Point Sphere::UVAt(const Math::Vector &Point) const
	{
		const Double x = Point[0], y = Point[1];
		Math::Vector w = Point - Center;
		Double V = std::acos(y / Radius) / Math::PI;
		Double H = std::acos(x /
				     (Radius * 
				      std::sin(Math::PI * V)));
		Double U;
		if (y > 0.0)
			U = H / 2 / Math::PI;
		else
			U = (Math::PI + H) / 2 / Math::PI;
	
		return Math::Point(U, V);
	}


	std::string Sphere::Dump() const
	{
		std::stringstream s;
		s << "[Sphere Center="
		  << this->Center
		  << " Radius=" 
		  << this->Radius
		  << " Mat="
		  << M
		  << "]";
		return s.str();
	}
};

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

#include <cmath>

#include "General/Types.hh"
#include "Math/Vector.hh"
#include "Render/Ray.hh"

namespace Render {
	Ray Ray::RayFromPoints(const Math::Vector &Source,
			       const Math::Vector &Destination)
	{
		return Ray(Source, (Destination - Source).Normalize());
	}


	Ray Ray::Reflect(const Math::Vector &Normal,
			 const Math::Vector &Point) const
	{
		/**
		 * tmp = 2 * (Normal . Direction)
		 * New Direction = ||Direction - (Normal * tmp1)||
		 */
		const Math::Vector tmp = Normal * (2 * Normal.Dot(this->D));
		return Ray(Point, this->D - tmp);
	}

	Ray Ray::Refract(const Math::Vector &Normal,
			 const Math::Vector &Point,
			 Double FromN, Double IntoN) const
	{
		const Double n = FromN / IntoN;
		const Double c1 = - Normal.Dot(this->D);
		const Double c2 = std::sqrt(1.0 - n*n * (1.0 - c1*c1) );
		const Double c3 = n * c1 - c2;
		const Math::Vector Dir = (this->D * n) + (Normal * c3);
		return Ray(Point, Dir);
	}

	std::ostream &operator<<(std::ostream &os, const Ray &R)
	{
		os << "[Ray Start="
		   << R.Start()
		   << " Dir="
		   << R.Direction()
		   << "]";
		return os;
	}

}

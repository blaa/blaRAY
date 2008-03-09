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

#ifndef _RAY_H_
#define _RAY_H_

#include <iostream>

#include "Math/Matrix.hh"
#include "Math/Transform.hh"
#include "Math/Vector.hh"

#include "World/Color.hh"

/**
 * \brief
 *	Classes used for rendering
 *
 */
namespace Render {
	/**
	 * \brief
	 *	Contains ray data + basic operations
	 *
	 * Class is supposed to abstract a light ray shot from an eye or from
	 * the light into the scene. Can calculate reflected and refracted rays
	 * given normal, and collision point.
	 */
	class Ray {
	private:
		/** Ray start vector */
		Math::Vector S;

		/** Ray direction vector */
		Math::Vector D;

		/** Current ray color */
		/*		World::Color C; */

	public:
		/** Create new ray
		 * \param S	Ray start vector
		 * \param D	Ray direction vector
		 */
		Ray(const Math::Vector &S, const Math::Vector &D) : S(S), D(D)
		{
		}

		/** Create a ray starting at 'Start' point
		 * and directed into 'Destination' */
		static Ray RayFromPoints(const Math::Vector &Start,
					 const Math::Vector &Destination);

		/** Create reflected ray.
		 * \param Normal	normal at collision point
		 * \param Point		Point of ray collision
		 */
		Ray Reflect(const Math::Vector &Normal,
			    const Math::Vector &Point) const;

		/** Create refracted ray.
		 * \param Normal	normal at collision point
		 * \param Point		Point of ray collision
		 * \param FromN		N coeff of ray environment.
		 * \param IntoN		N coeff of refracted ray environment.
		 */
		Ray Refract(const Math::Vector &Normal,
			    const Math::Vector &Point,
			    Double FromN, Double IntoN) const;

		/** Start vector accessor */
		inline const Math::Vector &Start() const
		{
			return S;
		}

		/** Direction vector accessor */
		inline const Math::Vector &Direction() const
		{
			return D;
		}

		Math::Vector GetPoint(Double Loc) const
		{
			return this->S + (this->D * Loc);
		}

		friend std::ostream &operator<<(std::ostream &os, const Ray &R);
	};

};

#endif

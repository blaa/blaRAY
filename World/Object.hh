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

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <iostream>
#include <string>

#include "Math/Matrix.hh"
#include "Math/Vector.hh"
#include "Render/Ray.hh"

#include "World/Material.hh"

namespace World {

	/**
	 * \brief
	 *	Basic object data and interface
	 *
	 * All classes which define scene renderable objects
	 * derive from this one.
	 *
	 * It enforces interface which allows scene to check
	 * collisions with objects and to store them in an octree.
	 *
	 * Furthermore it provides functions to query object
	 * for it's color at given intersection coordinates.
	 */
	class Object {
	protected:
		/** Object material */
		const Material &M;

		/** Object visibility */
		Bool Visible;

		/** Debug function */
		virtual std::string Dump() const;
	public:
		/** Initializes object */
		Object(const Material &M,
		       Bool Visible = true)
			: M(M), Visible(Visible)
		{
		}

		/** Virtual destructor */
		virtual ~Object();

		/** Debug function */
		friend std::ostream &operator<<(std::ostream &os,
						const Object &O);

		/** Find a collision point with a ray */
		virtual Bool Collide(const Render::Ray &R,
				     Double &RayPos) const = 0;

		/** Find a normal to the object at it's surface */
		virtual Math::Vector NormalAt(
			const Math::Vector &Point) const = 0;

		/** Find object UV coordinates at specified surface location */
		virtual Math::Point UVAt(const Math::Vector &Point) const = 0;

		/** Get color of specified material filter at given object point */
		inline const Color ColorAt(const Math::Vector &Point,
					   const Material::Filter F) const {
			return this->M.GetColor(F, UVAt(Point));
		}

		/** Get a property of object material */
		inline Double GetProperty(Material::Property P) const {
			return this->M.GetProperty(P);
		}
	};
};

#endif

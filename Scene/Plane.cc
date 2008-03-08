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

#include "Scene/Scene.hh"
#include "Scene/Plane.hh"

namespace Scene {


	Bool Plane::Collide(const Render::Ray &R, Double &RayPos) const
	{
/* @ - Vector.Dot()
	      let t =
                (-. (p.normal @ ray_start) +. p.distance) /.
		  (p.normal @ ray_dir)
	      in
                if t > 0.0000001 && t < best_t then (
		  let opacity' = 1.0 -.
		    (Material.get_property
		       ~material:(get_object_material obj)
		       ~property:`TRANSPARENCY) in
		  (t, obj, opacity' +. opacity)
                )else (
		  (best_t, best_obj, opacity)
                )
*/	/****************************************************/

		Double t = (- this->Normal.Dot(R.Start()) + this->Distance)
			/ this->Normal.Dot(R.Direction());

		if (t > NearestCollision)
		{
			RayPos = t;
			return true;
		}
		return false;
	}

	Math::Vector Plane::NormalAt(const Math::Vector &Point) const
	{
		return this->Normal;
	}

	Math::Point Plane::UVAt(const Math::Vector &Point) const
	{
		return Math::Point(Point[0], Point[2]);
	}

	std::string Plane::Dump() const
	{
		std::stringstream s;
		s << "[Plane Normal="
		  << this->Normal
		  << " Dist="
		  << this->Distance
		  << " Mat="
		  << M
		  << "]";
		return s.str();
	}
}

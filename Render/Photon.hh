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

#ifndef _PHOTON_H_
#define _PHOTON_H_

#include "Math/Vector.hh"
#include "Scene/Color.hh"

namespace Render {

	/**
	 * \brief
	 * 	Photon Class
	 *
	 * Class abstracts a photon illuminating some 
	 * part of the scene. It's created in a process
	 * of tracing rays shot from a light source 
	 * and stored in a structure suitable for 
	 * Nearest Neighboor algorithm (kd-tree).
	 *
	 */
	class Photon {
	protected:
		/** Photon position */
		Math::Vector Position;

		/** Photon color; wonder if it will be used */
		Scene::Color Color;

		/** Needed for luminance calculations */
		Double Intensity;
	public:
		/** Initialize photon */
		Photon(const Math::Vector &Position, const Scene::Color &Color)
			: Position(Position), Color(Color) {}


		/** Photon position accessor */
		inline const Math::Vector &GetPosition()
		{
			return Position;
		}

		/** Photon color accessor */
		inline const Scene::Color &GetColor()
		{
			return Color;
		}

		/** Photon intensity accessor */
		inline Double GetIntensity()
		{
			return Intensity;
		}
	};
}

#endif

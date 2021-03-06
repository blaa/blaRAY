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

#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include <vector>

#include "General/Types.hh"
#include "Render/Renderer.hh"

#include "Math/Matrix.hh"
#include "Math/Transform.hh"
#include "Math/Vector.hh"

#include "World/Scene.hh"
#include "World/Camera.hh"

#include "Graphics/Image.hh"
#include "Graphics/Screen.hh"

/**
 * \brief
 *	Raytracers, photon mappers and helper classes (like Ray)
 *
 * Namespace constains all classes used for rendering. This includes
 * Ray abstraction, classical raytracer, photon mapper and other
 * helper classes.
 *
 * \bug Specular color should be calculated otherwise. Check it.
 */
namespace Render {

	/**
	 * \brief
	 *	Basic raytracer.
	 *
	 */
	class Raytracer : public Renderer {
		/** Scene to be rendered */
		const World::Scene &Scene;

		/** Anti-aliasing */
		const Bool Antialiasing;

		/** Antialiasing setting:
		 *  number of pixels creating one picture-pixel */
		static const Int AASize;

		/** Max depth to recur during rendering */
		const Int MaxDepth;

		/**@{ Statistics */
		Int ShadowRays;
		Int ReflectedRays;
		Int RefractedRays;
		/*@}*/

		/**
		 * Stack of refractive indices of materials we have entered.
		 */
		std::vector<Double> RefractiveStack;

		/**
		 * Check all lights positions. Check if we are shadowed
		 * from the light or no. Calculate diffuse and specular
		 * coefficients.
		 *
		 * \param ColPoint	Collision point
		 * \param Normal	Normal at collision point
		 * \param Reflect	Reflect direction vector
		 * \param Diffuse	Object relevant parameters
		 * \param Specular	Object relevant parameters
		 */
		inline void TraceLights(
			const Math::Vector &ColPoint,
			const Math::Vector &Normal,
			const Ray &Reflect,
			World::Color &Diffuse,
			World::Color &Specular);


		/**
		 * Trace a ray. Check collisions, create recursively
		 * shadow rays, reflected rays and refracted rays.
		 *
		 * If we won't hit anything we must place a background color
		 * on the resulting image, but this color shouldn't be added
		 * into rays calculations. Therefore we mark if we hit anything.
		 *
		 * \return true if ray hit object, false if it was directed to the
		 * background.
		 */
		Bool Trace(const Ray &R,
			   World::Color &C,
			   const Int Depth,
			   const Double CurIdx);

	public:
		/** Initialize renderer
		 * \param Scene   scene to be rendered
		 * \param Antialiasing	Is antialiasing enabled?
		 * \param MaxDepth	How much should be recurr
		 */
		Raytracer(const World::Scene &Scene,
			  const Bool Antialiasing = true,
			  const Int MaxDepth = 5);

		/** Renders scene into Image buffer
		 * \param Img	Drawable object (Screen or Image)
		 */
		void Render(Graphics::Drawable &Img);

	};
};

#endif

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

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <cmath>

#include "General/Types.hh"
#include "Math/Vector.hh"
#include "Render/Ray.hh"

namespace World {

	/**
	 * \brief
	 *	Camera data, algorithms and functions used for creating
	 *	camera rays.
	 *
	 * Used for creating rays shooting from camera. Describes
	 * camera using Top, Direction, Position vectors and Field of View.
	 */
	class Camera {
	protected:
		/** Camera Field of View in radians */
		Double FOV;

		/** Position at which camera is located */
		Math::Vector Pos;

		/** Direction the camera is pointing. */
		Math::Vector Dir;

		/** Vector defining 'top' direction of camera;
		 *  can be calculated automatically.
		 */
		Math::Vector Top;

	public:
		/** \brief Generates rays shooting from camera */
		class View {
			friend class Camera;

		protected:
			/** Camera position */
			const Math::Vector &Pos;

			/** Camera direction */
			const Math::Vector &Dir;

			/** Indicates distance between two nearest
			 * horizontal pixels of camera */
			const Math::Vector XVect;

			/** Indicates distance between two nearest
			 * vertical camera pixels */
			const Math::Vector YVect;

			/**@{ Half of the screen resolution stored as double type */
			Double XResHalf, YResHalf;
			/*@}*/

			/** Initialize View */
			View(Int XRes, Int YRes,
			     const Math::Vector &Pos, const Math::Vector &Dir,
			     const Math::Vector XVect, const Math::Vector YVect)
				: Pos(Pos), Dir(Dir), XVect(XVect), YVect(YVect),
				  XResHalf(XRes/2.0), YResHalf(YRes/2.0)
			{
			}
		public:
			/** Shoots a ray from the camera which
			 * passes by (x,y) point of camera screen. */
			Render::Ray At(Int x, Int y) const;
		};

		/** Utility function to convert degrees into radians */
		static inline Double DegreeToFOV(Double Degree)
		{
			return (double)Degree / 360.0 * 2 * M_PI;
		}

		/** Initializes camera with default values
		 *
		 * Function can calculate the Top vector so that camera
		 * "horizon line" is parallel to scene horizon.
		 * \param Pos  Camera position vector
		 * \param Dir  Direction of the camera view
		 * \param FOV  Camera "Field of View" in radians
		 * \param AutoTop should top vector be automatically calculated?
		 * \param Top if AutoTop = false, we use this vector
		 */
		Camera(const Math::Vector &Pos = Math::Vector(0.0, 1.0, -1.0),
		       const Math::Vector &Dir = Math::Vector(0.0, 0.0, 1.0),
		       Double FOV = DegreeToFOV(45.0),
		       Bool AutoTop = true,
		       const Math::Vector &Top = Math::Vector(0.0, 1.0, 0.0));

		/** Performs few vector calculations and returns object
		 * which is then used to create rays */
		View CreateView(Int XRes, Int YRes) const;

		/** Pretty-printer */
		friend std::ostream &operator<<(std::ostream &os, const Camera &C);
	};
};

#endif

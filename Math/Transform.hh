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

#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Matrix.hh"

namespace Math {

	/**
	 * \brief
	 *	Defining matrices of any 3D transformations like scaling,
	 *	rotation and translation.
	 *
	 * Transformations are done by multiplying a vector by a
	 * transformation matrix created by this object. They can
	 * be easily combined by their multiplication.
	 *
	 */
	class Transform : public Matrix {
	public:
		/** Names of axes for Transform() function */
		enum Direction { ROT_X, ROT_Y, ROT_Z };

		/** Create identity transformation */
		Transform();

		/** Create transformation from existing matrix */
		Transform(const Matrix &M);

		/** Translation by a vector [x,y,z]
		 * \see Translate */
		Transform(Double x, Double y, Double z);

		/** Rotation around specified axis */
		Transform(Direction D, Double Angle);

		void RotateX(Double Angle); /**< Rotation around X axis */
		void RotateY(Double Angle); /**< Rotation around Y axis */
		void RotateZ(Double Angle); /**< Rotation around Z axis */

		/** Translation by a vector [x,y,x] */
		void Translate(Double x, Double y, Double z);

		/** Scaling in three directions by factors x, y, z */
		void Resize(Double x, Double y, Double z);

	};

	/** \brief Rotation factory */
	class Rotate {
	public:
		/** Helper for rotation around X axis. */
		inline static Transform X(Double Angle) {
			return Transform(Transform::ROT_X, Angle);
		}

		/** Helper for rotation around Y axis. */
		inline static Transform Y(Double Angle) {
			return Transform(Transform::ROT_Y, Angle);
		}

		/** Helper for rotation around Z axis. */
		inline static Transform Z(Double Angle) {
			return Transform(Transform::ROT_Z, Angle);
		}
	};

	/**
	 * \brief
	 *	Helper class for translating by [x,y,z] vector coordinates.
	 */
	class Translate : public Transform {
	public:
		/** Initialize transformation */
		Translate(Double x, Double y, Double z) : Transform(x, y, z) {};
	};

	/**
	 * \brief
	 *	Helper class for scaling by a given [x,y,z] factors.
	 */
	class Scale : public Transform {
	public:
		/** Initialize transformation */
		Scale(Double x, Double y, Double z) : Transform()
		{
			Resize(x, y, z);
		};
	};

};

#endif

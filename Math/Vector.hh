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

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdexcept>
#include <string>
#include "General/Debug.hh"
#include "General/Types.hh"
#include "Math/Matrix.hh"
#include "Math/Tuple.hh"

namespace Math {

	/**
	 * \brief
	 *	Class which holds point-related information.
	 *
	 * With the help of this class one can define a vector in 3D environment,
	 * add it or substract from another vector, calculate their dot or cross product.
	 * Vectors can be also normalized and transformed with transformation matrices.
	 *
	 */
	class Vector : public Tuple<Double, false, 3> {
	public:
		enum {X, Y, Z};
		/** Initialize vector with given parameters */
		inline Vector(Double x, Double y, Double z) {
			D[0] = x;
			D[1] = y;
			D[2] = z;
		}

		inline Vector() {
			D[0] = D[1] = D[2] = 0.0;
		}

		inline Vector(const Tuple<Double, false, 3> &T) {
			D[0] = T[0]; /* Can it be more optimal? Rather no... */
			D[1] = T[1];
			D[2] = T[2];
		}


		/** Debug function.
		 * \see Matrix::Dump(); */
		friend std::ostream &operator<<(std::ostream &os, const Vector &V);

		/** Set all vector coordinates at once */
		inline void Set(Double x, Double y, Double z)
		{
			this->D[0] = x;
			this->D[1] = y;
			this->D[2] = z;
		}

		/**
		 * Transforms vector in place by a specified transformation matrix
		 * \see Vector::operator*(const Matrix &M)
		 */
		void Transform(const Matrix &M);

		/**
		 * Returns vector transformed by matrix M.
		 * \see Transform()
		 */
		Vector operator*(const Matrix &M) const;
		using Tuple<Double, false, 3>::operator*;

		/** \return New vector which is a cross product of this,
		 * and specified vector. */
		Vector Cross(const Vector &M) const;

		/** \return Double which is equal a dot product of this,
		 * and specified vector. */
		inline Double Dot(const Vector &M) const {
			return	D[0] * M.D[0] +
				D[1] * M.D[1] +
				D[2] * M.D[2];
		}


	};
};

#endif

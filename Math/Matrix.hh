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

#ifndef _MATRIX_H
#define _MATRIX_H

#include <iostream>
#include <string>
#include "General/Types.hh"
/**
 * \brief
 *	Manipulation of mathematical objects (Matrices, Vectors)
 *	used for easy manipulation of 3D Objects.
 */
namespace Math {

	/**
	 * \brief
	 *	Operations on matrices.
	 *
	 * Using this class we can implement various functions which define
	 * various 3D tranformations, which can by then combined by
	 * a single multiplication of their matrices and then applied to
	 * vectors -- also by a means of multiplication.
	 *
	 */
	class Matrix {
	protected:
		/** Matrix internal representation in row-major order */
		Double D[16];

	public:
		/*** Basic operations ***/
		/** Fill matrix with zeroes. */
		void Zero();

		/** Reset to identity matrix. */
		void LoadIdentity();

		/** Pretty-printer */
		friend std::ostream &operator<<(std::ostream &os, const Matrix &M);

		/** Read specified matrix element.
		 * \see SetXY
		 * \param x	column number
		 * \param y	row number
		 * \return matrix element
		 */
		Double GetXY(Int x, Int y) const;

		/** Set specified matrix element.
		 * \see GetXY
		 * \param x	column number
		 * \param y	row number
		 * \param D	value to set
		 */
		void SetXY(Int x, Int y, Double D);

		/** Set all elements from an array */
		void Set(Double (&D)[16]);

		/** Set all elements from another matrix */
		void Set(const Matrix &M);

		/** Multiplication "in place" of matrix */
		void Multiply(const Matrix &M2);

		/*** Operators ***/
		/** \return determinant of a matrix */
		Double operator!() const;

		Matrix operator+(const Matrix &M) const; /**< Matrix addition */
		Matrix operator-(const Matrix &M) const; /**< Matrix substraction */
		Matrix operator*(const Matrix &M) const; /**< Matrix multiplication */
		void operator=(const Matrix &M); /**< \see Set */

		/**
		 * Provides read-only access to the internal matrix representation.
		 *
		 * It's used in Vector class for vector by matrix multiplication
		 * \param i	matrix element in row-major order
		 */
		Double operator[](Int i) const;
	};
};

#endif

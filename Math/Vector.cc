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
#include <iomanip>
#include <cmath>

#include "General/Debug.hh"
#include "Matrix.hh"
#include "Vector.hh"

namespace Math {

	std::ostream &operator<<(std::ostream &os, const Vector &V)
	{
		os << std::setiosflags(std::ios::fixed)
		   << std::setprecision(2)
		   << "[Vect " << V.Dump();
		os << "]";

		return os;
	}

	void Vector::Transform(const Matrix &M)
	{
		/*
		 * [ X ]   [ a b c d ]   [ x ]
		 * [ Y ] * [ e f g h ] = [ y ]
		 * [ Z ]   [ j i k l ]   [ z ]
		 * [ W ]   [ m n o p ]   [ 1 ]
		 *
		 * x = a*X + b*Y + c*Z + d*W
		 * y = e*X + f*Y + g*Z + h*W
		 * z = j*X + i*Y + k*Z + l*W
		 * w = m*X + n*Y + o*Z + p*W
		 *
		 */

		/* Shortcuts/faster addressing */
		const Double
			&a=M[0],  &b=M[1],  &c=M[2],  &d=M[3],
			&e=M[4],  &f=M[5],  &g=M[6],  &h=M[7],
			&i=M[8],  &j=M[9],  &k=M[10], &l=M[11],

			&X = D[0], &Y = D[1], &Z = D[2], &W = 1.0;

		Double	x = a*X + b*Y + c*Z + d*W,
			y = e*X + f*Y + g*Z + h*W,
			z = j*X + i*Y + k*Z + l*W;

		D[0] = x;
		D[1] = y;
		D[2] = z;
	}

	Vector Vector::operator*(const Matrix &M) const
	{
		Vector NewV(*this);
		NewV.Transform(M);
		return NewV;
	}

	Vector Vector::Cross(const Vector &M) const
	{
		/* (a2b3 - a3b2) i + (a3b1 - a1b3) j + (a1b2 - a2b1) k =
		 * (a2b3 - a3b2, a3b1 - a1b3, a1b2 - a2b1)
		 * -1 for indexes gives:
		 * a1b2 - a2b1; a2b0 - a0b2; a0b1 - a1b0
		 */
		return Vector(
			D[1] * M.D[2] - D[2] * M.D[1],
			D[2] * M.D[0] - D[0] * M.D[2],
			D[0] * M.D[1] - D[1] * M.D[0]
		);
	}

};

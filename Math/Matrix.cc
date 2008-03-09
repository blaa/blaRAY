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

#include "Matrix.hh"
#include "General/Debug.hh"

namespace Math {
	void Matrix::Zero()
	{
		for (Int i=0; i<16; i++)
			this->D[i] = 0.0;
	}

	std::ostream &operator<<(std::ostream &os, const Matrix &M)
	{
		os << "[Matrix "
		   << setiosflags(std::ios::fixed)
		   << std::setprecision(2);
		for (Int y=0; y<4; y++) {
			for (Int x=0; x<4; x++) {
				os << M.D[y * 4 + x];
				if (x != 3)
					os << " ";
			}
			os << ";";
		}
		os << "Det=" << !M;
		return os;
	}

	void Matrix::LoadIdentity()
	{
		const Double ID[16] = {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0
		};
		memcpy(this->D, ID, sizeof(ID));
	}

	void Matrix::SetXY(Int x, Int y, Double D)
	{
		if (DEBUG)
			if (x>3 || y > 3 || x < 0 || y < 0) {
				std::cout << "Assert: Math::Matrix SetXY Range error" << std::endl;
				exit(-1);
			}
		this->D[y * 4 + x] = D;
	}

	Double Matrix::GetXY(Int x, Int y) const
	{
		if (DEBUG)
			if (x>3 || y > 3 || x < 0 || y < 0) {
				std::cout << "Assert: Math::Matrix GetXY Range error" << std::endl;
				exit(-1);
			}
		return this->D[y * 4 + x];
	}

	void Matrix::Set(Double (&D)[16])
	{
		memcpy(this->D, D, sizeof(this->D));
	}

	void Matrix::Set(const Matrix &M)
	{
		memcpy(this->D, M.D, sizeof(this->D));
	}

	void Matrix::Multiply(const Matrix &M2)
	{
		/*
		 * [ a b c d ] Is stored as:
		 * [ e f g h ] a b c d | e f g h | i  j  k  l   m  n  o  p
		 * [ j i k l ] 0 1 2 3 | 4 5 6 7 | 8  9 10 11  12 13 14 15
		 * [ m n o p ]
		 */
		/* Shortcuts */
		Double (&X)[16] = this->D;
		const Double (&Y)[16] = M2.D;
		Double Z[16];
		enum {
			A = 0, B, C, D,
			E, F, G, H,
			I, J, K, L,
			M, N, O, P
		};
		/* Z = X * Y */
		Z[A] =	X[A]*Y[A] + X[E]*Y[B] +	X[J]*Y[C] + X[M]*Y[D];
		Z[B] =	X[B]*Y[A] + X[F]*Y[B] + X[I]*Y[C] + X[N]*Y[D];
		Z[C] =	X[C]*Y[A] + X[G]*Y[B] + X[K]*Y[C] + X[O]*Y[D];
		Z[D] =	X[D]*Y[A] + X[H]*Y[B] + X[L]*Y[C] + X[P]*Y[D];

		/* E F G H */
		Z[E] =	X[A]*Y[E] + X[E]*Y[F] + X[J]*Y[G] + X[M]*Y[H];
		Z[F] =	X[B]*Y[E] + X[F]*Y[F] + X[I]*Y[G] + X[N]*Y[H];
		Z[G] =	X[C]*Y[E] + X[G]*Y[F] + X[K]*Y[G] + X[O]*Y[H];
		Z[H] =	X[D]*Y[E] + X[H]*Y[F] + X[L]*Y[G] + X[P]*Y[H];

		/* J I K L */
		Z[J] =	X[A]*Y[J] + X[E]*Y[I] + X[J]*Y[K] + X[M]*Y[L];
		Z[I] =	X[B]*Y[J] + X[F]*Y[I] + X[I]*Y[K] + X[N]*Y[L];
		Z[K] =	X[C]*Y[J] + X[G]*Y[I] + X[K]*Y[K] + X[O]*Y[L];
		Z[L] =	X[D]*Y[J] + X[H]*Y[I] + X[L]*Y[K] + X[P]*Y[L];

		/* M N O P */
		Z[M] =	X[A]*Y[M] + X[E]*Y[N] + X[J]*Y[O] + X[M]*Y[P];
		Z[N] =	X[B]*Y[M] + X[F]*Y[N] + X[I]*Y[O] + X[N]*Y[P];
		Z[O] =	X[C]*Y[M] + X[G]*Y[N] + X[K]*Y[O] + X[O]*Y[P];
		Z[P] =	X[D]*Y[M] + X[H]*Y[N] + X[L]*Y[O] + X[P]*Y[P];

		Set(Z);
	}

	Double Matrix::operator!() const
	{
		/*
		 * [ a b c d ]
		 * [ e f g h ]
		 * [ j i k l ]
		 * [ m n o p ]
		 */

		/* Shortcuts/faster addressing */
		const double
			&a=D[0],  &b=D[1],  &c=D[2],  &d=D[3],
			&e=D[4],  &f=D[5],  &g=D[6],  &h=D[7],
			&i=D[8],  &j=D[9],  &k=D[10], &l=D[11],
			&m=D[12], &n=D[13], &o=D[14], &p=D[15];
		return
			  a*(f*(k*p - l*o) - g*(j*p - l*n) + h*(j*o - k*n))
			- b*(e*(k*p - l*o) - g*(i*p - l*m) + h*(i*o - k*m))
			+ c*(e*(j*p - l*n) - f*(i*p - l*m) + h*(i*n - j*m))
			- d*(e*(j*o - k*n) - f*(i*o - k*m) + g*(i*n - j*m));
	}

	Matrix Matrix::operator+(const Matrix &M) const
	{
		Matrix N;
		for (Int i=0; i<16; i++)
			N.D[i] = this->D[i] + M.D[i];
		return N;
	}

	Matrix Matrix::operator-(const Matrix &M) const
	{
		Matrix N;
		for (Int i=0; i<16; i++)
			N.D[i] = this->D[i] - M.D[i];
		return N;
	}

	void Matrix::operator=(const Matrix &M)
	{
		memcpy(this->D, M.D, sizeof(this->D));
	}

	Matrix Matrix::operator*(const Matrix &M2) const
	{
		/*
		 * [ a b c d ] Is stored as:
		 * [ e f g h ] a b c d | e f g h | i  j  k  l   m  n  o  p
		 * [ j i k l ] 0 1 2 3 | 4 5 6 7 | 8  9 10 11  12 13 14 15
		 * [ m n o p ]
		 */
		Matrix Output;

		/* Shortcuts */
		const Double (&X)[16] = this->D;
		const Double (&Y)[16] = M2.D;
		Double (&Z)[16] = Output.D;
		enum {
			A = 0, B, C, D,
			E, F, G, H,
			I, J, K, L,
			M, N, O, P
		};
		/* Z = X * Y */
		Z[A] =	X[A]*Y[A] + X[E]*Y[B] +	X[J]*Y[C] + X[M]*Y[D];
		Z[B] =	X[B]*Y[A] + X[F]*Y[B] + X[I]*Y[C] + X[N]*Y[D];
		Z[C] =	X[C]*Y[A] + X[G]*Y[B] + X[K]*Y[C] + X[O]*Y[D];
		Z[D] =	X[D]*Y[A] + X[H]*Y[B] + X[L]*Y[C] + X[P]*Y[D];

		/* E F G H */
		Z[E] =	X[A]*Y[E] + X[E]*Y[F] + X[J]*Y[G] + X[M]*Y[H];
		Z[F] =	X[B]*Y[E] + X[F]*Y[F] + X[I]*Y[G] + X[N]*Y[H];
		Z[G] =	X[C]*Y[E] + X[G]*Y[F] + X[K]*Y[G] + X[O]*Y[H];
		Z[H] =	X[D]*Y[E] + X[H]*Y[F] + X[L]*Y[G] + X[P]*Y[H];

		/* J I K L */
		Z[J] =	X[A]*Y[J] + X[E]*Y[I] + X[J]*Y[K] + X[M]*Y[L];
		Z[I] =	X[B]*Y[J] + X[F]*Y[I] + X[I]*Y[K] + X[N]*Y[L];
		Z[K] =	X[C]*Y[J] + X[G]*Y[I] + X[K]*Y[K] + X[O]*Y[L];
		Z[L] =	X[D]*Y[J] + X[H]*Y[I] + X[L]*Y[K] + X[P]*Y[L];

		/* M N O P */
		Z[M] =	X[A]*Y[M] + X[E]*Y[N] + X[J]*Y[O] + X[M]*Y[P];
		Z[N] =	X[B]*Y[M] + X[F]*Y[N] + X[I]*Y[O] + X[N]*Y[P];
		Z[O] =	X[C]*Y[M] + X[G]*Y[N] + X[K]*Y[O] + X[O]*Y[P];
		Z[P] =	X[D]*Y[M] + X[H]*Y[N] + X[L]*Y[O] + X[P]*Y[P];

		return Output;
	}

	Double Matrix::operator[](Int i) const
	{
		if (DEBUG)
			if (i>15) {
				std::cout << "Assert: Math::Matrix [] Range error" << std::endl;
				exit(-1);
			}
		return this->D[i];
	}

};

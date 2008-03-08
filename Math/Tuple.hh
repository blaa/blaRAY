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

#ifndef _TUPLE_H_
#define _TUPLE_H_

#include <stdexcept>
#include <string>
#include <sstream>
#include <cmath>
#include "General/Debug.hh"
#include "Math/Matrix.hh"

namespace Math {
	/**
	 * \brief
	 *	Tuple of some numerical elements in range 0 - 1.0
	 *	with defined operations. Suitable for creating
	 *	vectors and colors.
	 *
	 */
	template<typename T, bool DoCropping=false, int Count=3>
	class Tuple {
	protected:
		/** Tuple coordinates (X,Y,Z,W=1)
		 *
		 * Fourth coordinate is defined so we can multiply vector
		 * by a matrix. */
		T D[Count];

		/** Crop tuple into bounds 
		 * \bug Split into CropUpper and CropLower
		 */
		inline void Crop() {
			for (Int i = 0; i < Count; i++) {
				if (D[i] > 1.0)
					D[i] = 1.0;
				else
					if (D[i] < 0.0)
						D[i] = 0.0;
			}
		}

		std::string Dump() const {
			std::stringstream s;
			for (Int i = 0; i < Count - 1; i++) {
				s << D[i] << ", ";
			}
			s << D[Count - 1];
			return s.str();
		}

	public:
		/** Initialize with lower bound values */
		Tuple() {
			for (Int i = 0; i < Count; i++)
				D[i] = 0.0;
		}

		/*** Basic operations ***/

		/** \return Square length of tuple */
		T SquareLength() const {
			T V = 0.0;
			for (Int i = 0; i < Count; i++) {
				V += D[i] * D[i];
			}
			return V;
		}

		/** \return Length of tuple. */
		T Length() const {
			return std::sqrt(SquareLength());
		}

		/** Normalizes tuple in place
		 * \return Reference to this object */
		Tuple &Normalize() {
			const T L = Length();
			for (Int i = 0; i < Count; i++)
				D[i] /= L;
			return *this;
		}

		/*** Compare operators ***/
		/** Equality comparator */
		inline Bool operator==(const Tuple &M) const {
			for (Int i = 0; i < Count; i++)
				if (D[i] != M.D[i]) return false;
			return true;
		}

		/** Inequality comparator */
		inline Bool operator!=(const Tuple &M) const {
			return !(*this == M);
		}

		/*** Modifications ***/
		/** In-place addition of tuples */
		Tuple &operator+=(const Tuple &M) {
			for (Int i = 0; i < Count; i++) {
				D[i] += M.D[i];
			}
			if (DoCropping == true) Crop();
			return *this;
		}

		/** In-place substraction of tuples elements */
		Tuple &operator-=(const Tuple &M) {
			for (Int i = 0; i < Count; i++) {
				D[i] -= - M.D[i];
			}
			if (DoCropping == true) Crop();
			return *this;
		}

		/** In-place multiplication of tuples elements */
		Tuple &operator*=(const Tuple &M) {
			for (Int i = 0; i < Count; i++) {
				D[i] *= M.D[i];
			}
			if (DoCropping == true) Crop();
			return *this;
		}

		/** In-place addition of a constant to all elements */
		Tuple &operator+=(T M) {
			for (Int i = 0; i < Count; i++) {
				D[i] += M;
			}
			if (DoCropping == true) Crop();
			return *this;
		}

		/** In-place substraction of a constant from all elements */
		Tuple &operator-=(T M) {
			for (Int i = 0; i < Count; i++) {
				D[i] -= M;
			}
			if (DoCropping == true) Crop();
			return *this;
		}

		/** In-place multiplication of all elements by a constant */
		Tuple &operator*=(T M) {
			for (Int i = 0; i < Count; i++) {
				D[i] *= M;
			}
			if (DoCropping == true) Crop();
			return *this;
		}

		/** In-place division of all elements by a constant */
		Tuple &operator/=(T M) {
			for (Int i = 0; i < Count; i++) {
				D[i] /= M;
			}
			if (DoCropping == true) Crop();
			return *this;
		}


		/** Tuple addition */
		Tuple operator+(const Tuple &M) const {
			Tuple<T, DoCropping, Count> NewT;
			for (Int i = 0; i < Count; i++) {
				NewT.D[i] = this->D[i] + M.D[i];
			}
			if (DoCropping == true) NewT.Crop();
			return NewT;
		}

		/** Tuple substraction */
		Tuple operator-(const Tuple &M) const {
			Tuple<T, DoCropping, Count> NewT;
			for (Int i = 0; i < Count; i++) {
				NewT.D[i] = this->D[i] - M.D[i];
			}
			if (DoCropping == true) NewT.Crop();
			return NewT;
		}

		/** Tuple element-by-element multiplication */
		Tuple operator*(const Tuple &M) const {
			Tuple<T, DoCropping, Count> NewT;
			for (Int i = 0; i < Count; i++) {
				NewT.D[i] = this->D[i] * M.D[i];
			}
			if (DoCropping == true) NewT.Crop();
			return NewT;
		}

		/** Add a constant to all elements */
		Tuple operator+(T M) const {
			Tuple<T, DoCropping, Count> NewT;
			for (Int i = 0; i < Count; i++) {
				NewT.D[i] = this->D[i] + M;
			}
			if (DoCropping == true) NewT.Crop();
			return NewT;
		}

		/** Substract a constant from all elements */
		Tuple operator-(T M) const {
			Tuple<T, DoCropping, Count> NewT;
			for (Int i = 0; i < Count; i++) {
				NewT.D[i] = this->D[i] - M;
			}
			if (DoCropping == true) NewT.Crop();
			return NewT;
		}

		/** Multiply all elements by a constant */
		Tuple operator*(T M) const {
			Tuple<T, DoCropping, Count> NewT;
			for (Int i = 0; i < Count; i++) {
				NewT.D[i] = this->D[i] * M;
			}
			if (DoCropping == true) NewT.Crop();
			return NewT;
		}

		/** Divide all elements by a constant */
		Tuple operator/(T M) const {
			Tuple<T, DoCropping, Count> NewT;
			for (Int i = 0; i < Count; i++) {
				NewT.D[i] = this->D[i] / M;
			}
			if (DoCropping == true) NewT.Crop();
			return NewT;
		}


		/** Provides read/write access to tuple values
		 * \return Reference to an element. */
		inline T &operator[](Int i) {
			if (DEBUG)
				if (i>=Count || i < 0) {
					throw std::invalid_argument(
						"Assert: Tuple [] Range error");
				}

			return D[i];
		}

		/** Provides read only access to tuple elements */
		inline T operator[](Int i) const {
			if (DEBUG)
				if (i>=Count || i < 0) {
					throw std::invalid_argument(
						"Assert: Tuple [] Range error");
				}

			return D[i];
		}

		/** \return Tuple length. */
		inline T operator!() const {
			return Length();
		}
	};
};

#endif

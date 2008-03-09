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

#ifndef _POINT_H_
#define _POINT_H_

#include <utility>
#include "General/Types.hh"

namespace Math {
	/**
	 * \brief
	 *	Simple 2D point.
	 *
	 * Class used for accessing texture colors,
	 * generating rays for camera, etc.
	 * Has constructor taking two doubles.
	 */
	class Point : public std::pair<Double, Double> {
	public:
		/** Construct point given two values */
		Point(Double U, Double V)
			: std::pair<Double, Double>(U, V) {}


		/**@{ Named accesor */
		inline Double GetU() const
		{
			return first;
		}

		inline Double GetV() const
		{
			return second;
		}

		inline void SetU(Double u)
		{
			first = u;
		}

		inline void SetV(Double v)
		{
			second = v;
		}
		/*@}*/
	};
}

#endif

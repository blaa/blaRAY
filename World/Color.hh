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

#ifndef _COLOR_H_
#define _COLOR_H_

#include <iostream>

#include <stdexcept>
#include "General/Types.hh"
#include "General/Debug.hh"
#include "Math/Tuple.hh"


namespace World {
	/**
	 * \brief
	 *	Color data and operations.
	 *
	 * Class holds RGB color data and performs simple color
	 * manipulations like mixing colors, filtering (adding,
	 * multiplying).
	 *
	 * \bug
	 *	Make it more vector like; instead of R,G,B use C[3]
	 *	with some enum.
	 *
	 * \bug It should possibly be moved to Graphics namespace as
	 *	it's more general.
	 *
	 */
	class Color : public Math::Tuple<Double, true, 3> {
	public:
		enum { R=0, G=1, B=2 };
		/** Creates default=black color */
		Color() : Math::Tuple<Double, true, 3>()
		{
		};

		/** Color of 0.0-1.0 Double values */
		Color(Double r, Double g, Double b);

		Color(const Math::Tuple<Double, true, 3> &T) {
			D[0] = T[0];
			D[1] = T[1];
			D[2] = T[2];
		}

		/** Debug function */
		friend std::ostream &operator<<(std::ostream &os, const Color &C);
	};

	/**
	 * \brief
	 *	Color library
	 */
	namespace ColLib {
		/**@{ Creates simple, named color object */
		inline Color White()
		{
			return Color(1.0, 1.0, 1.0);
		}

		inline Color Black()
		{
			return Color(0.0, 0.0, 0.0);
		}

		inline Color Red()
		{
			return Color(1.0, 0.0, 0.0);
		}

		inline Color Green()
		{
			return Color(0.0, 1.0, 0.0);
		}

		inline Color Blue()
		{
			return Color(0.0, 0.0, 1.0);
		}

		inline Color Gray()
		{
			return Color(0.5, 0.5, 0.5);
		}

		/*@}*/
	};

}

#endif

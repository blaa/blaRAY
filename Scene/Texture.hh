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

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <iostream>
#include <string>

#include <cmath>

#include "Math/Point.hh"
#include "Scene/Color.hh"

namespace Scene {

	/**
	 * \brief
	 *	Texture interface
	 *
	 * Texture is a function which returns color
	 * for given u,v coordinates
	 */
	class Texture {
	protected:
		/**@{ Texture Size */
		const Double SizeU, SizeV;
		/**@}*/
		const Bool Tiled; /**< Tilling setting */

		/** Default constructor for creating textures. */
		Texture(const Double SizeU=1.0,
			const Double SizeV=1.0,
			const Bool Tiled=true)
			: SizeU(SizeU), SizeV(SizeV), Tiled(Tiled) {}

		/** Change u,v coords if they are outside (0, Size) range */
		void Tile(Math::Point &UV) const
		{
			Double u = UV.GetU();
			Double v = UV.GetV();
			UV.SetU(u - std::floor(u / SizeU) * SizeU);
			UV.SetV(v - std::floor(v / SizeV) * SizeV);
		}

		/** Debug function; dumps information about texture */
		virtual std::string Dump() const;

	public:
		virtual ~Texture() /**< FIXME: Check if non-virtual constuctor will break things */
		{
		}

		/** Get texture color at point (u,v) */
		virtual Color Get(Math::Point UV) const = 0;

		/** Debug function */
		friend std::ostream &operator<<(std::ostream &os, const Texture &T);
	};

	/** \brief
	 *	Texture library
	 *
	 * Handles nice, functional-like, creation of basic textures.
	 */
        namespace TexLib {
		/** \brief Plain texture class */
		class Plain : public Texture {
		protected:
			/** Plain texture color */
			const Color C;

			virtual std::string Dump() const;
		public:
			/** Initialize plain texture */
			Plain(const Color &C) : C(C) {}

			virtual Color Get(Math::Point UV) const	{
				return C;
			}
		};

		/** \brief Checked texture class */
		class Checked : public Texture {
		protected:
			/**@{ Checked texture uses two colors */
			const Color A, B;
			/*@}*/

			virtual std::string Dump() const;
		public:
			/** Initialize checked texture */
			Checked(
				const Color &A = Scene::ColLib::White(),
				const Color &B = Scene::ColLib::Black(),
				const Double SizeU = 1.0,
				const Double SizeV = 1.0,
				const Bool Tiled = true)
				: Texture(SizeU, SizeV, Tiled),
				  A(A), B(B) {}

			virtual Color Get(Math::Point UV) const {
				const Double HalfU = SizeU / 2;
				const Double HalfV = SizeV / 2;

				if (Tiled) Tile(UV);
				else {
					if (UV.GetU() > SizeU ||
					    UV.GetV() > SizeV)
						return ColLib::Black();
				}
				if ((UV.GetU() < HalfU && UV.GetV() < HalfV) ||
				    (UV.GetU() > HalfU && UV.GetV() > HalfV))
					return A;
				return B;
			}
		};

		/** \brief Plain texture - whole in one color */
		/** Create plain texture */
		Texture *NewPlain(const Color &C);

		/** Create checked texture */
		Texture *NewChecked(const Color &A,
				    const Color &B,
				    const Double SizeU = 1.0,
				    const Double SizeV = 1.0,
				    const Bool Tiled = true);

		/**@{ Static plain texture */
		const Texture &Red();
		const Texture &Green();
		const Texture &Blue();
		const Texture &White();
		const Texture &Black();
		const Texture &Gray();
		/*@}*/


	};
}
#endif

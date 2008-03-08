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

#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <string>
#include <iostream>
#include <stdexcept>

#include "Scene/Texture.hh"

namespace Scene {
	/**
	 * \brief
	 *	Material data and operations.
	 *
	 * Class holds information about material. Including:
	 * diffuse, specular, refract, reflect textures and
	 * reflective, refractive, absorptive (doubles) properties.
	 * Provides means of getting material color at given point.
	 */
	class Material {
	private:
		/* Material textures (Color filters) */
		const Texture &Diffuse;  /**< Material diffuse color */
		const Texture &Specular; /**< Specular color */
		const Texture &Refract;  /**< Applied to refracting rays */
		const Texture &Reflect;  /**< Applied to reflected rays */


		/* Material properties */
		Double Reflective; /**< How many photons are reflected */
		Double Refractive; /**< How many photons are refracted */
		Double Absorptive; /**< How many photons are absorbed */
		Double Shininess; /**< Used for specular calculations in raytracer */
		Double Index; /**< Refractive index of material */

	public:
		/** Creates material using default values */
		Material(const Texture &Diffuse = TexLib::Red(),
			 const Texture &Specular = TexLib::White(),
			 const Texture &Refract = TexLib::Black(),
			 const Texture &Reflect = TexLib::Black(),
			 Double Reflective = 0.0,
			 Double Refractive = 0.0,
			 Double Absorptive = 0.9,
			 Double Shininess = 12.0,
			 Double Index = 1.0)
			: Diffuse(Diffuse), Specular(Specular),
			  Refract(Refract), Reflect(Reflect),
			  Reflective(Reflective), Refractive(Refractive),
			  Absorptive(Absorptive), Shininess(Shininess),
			  Index(Index)
		{
		}

		/** Filter selection */
		enum Filter { DIFFUSE, SPECULAR, REFRACT, REFLECT };
		/** Material property selection */
		enum Property { REFLECTIVE, REFRACTIVE, ABSORPTIVE, SHININESS, INDEX };

		/** Get one of material textures */
		inline const Texture &GetTexture(Filter f) const
		{
			switch (f) {
			case DIFFUSE: return this->Diffuse;
			case SPECULAR: return this->Specular;
			case REFRACT: return this->Refract;
			case REFLECT: return this->Reflect;
			default:
				throw std::invalid_argument("Illegal texture filter specified");
			}
		}

		/** Get material property */
		inline Double GetProperty(Property p)
		{
			switch (p) {
			case REFLECTIVE: return Reflective;
			case REFRACTIVE: return Refractive;
			case ABSORPTIVE: return Absorptive;
			case SHININESS: return Shininess;
			case INDEX: return this->Index;
			default:
				throw std::invalid_argument("Illegal material property specified");

			}
		}

		/** Get color of one material at given point */
		inline Color GetColor(Filter f, const Math::Point &UV) const
		{
			return GetTexture(f).Get(UV);
		}

		/** Debug function */
		friend std::ostream &operator<<(std::ostream &os, const Material &M);
	};

	/**
	 * \brief
	 *	Material library
	 */
	namespace MatLib {
		/**@{ Refractive index library */
		const Double IdxVacuum = 1.0;
		const Double IdxAir = 1.0002926;
		const Double IdxWater = 1.333;
		const Double IdxDiamond = 2.419;
		const Double IdxAmber = 1.55;
		const Double IdxSalt = 1.544;
		const Double IdxIce = 1.31;
		const Double IdxGlass = 1.60;
		/*@}*/
			

		/**@{ Default, one-color, non-reflective material */
		const Material &Red();
		const Material &Green();
		const Material &Blue();
		const Material &White();
		const Material &Black();
		const Material &Gray();
		/*@}*/

		/** Glass material */
		const Material &Glass();
	};
};

#endif

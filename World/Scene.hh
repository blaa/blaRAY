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

#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include <stdexcept>

#include "General/Debug.hh"
#include "Render/Ray.hh"

#include "World/Color.hh"
#include "World/Texture.hh"
#include "World/Material.hh"

#include "World/Object.hh"
#include "World/Plane.hh"
#include "World/Sphere.hh"

#include "World/Light.hh"
#include "World/Camera.hh"

/**
 * \brief
 *	Scene abstraction
 *
 * Classes in this namespace hold scene data: Location of objects,
 * colors, textures, cameras, lights`, etc.
 *
 */
namespace World {
	/** Nearest taken in account Ray collision */
	static const Double NearestCollision(0.001);

	/**
	 * \brief
	 *	Collision detection and scene management.
	 *
	 * This object holds scene structure, allows to check
	 * collisions with all scene objects and allows iteration 
	 * of scene lights. Scene frees in it's destructor memory 
	 * allocated for objects in the scene (they must be added
	 * to the structure with Add* functions.
	 */
	class Scene {
		/** Scene materials to be freed */
		std::vector<Material *> Materials;

		/** Scene textures to be freed */
		std::vector<Texture *> Textures;

		/** Scene objects to be freed, we check
		 * collisions with this objects.
		 * \bug rewrite implementation to use octree */
		std::vector<Object *> Objects;

		/** Lights we iterate during shadowpass. 
		 * Freed during scene destruction */
		std::vector<Light *> Lights;

		/** Scene background color */
		Color Background;

		/** Atmosphere refractive index */
		Double AtmosphereIdx;

	public:
		/** Initialize scene management */
		Scene(const Color &Background = ColLib::Black(),
		      const Double AtmosphereIdx = MatLib::IdxAir)
			: Background(Background),
			  AtmosphereIdx(AtmosphereIdx)
		{
			Materials.reserve(20);
			Objects.reserve(20);
			Lights.reserve(20);
			Textures.reserve(20);
		}

		/**
		 * Free all added to scene objects
		 */
		~Scene();

		/** Add object to scene. It will be freed by 
		 * scene destructor */
		inline void AddObject(Object *O)
		{
			if (DEBUG && O == NULL)
				throw std::invalid_argument
					("Argument can't be a NULL pointer");
			Objects.push_back(O);
		}

		/** Add light to the scene. It will be freed
		 * by scene destructor */
		inline void AddLight(Light *L)
		{
			if (DEBUG && L == NULL)
				throw std::invalid_argument
					("Argument can't be a NULL pointer");
			Lights.push_back(L);
		}

		/** Add material to the scene. It will be freed
		 * by scene destructor */
		inline void AddMaterial(Material *M)
		{
			if (DEBUG && M == NULL)
				throw std::invalid_argument
					("Argument can't be a NULL pointer");
			Materials.push_back(M);
		}

		/** Add texture to the scene. It will be freed
		 * by scene destructor */
		inline void AddTexture(Texture *T)
		{
			if (DEBUG && T == NULL)
				throw std::invalid_argument
					("Argument can't be a NULL pointer");
			Textures.push_back(T);
		}

		/**
		 * Finds nearest collision of ray with scene object.
		 * \bug This should be implemented using an octree, not a vector.
		 */
		Bool Collide(const Render::Ray &R, Double &RayPos, const Object* &O) const;


		/** Scene background accessor */
		inline const Color &GetBackground() const
		{
			return this->Background;
		}

		/** Scene atmosphere accessor */
		inline Double GetAtmosphere() const
		{
			return this->AtmosphereIdx;
		}

		/** \brief Template class for simplified scene iterators */
		template<typename T>
		class Iterator {
		protected:
			/** Current vector element */
			typename std::vector<T *>::const_iterator Cur;
			/** Element after the last element of vector */
			typename std::vector<T *>::const_iterator End;

		public:
			/** Initialize iterator from Scene. */
			Iterator(const Scene &S);

			/** Get next item */
			inline const T *Next() {
				if (Cur == End)
					return NULL;
				else	return *(Cur++);
			}

			friend class Scene;
		};

		/**@{Concretized iterators */
		typedef Iterator<Light> LightIterator;
		typedef Iterator<Object> ObjectIterator;
		typedef Iterator<Texture> TextureIterator;
		typedef Iterator<Material> MaterialIterator;
		/*@}*/
	};

};

#endif

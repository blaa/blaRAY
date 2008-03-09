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

#include "Render/Ray.hh"

#include "Scene/Color.hh"
#include "Scene/Texture.hh"
#include "Scene/Material.hh"

#include "Scene/Object.hh"
#include "Scene/Plane.hh"
#include "Scene/Sphere.hh"

#include "Scene/Light.hh"
#include "Scene/Camera.hh"

/**
 * \brief
 *	Renderable object classes + scene management
 *
 * Classes in this namespace hold scene data: Location of objects,
 * colors, textures, cameras, lights`, etc.
 *
 */
namespace Scene {
	/** Nearest taken in account Ray collision */
	static const Double NearestCollision(0.001);

	/**
	 * \brief
	 *	Collision detection (OCTree), holds scene structure.
	 */
	class Scene {
		std::vector<Material *> Materials;
		std::vector<Texture *> Textures;
		std::vector<Object *> Objects;
		std::vector<Light *> Lights;

		Color Background;
	public:
		Scene(const Color &Background = ColLib::Black())
			: Background(Background)
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

		inline void AddObject(Object *O)
		{
			Objects.push_back(O);
		}

		inline void AddLight(Light *L)
		{
			Lights.push_back(L);
		}

		inline void AddMaterial(Material *M)
		{
			Materials.push_back(M);
		}

		inline void AddTexture(Texture *T)
		{
			Textures.push_back(T);
		}


		/**
		 * Finds nearest collision of ray with scene object.
		 * \bug This should be implemented using an octree, not a vector.
		 */
		Bool Collide(const Render::Ray &R, Double &RayPos, const Object* &O) const;
		Color CheckShadow(const Math::Vector &From) const;

		inline const Color &GetBackground() const
		{
			return this->Background;
		}

		/** \brief Template class for scene iterators */
		template<typename T>
		class Iterator {
		protected:
			typename std::vector<T *>::const_iterator Cur;
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

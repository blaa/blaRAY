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
#include <map>
#include <stdexcept>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

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

		/** Camera used to render the scene */
		Camera C;

		/*** Facilities for reading XML Files */
		/*@{ XML Readers */

		/** Create basic library of color, texture, indices names */
		void CreateLibrary();
		
		/** Debug function. Dumps currently used id library */
		void DumpLibrary();

		/** Parse color (return it and name it) */
		Color ParseColor(xmlNodePtr Node);
		/** Parse vector (return it's value only) */
		Math::Vector ParseVector(xmlNodePtr Node);
		/** Parse named or given refractive index */
		Double ParseIdx(xmlNodePtr Node);

		/** Lookup material in library */
		const Material *GetMaterial(const std::string &id);
		/** Lookup texture in library */
		const Texture *GetTexture(const std::string &id);

		/** Texture parser */
		void ParseTexture(xmlNodePtr Node);
		/** Material parser */
		void ParseMaterial(xmlNodePtr Node);
		/** Point/Ambient light parser */
		void ParseLight(xmlNodePtr Node);

		/** Parse camera data */
		void ParseCamera(xmlNodePtr Node);

		/** Sphere parser */
		void ParseSphere(xmlNodePtr Node);
		/** Plane parser */
		void ParsePlane(xmlNodePtr Node);
		/*@}*/

		/**@{ Loaded items + default items library */
		std::map<std::string, Color> ColMap;
		std::map<std::string, const Texture *> TexMap;
		std::map<std::string, const Material *> MatMap;
		std::map<std::string, const Double> IdxMap;

		typedef std::map<std::string, Color>::iterator ColIter;
		typedef std::map<std::string, const Texture *>::iterator TexIter;
		typedef std::map<std::string, const Material *>::iterator MatIter;
		typedef std::map<std::string, const Double>::iterator IdxIter;
		/*@}*/

	public:
		/** Initialize scene management */
		Scene(const Camera &C = Camera(),
		      const Color &Background = ColLib::Black(),
		      const Double AtmosphereIdx = MatLib::IdxAir)
			: Background(Background),
			  AtmosphereIdx(AtmosphereIdx),
			  C(C) {
			Materials.reserve(20);
			Objects.reserve(20);
			Lights.reserve(20);
			Textures.reserve(20);
		}

		/** Free memory */
		~Scene();

		/** Frees all added to scene objects */
		void Purge();

		/** Add object to scene. It will be freed by
		 * scene destructor */
		inline void AddObject(Object *O) {
			if (DEBUG && O == NULL)
				throw std::invalid_argument
					("Argument can't be a NULL pointer");
			Objects.push_back(O);
		}

		/** Add light to the scene. It will be freed
		 * by scene destructor */
		inline void AddLight(Light *L) {
			if (DEBUG && L == NULL)
				throw std::invalid_argument
					("Argument can't be a NULL pointer");
			Lights.push_back(L);
		}

		/** Add material to the scene. It will be freed
		 * by scene destructor */
		inline void AddMaterial(Material *M) {
			if (DEBUG && M == NULL)
				throw std::invalid_argument
					("Argument can't be a NULL pointer");
			Materials.push_back(M);
		}

		/** Add texture to the scene. It will be freed
		 * by scene destructor */
		inline void AddTexture(Texture *T) {
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

		/** Reader */
		Bool ParseFile(const std::string &File);

		/** Scene background accessor */
		inline const Color &GetBackground() const {
			return this->Background;
		}

		/** Scene atmosphere accessor */
		inline Double GetAtmosphere() const {
			return this->AtmosphereIdx;
		}

		/** Scene camera */
		inline const Camera &GetCamera() const {
			return this->C;
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

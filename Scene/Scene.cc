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

#include <cmath>
#include <limits>
#include <vector>

#include "Scene/Scene.hh"

namespace Scene {
	Scene::~Scene()
	{
		for (std::vector<Object *>::iterator i = this->Objects.begin();
		     i != this->Objects.end();
		     i++)
			delete *i;

		for (std::vector<Light *>::iterator i = this->Lights.begin();
		     i != this->Lights.end();
		     i++)
			delete *i;

		for (std::vector<Material *>::iterator i = this->Materials.begin();
		     i != this->Materials.end();
		     i++)
			delete *i;

		for (std::vector<Texture *>::iterator i = this->Textures.begin();
		     i != this->Textures.end();
		     i++)
			delete *i;

	}

	Bool Scene::Collide(const Render::Ray &R, Double &RayPos, const Object* &O) const
	{
		Bool SceneCol = false;
		RayPos = std::numeric_limits<double>::infinity();
		std::vector<Object *>::const_iterator i;
		for (i = this->Objects.begin();
		     i != this->Objects.end();
		     i++) {
			Double t;
			Bool ObjCol(false);
			const Object &Cur = **i;
			ObjCol = Cur.Collide(R, t);
			if (ObjCol == true && t < RayPos) {
				RayPos = t;
				O = &Cur;
				SceneCol = true;
			}
		}
		return SceneCol;
	}

	template<> Scene::Iterator<Light>::Iterator(const Scene &S)
		: Cur(S.Lights.begin()), End(S.Lights.end()) {}
	template<> Scene::Iterator<Object>::Iterator(const Scene &S)
		: Cur(S.Objects.begin()), End(S.Objects.end()) {}
	template<> Scene::Iterator<Texture>::Iterator(const Scene &S)
		: Cur(S.Textures.begin()), End(S.Textures.end()) {}
	template<> Scene::Iterator<Material>::Iterator(const Scene &S)
		: Cur(S.Materials.begin()), End(S.Materials.end()) {}
};

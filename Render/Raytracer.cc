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
#include <iomanip>
#include <cmath>

#include "General/Types.hh"
#include "Render/Raytracer.hh"

namespace Render {
	const Int Raytracer::AASize = 2;
	const Int Raytracer::MaxDepth = 10;


	Raytracer::Raytracer(const Scene::Scene &Scene,
			     const Scene::Camera &Camera,
			     const Bool Antialiasing)
		: Scene(Scene), Camera(Camera), 
		  Antialiasing(Antialiasing),
		  ShadowRays(0),
		  ReflectedRays(0),
		  RefractedRays(0)
	{
	}

	inline void Raytracer::TraceLights(
		const Math::Vector &ColPoint,
		const Math::Vector &Normal,
		const Ray &Reflect,
		const Double Shininess,
		Scene::Color &Diffuse,
		Scene::Color &Specular)
	{
		/* Init resulting color */
		Diffuse = Specular = Scene::ColorLib::Black();

		/* Shadow rays */
		Double ColPos;

		Scene::Scene::LightIterator Iter(this->Scene);
		while (const Scene::Light *l = Iter.Next()) {
			/* Raytracing works only for point and ambient lights */
			
			
			{
				const Scene::AmbientLight *P;
				P = dynamic_cast<const Scene::AmbientLight *>(l);
				if (P != NULL) {
					/* Ambient light */
					Diffuse += P->GetColor();
					continue;
				}
			}

			const Scene::PointLight *P;
			P = dynamic_cast<const Scene::PointLight *>(l);
			if (P == NULL)
				continue; 

			/* Check if we are shadowed from this light */
			this->ShadowRays++;
			Ray ToLight = Ray::RayFromPoints(ColPoint,
							 P->GetPosition());
			const Scene::Object *tmp;
			if (this->Scene.Collide(ToLight, ColPos, tmp) == true)
				continue;


			/* Unshadowed light */
			const Math::Vector &LightDir = ToLight.Direction();
			const Scene::Color &LightColor = P->GetColor();

			/* Calculate coefficients */
			Double CoeffDiffuse = Normal.Dot(LightDir);
			Double CoeffSpecular = Reflect.Direction().Dot(LightDir);
			if (CoeffSpecular < 0.0)
				CoeffSpecular = 0.0;
			else
				CoeffSpecular = std::pow(CoeffSpecular, Shininess);

			Diffuse += LightColor * CoeffDiffuse;
			Specular += LightColor * CoeffSpecular;
		}

	}
 
	Bool Raytracer::Trace(const Ray &R, Scene::Color &C, const Int Depth)
	{
		Double ColPos = 0.0;
		Bool Collision = false;
		const Scene::Object *Obj = NULL;

		/* Check collision with scene objects */
		Collision = this->Scene.Collide(R, ColPos, Obj);
		if (Collision == false) {
			return false;
		}
		const Math::Vector ColPoint = R.GetPoint(ColPos);
		const Math::Vector Normal = Obj->NormalAt(ColPoint);
		const Ray ReflectRay = R.Reflect(Normal, ColPoint);

		/* Found collision with object Obj, at ColPoint
		 * with normal Normal.
		 */
		Scene::Color Diffuse, Specular, Reflect, Refract;
		TraceLights(ColPoint,
			    Normal,
			    ReflectRay,
			    12.0,
			    Diffuse,
			    Specular);

		const Scene::Color &ObjDiff =
			Obj->ColorAt(ColPoint, Scene::Material::DIFFUSE);
		const Scene::Color &ObjSpec =
			Obj->ColorAt(ColPoint, Scene::Material::SPECULAR);
		const Scene::Color &ObjRefl =
			Obj->ColorAt(ColPoint, Scene::Material::REFLECT);
		const Scene::Color &ObjRefr =
			Obj->ColorAt(ColPoint, Scene::Material::REFRACT);

		if (Depth < MaxDepth) {
			/* Reflection tracing */
			if (ObjRefl[0] != 0.0 ||
			    ObjRefl[1] != 0.0 ||
			    ObjRefl[2] != 0.0)
			{
				this->ReflectedRays++;
				if (!Trace(ReflectRay, Reflect, Depth + 1))
					Reflect = Scene::ColorLib::Black();
			}
			
			/* Refraction tracing */
			/*  if (ObjRefr[0] != 0.0 ||
			    ObjRefr[1] != 0.0 ||
			    ObjRefr[2] != 0.0)
			{
				const Ray RefractRay = R.Refract(Normal, ColPoint);
				this->RefractedRays++;
				if (!Trace(ReflectRay, Reflect, Depth + 1))
					Reflect = Scene::ColorLib::Black();
			} */


		}

/*		const Scene::Color &ObjC3 =
		Obj->ColorAt(ColPoint, Scene::Material::DIFFUSE); */
		C = Diffuse * ObjDiff + Specular * ObjSpec + Reflect * ObjRefl;
/*		C = ShadowColor;*/
		return true;
	}

	void Raytracer::Render(Graphics::Drawable &Img)
	{
		Int Width = Img.GetWidth();
		Int Height = Img.GetHeight();
		const Scene::Color &Background = Scene.GetBackground();
		const Scene::Camera::View V =
			this->Camera.CreateView(
				Antialiasing ? Int(Width * AASize) : Width,
				Antialiasing ? Int(Height * AASize) : Height);


		ShadowRays = ReflectedRays = RefractedRays = 0;

		std::cout << "*** Raytracing renderer ***" << std::endl;
		
		Scene::Color C;
		/* Iterate over rays created from camera. */
		if (!this->Antialiasing)
			for (Int x = 0; x < Width; x++) {
				for (Int y = 0; y < Height; y++) {
					Ray R = V.At(x, y);
					if (this->Trace(R, C, 0) == true) {
						Img.PutPixel(x, y, C);
					} else {
						Img.PutPixel(x, y, Background);
					}
				}
			}
		else {
			Double R, G, B;
			for (Int x = 0; x < Width; x++)
			for (Int y = 0; y < Height; y++) {
				R = G = B = 0.0;
				
				for (Int aa_x = 0;
				     aa_x < AASize;
				     aa_x++)
				for (Int aa_y = 0;
				     aa_y < AASize;
				     aa_y++) {
					Ray TracedRay = V.At(
						x * AASize + aa_x,
						y * AASize + aa_y);
					if (this->Trace(TracedRay, C, 0) == true) {
						R += C[0];
						G += C[1];
						B += C[2];
					} else {
						R += Background[0];
						G += Background[1];
						B += Background[2];
					}
				}
				C = Scene::Color(R/AASize/AASize,
						 G/AASize/AASize,
						 B/AASize/AASize);
//				C = Scene::ColorLib::Red();
				Img.PutPixel(x, y, C);
			}
		}


		std::cout << "*** Raytracing Stats ***" << std::endl;
		std::cout << "*** Rays: Reflected="
			  << ReflectedRays
			  << " refracted="
			  << RefractedRays
			  << " shadow="
			  << ShadowRays
			  << " all=" << ReflectedRays + RefractedRays + ShadowRays
			  << std::endl;
	}
};

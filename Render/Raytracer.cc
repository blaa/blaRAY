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
	/** Square of number of rays used for
	 * calculating a single pixel on the screen */
	const Int Raytracer::AASize = 2;

	Raytracer::Raytracer(const World::Scene &Scene,
			     const World::Camera &Camera,
			     const Bool Antialiasing,
			     const Int MaxDepth)

		: Scene(Scene), Camera(Camera),
		  Antialiasing(Antialiasing),
		  MaxDepth(MaxDepth),
		  ShadowRays(0),
		  ReflectedRays(0),
		  RefractedRays(0)
	{
	}

	inline void Raytracer::TraceLights(
		const Math::Vector &ColPoint,
		const Math::Vector &Normal,
		const Ray &Reflect,
		World::Color &Diffuse,
		World::Color &Specular)
	{
		/* Init resulting color */
		Diffuse = Specular = World::ColLib::Black();

		/* Shadow rays */
		Double ColPos;

		World::Scene::LightIterator Iter(this->Scene);
		while (const World::Light *l = Iter.Next()) {
			/* Raytracing works only for point and ambient lights */

			{
				const World::AmbientLight *P;
				P = dynamic_cast<const World::AmbientLight *>(l);
				if (P != NULL) {
					/* Ambient light */
					Diffuse += P->GetColor();
					continue;
				}
			}

			const World::PointLight *P;
			P = dynamic_cast<const World::PointLight *>(l);
			if (P == NULL)
				continue;

			/* Check if we are shadowed from this light */
			this->ShadowRays++;
			Ray ToLight = Ray::RayFromPoints(ColPoint,
							 P->GetPosition());
			const World::Object *tmp;
			if (this->Scene.Collide(ToLight, ColPos, tmp) == true)
				continue;

			/* Unshadowed light */
			const Math::Vector &LightDir = ToLight.Direction();
			const World::Color &LightColor = P->GetColor();

			/* Calculate coefficients */
			Double CoeffDiffuse = Normal.Dot(LightDir);
			Double CoeffSpecular = Reflect.Direction().Dot(LightDir);
			if (CoeffSpecular < 0.0)
				CoeffSpecular = 0.0;

			Diffuse += LightColor * CoeffDiffuse;
			Specular += LightColor * CoeffSpecular;
		}

	}

	Bool Raytracer::Trace(const Ray &R,
			      World::Color &C,
			      const Int Depth,
			      const Double CurIdx)
	{
		const World::Object *Obj = NULL;

		/* Check collision with scene objects */
		Double ColPos = 0.0;
		if (this->Scene.Collide(R, ColPos, Obj) == false) {
			return false;
		}
		const Math::Vector ColPoint = R.GetPoint(ColPos);
		const Math::Vector Normal = Obj->NormalAt(ColPoint);
		const Ray ReflectRay = R.Reflect(Normal, ColPoint);
		/* Found collision with object Obj, at ColPoint
		 * with normal Normal.
		 */

		/* Parts of resulting pixel color */
		World::Color
			Diffuse = World::ColLib::Black(),
			Specular = World::ColLib::Black(),
			Reflect = World::ColLib::Black(),
			Refract = World::ColLib::Black();

		const World::Color &ObjDiff =
			Obj->ColorAt(ColPoint, World::Material::DIFFUSE);
		const World::Color &ObjSpec =
			Obj->ColorAt(ColPoint, World::Material::SPECULAR);
		const World::Color &ObjRefl =
			Obj->ColorAt(ColPoint, World::Material::REFLECT);
		const World::Color &ObjRefr =
			Obj->ColorAt(ColPoint, World::Material::REFRACT);
		const Double Shininess =
			Obj->GetProperty(World::Material::SHININESS);
		const Double NewIdx =
			Obj->GetProperty(World::Material::INDEX);

		TraceLights(ColPoint, Normal,
			    ReflectRay,
			    Diffuse, Specular);

		if (Depth < MaxDepth) {
			/* Reflection tracing */
			if (ObjRefl[0] != 0.0 ||
			    ObjRefl[1] != 0.0 ||
			    ObjRefl[2] != 0.0)
			{
				this->ReflectedRays++;
				if (!Trace(ReflectRay, Reflect,
					   Depth + 1, CurIdx))
					Reflect = World::ColLib::Black();
			}

			/* Refraction tracing */
			if (ObjRefr[0] != 0.0 ||
			    ObjRefr[1] != 0.0 ||
			    ObjRefr[2] != 0.0)
			{
				Double IntoIdx;
				Math::Vector RealNormal = Normal;
				if (NewIdx == CurIdx) {
					/* We are currently in this object,
					   leave it - it's not a good way
					   of handling this situation. */
					IntoIdx = this->Scene.GetAtmosphere();
					RealNormal = - Normal;
				}
				else IntoIdx = NewIdx;

				const Ray RefractRay =
					R.Refract(RealNormal, ColPoint,
						  CurIdx, IntoIdx);
				this->RefractedRays++;
				if (!Trace(RefractRay,
					   Refract,
					   Depth + 1,
					   NewIdx));
			}
		}
		C =
			Diffuse * ObjDiff +
			(Specular * ObjSpec).Pow(Shininess) +
			Reflect * ObjRefl +
			Refract * ObjRefr;
		return true;
	}

	void Raytracer::Render(Graphics::Drawable &Img)
	{
		Int Width = Img.GetWidth();
		Int Height = Img.GetHeight();
		const World::Color &Background = Scene.GetBackground();
		const World::Camera::View V =
			this->Camera.CreateView(
				Antialiasing ? Int(Width * AASize) : Width,
				Antialiasing ? Int(Height * AASize) : Height);


		ShadowRays = ReflectedRays = RefractedRays = 0;

		std::cout << "*** Raytracing renderer ***" << std::endl;

		World::Color C;

		/* Debug part */
/*		Ray RR = V.At(327,263);
		this->Trace(RR, C, 0, AtmosphereIdx);
		return; */
		/* End of debug part */

		/* Iterate over rays created from camera. */
		if (!this->Antialiasing)
			for (Int x = 0; x < Width; x++) {
				for (Int y = 0; y < Height; y++) {
					Ray R = V.At(x, y);
					if (this->Trace(
						    R, C,
						    0, Scene.GetAtmosphere())
					    == true) {
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
					if (this->Trace(
						    TracedRay, C, 0,
						    Scene.GetAtmosphere())
					    == true) {
						R += C[0];
						G += C[1];
						B += C[2];
					} else {
						R += Background[0];
						G += Background[1];
						B += Background[2];
					}
				}
				C = World::Color(R/AASize/AASize,
						 G/AASize/AASize,
						 B/AASize/AASize);
//				C = World::ColLib::Red();
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

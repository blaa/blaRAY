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

#include "General/Debug.hh"
#include "General/Types.hh"

#include "Graphics/Screen.hh"
#include "Scene/Scene.hh"
#include "Render/Raytracer.hh"

using namespace std;

namespace Testcases {
	void Fail(std::string s) {
		cout << "Testcase FAILED (" << s << ")" << std::endl;
		throw 42;
	}

	void Math()
	{
		/* Math testcase */
		cout << "*** Math testcase ***" << endl;
		Math::Vector a(1.0, 1.0, 0.0);
		Math::Vector b = a * Math::Rotate::X(3.14/2);
		cout << a << " Other: " << b;

		Math::Vector c = (b * Math::Scale(5.0, 5.0, 5.0));
		cout << c << "[] operator test : " << c[0] << endl;

		/* This mustn't compile */
/*		Scene::Color Col(1.0, 0.0, 1.0);
		c = a * Col; */
		{
			Math::Vector a(1.0, 0.0, 0.0);
			Math::Vector b(0.0, 1.0, 0.0);
			if (a + b != Math::Vector(1.0, 1.0, 0.0))
				Fail("Vector addition failed");
			if ((a+b) * a != a)
				Fail("Vector multiplication failed");
			if (a[0] + b[1] != 2.0)
				Fail("Vector [] operator addition failed");

			if (a.Dot(b) != 0.0) {
				Fail("Dot product error");
			}
			Math::Vector c;
			if (c != Math::Vector(0.0, 0.0, 0.0))
				Fail("Vector default constructor");
			c[2] = 1.0;
			if (a.Cross(b) != c) {
				Fail("Cross product error");
			}
			cout << "Vector .dot and .cross seems ok" << endl;
		}
	}

	void Render()
	{
		using namespace Render;
		using namespace Math;
		/* Render testcase */
		cout << "*** Render testcase ***" << endl;

		{
			Ray R(Vector(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
			if (R.GetPoint(10.0) != Vector(0.0, 0.0, 10.0)) {
				cout << "Ray=" << R << endl;
				cout << "\tColPoint at 10 = " << R.GetPoint(10.0) << endl;
				Fail("Ray Getpoint failed!");
			}
		}

		const Math::Vector V1(0.0, 0.0, 0.0);
		const Math::Vector V2(0.0, 0.0, 1.0);

		Scene::Scene S;
		S.AddObject(new Scene::Sphere(
				    Math::Vector(0.0, 0.0, 10.0),
				    1.0)
			);
		S.AddLight(new Scene::PointLight(Math::Vector(0.0, 10.0, 7.0)));

		Scene::Camera C(V1, V2);
		Render::Raytracer R(S, C);
		Render::Ray(V1, V2);

		Graphics::Image Img(4,4);
		R.Render(Img);
	}



	void Scene()
	{
		/* Textures/Colors  */
		cout << "*** Scene testcase ***" << endl;
		Scene::Color C1 = Scene::ColorLib::White();
		Scene::Color C2 = Scene::ColorLib::Red();

		cout << C1 << " " << C2 << endl;

		std::auto_ptr<Scene::Texture> Tex1(Scene::TexLib::NewPlain(C1) );
		std::auto_ptr<Scene::Texture> Tex2(Scene::TexLib::NewChecked(C1, C2, 1.0, 2.0, true) );
		const Scene::Texture &Tex3 = Scene::TexLib::Red();

		cout << *Tex1 << endl
		     << *Tex2 << endl
		     << Tex3 << endl;

		/* Primitives and collisions */

		/* Plane - ray collision */
		Scene::Plane Plane1(Math::Vector(0.0, 1.0, 0.0),
				    0.0);

		Render::Ray Ray1(Math::Vector(0.0, 3.0, 0.0),
				 Math::Vector(0.0, -0.5, 2.0));

		cout << "** Collision between plane:" << endl
		     << Plane1 << endl
		     << "And a ray: "
		     << Ray1 << endl;

		Double Loc;
		bool Result = Plane1.Collide(Ray1, Loc);
		if (Result) {
			Math::Vector ColPoint = Ray1.GetPoint(Loc);
			Math::Vector ColPoint_should(0.0, 0.0, 12.0);
			cout << "Collided at: " << ColPoint << endl;
			if (ColPoint != ColPoint_should) {
				cout << "Wrong point! TESTCASE FAILED" << endl;
				throw 42;
			} else
				cout << "Testcase OK" << endl;
		} else {
			cout << "No collision TESTCASE FAILED" << endl;
			throw 42;
		}

		/* Sphere - Ray collision  */

		Scene::Sphere Sphere(Math::Vector(0.0, 0.0, 10.0),
				     1.0);

		Render::Ray Ray2(Math::Vector(0.0, 0.0, 0.0),
				 Math::Vector(0.0, 0.0, 1.0));

		cout << endl << "** Collision between sphere:" << endl
		     << Sphere << endl
		     << "And a ray:"
		     << Ray2 << endl;

		Result = Sphere.Collide(Ray2, Loc);
		if (Result) {
			Math::Vector ColPoint = Ray2.GetPoint(Loc);
			Math::Vector ColPoint_should(0.0, 0.0, 9.0);
			cout << "Collided at: " << ColPoint << endl;
			if (ColPoint != ColPoint_should) {
				cout << "Wrong point! TESTCASE FAILED" << endl;
				throw 42;
			} else
			cout << "Testcase OK" << endl;
		} else {
			cout << "No collision TESTCASE FAILED" << endl;
			throw 42;
		}


		/*** Scene testcase ***/
		Scene::Scene S;
		S.AddObject(new Scene::Plane(Plane1));
		S.AddLight(new Scene::PointLight(Math::Vector(1.0, 1.0, 1.0)));

		cout << "*** Dumping lights" << endl;
		{
			Scene::Scene::LightIterator Iter(S);
			while (const Scene::Light *l = Iter.Next()) {
				cout << *l << endl;
			}
		}

		cout << "*** Dumping objects" << endl;
		{
			Scene::Scene::ObjectIterator Iter(S);
			while (const Scene::Object *l = Iter.Next()) {
				cout << *l << endl;
			}
		}


	}

	void Graphics()
	{
		Graphics::Screen S(640,480);
		Scene::Color C1 = Scene::ColorLib::Red();

		for (Int x=0; x<50; x++) {
			for (Int y = 0; y < 50; y++)
				S.PutPixel(x, y, C1);
		}
		S.EventWait();
	}

	void Explicit()
	{

		/** Compile time testcase: things that should compile */
#if 0

		e_t(double)		nonconst_(10.0);
		const Double	const_(15.0);

		/* Basic arithmetic const -> nonconst */
		const Double	A(13.0);
		const Double	B(2.0);
		e_t(double)		C = A * B + nonconst_;

		/* Tables */
		e_t(double) Table[10];
		Table[0] = Table[1];
		Table[1] = nonconst_;
		Table[2] = const_;

		/* from const type */
		e_t(double)			to(0.0);
		const const Double		first(10.0);
		const e_t(double)		second(10.0);
		to = first * first;
		to = first * second;
		to = second * second;
#endif

		/** Things that shouldn't compile.
		 * Select 'pass' and prove it to yourself. ;) */
#define __Pass 0
#if     (__Pass == 1)
#warning "Pass = 1"
		Double a = 1;
#elif (__Pass == 2)
		Int a = 1.0;
#elif (__Pass == 3)
		Double a = 1.0;
		Int b = a;
#elif (__Pass == 4)
		e_t(const double) a = 1.0;
		a = 2.0;
#elif (__Pass == 5)
		const Double a = 1.0;
		a = 2.0;
#elif (__Pass == 6)
		const Double a = 1.0;
		const Double b(a);
		a = b;
		a(10.0);
#elif (__Pass == 7)
		const Double a = 1.0;
		const Double &b = a;	/* This is ok */
		Double &c = a;		/* This is not */
		double &d = a;		/* This is not... */

#endif

	}

	void All()
	{
		Math();
		Render();
		Scene();
		Explicit();
//		Graphics();
	}
};

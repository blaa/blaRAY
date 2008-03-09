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
#include <sys/time.h>

#include "General/Types.hh"

#include "Math/Matrix.hh"
#include "Math/Vector.hh"
#include "Math/Transform.hh"
#include "Render/Raytracer.hh"

#include "General/Testcases.hh"

/** \mainpage blaRAY raytracer/photon mapper
 *
 * This is a documentation for blaRAY - photon mapping raytracer
 * written in 2008 by Tomasz bla Fortuna as a project for studies.
 * Documentation contains description of API: class, members and namespaces
 * description. For user documentation check Docs directory.
 *
 * Happy tracing!
 *
 */

/**
 * \bug We need better defined policy of which functions
 * should be declared class-inline and which shouldn't.
 * Good job would do a profiler.
 */

void Render1(Graphics::Screen &Scr)
{
	using namespace World;
	const Math::Vector V1(0.0, 0.0, 0.0);
	const Math::Vector V2(0.0, 0.0, 1.0);

	Scene::Scene S(ColLib::Black());

	const Texture &Plain = TexLib::Plain(
		Color(0.2, 0.2, 0.2));

	TexLib::Checked Checked = TexLib::Checked();

	Material SphereMat(
		TexLib::Red(),
		TexLib::White(),
		TexLib::Black(),
		Plain);

	Material PlaneMat(
		Checked,
		TexLib::Black(),
		TexLib::Black(),
		TexLib::White()
		);

	S.AddObject(new Sphere(
			    Math::Vector(0.0, 0.0, 10.0),
			    1.0,
			    SphereMat));

	S.AddObject(new Plane(
			    Math::Vector(0.0, 1.0, 0.0),
			    -1.0,
			    PlaneMat));

	S.AddLight(new PointLight(Math::Vector(3.0, 10.0, 7.0)));
	S.AddLight(new AmbientLight(Color(0.05, 0.05, 0.05)));

	Camera C(V1, V2);
	Render::Raytracer R(S, C, true);

	std::cout << "Raytracing with " << C;

	R.Render(Scr);
}

void Render2(Graphics::Screen &Scr)
{
	using namespace World;

	Scene::Scene S(ColLib::Black());

	TexLib::Checked Checked = TexLib::Checked(
		ColLib::Black(),
		Color(0.8, 0.8, 0.8));

	TexLib::Checked Checked2 = TexLib::Checked(
		ColLib::Blue(),
		ColLib::Red(),
		0.2, 0.2);

	Material SphereMat = MatLib::Glass();
	Material SphereMat2(
		Checked2,
		TexLib::Black(), TexLib::Black(),
		TexLib::Black());

	Material PlaneMat(
		Checked,
		TexLib::Black(),
		TexLib::Black(),
		TexLib::Black()
		);

	S.AddObject(new Sphere(
			    Math::Vector(1.0, 0.0, 8.0),
			    1.0, SphereMat));

	S.AddObject(new Sphere(
			    Math::Vector(0.0, 0.0, 10.0),
			    1.0, SphereMat2));

	S.AddObject(new Sphere(
			    Math::Vector(-1.5, 0.0, 8.0),
			    1.0, SphereMat));

	S.AddObject(new Plane(
			    Math::Vector(0.0, 1.0, 0.0),
			    -1.0,
			    PlaneMat));

	S.AddLight(new PointLight(Math::Vector(3.0, 10.0, 7.0)));
	S.AddLight(new AmbientLight(Color(0.05, 0.05, 0.05)));

	const Math::Vector Pos(-2.0, 3.0, -2.0);
	const Math::Vector Dir(0.2, -0.3, 1.0);
	const Camera C(Pos, Dir);
	Render::Raytracer R(S, C, true);

	std::cout << "Raytracing with " << C;

	R.Render(Scr);
}


int main(void)
{
	struct timeval A, B;
	Testcases::All();

	/* Render something */
	Graphics::Screen Scr(640, 480);
/*	Graphics::Screen Scr(50, 50); */

	gettimeofday(&A, NULL);
	Render2(Scr);
	gettimeofday(&B, NULL);
	e_t(double) ATime = A.tv_sec + 0.000001 * A.tv_usec;
	e_t(double) BTime = B.tv_sec + 0.000001 * B.tv_usec;

	std::cout << "*** Rendering took "
		  << BTime - ATime
		  << " seconds" << std::endl;

	Scr.Refresh();
	Scr.Save("Last.bmp");
	Scr.EventWait();

	return 0;
}

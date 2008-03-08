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
	using namespace Scene;
	const Math::Vector V1(0.0, 0.0, 0.0);
	const Math::Vector V2(0.0, 0.0, 1.0);

	::Scene::Scene S(ColorLib::Black());

	const Texture &Plain = TexLib::Plain(
		Color(0.2, 0.2, 0.2));

//	TexLib::Plain Plain = TexLib::Plain(
	//	Color(0.2, 0.2, 0.2));
	TexLib::Checked Checked = TexLib::Checked();

//	S.AddTexture(Plain);
//	S.AddTexture(Checked);

	Material SphereMat(
		TexLib::Red(),
		TexLib::White(),
		TexLib::Black(),
		Plain);
//		::Scene::TexLib::Plain(::Scene::Color(0.2, 0.2, 0.2)));

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


int main(void)
{
	struct timeval A, B;
	Testcases::All();

	/* Render something */
	Graphics::Screen Scr(640, 480);

	gettimeofday(&A, NULL);
	Render1(Scr);
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

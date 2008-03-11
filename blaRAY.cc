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
#include <string>
#include <sstream>

#include <sys/time.h>
#include <unistd.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <getopt.h>

#include "General/Debug.hh"
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

	Scene::Scene S(Camera(V1, V2), ColLib::Black());

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

	Render::Raytracer R(S, true);

	std::cout << "Raytracing with " << S.GetCamera();

	R.Render(Scr);
}

void Render2(Graphics::Screen &Scr)
{
	using namespace World;

	const Math::Vector Pos(-2.0, 3.0, -2.0);
	const Math::Vector Dir(0.2, -0.3, 1.0);
	Scene::Scene S(Camera(Pos, Dir), ColLib::Black());

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

	Render::Raytracer R(S, true);

	std::cout << "Raytracing with " << S.GetCamera();

	R.Render(Scr);
}

void Render3(Graphics::Screen &Scr)
{
	using namespace World;
	Scene S;
	if (S.ParseFile("Examples/Scene1.xml") == false)
		throw 42;
	Render::Raytracer R(S, true);
	R.Render(Scr);
}

static void Demo()
{
}

static void Help()
{
	using namespace std;
	cout
	<< "Usage: ./blaRAY --demo" << endl
	<< "       ./blaRAY [-x width] [-y height] [-a] --file <path>" << endl
	<< "List of options:" << endl
	<< "	--scene|-s <filename>	- Scene description to render" << endl
	<< "	--demo			- Render demo scene instead of file" << endl
	<< "	--width|-x <arg>	- sets screen width (default:640)" << endl
	<< "	--height|-y <arg>	- sets screen height (default:480)" << endl
	<< "	--antialiasing|-a	- Turn antialiasing on" << endl
	<< "	--help|-h		- Show this help" << endl
	<< endl
	<< "blaRAY (C) 2008 by Tomasz bla Fortuna" << endl;
}

int main(int argc, char **argv)
{
	using namespace std;
	enum { WIDTH=0, HEIGHT, SCENE, ANTIALIASING, DEMO, HELP };
	static struct {
		Int Width;
		Int Height;
		std::string Filename;
		Bool Antialiasing;
	} Configuration = {
		640, 480, "", false
	};

	static struct option long_options[] = {
		{"width", 1, 0, 0},
		{"height", 1, 0, 0},
		{"scene", 1, 0, 0},
		{"antialiasing", 0, 0, 0},
		{"demo", 0, 0, 0},
		{"help", 0, 0, 0},
		{NULL, 0, 0, 0}
	};

	for (;;) {
		int c, index;
		c = getopt_long(argc, argv, "x:y:s:adh",
				long_options, &index);
		if (c == -1)
			break; /* End of parameters */

		cout << (int)c << " == option" << " index:" << index << endl;

		switch (c) {
		case 0: break;
		case 'x': index = WIDTH; break;
		case 'y': index = HEIGHT; break;
		case 's': index = SCENE; break;
		case 'a': index = ANTIALIASING; break;
		case 'd': index = DEMO; break;
		case 'h': index = HELP; break;
		}

		std::string opt("");
		if (optarg) opt = optarg;
		stringstream s(opt);

		switch (index) {
		case WIDTH:
			s >> Configuration.Width;
			break;
		case HEIGHT:
			s >> Configuration.Height;
			break;
		case SCENE:
			s >> Configuration.Filename;
			break;
		case ANTIALIASING:
			Configuration.Antialiasing = true;
			break;

		case DEMO:
			Demo();
			break;

		case HELP:
			Help();
			return 0;
		}
	}

	cout << "Configuration:" << endl
	     << "Width = " << Configuration.Width << endl
	     << "Height= " << Configuration.Height << endl
	     << "SceneFile = " << Configuration.Filename << endl
	     << "Antialiasing = " << (Configuration.Antialiasing
				      ? "true" : "false") << endl;

	return 0;


	struct timeval A, B;
	if (DEBUG)
		Testcases::All();

	/* Render something */
	Graphics::Screen Scr(640, 480);
/*	Graphics::Screen Scr(50, 50); */

	gettimeofday(&A, NULL);

	Render3(Scr);

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

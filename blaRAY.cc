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

/** Demo function */
static void Render1(Graphics::Screen &Scr, Bool Antialiasing)
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

	Render::Raytracer R(S, Antialiasing);

	std::cout << "Raytracing with " << S.GetCamera();

	R.Render(Scr);
}

/** Second demo function */
static void Render2(Graphics::Screen &Scr, Bool Antialiasing)
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

	Render::Raytracer R(S, Antialiasing);

	std::cout << "Raytracing with " << S.GetCamera();

	R.Render(Scr);
}

/** Render scene described in XML file */
static void RenderFile(Int Width, Int Height, 
		       Bool Antialiasing,
		       const std::string &SceneFile,
		       const std::string &OutputFile)
{
	using namespace World;
	struct timeval A, B;
	Scene S;
	if (S.ParseFile(SceneFile) == false) {
		std::cout 
			<< "Error while parsing file, finishing" 
			<< std::endl;
		return;
	}

	Graphics::Screen Scr(Width, Height);
	Render::Raytracer R(S, Antialiasing);

	gettimeofday(&A, NULL);
	R.Render(Scr);
	gettimeofday(&B, NULL);

	Double ATime = A.tv_sec + 0.000001 * A.tv_usec;
	Double BTime = B.tv_sec + 0.000001 * B.tv_usec;

	std::cout << "*** Rendering took "
		  << BTime - ATime
		  << " seconds" << std::endl;

	Scr.Refresh();
	if (OutputFile != "")
		Scr.Save(OutputFile);
	Scr.EventWait();

}

/** Handle demo selection */
static void Demo(Int Width, Int Height,
		 Bool Antialiasing, Int Which, const std::string &Output)
{
	std::cout << "*** Rendering demo " << Which << std::endl;
	/* Render something */
	Graphics::Screen Scr(Width, Height);

	struct timeval A, B;
	gettimeofday(&A, NULL);
	switch ((const int)Which) {
	case 1:	Render1(Scr, Antialiasing);
		break;
	case 2:	Render2(Scr, Antialiasing);
		break;
	default:
		std::cout << "Wrong demo specified. "
			  << "Possible values: 1, 2" << std::endl;
		return;
	}
	gettimeofday(&B, NULL);

	Double ATime = A.tv_sec + 0.000001 * A.tv_usec;
	Double BTime = B.tv_sec + 0.000001 * B.tv_usec;

	std::cout << "*** Rendering took "
		  << BTime - ATime
		  << " seconds" << std::endl;

	Scr.Refresh();
	if (Output != "")
		Scr.Save(Output);
	Scr.EventWait();
}

/** Prints help message */
static void Help()
{
	using namespace std;
	cout
	<< "Usage: ./blaRAY --demo" << endl
	<< "       ./blaRAY [-x width] [-y height] [-a] --file <path>" << endl
	<< "List of options:" << endl
	<< "	--scene|-s <filename>	- Scene description to render" << endl
	<< "	--demo|-d <num>		- Render demo 1 or 2 instead of a file" << endl
	<< "	--output|-o <filename>	- Output rendered scene to filename (BMP)" << endl
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
	enum { WIDTH=0, HEIGHT, SCENE, OUTPUT, ANTIALIASING, DEMO, HELP };
	static struct {
		Int Width;
		Int Height;
		std::string SceneFile;
		std::string OutputFile;
		Bool Antialiasing;
		Int Demo;
	} Configuration = {
		640, 480, "", "", false, 0
	};

	static struct option long_options[] = {
		{"width", 1, 0, 0},
		{"height", 1, 0, 0},
		{"scene", 1, 0, 0},
		{"output", 1, 0, 0},
		{"antialiasing", 0, 0, 0},
		{"demo", 1, 0, 0},
		{"help", 0, 0, 0},
		{NULL, 0, 0, 0}
	};

	for (;;) {
		int c, index;
		c = getopt_long(argc, argv, "x:y:s:o:ad:h",
				long_options, &index);
		if (c == -1)
			break; /* End of parameters */

		cout << (int)c << " == option" << " index:" << index << endl;

		switch (c) {
		case 0: break;
		case 'x': index = WIDTH; break;
		case 'y': index = HEIGHT; break;
		case 's': index = SCENE; break;
		case 'o': index = OUTPUT; break;
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
			s >> Configuration.SceneFile;
			break;
		case OUTPUT:
			s >> Configuration.OutputFile;
			break;

		case ANTIALIASING:
			Configuration.Antialiasing = true;
			break;

		case DEMO:
			s >> Configuration.Demo;
			break;

		case HELP:
			Help();
			return -1;
		}
	}

	if (DEBUG)
		cout << "Configuration:" << endl
		     << "Width = " << Configuration.Width << endl
		     << "Height= " << Configuration.Height << endl
		     << "SceneFile = " << Configuration.SceneFile << endl
		     << "OutputFile = " << Configuration.OutputFile << endl
		     << "Demo = " << Configuration.Demo << endl
		     << "Antialiasing = " << (Configuration.Antialiasing
					      ? "true" : "false") << endl;

	if (Configuration.Demo != 0) {
		Demo(Configuration.Width,
		     Configuration.Height,
		     Configuration.Antialiasing,
		     Configuration.Demo,
		     Configuration.OutputFile);
		return 0;
	}

	if (Configuration.SceneFile == "") {
		cout << "ERROR: You must specify scene file to render (or --demo)"
		     << endl;
		return -1;
	}

	if (DEBUG)
		Testcases::All();

	/* Render something */
	RenderFile(Configuration.Width,
		   Configuration.Height,
		   Configuration.Antialiasing,
		   Configuration.SceneFile,
		   Configuration.OutputFile);
	return 0;
}

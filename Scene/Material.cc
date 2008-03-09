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

#include "General/Debug.hh"

#include "Scene/Texture.hh"
#include "Scene/Material.hh"


namespace Scene {

	std::ostream &operator<<(std::ostream &os, const Material &M)
	{
		os << "[Material Diffuse=" << M.Diffuse << std::endl
		   << "*\tSpecular=" << M.Specular << std::endl
		   << "*\tReflect=" << M.Reflect << std::endl
		   << "*\tRefract=" << M.Refract << std::endl
		   << "*\t" << M.Reflective
		   << "/" << M.Refractive
		   << "/" << M.Absorptive
		   << "/" << M.Shininess << std::endl;
		return os;
	}

	const Material &MatLib::Red()
	{
		static Material M(TexLib::Red());
		return M;
	}

	const Material &MatLib::Green()
	{
		static Material M(TexLib::Green());
		return M;
	}

	const Material &MatLib::Blue()
	{
		static Material M(TexLib::Blue());
		return M;
	}

	const Material &MatLib::White()
	{
		static Material M(TexLib::White());
		return M;
	}

	const Material &MatLib::Black()
	{
		static Material M(TexLib::Black());
		return M;
	}

	const Material &MatLib::Gray()
	{
		static Material M(TexLib::Gray());
		return M;
	}

	const Material &MatLib::Glass()
	{
		static const TexLib::Plain Diff = TexLib::Plain(Color(0.0, 0.0, 0.2));
		static const TexLib::Plain Spec = TexLib::Plain(Color(0.8, 0.8, 0.8));
		static const TexLib::Plain Refr = TexLib::Plain(Color(0.8, 0.8, 0.8));
		static const TexLib::Plain Refl = TexLib::Plain(Color(0.1, 0.1, 0.1));

		static Material M(Diff, Spec, Refr, Refl,
				  0.9, 0.9, 1.0, 7.8, IdxGlass);
		return M;
	}
};

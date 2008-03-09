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
#include <cmath>

#include "General/Types.hh"
#include "General/Debug.hh"
#include "World/Camera.hh"
#include "Math/Abs.hh"

namespace World {
	Camera::Camera(const Math::Vector &Pos, const Math::Vector &Dir,
		       Double FOV, Bool AutoTop,
		       const Math::Vector &Top)
		: FOV(FOV), Pos(Pos), Dir(Dir)
	{
	/*
	  Auto top calculations:
	  load(vect);
	  [ cross([Tx, Ty, Tz], [Dx, Dy, Dz])[2] = 0, Tx*Dx + Ty*Dy + Tz*Dz = 0, Tx = Dx];

	  Ty = - (Dz*Dz + Dx*Dx) / Dy
	  Tz = Dz
	*/
		if (AutoTop == false) {
			this->Top = Top;
			this->Top.Normalize();
		}
		else {
			Double x=Dir[0], y=Dir[1], z=Dir[2];
			Double w;
			w = - (z*z + x*x) / y;
			this->Top.Set(x, w, z);
			this->Top.Normalize();
			if (Math::Abs(y) < 0.0001)
				this->Top[Math::Vector::Y] = 1.0;
		}

	}

	Camera::View Camera::CreateView(Int XRes, Int YRes) const
	{
		/* Calculate 'world' dimensions of camera screen */
		Double Ratio = (double)XRes / YRes;
		Double XWidth = std::tan(this->FOV / 2.0);
		Double YWidth = XWidth / Ratio;

		/* Calculate 'world' distance between camera screen pixels */
		Double XDist = XWidth / (double)XRes;
		Double YDist = YWidth / (double)YRes;

		/* Calculate X and Y movement vector */
		Math::Vector XVect = Top.Cross(Dir).Normalize() * XDist;
		Math::Vector YVect = Top * YDist;
		std::cout << Top << std::endl;
		if (DEBUG)
			std::cout << "CreateView: " << std::endl
				  << "\tworld dimensions: "
				  << XWidth << " x " << YWidth << std::endl
				  << "\tworld pixel distance: "
				  << XDist << " x " << YDist << std::endl
				  << "\tXVect: " << XVect << std::endl
				  << "\tYVect: " << YVect << std::endl;

		return View(XRes, YRes, Pos, Dir, XVect, YVect);
	}

	Render::Ray Camera::View::At(Int x, Int y) const
	{
		/* Calculate the vector pointing
		   at (x,y) point on camera screen */
		Math::Vector Base =
			(XVect * (x - XResHalf))   +
			(YVect * (YResHalf - y));

		/* Ray starts from camera position,
		 * goes through (x,y) screen point */
		return Render::Ray(Pos, Base + Dir);
	}

	std::ostream &operator<<(std::ostream &os, const Camera &C)
	{
		os << "[Camera FOV=" << C.FOV << std::endl
		   << "*\tPos=" << C.Pos << std::endl
		   << "*\tDir=" << C.Dir << std::endl
		   << "*\tTop=" << C.Top << std::endl
		   << "* ]" << std::endl;
		return os;
	}

}

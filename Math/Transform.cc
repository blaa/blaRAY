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

#include <math.h>
#include "iostream"
#include "Transform.hh"

namespace Math {
	Transform::Transform()
	{
		LoadIdentity();
	}

	Transform::Transform(const Matrix &M)
	{
		this->Set(M);
	}

	Transform::Transform(Double x, Double y, Double z)
	{
		Translate(x, y, z);
	}

	Transform::Transform(Direction D, Double Angle)
	{
		switch (D) {
		case ROT_X:
			RotateX(Angle);
			return;

		case ROT_Y:
			RotateY(Angle);
			return;

		case ROT_Z:
			RotateZ(Angle);
			return;
		}
	}
	
	void Transform::RotateX(Double Angle)
	{
		/* X rotation: 
		 * [ 1    0    0    0 ]
		 * [ 0    cos  sin  0 ]
		 * [ 0    -sin cos  0 ]
		 * [ 0    0    0    1 ]
		 */
		Double s = sin(Angle), c = cos(Angle);
		LoadIdentity();
		SetXY(1, 1, c);
		SetXY(2, 1, s);
		SetXY(1, 2, -s);
		SetXY(2, 2, c);
	}

	void Transform::RotateY(Double Angle)
	{
		/* Y rotation:
		 * [ cos  0    -sin 0 ]
		 * [ 0    1    0    0 ]
		 * [ sin  0    cos  0 ]
		 * [ 0    0    0    1 ]
		 */
		Double s = sin(Angle), c = cos(Angle);
		LoadIdentity();
		SetXY(0, 0, c);
		SetXY(2, 0, -s);
		SetXY(0, 2, s);
		SetXY(2, 2, c);
	}

	void Transform::RotateZ(Double Angle)
	{
		/* Z rotation:
		 * [ cos  sin  0    0 ]
		 * [ -sin cos  0    0 ]
		 * [ 0    0    1    0 ]
		 * [ 0    0    0    1 ]
		 */
		Double s = sin(Angle), c = cos(Angle);
		LoadIdentity();
		SetXY(0, 0, c);
		SetXY(1, 0, s);
		SetXY(0, 1, -s);
		SetXY(1, 1, c);
	}

	void Transform::Translate(Double x, Double y, Double z)
	{
		LoadIdentity();
		SetXY(3,0, x);
		SetXY(3,1, y);
		SetXY(3,2, z);
	}

	void Transform::Resize(Double x, Double y, Double z)
	{
		LoadIdentity();
		SetXY(0,0, x);
		SetXY(1,1, y);
		SetXY(2,2, z);
	}
};


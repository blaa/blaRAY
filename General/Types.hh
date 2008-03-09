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

#ifndef _TYPES_H_
#define _TYPES_H_

#include <iostream>
#include "General/Debug.hh"

/** Original template
 *	Copyright (c) 2006
 *	Michal 'Sektor' Malecki
 *	Pawel Sikora
 *	Distributed under the Boost Software License, Version 1.0.
 *	( See at http://www.boost.org/LICENSE_1_0.txt )
 *
 *	template<class T>
 *	struct explicit_t
 *	{
 *	private:
 *		T value;
 *		template< class V > explicit_t( V t );
 *	public:
 *		operator T & () { return value; }
 *		explicit_t( const T& c ): value( c ) { }
 *	};
 **/

/**
 * \author Michal 'Sektor' Malecki
 * \author Pawel Sikora
 * \author Tomasz bla Fortuna
 * \date 2006, 2008
 *
 * \brief Kills implicit conversions.
 *
 * Original license:
 *	Distributed under the Boost Software License, Version 1.0.
 *	( See at http://www.boost.org/LICENSE_1_0.txt )
 *
 * Template allows us to kill any implicit conversions
 * of built-in types. Especially int, short, unsigned *
 * and double. Modifications allows to initialize arrays
 * in classes and allows us creating T types from const T types.
 *
 */
template<typename T>

struct explicit_t {
private:
	/** Private constructor from any other type */
	template<typename V> explicit_t(const V &t);
	T value;
public:

	/**@{ Constructor from allowed types */
	explicit_t(const T &c) : value(c) {}
	explicit_t() {};
	/*@}*/

	/** Returns type */
	operator T& () { return value; }
	operator const T& () const { return value; }
};

#if (DEBUG == 1)
#	define e_t(t) explicit_t<t>
#else
#	define e_t(t) t
#endif

/**@{ Basic types (not const!) */
typedef e_t(int)		Int;
typedef e_t(double)		Double;
typedef e_t(char)		Char;
typedef e_t(bool)		Bool;
typedef e_t(short)		Short;
typedef e_t(unsigned int)	UInt;
typedef e_t(unsigned short)	UShort;
typedef e_t(unsigned char)	UChar;
typedef e_t(double)		Double;
typedef void			Void;
/*@}*/

#endif

/*
	math3d++ - A C++ 3d math library
	Copyright (c) 2004, Trenkwalder Markus
	All rights reserved. 
	
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:
	
	- Redistributions of source code must retain the above copyright notice,
	  this list of conditions and the following disclaimer.
	  
	- Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	  
	- Neither the name of the math3d++'s copyright owner nor the names
	  of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.
	  
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
	TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
	PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
	LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	
	Contact info:
	email: trenki2@gmx.net web: trenki.50free.org
*/

#ifndef MATH3DPP_H
#define MATH3DPP_H

/** @file
 ** Main file of the math3d++ library 
 */

#include <cmath>
#include <limits>

/// All the classes and functions of the library are included in this namespace
namespace math3d {
	
	template <class _T>
	inline bool fequal(_T a, _T b) {
		return std::abs(a - b) <= std::numeric_limits<_T>::epsilon();
	}
	
	// if defined the function fequal defined above will be
	// used to compare for equality of vectors, matrices and quaternions
	// #define USE_FEQUAL_COMPARE
	
	/// The type Real is used in the whole math3d++ library
	typedef float Real;
	
	#include "vec3.h"
	#include "vec4.h"
	
	#include "mat3.h"
	#include "mat4.h"
	
	#include "matvec_util.h"
	
	#include "quat.h"
	#include "quat_util.h"

}	// end namespace

/** @mainpage
 * Documentation for the main page has still to be done.:-)
 */

#endif

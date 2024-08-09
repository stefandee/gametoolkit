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

#ifndef MATH3DPP_MATVEC_UTIL_H
#define MATH3DPP_MATVEC_UTIL_H

/******************************************************************************/

/// Multiplies the vector v1 by the transposed vector v2 and returns the
/// resulting matrix.
/// <PRE>
///   x1                    m00 m01 m02
/// ( y1 ) * (x2 y2 z2) = ( m10 m11 m12 )
///   z1                    m20 m21 m22
/// </PRE>
inline mat3 multvvt(const vec3& v1, const vec3& v2) {
	mat3 r;
	
	for ( int j = 0; j < 3; ++j )
	for ( int k = 0; k < 3; ++k )
		r[j][k] = v1[j] * v2[k];
	
	return r;
}

/// Multiplies the vector v1 by the transposed vector v2 and returns the
/// resulting matrix.
/// <PRE>
///   x1                       m00 m01 m02 m03
/// ( y1 ) * (x2 y2 z2 w2) = ( m10 m11 m12 m13 )
///   z1                       m20 m21 m22 m13
///   w1                       m30 m31 m32 m33
/// </PRE>
inline mat4 multvvt(const vec4& v1, const vec4& v2) {
	mat4 r;
	
	for ( int j = 0; j < 4; ++j )
	for ( int k = 0; k < 4; ++k )
		r[j][k] = v1[j] * v2[k];
	
	return r;
}

/******************************************************************************/
// matrix utility functions

/// Returns a translation matrix
inline mat4 translate(Real x, Real y, Real z) {
	return mat4(
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
	);
}

/// Returns a translation matrix
inline mat4 translate(const vec3& v) {
	return translate(v[0], v[1], v[2]);
}


/// Returns a scaling matrix
inline mat4 scale(Real x, Real y, Real z) {
return mat4(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	);
}

/// Returns a scaling matrix
inline mat4 scale(const vec3& v) {
	return scale(v[0], v[1], v[2]);
}

/// Returns a frustum transformation matrix
inline mat4 frustum(Real l, Real r, Real b, Real t, Real n, Real f) {
	return mat4(
		(2.0*n)/(r-l), 0.0, (r+l)/(r-l), 0.0,
		0.0, (2.0*n)/(t-b), (t+b)/(t-b), 0.0,
		0.0, 0.0, -(f+n)/(f-n), -(2.0*f*n)/(f-n),
		0.0, 0.0, -1.0, 0.0
	);
}

/// Returns an orthograhic transformation matrix
inline mat4 ortho(Real l, Real r, Real b, Real t, Real n, Real f) {
	return mat4(
		2.0/(r-l), 0.0, 0.0, -(r+l)/(r-l),
		0.0, 2.0/(t-b), 0.0, -(t+b)/(t-b),
		0.0, 0.0, -2.0/(f-n), -(f+n)/(f-n),
		0.0, 0.0, 0.0, 1.0
	);
}

/// Returns a rotation matrix
inline mat4 rotate(Real angle, const vec3& v) {
	Real a = angle * M_PI / 180.0;
	vec3 u = normalized(v);
	
	mat3 S(
		    0, -u[2],  u[1],
		 u[2],     0, -u[0],
		-u[1],  u[0],     0 
	);
	
	mat3 uut = multvvt(u, u);
	mat3 R = uut + std::cos(a) * (identity3() - uut) + std::sin(a) * S;
	
	return mat4(R);
}

/// Returns a rotation matrix
inline mat4 rotate(Real a, Real x, Real y, Real z) {
	return rotate(a, vec3(x, y, z));
}

/******************************************************************************/

/// matrix * vector multiplication
inline vec3 operator* (const mat3& m, const vec3& v) {
	vec3 r;
	
	for ( int i = 0; i < 3; ++i ) {
		r[i] = 0;
		for ( int j = 0; j < 3; ++j )
			r[i] += m[i][j] * v[j];
	}
	
	return r;
}

/// matrix * vector multiplication
inline vec4 operator* (const mat4& m, const vec4& v) {
	vec4 r;
	
	for ( int i = 0; i < 4; ++i ) {
		r[i] = 0;
		for ( int j = 0; j < 4; ++j )
			r[i] += m[i][j] * v[j];
	}
	
	return r;
}

/******************************************************************************/
/// Return a minor of the supplied 4x4 matrix by omitting the _jth row
/// and _kth column.
/// It defaults to return the left upper 3x3 matrix
inline mat3 minor(const mat4& m, int _j = 3, int _k = 3) {
	mat3 r;
	
	for ( int j = 0; j < 3; ++j )
	for ( int k = 0; k < 3; ++k ) {
		int jp = (j >= _j) ? 1 : 0;
		int kp = (k >= _k) ? 1 : 0;
		r[j][k] = m[j + jp][k + kp];
	}
	
	return r;
}

/******************************************************************************/

/// Returns the inverse of a 4x4 matrix. It is assumed that the matrix passed
/// as argument describes a transformation that preserves lengths and angles i.e.
/// matrices composed only from rotations and translations (no scaling,
/// projection, sheering, ...)
inline mat4 fastInverse(const mat4& m) {
	// upper 3x3 is transposed
	return mat4(
		m[0][0], m[1][0], m[2][0], -m[0][3],
		m[0][1], m[1][1], m[2][1], -m[1][3],
		m[0][2], m[1][2], m[2][2], -m[2][3],
		0, 0, 0, 1
	);
}

/******************************************************************************/

#endif

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

#ifndef MATH3DPP_VEC4_H
#define MATH3DPP_VEC4_H

/// 4d vector class
class vec4 {
private:
	friend class vec3;
	Real data[4];
public:
	vec4();
	vec4(Real x, Real y, Real z, Real w);
	
	explicit vec4(const vec3& v);
	vec3 toVec3();
	
	vec4& operator+= (const vec4& v);
	vec4& operator-= (const vec4& v);
	
	vec4& operator*= (Real s);	
	vec4& operator/= (Real s);
	
	operator Real* ();
	operator const Real* () const;
};

// constructor

/// initialization to (0,0,0,0)
inline vec4::vec4() {
	for ( int i = 0; i < 4; ++i )
		data[i] = 0.0;
}

/// initialization to (x,y,z,w)
inline vec4::vec4(Real x, Real y, Real z, Real w) {
	data[0] = x;
	data[1] = y;
	data[2] = z;
	data[3] = w;
}

/// Construction from a 3d vector.
/// x = v.x; y = v.y; z = v.z; w = 1
inline vec4::vec4(const vec3& v) {
	data[0] = v[0];
	data[1] = v[1];
	data[2] = v[2];
	data[3] = 1;	// w
}

/// conversion to 3d vector by omitting the w coordinate
inline vec3 vec4::toVec3() {
	return vec3(data[0], data[1], data[2]);
}

// Vector arithmetic operators

inline vec4& vec4::operator+= (const vec4& v) {
	for ( int i = 0; i < 4; ++i )
		data[i] += v.data[i];
		
	return *this;
}


inline vec4& vec4::operator-= (const vec4& v) {
	for ( int i = 0; i < 4; ++i )
		data[i] -= v.data[i];
		
	return *this;
}


inline vec4& vec4::operator*= (Real s) {
	for ( int i = 0; i < 4; ++i )
		data[i] *= s;
		
	return *this;
}


inline vec4& vec4::operator/= (Real s) {
	for ( int i = 0; i < 4; ++i )
		data[i] /= s;
		
	return *this;
}

/// Conversion operator for subscripting.
/// You can write v[3] = value and also value = v[3]
inline vec4::operator Real* () {
	return data;
}

/// Conversion operator for subscripting.
/// You can write value = v[3]
inline vec4::operator const Real* () const {
	return data;
}

/******************************************************************************/
// non-member operators

/// @relates vec4
/// Vector addition
inline vec4 operator+ (const vec4& a, const vec4& b) {
	vec4 r = a;
	return r += b;
}

/// @relates vec4
/// Vector subtraction
inline vec4 operator- (const vec4& a, const vec4& b) {
	vec4 r = a;
	return r -= b;
}

/// @relates vec4
/// Multiplication by a scalar
inline vec4 operator* (Real s, const vec4& v) {
	vec4 r = v;
	return r *= s;
}

/*
inline vec4 operator* (const vec4& v, Real s) {
	vec4 r = v;
	return r *= s;
}
*/

/// @relates vec4
/// Divides each vector component by the scalar s
inline vec4 operator/ (const vec4& v, Real s) {
	vec4 r = v;
	return r /= s;
}

/// @relates vec4
/// Unary minus
inline vec4 operator- (const vec4& v) {
	vec4 r = v;
	for ( int i = 0; i < 4; ++i )
		r[i] = -r[i];

	return r;
}

/// @relates vec4
/// Vector comparison
inline bool operator== (const vec4& a, const vec4& b) {
	for ( int i = 0; i < 3; ++i ) {
	#ifndef USE_FEQUAL_COMPARE
		if ( a[i] != b[i] ) return false;
	#else
		if ( !fequal(a[i], b[i]) ) return false;
	#endif
	}
	
	return true;
}

/// @relates vec4
/// Vector comparison
inline bool operator!= (const vec4& a, const vec4& b) {
	return !operator==(a,b);
}

/// @relates vec4
/// Vector dot product
inline Real operator* (const vec4& a, const vec4& b) {
	Real dotprod = 0.0;
	for ( int i = 0; i < 4; ++i )
		dotprod += a[i] * b[i];
	
	return dotprod;
}

/// @relates vec4
/// Vector dot product
inline Real dot(const vec4& a, const vec4&b) {
	return a * b;
}

/// @relates vec4
/// Returns length^2
inline Real length2(const vec4& v) {
	Real l = 0.0;
	for ( int i = 0; i < 4; ++i )
		l += v[i] * v[i];
	
	return l;
}

/// @relates vec4
/// Returns the length of the vector
inline Real length(const vec4& v) {
	return std::sqrt(length2(v));
}

/// @relates vec4
/// Returns the vector scaled to unit length
inline vec4 normalized(const vec4& v)  {
	Real l = length(v);
	
	if ( l != 0.0 ) {
		vec4 result(v[0] / l, v[1] / l, v[2] / l, v[3] / l);
		return result;
	}
	
	return v;
}

/// @relates vec4
/// Scales the vector passed as argument to unit length and return a reference
inline vec4& normalize(vec4& v) {
	Real l = length(v);
	
	if ( l != 0.0 ) {
		v[0] /= l;
		v[1] /= l;
		v[2] /= l;
		v[3] /= l;
	}
	
	return v;
}

#endif

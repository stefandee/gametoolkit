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

#ifndef MATH3DPP_QUAT_H
#define MATH3DPP_QUAT_H

/// Quaternion class
class quat {
	friend quat conjugate(const quat& q);
	friend Real norm(const quat& q);
	friend quat& normalize(quat& q);
	friend quat normalized(const quat& q);
	
	friend quat quatFromAA(const vec3& axis, Real angle);
	friend void quatToAA(const quat& q, vec3* axis, Real* angle);
	
	// scalar/vector representation
	// [w,(x,y,z)]
	//  s    v
	Real s;
	vec3 v;
public:
	quat();
	quat(Real q0, Real q1, Real q2, Real q3);
	
	quat& operator+= (const quat& q);
	quat& operator-= (const quat& q);
	quat& operator*= (const quat& q);
	quat& operator/= (const quat& q);

	quat& operator*= (Real f);
	quat& operator/= (Real f);
	
	Real& operator[] (int i);
	Real  operator[] (int i) const;
};

// constructors

/// Constructs the unit quaternion (0,1,0,0)
inline quat::quat()
	: s(0), v(1,0,0)
{}


inline quat::quat(Real q0, Real q1, Real q2, Real q3)
	: s(q0), v(q1, q2, q3)
{}


// quaternion operators
inline quat& quat::operator+= (const quat& q) {
	s += q.s;
	v += q.v;
	return *this;
}

inline quat& quat::operator-= (const quat& q) {
	s -= q.s;
	v -= q.v;
	return *this;
}

inline quat& quat::operator*= (const quat& q) {
	//q1 x q2 = [s1,v1] x [s2,v2] = [(s1*s2 - v1*v2),(s1*v2 + s2*v1 + v1xv2)].
	quat r;
	
	r.s = s * q.s - v * q.v;
	r.v = s * q.v + q.s * v + cross(v, q.v);
	
	*this = r;
	
	return *this;
}

// forward declaration of quaternion inverse (needed for division)
/// @relates quat
/// Returns the quaternion inverse
quat inverse(const quat& q);

inline quat& quat::operator/= (const quat& q) {
	return (*this) *= inverse(q);
}

inline quat& quat::operator*= (Real f) {
	s *= f;
	v *= f;
	
	return *this;
}

inline quat& quat::operator/= (Real f) {
	s /= f;
	v /= f;
	
	return *this;
}

/// subscription operators for accessing the individual data elements directly
/// [w,(x,y,z)] = [ q[0], q[1], q[2], q[3] ]
inline Real& quat::operator[] (int i) {
	if ( i == 0 )
		return s;
	else
		return v[i - 1];
}

/// subscription operators for accessing the individual data elements directly
/// [w,(x,y,z)] = [ q[0], q[1], q[2], q[3] ]
inline Real quat::operator[] (int i) const {
	if ( i == 0 )
		return s;
	else
		return v[i - 1];
}

// non-member operators

/// @relates quat
/// Quaternion addition
inline quat operator+ (const quat& q1, const quat& q2) {
	quat r = q1;
	return r += q2;
}

/// @relates quat
/// Quaternion subtraction
inline quat operator- (const quat& q1, const quat& q2) {
	quat r = q1;
	return r -= q2;
}

/// @relates quat
/// Quaternion multiplication
inline quat operator* (const quat& q1, const quat& q2) {
	quat r = q1;
	return r *= q2;
}

/// @relates quat
/// Multiplication by a scalar (s * q)
inline quat operator* (Real f, const quat& q) {
	quat r = q;
	return r *= f;
}

/// @relates quat
/// Division by a scalar
inline quat operator/ (const quat& q, Real f) {
	quat r = q;
	return r /= f;
}

/// @relates quat
/// Quaternion division
inline quat operator/ (const quat& q1, const quat& q2) {
	quat r = q1;
	return r /= q2;
}

/// @relates quat
/// Quaternion comparison
inline bool operator== (const quat& q1, const quat& q2) {
	for ( int i = 0; i < 4; ++i ) {
	#ifndef USE_FEQUAL_COMPARE
		if ( q1[i] != q2[i] ) return false;
	#else
		if ( !fequal(q1[i], q2[i]) ) return false;
	#endif
	}
	
	return true;
}

/// @relates quat
/// Quaternion comparison
inline bool operator!= (const quat& q1, const quat& q2) {
	return !operator==(q1,q2);
}

/*******************************************************************************/

/// @relates quat
/// Returns length^2
inline Real length2(const quat& q) {
	Real l = 0;
	for ( int i = 0; i < 4; ++i )
		l += q[i] * q[i];
	
	return l;
}


/// @relates quat
/// Returns the quaternion length.
/// The length is calculated by interpreting the quaternion as a 4d vector.
inline Real length(const quat& q) {
	return std::sqrt(length2(q));
}

/// @relates quat
/// Returns the quaternion scaled to unit length
inline quat normalized(const quat& q)  {
	Real l = length(q);
	
	if ( l != 0 ) {
		quat r;
		r.s = q.s / l;
		r.v = q.v / l;
		
		return r;
	}

	return q;
}

/// @relates quat
/// Normalizes the passed quaternion and returns a reference
inline quat& normalize(quat& q) {
	Real l = length(q);
	
	if ( l != 0 ) {
		q.s /= l;
		q.v /= l;
	}
	
	return q;
}

/// @relates quat
/// NOTE: The function returns length^2 of the quaternion
inline Real norm(const quat& q) {
	return length(q);
}

/// @relates quat
/// Returns the quaternion conjugate
inline quat conjugate(const quat& q) {
	return quat(q.s, -q.v[0], -q.v[1], -q.v[2]);
}

/// @relates quat
/// Returns the quaternion inverse
inline quat inverse(const quat& q) {
	Real n = norm(q);
	
	if ( n > 0 ) {
		return conjugate(q) / n;
	}
	else {
		return quat(0, 0, 0, 0);
	}
}

#endif

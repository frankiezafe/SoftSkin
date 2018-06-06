/*
 *  
 *  
 *  _________ ____  .-. _________/ ____ .-. ____ 
 *  __|__  (_)_/(_)(   )____<    \|    (   )  (_)
 *                  `-'                 `-'      
 *  
 *  
 *  art & game engine
 *  
 *  ____________________________________  ?   ____________________________________
 *                                      (._.)
 *  
 *  
 *  This file is part of softskin library
 *  For the latest info, see http://polymorph.cool/
 *  
 *  Copyright (c) 2018 polymorph.cool
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *  
 *  ___________________________________( ^3^)_____________________________________
 *  
 *  ascii font: rotated by MikeChat & myflix
 *  have fun and be cool :)
 *  
 * 
 */

#include "vector3ptr.h"

Vector3ptr::Vector3ptr() : 
	_x( 0 ), 
	_y( 0 ), 
	_z( 0 ), 
	_local(false),
	_inititalised(false)
{}

Vector3ptr::Vector3ptr( const Vector3* v3 ) : 
	_local(false),
	_inititalised(false)
{
	
	init( v3 );
	
}

Vector3ptr::Vector3ptr( const real_t& x, const real_t& y, const real_t& z ): 
	_local(true) ,
	_inititalised(false)
{
	
	_x = new real_t(x);
	_y = new real_t(y);
	_z = new real_t(z);
	
}

Vector3ptr::~Vector3ptr() {
	
	purge();
	
}

void Vector3ptr::purge() {
	
	if ( _local ) {
		delete _x;
		delete _y;
		delete _z;
	}
	
	_x = 0;
	_y = 0;
	_z = 0;
	_local = false;
	_inititalised = false;
	
}

void Vector3ptr::init( const Vector3* v3 ) {
	
	purge();
	
	if ( v3 ) {
		_local = false;
		Vector3* v = (Vector3*) v3;
		_x = &(v->coord[0]);
		_y = &(v->coord[1]);
		_z = &(v->coord[2]);
		_inititalised = true;
	}
	
}

void Vector3ptr::init( const real_t& x, const real_t& y, const real_t& z ) {
	
	purge();
	_local = true;
	_x = new real_t(x);
	_y = new real_t(y);
	_z = new real_t(z);
	_inititalised = true;
	
}

void Vector3ptr::set( const real_t& x, const real_t& y, const real_t& z ) {
	
	assert( _inititalised );
	
	(*_x) = x;
	(*_y) = y;
	(*_z) = z;
	
}

void Vector3ptr::operator = ( const Vector3& v3 ) {
	
	assert( _inititalised );
	
	(*_x) = v3.x;
	(*_y) = v3.y;
	(*_z) = v3.z;
	
}

void Vector3ptr::operator = ( const Vector3ptr& v3ptr ) {
	
	assert( _inititalised );
	
	(*_x) = v3ptr.x();
	(*_y) = v3ptr.y();
	(*_z) = v3ptr.y();
	
}

void Vector3ptr::operator += (const Vector3& v3 ) {
	
	assert( _inititalised );
	
	(*_x) += v3.x;
	(*_y) += v3.y;
	(*_z) += v3.z;
	
}

void Vector3ptr::operator -= (const Vector3& v3 ) {
	
	assert( _inititalised );
	
	(*_x) -= v3.x;
	(*_y) -= v3.y;
	(*_z) -= v3.z;
	
}

const real_t& Vector3ptr::operator [] ( const uint8_t& i ) const {
	
	assert( _inititalised );
	assert(i>2);
	
	switch( i ) {
		case 0:
			return x();
		case 1:
			return y();
		case 2:
			return z();
	}
	
}

void Vector3ptr::operator >> ( Vector3* v3 ) const {
	
	assert( _inititalised );
	
	v3->x = x();
	v3->y = y();
	v3->z = z();
	
}

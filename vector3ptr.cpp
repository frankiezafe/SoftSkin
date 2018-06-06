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
	_v3( 0 ), 
	_local(false),
	_inititalised(false)
{}

Vector3ptr::Vector3ptr( Vector3* v3 ) : 
	_v3( v3 ), 
	_local(false),
	_inititalised(false)
{}

Vector3ptr::Vector3ptr( const float& x, const float& y, const float& z ): 
	_local(true) ,
	_inititalised(false)
{
	
	_v3 = new Vector3( x,y,z );
	
}

Vector3ptr::~Vector3ptr() {
	
	purge();
	
}

void Vector3ptr::purge() {
	
	if ( _local ) {
		delete _v3;
	}
	_v3 = 0;
	_local = false;
	_inititalised = false;
	
}

void Vector3ptr::init( Vector3* v3 ) {
	
	purge();
	
	if ( v3 ) {
		_local = false;
		_v3 = v3;
		_inititalised = true;
	}
	
}

void Vector3ptr::init( const float& x, const float& y, const float& z ) {
	
	purge();
	_local = true;
	_v3 = new Vector3( x,y,z );
	_inititalised = true;
	
}

void Vector3ptr::set( const float& x, const float& y, const float& z ) {
	
	assert( _inititalised );
	
	_v3->x = x;
	_v3->y = y;
	_v3->z = z;
	
}

void Vector3ptr::operator = ( const Vector3& v3 ) {
	
	assert( _inititalised );
	
	_v3->x = v3.x;
	_v3->y = v3.y;
	_v3->z = v3.z;
	
}

void Vector3ptr::operator = ( const Vector3ptr& v3ptr ) {
	
	assert( _inititalised );
	
	_v3->x = v3ptr.ptr()->x;
	_v3->y = v3ptr.ptr()->y;
	_v3->z = v3ptr.ptr()->z;
	
}

void Vector3ptr::operator += (const Vector3& v3 ) {
	
	assert( _inititalised );
	
	_v3->x += v3.x;
	_v3->y += v3.y;
	_v3->z += v3.z;
	
}

void Vector3ptr::operator -= (const Vector3& v3 ) {
	
	assert( _inititalised );
	
	_v3->x -= v3.x;
	_v3->y -= v3.y;
	_v3->z -= v3.z;
	
}

const float& Vector3ptr::operator [] ( const uint8_t& i ) const {
	
	assert( _inititalised );
	assert(i>2);
	
	return (*_v3)[i];
	
}

void Vector3ptr::operator >> ( Vector3* v3 ) const {
	
	assert( _inititalised );
	
	v3->x = _v3->x;
	v3->y = _v3->y;
	v3->z = _v3->z;
	
}

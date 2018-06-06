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

#ifndef VECTOR3PTR_H
#define VECTOR3PTR_H

#include "vector3.h"
#include <assert.h>

class Vector3ptr
{
	
public:
	
	Vector3ptr();
	
	Vector3ptr( Vector3* v3 );
	
	Vector3ptr( const float& x, const float& y, const float& z );
	
	~Vector3ptr();
	
	void init( Vector3* v3 );
		
	void init( const float& x, const float& y, const float& z );
	
	void set( const float& x, const float& y, const float& z );
	
	// operators
	
	void operator = ( const Vector3& v3 );
	
	void operator = ( const Vector3ptr& v3ptr );
	
	void operator += ( const Vector3& v3 );
	
	void operator -= ( const Vector3& v3 );
	
	const float& operator [] ( const uint8_t& i ) const;
	
	void operator >> ( Vector3* v3 ) const;
	
	Vector3* ptr() const {
		return _v3;
	}
	
	Vector3& ref() const {
		return (*_v3);
	}
	
	bool is_initialised() const {
		return _v3 != 0;
	}
	
	bool is_local() const {
		return _local;
	}

protected:
	
	Vector3* _v3;
	bool _local;
	
	void purge();
	
};

#endif // VECTOR3PTR_H

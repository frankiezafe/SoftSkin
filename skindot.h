/*
 * 
 * 
 * _________ ____  .-. _________/ ____ .-. ____ 
 * __|__  (_)_/(_)(   )____<    \|    (   )  (_)
 *                 `-'                 `-'      
 * 
 * 
 * art & game engine
 * 
 * ____________________________________  ?   ____________________________________
 *                                     (._.)
 * 
 * 
 * This file is part of softskin library
 * For the latest info, see http://polymorph.cool/
 * 
 * Copyright (c) 2018 polymorph.cool
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * ___________________________________( ^3^)_____________________________________
 * 
 * ascii font: rotated by MikeChat & myflix
 * have fun and be cool :)
 * 
 */


#ifndef SKINDOT_H
#define SKINDOT_H

#include "vector3.h"

class SkinDot {
	
public:
	
	SkinDot();
	
	SkinDot(float x, float y, float z);
	
	SkinDot( const SkinDot& src );
	
	void push(const Vector3& f);
	
	const Vector3& vert() const;
	
	const Vector3& force() const;
	
	const Vector3& normal() const;
	
	const float& damping() const;
	
	const float& kicks() const;
	
	void normal( float x, float y, float z );
	
	void operator = ( const SkinDot& src );
	
protected:
	
	static void set_v3( Vector3& v3, const float x, const float y, const float z );
	
	static void set_v3( Vector3& v3, const Vector3& src );

	static void _bind_methods();
	
private:
		
	Vector3 _vert;
	Vector3 _force;
	Vector3 _normal;
	float _damping;
	uint16_t _kicks;
	
};

#endif // SKINDOT_H

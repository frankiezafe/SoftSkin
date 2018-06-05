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

#include "skindot.h"

SkinDot::SkinDot() {
	set_v3( _vert, 0,0,0 );
	set_v3( _force, 0,0,0 );
	set_v3( _normal, 0,0,0 );
	_damping = 1;
	_kicks = 0;
}

SkinDot::SkinDot(float x, float y, float z) {
	set_v3( _vert, x, y, z );
	set_v3( _force, 0,0,0 );
	set_v3( _normal, 0,0,0 );
	_damping = 1;
	_kicks = 0;
}

SkinDot::SkinDot( const SkinDot& src ) {
	set_v3( _vert, src._vert );
	set_v3( _force, src._force );
	set_v3( _normal, src._normal );
	_damping = src._damping;
	_kicks = src._kicks;
}

void SkinDot::push(const Vector3& f) {
	_force += f;
	_kicks++;
}

const Vector3& SkinDot::vert() const {
	return _vert;
}

const Vector3& SkinDot::force() const {
	return _force;
}

const Vector3& SkinDot::normal() const {
	return _normal;
}

const float& SkinDot::damping() const {
	return _damping;
}

const float& SkinDot::kicks() const {
	return _kicks;
}

void SkinDot::vert( float x, float y, float z ) {
	set_v3( _vert, x, y, z );
}

void SkinDot::normal( float x, float y, float z ) {
	set_v3( _normal, x, y, z );
}

void SkinDot::damping( float d ) {
	_damping = d;
}

void SkinDot::operator = ( const SkinDot& src ) {
	set_v3( _vert, src.vert() );
	set_v3( _force, src.force() );
	set_v3( _normal, src.normal() );
	_damping = src.damping();
	_kicks = src.kicks();
}

void SkinDot::set_v3( Vector3& v3, const float x, const float y, const float z ) {
	v3.x = x;
	v3.y = y;
	v3.z = z;
}

void SkinDot::set_v3( Vector3& v3, const Vector3& src ) {
	v3.x = src.x;
	v3.y = src.y;
	v3.z = src.z;
}

// void SkinDot::_bind_methods() {
// 	ClassDB::bind_method(D_METHOD("init", "port"), &OSCreceiver::init);
// }

void SkinDot::update(float delta_time) {
	
	if (_kicks < 2) {
		_kicks = 2;
	};
		
	Vector3 consumed = _force * _damping / sqrt(_kicks - 1);
	_force -= consumed;
	_vert += consumed;
	_vert += _normal * delta_time * 0.1;
	//        (*this) += _force * ( _damping / sqrt( _kicks ) );
	//        _force.set(0,0,0);
	_kicks = 0;
	
}
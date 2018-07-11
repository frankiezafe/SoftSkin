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
#include "skincommon.h"

SkinDot::SkinDot() :
_inititalised(false),
_gravity(0) {
}

SkinDot::SkinDot(const float& x, const float& y, const float& z) :
_inititalised(false),
_gravity(0) {

    init(x, y, z);

}

SkinDot::SkinDot(Vector3* vert, Vector3* normal, Vector3* force) :
_inititalised(false),
_gravity(0) {

    init(vert, normal, force);

}

void SkinDot::init(const float& x, const float& y, const float& z) {

    _vert.init(x, y, z);
    _normal.init(0, 0, 0);
    _force.init(0, 0, 0);
    init_internal();

}

void SkinDot::init(Vector3* vert, Vector3* normal, Vector3* force) {

    _vert.init(vert);
    _normal.init(normal);
    _force.init(force);
    init_internal();

}

void SkinDot::init_internal() {

    _damping = 1;
    _kicks = 0;

    mirror_verts.src = &_vert;
    mirror_normals.src = &_normal;
    mirror_forces.src = &_force;

    _inititalised = true;

}

void SkinDot::register_vert(Vector3* vert) {

    assert(_inititalised);

    mirror_verts.add(vert);

}

void SkinDot::register_normal(Vector3* normal) {

    assert(_inititalised);

    mirror_normals.add(normal);

}

void SkinDot::register_force(Vector3* force) {

    assert(_inititalised);

    mirror_forces.add(force);

}

void SkinDot::push(const Vector3& f) {

    _force += f;
    _kicks++;

}

const Vector3ptr& SkinDot::vert() const {

    return _vert;

}

const Vector3ptr& SkinDot::force() const {

    return _force;

}

const Vector3ptr& SkinDot::normal() const {

    return _normal;

}

const float& SkinDot::damping() const {

    return _damping;

}

const float& SkinDot::kicks() const {

    return _kicks;

}

void SkinDot::vert(const float& x, const float& y, const float& z) {

    _vert.set(x, y, z);

}

void SkinDot::normal(const float& x, const float& y, const float& z) {

    _normal.set(x, y, z);

}

void SkinDot::damping(const float& d) {

    _damping = d;

}

void SkinDot::gravity(Vector3* g) {

    _gravity = g;

}

void SkinDot::operator=(const SkinDot& src) {

    _vert = src.vert();
    _normal = src.normal();
    _force = src.force();
    _damping = src.damping();
    _kicks = src.kicks();

}

void SkinDot::update(const float& delta_time) {

    if (_kicks < 2) {
        _kicks = 2;
    };

    Vector3 consumed = _force.ref() * _damping / sqrt(_kicks - 1);
    _force -= consumed;
    _vert += consumed;
    if (_gravity) {
        _vert += (*_gravity) * delta_time;
    }
    // 	_vert += _normal.ref() * delta_time * 0.1;
    _kicks = 0;

    mirror_verts.sync();
    mirror_normals.sync();
    mirror_forces.sync();

}

void SkinDot::ray_distance(
        const Vector3& origin,
        const Vector3& ray,
        SkinRayResult& result) {
    
    Vector3 diff = _vert.ref() - origin;
    result.distance_to_origin = diff.length();
    result.dot_to_ray = ray.dot( diff / result.distance_to_origin );
    Vector3 perp = diff - ray * result.dot_to_ray * result.distance_to_origin;
    result.distance_to_ray = perp.length();

}
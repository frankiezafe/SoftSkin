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

#include "skinfiber.h"

SkinFiber::SkinFiber() :
_head_dot(0),
_head_vec3(0),
_tail(0),
_type(sf_UNDEFINED),
_rest_len(0),
_rest_len_multiplier(1),
_init_rest_len(0),
_stiffness(1),
_muscled(false),
_muscle_min_len(0),
_muscle_max_len(0),
_muscle_delta_len(0),
_muscle_a(0),
_muscle_phase_shift(0),
_muscle_frequency(0) {
}

bool SkinFiber::fiber(SkinDot* a, SkinDot* b) {

    if (_type != sf_UNDEFINED) {
        std::cout <<
                "SkinFiber::init, this object is already initialised " <<
                print_type(_type) <<
                std::endl;
        return false;
    }

    _head_dot = a;
    _tail = b;

    const Vector3& av = _head_dot->vert().ref();
    const Vector3& bv = _tail->vert().ref();

    _rest_len = av.distance_to(bv);
    _init_rest_len = _rest_len;
    _dir = ((av)-(bv)).normalized();
    _middle = av + _dir * _rest_len * 0.5;

    _type = sf_FIBER;

    return true;

}

bool SkinFiber::fiber(SkinDot* a, SkinDot* b, float len) {

    if (!fiber(a, b)) {
        return false;
    }

    _rest_len = len;
    _init_rest_len = len;

    return true;

}

bool SkinFiber::ligament(Vector3* a, SkinDot* b) {

    if (_type != sf_UNDEFINED) {
        std::cout <<
                "SkinFiber::init, this object is already initialised " <<
                print_type(_type) <<
                std::endl;
        return false;
    }

    _head_vec3 = a;
    _tail = b;

    const Vector3& av = (*_head_vec3);
    const Vector3& bv = _tail->vert().ref();

    _rest_len = av.distance_to(bv);
    _init_rest_len = _rest_len;
    _dir = ((av)-(bv)).normalized();
    _middle = av + _dir * _rest_len * 0.5;

    _type = sf_LIGAMENT;

    return true;

}

bool SkinFiber::muscle(bool enable) {

    if (
            (enable && (_type != sf_FIBER && _type != sf_LIGAMENT)) ||
            (!enable && (_type != sf_TENSOR && _type != sf_MUSCLE))
            ) {
        std::cout << "SkinFiber::muscle, muscle can not be set to " << enable << std::endl;
        return false;
    }

    _muscled = enable;
    _muscle_max_len = _init_rest_len;

    if (_type == sf_FIBER) {

        _type = sf_TENSOR;

    } else if (_type == sf_LIGAMENT) {

        _type = sf_MUSCLE;

    }

    return true;

}

bool SkinFiber::musclise(float min, float max, float freq, float shift) {

    if (!muscle(true)) {
        return false;
    }

    _muscle_a = 0;
    _muscled = true;
    muscle_min_max(min, max);
    muscle_frequency(freq);
    muscle_phase_shift(shift);

    return true;

}

void SkinFiber::update(const float& delta_time) {

    if (_type == sf_UNDEFINED) {
        return;
    }

    if (_muscled) {
        update_muscle(delta_time);
    }

    switch (_type) {

        case sf_FIBER:
        case sf_TENSOR:
            update_fiber();
            break;

        case sf_LIGAMENT:
        case sf_MUSCLE:
            update_ligament();
            break;

        default:
            break;

    }

}

void SkinFiber::update_muscle(float delta_time) {

    _muscle_a += delta_time * Math_TAU * 2 * _muscle_frequency;
    _rest_len =
            _muscle_min_len +
            sin(_muscle_phase_shift + _muscle_a) *
            _muscle_delta_len;

}

void SkinFiber::update_fiber() {

    const Vector3& av = _head_dot->vert().ref();

    _dir = _tail->vert().ref() - av;
    _current_len = _dir.length();

    float dl = _current_len - (_rest_len * _rest_len_multiplier);
    _dir.normalize();

    Vector3 d = _dir * dl * _stiffness;
    d *= 0.5;
    _head_dot->push(d);

    _tail->push(-d);

    _middle = av + _dir * _current_len * 0.5;

}

void SkinFiber::update_ligament() {
    
    const Vector3& av = (*_head_vec3);

    _dir = _tail->vert().ref() - av;
    _current_len = _dir.length();
    _dir.normalize();

    _tail->push(-_dir * _stiffness * _current_len);

    _middle = av + _dir * _current_len * 0.5;

}

SkinDot*& SkinFiber::head_dot() {

    return _head_dot;

}

Vector3*& SkinFiber::head_vec3() {

    return _head_vec3;

}

SkinDot*& SkinFiber::tail() {

    return _tail;

}

const Vector3& SkinFiber::dir() const {

    return _dir;

}

const Vector3& SkinFiber::middle() const {

    return _middle;

}

const float& SkinFiber::init_rest_len() const {

    return _init_rest_len;

}

const float& SkinFiber::current_len() const {

    return _current_len;

}

const float& SkinFiber::rest_len() const {

    return _rest_len;

}

const float& SkinFiber::rest_len_multiplier() const {

    return _rest_len_multiplier;

}

const float& SkinFiber::stiffness() const {

    return _stiffness;

}

const bool& SkinFiber::muscle() const {

    return _muscled;

}

const skin_fiber_t& SkinFiber::type() const {

    return _type;

}

const float& SkinFiber::muscle_frequency() const {

    return _muscle_frequency;

}

void SkinFiber::rest_len(const float& l) {

    _rest_len = l;

}

void SkinFiber::rest_len_multiplier(const float& l) {

    _rest_len_multiplier = l;

}

void SkinFiber::stiffness(const float& s) {

    _stiffness = s;

}

void SkinFiber::muscle_min_max(float min, float max) {

    _muscle_delta_len = (max - min) * 0.5;
    _muscle_min_len = min + _muscle_delta_len;
    _muscle_max_len = max;

}

void SkinFiber::muscle_frequency(float f) {

    _muscle_frequency = f;

}

void SkinFiber::muscle_phase_shift(float shift) {

    _muscle_phase_shift = shift;

}
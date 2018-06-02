/*
 * Copyright 2018 <copyright holder> <email>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#include "skinfiber.h"

SkinFiber::SkinFiber() {
	_a = 0;
	_b = 0;
	_rest_len = 0;
}

void SkinFiber::init(SkinDot* a, SkinDot* b) {
	_a = a;
	_b = b;	
	const Vector3& av = _a->vert();
	const Vector3& bv = _b->vert();
	_rest_len = av.distance_to(bv);
	_init_rest_len = _rest_len;
	_dir = ((av)-(bv)).normalized();
	_middle = av + _dir * _rest_len * 0.5;
	_muscled = false;
	muscle_min_max(0, _init_rest_len);
	muscle_freq(1);
	muscle_phase_shift(0);
}

void SkinFiber::init(SkinDot* a, SkinDot* b, float len) {
	init(a, b);
	_rest_len = len;
	_init_rest_len = len;
	muscle_min_max(0, _init_rest_len);
}

void SkinFiber::muscle(bool enable) {
	_muscled = enable;
}

void SkinFiber::musclise(float min, float max, float freq, float shift) {
	_muscle_a = 0;
	_muscled = true;
	muscle_min_max(min, max);
	muscle_freq(freq);
	muscle_phase_shift(shift);
}

void SkinFiber::update(float delta_time) {
	if (!_a || !_b) return;
	if (_muscled) {
		update_muscle(delta_time);
	}
	const Vector3& av = _a->vert();
	const Vector3& bv = _b->vert();
	_dir = bv - av;
	_current_len = _dir.length();
	float dl = _current_len - _rest_len;
	_dir.normalize();
	Vector3 d = _dir;
	d *= dl * 0.5;
	_a->push(d);
	_b->push(-d);
	_middle = av + _dir * _current_len * 0.5;
}

// drawing

// void SkinFiber::gl_current_line() {
// 	if (!_a || !_b) return;
// 	glVertex3f(_a->x, _a->y, _a->z);
// 	glVertex3f(_b->x, _b->y, _b->z);
// }
// 
// void SkinFiber::gl_rest_line() {
// 	if (!_a || !_b) return;
// 	Vector3 ta = _middle - _dir * _rest_len * 0.5;
// 	Vector3 tb = _middle + _dir * _rest_len * 0.5;
// 	glVertex3f(ta.x, ta.y, ta.z);
// 	glVertex3f(tb.x, tb.y, tb.z);
// }

// getters

SkinDot*& SkinFiber::a() {
	return _a;
}

SkinDot*& SkinFiber::b() {
	return _b;
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

const bool& SkinFiber::muscle() const {
	return _muscled;
}

// setters

void SkinFiber::rest_len(const float& l) {
	_rest_len = l;
}

void SkinFiber::muscle_min_max(float min, float max) {
	_muscle_delta_len = (max - min) * 0.5;
	_muscle_min_len = min + _muscle_delta_len;
	_muscle_max_len = max;
}

void SkinFiber::muscle_freq(float f) {
	_muscle_frequency = f;
}

void SkinFiber::muscle_phase_shift(float shift) {
	_muscle_phase_shift = shift;
}

void SkinFiber::update_muscle(float delta_time) {
	_muscle_a += delta_time * Math_TAU * 2 * _muscle_frequency;
	_rest_len = _muscle_min_len +
	sin(_muscle_phase_shift + _muscle_a) *
	_muscle_delta_len;
}
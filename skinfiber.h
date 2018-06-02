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

#ifndef SKINFIBER_H
#define SKINFIBER_H

#include "skindot.h"

class SkinFiber {

public:
	
	SkinFiber();
	
	void init(SkinDot* a, SkinDot* b);
	
	void init(SkinDot* a, SkinDot* b, float len);
	
	void muscle(bool enable);
	
	void musclise(float min, float max, float freq = 1, float shift = 0);
	
	void update(float delta_time);
	
	// getters
	
	SkinDot*& a();
	
	SkinDot*& b();
	
	const Vector3& dir() const;
	
	const Vector3& middle() const;
	
	const float& init_rest_len() const;
	
	const float& current_len() const;
	
	const float& rest_len() const;
	
	const bool& muscle() const;
	
	// setters
	
	void rest_len(const float& l);
	
	void muscle_min_max(float min, float max);
	
	void muscle_freq(float f);
	
	void muscle_phase_shift(float shift);
	
private:
	
	SkinDot* _a;
	SkinDot* _b;
	
	Vector3 _dir;
	Vector3 _middle;
	float _current_len;
	float _rest_len;
	float _init_rest_len;
	
	bool _muscled;
	float _muscle_min_len;
	float _muscle_max_len;
	float _muscle_delta_len;
	float _muscle_a;
	float _muscle_phase_shift;
	float _muscle_frequency;
	
	void update_muscle(float delta_time);
	
};

#endif // SKINFIBER_H

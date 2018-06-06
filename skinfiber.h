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
	
	void update( const float& delta_time );
	
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

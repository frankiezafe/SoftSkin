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
	
	enum skin_fiber_t {
		
		sf_UNDEFINED, 	// type before assignation
		sf_FIBER, 		// a fiber connecting 2 skindots, passive behaviour
		sf_TENSOR, 		// a fiber connecting 2 skindots, able to contract
		sf_LIGAMENT,	// a fiber connecting 1 skindots to a fixed 3d position, passive behaviour
		sf_MUSCLE		// a fiber connecting 1 skindots to a fixed 3d position, able to contract
		
	};
	
	static std::string print_type( const skin_fiber_t& t ) {
		switch( t ) {
			case sf_UNDEFINED:
				return "sf_UNDEFINED";
			case sf_FIBER:
				return "sf_FIBER";
			case sf_TENSOR:
				return "sf_TENSOR";
			case sf_LIGAMENT:
				return "sf_LIGAMENT";
			case sf_MUSCLE:
				return "sf_MUSCLE";
			default:
				return "INVALID TYPE!";
		}
	}
	
	SkinFiber();
	
	/* 
	 * Initialisation of a sf_FIBER or a sf_TENSOR 
	 */
	bool fiber( SkinDot* a, SkinDot* b );
	
	
	/* 
	 * Initialisation of a sf_FIBER or a sf_TENSOR
	 * @param len: arbritrary rest length for the fiber
	 */
	bool fiber( SkinDot* a, SkinDot* b, float len );
	
	/* 
	 * Initialisation of a sf_LIGAMENT or a sf_MUSCLE 
	 */
	bool ligament( Vector3* a, SkinDot* b );
	
	bool muscle( bool enable );
	
	bool musclise( float min, float max, float freq = 1, float shift = 0 );
	
	void update( const float& delta_time );
	
	// getters
	
	SkinDot*& head_dot();
	
	Vector3*& head_vec3();
	
	SkinDot*& tail();
	
	const Vector3& dir() const;
	
	const Vector3& middle() const;
	
	const float& init_rest_len() const;
	
	const float& current_len() const;
	
	const float& rest_len() const;
	
	const float& rest_len_multiplier() const;
	
	const float& stiffness() const;
	
	const bool& muscle() const;
	
	const int& type() const;
	
	// setters
	
	void rest_len( const float& l );
	
	void rest_len_multiplier( const float& m );
	
	void stiffness( const float& s );
	
	void muscle_min_max( float min, float max );
	
	void muscle_freq( float f );
	
	void muscle_phase_shift( float shift );
	
private:
	
	SkinDot* _head_dot;
	Vector3* _head_vec3;
	SkinDot* _tail;
	
	skin_fiber_t _type;
	
	Vector3 _dir;
	Vector3 _middle;
	float _current_len;
	float _rest_len;
	float _rest_len_multiplier;
	float _init_rest_len;
	
	float _stiffness;
	
	bool _muscled;
	float _muscle_min_len;
	float _muscle_max_len;
	float _muscle_delta_len;
	float _muscle_a;
	float _muscle_phase_shift;
	float _muscle_frequency;
	
	void update_muscle( float delta_time );
	void update_fiber();
	void update_ligament();
	
};

#endif // SKINFIBER_H

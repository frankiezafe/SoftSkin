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
 *  []
 *  ascii font: rotated by MikeChat & myflix
 *  have fun and be cool :)
 */

#ifndef SKIN_H
#define SKIN_H

#include <iostream>
#include <vector>

#include "skindot.h"
#include "skinfiber.h"

#include "skincommon.h"

class Skin: public VisualInstance {
	
	GDCLASS(Skin, VisualInstance);
	
	enum surface_position_t {
		surf_MAIN = 0,
		surf_FIBER = 1,
		surf_TENSOR = 2,
		surf_LIGAMENT = 3,
		surf_MUSCLE = 4,
		surf_COUNT = 5
	};
	
public:
	
	Skin();
	
	~Skin();
	
	void cube();
	
	void grid( uint32_t divx, uint32_t divy );

	void parse( const String& path );
	
	void update(const float& delta_time );
	
	void set_type( const int& type );
	
	void set_soft_skin_path( const String& path );
	
	void set_ligament_strength( const float& s );
	
	void set_main_display( const bool& display );
	void set_fiber_display( const bool& display );
	void set_tensor_display( const bool& display );
	void set_ligament_display( const bool& display );
	void set_muscle_display( const bool& display );
	
	void set_main_material( const Ref<Material> &material );
	
	void set_fiber_material( const Ref<Material> &material );
	
	void set_tensor_material( const Ref<Material> &material );
	
	void set_ligament_material( const Ref<Material> &material );
	
	void set_muscle_material( const Ref<Material> &material );
	
	float get_ligament_strength() const;
	
	int get_type() const;
	
	String get_soft_skin_path() const;
	
	bool get_main_display() const;
	bool get_fiber_display() const;
	bool get_tensor_display() const;
	bool get_ligament_display() const;
	bool get_muscle_display() const;
	
	Ref<Material> get_main_material() const;
	
	Ref<Material> get_fiber_material() const;
	
	Ref<Material> get_tensor_material() const;
	
	Ref<Material> get_ligament_material() const;
	
	Ref<Material> get_muscle_material() const;
	
	// mandatory methods for VisualInstance
	virtual AABB get_aabb() const;
	
	virtual PoolVector<Face3> get_faces(uint32_t p_usage_flags) const;
	
protected:
	
	static void _bind_methods();
	
	AABB aabb;
	
private:
		
	uint32_t dots_num;
	uint32_t fibers_num;
	uint32_t faces_num;
	SkinDot* dots;
	Vector3* forces;
	Vector3** ligaments_heads;
	SkinFiber* fibers;
	
	Vector3 _gravity;
	float ligament_strength;
		
	Ref<ArrayMesh> root_mesh;
	Vector<ShapeUpdateSurface> surfaces;
	
	String soft_skin_path;
	
	bool main_display;
	bool fiber_display;
	bool tensor_mdisplay;
	bool ligament_display;
	bool muscle_display;
	
	Ref<Material> main_material;
	Ref<Material> fiber_material;
	Ref<Material> tensor_material;
	Ref<Material> ligament_material;
	Ref<Material> muscle_material;
	
	void purge();
	
	void generate( SkinRaw& raw );
	
	void unbind_root();
	
	void bind_root();
	
};

#endif // SKIN_H

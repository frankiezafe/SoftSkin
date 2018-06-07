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

#include "core/bind/core_bind.h"
#include "scene/3d/visual_instance.h"
#include "scene/resources/mesh.h"
#include "drivers/gles3/rasterizer_storage_gles3.h"

class Skin: public VisualInstance {
	
	GDCLASS(Skin, VisualInstance);
	
	// decompression data
	struct SkinRaw {
		bool vpass;
		bool epass;
		bool fpass;
		Vector< Vector<float> > verts;
		Vector< Vector<int> > edges;
		Vector< Vector<int> > faces;
		SkinRaw() : vpass(false), epass(false), fpass(false) {}
	};
	
	struct ShapeUpdateSurface {
		int type;
		PoolVector<Vector3> vertices;
		PoolVector<Vector3> normals;
		PoolVector<int> indices;
		PoolVector<Vector2> uvs;
		Ref<Material> material;
		int last_added;
		PoolVector<Vector3>::Write verticesw;
		PoolVector<Vector3>::Write normalsw;
		PoolVector<int>::Write indicesw;
		PoolVector<Vector2>::Write uvsw;
	};
	
public:
	
	Skin();
	
	~Skin();
	
	void cube();
	
	void parse( const String& path );
	
	void update(const float& delta_time );
	
	void set_ligament_strength( const float& s );
	
	void set_main_material( const Ref<Material> &material );
	
	void set_fiber_material( const Ref<Material> &material );
	
	void set_ligament_material( const Ref<Material> &material );
	
	float get_ligament_strength() const;
	
	Ref<Material> get_main_material() const;
	
	Ref<Material> get_fiber_material() const;
	
	Ref<Material> get_ligament_material() const;
	
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
	
	float ligament_strength;
	
	Ref<ArrayMesh> root_mesh;
	Vector<ShapeUpdateSurface> surfaces;
	
	Ref<Material> main_material;
	Ref<Material> fiber_material;
	Ref<Material> ligament_material;
	
	void purge();
	
	void generate( SkinRaw& raw );
	
	void unbind_root();
	
	void bind_root();
	
};

#endif // SKIN_H

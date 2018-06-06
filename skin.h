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
#include "scene/resources/mesh.h"
#include "scene/3d/visual_instance.h"
#include "drivers/gles3/rasterizer_storage_gles3.h"
#include "scene/resources/mesh.h"

class Skin: public GeometryInstance {
	
	GDCLASS(Skin, GeometryInstance);
	
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
	
public:
	
	Skin();
	
	~Skin();
	
	void cube();
	
	void parse( const String& path );
	
	void update(const float& delta_time );
	
	// mandatory methods for GeometryInstance
	
	virtual AABB get_aabb() const;
	
	virtual PoolVector<Face3> get_faces(uint32_t p_usage_flags) const;
	
	// surface material
	
	void set_surface_material(int p_surface, const Ref<Material> &p_material);
		
protected:
	
	static void _bind_methods();
	
	AABB aabb;
	
private:
	
// 	RID im;
// 	RasterizerStorageGLES3::Immediate* imm;
	
	RID mesh_rid;
	RasterizerStorageGLES3::Mesh* mesh;
	
	uint32_t dots_num;
	uint32_t fibers_num;
	SkinDot* dots;
	SkinFiber* fibers;
	Vector3* forces;
	
	void purge();
	
	void generate( SkinRaw& raw );
	
	void retrieve_immediate() {
		
// 		imm = (RasterizerStorageGLES3::Immediate*) im.get_data();
		mesh = (RasterizerStorageGLES3::Mesh*) mesh_rid.get_data();
		
	}
	
	Vector< Ref< Material > > materials;
	
};

#endif // SKIN_H

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
 *  
 *  ascii font: rotated by MikeChat & myflix
 *  have fun and be cool :)
 */

#include "skin.h"

Skin::Skin():
	imm(0),
	dots_num(0),
	fibers_num(0),
	dots(0),
	fibers(0),
	forces(0)
{
		
	im = VisualServer::get_singleton()->immediate_create();
	set_base(im);
	
}

Skin::~Skin() {
	
	purge();
	
}

void Skin::purge() {
	
	if ( dots != 0 ) {
		delete [] dots;
		dots = 0;
	}
	if ( fibers != 0 ) {
		delete [] fibers;
		fibers = 0;
	}
	if ( forces != 0 ) {
		delete [] forces;
		forces = 0;
	}
	
	dots_num = 0;
	fibers_num = 0;
	
	VisualServer::get_singleton()->immediate_clear(im);
	
}

void Skin::generate( SkinRaw& raw ) {
	
	dots_num = raw.verts.size();
	fibers_num = raw.edges.size();
	uint32_t faces_num = raw.faces.size();
	
	RID mat;
	
	// STD MESH ************
	
	VisualServer::get_singleton()->immediate_begin(
		im, 
		(VisualServer::PrimitiveType) Mesh::PRIMITIVE_TRIANGLES, 
		RID());
	
	mat = VisualServer::get_singleton()->material_create();
	VisualServer::get_singleton()->immediate_set_material( im, mat );
	
	for ( uint32_t i = 0; i < faces_num; ++i ) {
		
		Vector<int>& fids = raw.faces[i];
		int j = fids.size() - 1;
		
		while( j >= 0 ) {
			
			Vector<float>& vert = raw.verts[fids[j]];
			Vector3 p( vert[1], vert[2], vert[3] );
			Vector3 n( vert[4], vert[5], vert[6] );
			if ( i == 0 ) {
				aabb.position = p;
				aabb.size = Vector3();
			} else {
				aabb.expand_to( p );
			}
			VisualServer::get_singleton()->immediate_normal(im, n );
			VisualServer::get_singleton()->immediate_vertex(im, p );
			
			--j;
			
		}
		
	}
	
	VisualServer::get_singleton()->immediate_end(im);
	
	
	// DEBUG MESHES ************
	// FIBERS ************
	
	VisualServer::get_singleton()->immediate_begin(
		im, 
		(VisualServer::PrimitiveType) Mesh::PRIMITIVE_LINES, 
		RID());
	
	mat = VisualServer::get_singleton()->material_create();
	VisualServer::get_singleton()->immediate_set_material( im, mat );
	
	for ( uint32_t i = 0; i < fibers_num; ++i ) {
		Vector<int>& vs = raw.edges[i];
		for ( uint32_t j = 0; j < 2; ++j ) {
			Vector<float>& vert = raw.verts[vs[j]];
			Vector3 p( vert[1], vert[2], vert[3] );
			VisualServer::get_singleton()->immediate_vertex(im, p );
		}
	}
	
	VisualServer::get_singleton()->immediate_end(im);
	
	// LIGAMENTS ************
	VisualServer::get_singleton()->immediate_begin(
		im, 
		(VisualServer::PrimitiveType) Mesh::PRIMITIVE_LINES, 
		RID());
	
	mat = VisualServer::get_singleton()->material_create();
	VisualServer::get_singleton()->immediate_set_material( im, mat );
	
	for ( uint32_t i = 0; i < dots_num; ++i ) {
		Vector<float>& vert = raw.verts[i];
		Vector3 p( vert[1], vert[2], vert[3] );
		VisualServer::get_singleton()->immediate_vertex(im, p );
		VisualServer::get_singleton()->immediate_vertex(im, p );
	}
	
	VisualServer::get_singleton()->immediate_end(im);
	
	// all meshes created
	
	
	retrieve_immediate();
	
	// data has been pushed in godot engine,
	// we can now generate custom object that 
	// will interacts with it
	
	dots = new SkinDot[dots_num];
	forces = new Vector3[dots_num];
	Vector<Vector3>& vs = imm->chunks[0].vertices;
	Vector<Vector3>& ns = imm->chunks[0].normals;
		
	uint32_t fvs = 0;
	
	for ( uint32_t i = 0; i < faces_num; ++i ) {
		
		Vector<int>& fids = raw.faces[i];
		int j = fids.size() - 1;
		
		while( j >= 0 ) {
		
			uint32_t id = fids[j];
			SkinDot& sd = dots[id];
			if ( !sd.is_initialised() ) {
				sd.init(
					&vs[fvs],
					&ns[fvs],
					&forces[id]
				);
				sd.damping( 0.1 );
			} else {
				sd.register_vert( &vs[fvs] );
				sd.register_normal( &ns[fvs] );
			}
			
			--j;
			++fvs;
		
		}
		
	}
	
	// debugging ligaments
	Vector<Vector3>& vs_liga = imm->chunks[2].vertices;
	for ( uint32_t i = 0; i < dots_num; ++i ) {
		dots[i].register_vert( &vs_liga[(i*2)+1] );
	}
	
	fibers = new SkinFiber[fibers_num + dots_num];
	uint32_t fibid = 0;
	
	// generate fibers and tensors
	for( int i = 0; i < fibers_num; ++i, ++fibid ) {
		Vector<int>& vs = raw.edges[i];
		fibers[fibid].init( &dots[vs[0]], &dots[vs[1]] );
		if ( vs[2] != 0 ) {
			fibers[fibid].musclise(
				fibers[fibid].init_rest_len() * 0.2,
				fibers[fibid].init_rest_len() * 3.4,
				0.1, 0
				);
		}
	}
	
	// generate ligaments
	for( int i = 0; i < dots_num; ++i, ++fibid ) {
		fibers[fibid].init( &( dots[i].vert().ref() ), &dots[i] );
	}
	
	materials.resize( imm->chunks.size() );
	
	std::cout << "MATERIAL " <<
	materials.size() << " / " << 
	VisualServer::get_singleton()->immediate_get_material(im).get_id() << " / " <<
	VisualServer::get_singleton()->immediate_get_material(im).get_data() <<
	std::endl;
	
}

void Skin::parse( const String& path ) {
	
	purge();

	SkinRaw decompressed;
	
	_File f;
	f.open( path, _File::READ );
	
	String l = f.get_line();
	uint32_t i = 0;
	uint32_t index = 0;
	
	while (!f.eof_reached()) {
		
		if ( l.begins_with( "++vertices++" ) ) {
			
			decompressed.vpass = true;
			decompressed.epass = false;
			decompressed.fpass = false;
			index = 0;
			int num = l.right( 12 ).to_int();
			decompressed.verts.resize( num );
			
		} else if ( l.begins_with( "++edges++" ) ) {
			
			decompressed.vpass = false;
			decompressed.epass = true;
			decompressed.fpass = false;
			index = 0;
			int num = l.right( 9 ).to_int();
			decompressed.edges.resize( num );
			
		} else if ( l.begins_with( "++faces++" ) ) {
			
			decompressed.vpass = false;
			decompressed.epass = false;
			decompressed.fpass = true;
			index = 0;
			int num = l.right( 9 ).to_int();
			decompressed.faces.resize( num );
			
		} else if ( decompressed.vpass ) {
			
			Vector<float> vs = l.split_floats(" ");
			if ( vs.size() >= 7 ) {
				decompressed.verts[index] = vs; ++index;
			} else {
				std::cout << "Skin::parse, failed to decompress line " << i << std::endl;
				return;
			}
			
		} else if ( decompressed.epass ) {
			
			Vector<int> vs = l.split_ints(" ");
			if ( vs.size() >= 3 ) {
				decompressed.edges[index] = vs; ++index;
			} else {
				std::cout << "Skin::parse, failed to decompress line " << i << std::endl;
				return;
			}
			
		} else if ( decompressed.fpass ) {
			
			Vector<int> vs = l.split_ints(" ");
			if ( vs.size() >= 3 ) {
				decompressed.faces[index] = vs; ++index;
			} else {
				std::cout << "Skin::parse, failed to decompress line " << i << std::endl;
				return;
			}
			
		}
		
		l = f.get_line();
		++i;
	}
	
	f.close();
	
	if ( 
		decompressed.verts.empty() ||
		decompressed.edges.empty() ||
		decompressed.faces.empty()
		) {
		std::cout << "Skin::parse, inconsistent data: " <<
		"verts: " << decompressed.verts.size() << ". " <<
		"edges: " << decompressed.edges.size() << ". " <<
		"faces: " << decompressed.faces.size() << ". " <<
		std::endl;
		return;
	}
	
	decompressed.vpass = false;
	decompressed.epass = false;
	decompressed.fpass = false;
	
	generate( decompressed );
	
}

AABB Skin::get_aabb() const {
	
	return aabb;
	
}

PoolVector<Face3> Skin::get_faces(uint32_t p_usage_flags) const {
	
	return PoolVector<Face3>();
	
}

void Skin::update( const float& delta ) {
	
	if ( imm == 0 || dots == 0 ) {
		std::cout << "Skin::update, object not ready for update!" << std::endl;
		return;
	}
	
	for( uint32_t i = 0; i < dots_num; ++i ) {
		
		dots[i].update( delta );
		
	}
	
	for( uint32_t i = 0; i < fibers_num; ++i ) {
		
		fibers[i].update( delta );
		
	}
	
	imm->instance_change_notify();
	
}

void Skin::set_surface_material(int p_surface, const Ref<Material> &p_material) {
	
	ERR_FAIL_INDEX(p_surface, materials.size());
	
	materials[p_surface] = p_material;
	
	if (materials[p_surface].is_valid()) {
		std::cout << "Skin::set_surface_material, materials is valid" << std::endl;
		VisualServer::get_singleton()->instance_set_surface_material(get_instance(), p_surface, materials[p_surface]->get_rid());
	} else {
		std::cout << "Skin::set_surface_material, materials is INvalid" << std::endl;
		VisualServer::get_singleton()->instance_set_surface_material(get_instance(), p_surface, RID());
	}
	
}

void Skin::_bind_methods() {
	
	ClassDB::bind_method(D_METHOD("render_skin", "delta"), &Skin::update);
	ClassDB::bind_method(D_METHOD("parse", "path"), &Skin::parse);
	ClassDB::bind_method(D_METHOD("set_surface_material", "p_surface", "p_material"), &Skin::set_surface_material);
	
}

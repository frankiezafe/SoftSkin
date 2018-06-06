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
// 	imm(0),
	mesh(0),
	dots_num(0),
	fibers_num(0),
	dots(0),
	fibers(0),
	forces(0),
	last_delta(0)
{
		
// 	im = VisualServer::get_singleton()->immediate_create();
// 	set_base(im);
	
	mesh_rid = VisualServer::get_singleton()->mesh_create();
	set_base(mesh_rid);
	
}

Skin::~Skin() {
	
	purge();
	
}

void Skin::purge() {
	
	if ( dots ) {
		delete [] dots;
		dots = 0;
	}
	if ( fibers ) {
		delete [] fibers;
		fibers = 0;
	}
	if ( forces ) {
		delete [] forces;
		forces = 0;
	}
	
	dots_num = 0;
	fibers_num = 0;
	
// 	VisualServer::get_singleton()->immediate_clear(im);
	if ( mesh ) {
		VisualServer::get_singleton()->mesh_clear(mesh_rid);
	}
	
}

void Skin::generate() {
	
	dots_num = raw.verts.size();
	fibers_num = raw.edges.size();
	faces_num = raw.faces.size();
	
	dots = new SkinDot[dots_num];
	forces = new Vector3[dots_num];
	
	// strongly inspired by scens/resources/primitive_meshes.cpp
	
	materials.resize(3);
	
	PoolVector<Vector3> points;
	PoolVector<Vector3> normals;
	PoolVector<int> indices;
	
	for( uint32_t i = 0; i < dots_num; ++i ) {
		Vector<float>& vert = raw.verts[i];
		points.push_back( Vector3( vert[1], vert[2], vert[3] ) );
		normals.push_back( Vector3( vert[4], vert[5], vert[6] ) );
		dots[i].init(
			&points.read()[i],
			&normals.read()[i],
			&forces[i]
		);
		
	}
	
	for ( uint32_t i = 0; i < faces_num; ++i ) {
		Vector<int>& fids = raw.faces[i];
		int j = fids.size() - 1;
		while( j >= 0 ) {
			indices.push_back(fids[j]);
			--j;
		}
	}
	
	mesh_arr.resize(VS::ARRAY_MAX);
	mesh_arr[VS::ARRAY_VERTEX] = points;
	mesh_arr[VS::ARRAY_NORMAL] = normals;
	mesh_arr[VS::ARRAY_INDEX] = indices;
	
	VisualServer::get_singleton()->mesh_add_surface_from_arrays(
		mesh_rid,
		(VisualServer::PrimitiveType) Mesh::PRIMITIVE_TRIANGLES, 
		mesh_arr
	);
	
// 	VisualServer::get_singleton()->mesh_surface_set_material(
// 		mesh_rid, 0, 
// 		materials[0].is_null() ? RID() : materials[0]->get_rid()
// 	);
	
	retrieve_object();
	
	fibers = new SkinFiber[fibers_num];
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
	
	std::cout << "Skin::generate, surfaces: " << mesh->surfaces.size() << std::endl;
	
// 	RID mat;
// 	
// 	// STD MESH ************
// 	
// 	VisualServer::get_singleton()->immediate_begin(
// 		im, 
// 		(VisualServer::PrimitiveType) Mesh::PRIMITIVE_TRIANGLES, 
// 		RID());
// 	
// 	mat = VisualServer::get_singleton()->material_create();
// 	// see scenes/resources/material.cpp, line 230
// 	VisualServer::get_singleton()->material_set_param( mat, "albedo", Color(1,0,0,1) );
// 	VisualServer::get_singleton()->immediate_set_material( im, mat );
// 	
// 	for ( uint32_t i = 0; i < faces_num; ++i ) {
// 		
// 		Vector<int>& fids = raw.faces[i];
// 		int j = fids.size() - 1;
// 		
// 		while( j >= 0 ) {
// 			
// 			Vector<float>& vert = raw.verts[fids[j]];
// 			Vector3 p( vert[1], vert[2], vert[3] );
// 			Vector3 n( vert[4], vert[5], vert[6] );
// 			if ( i == 0 ) {
// 				aabb.position = p;
// 				aabb.size = Vector3();
// 			} else {
// 				aabb.expand_to( p );
// 			}
// 			VisualServer::get_singleton()->immediate_normal(im, n );
// 			VisualServer::get_singleton()->immediate_vertex(im, p );
// 			
// 			--j;
// 			
// 		}
// 		
// 	}
// 	
// 	VisualServer::get_singleton()->immediate_end(im);
// 
// 	
// 	// DEBUG MESHES ************
// 	// FIBERS ************
// 	
// 	VisualServer::get_singleton()->immediate_begin(
// 		im, 
// 		(VisualServer::PrimitiveType) Mesh::PRIMITIVE_LINES, 
// 		RID());
// 	
// 	for ( uint32_t i = 0; i < fibers_num; ++i ) {
// 		Vector<int>& vs = raw.edges[i];
// 		for ( uint32_t j = 0; j < 2; ++j ) {
// 			Vector<float>& vert = raw.verts[vs[j]];
// 			Vector3 p( vert[1], vert[2], vert[3] );
// 			VisualServer::get_singleton()->immediate_vertex(im, p );
// 		}
// 	}
// 	
// 	VisualServer::get_singleton()->immediate_end(im);
// 	
// 	// LIGAMENTS ************
// 	VisualServer::get_singleton()->immediate_begin(
// 		im, 
// 		(VisualServer::PrimitiveType) Mesh::PRIMITIVE_LINES, 
// 		RID());
// 	
// 	for ( uint32_t i = 0; i < dots_num; ++i ) {
// 		Vector<float>& vert = raw.verts[i];
// 		Vector3 p( vert[1], vert[2], vert[3] );
// 		VisualServer::get_singleton()->immediate_vertex(im, p );
// 		VisualServer::get_singleton()->immediate_vertex(im, p );
// 	}
// 	
// 	VisualServer::get_singleton()->immediate_end(im);
// 	
// 	// all meshes created
// 	
// 	
// 	retrieve_object();
// 	
// 	// data has been pushed in godot engine,
// 	// we can now generate custom object that 
// 	// will interacts with it
// 	
// 	dots = new SkinDot[dots_num];
// 	forces = new Vector3[dots_num];
// 	Vector<Vector3>& vs = imm->chunks[0].vertices;
// 	Vector<Vector3>& ns = imm->chunks[0].normals;
// 		
// 	uint32_t fvs = 0;
// 	
// 	for ( uint32_t i = 0; i < faces_num; ++i ) {
// 		
// 		Vector<int>& fids = raw.faces[i];
// 		int j = fids.size() - 1;
// 		
// 		while( j >= 0 ) {
// 		
// 			uint32_t id = fids[j];
// 			SkinDot& sd = dots[id];
// 			if ( !sd.is_initialised() ) {
// 				sd.init(
// 					&vs[fvs],
// 					&ns[fvs],
// 					&forces[id]
// 				);
// 				sd.damping( 0.1 );
// 			} else {
// 				sd.register_vert( &vs[fvs] );
// 				sd.register_normal( &ns[fvs] );
// 			}
// 			
// 			--j;
// 			++fvs;
// 		
// 		}
// 		
// 	}
// 	
// 	// debugging ligaments
// 	Vector<Vector3>& vs_liga = imm->chunks[2].vertices;
// 	for ( uint32_t i = 0; i < dots_num; ++i ) {
// 		dots[i].register_vert( &vs_liga[(i*2)+1] );
// 	}
// 	
// 	fibers = new SkinFiber[fibers_num + dots_num];
// 	uint32_t fibid = 0;
// 	
// 	// generate fibers and tensors
// 	for( int i = 0; i < fibers_num; ++i, ++fibid ) {
// 		Vector<int>& vs = raw.edges[i];
// 		fibers[fibid].init( &dots[vs[0]], &dots[vs[1]] );
// 		if ( vs[2] != 0 ) {
// 			fibers[fibid].musclise(
// 				fibers[fibid].init_rest_len() * 0.2,
// 				fibers[fibid].init_rest_len() * 3.4,
// 				0.1, 0
// 				);
// 		}
// 	}
// 	
// 	// generate ligaments
// 	for( int i = 0; i < dots_num; ++i, ++fibid ) {
// 		fibers[fibid].init( &( dots[i].vert().ref() ), &dots[i] );
// 	}
// 	
// 	materials.resize( imm->chunks.size() );

}

void Skin::parse( const String& path ) {
	
	purge();
	
	_File f;
	f.open( path, _File::READ );
	
	String l = f.get_line();
	uint32_t i = 0;
	uint32_t index = 0;
	
	while (!f.eof_reached()) {
		
		if ( l.begins_with( "++vertices++" ) ) {
			
			raw.vpass = true;
			raw.epass = false;
			raw.fpass = false;
			index = 0;
			int num = l.right( 12 ).to_int();
			raw.verts.resize( num );
			
		} else if ( l.begins_with( "++edges++" ) ) {
			
			raw.vpass = false;
			raw.epass = true;
			raw.fpass = false;
			index = 0;
			int num = l.right( 9 ).to_int();
			raw.edges.resize( num );
			
		} else if ( l.begins_with( "++faces++" ) ) {
			
			raw.vpass = false;
			raw.epass = false;
			raw.fpass = true;
			index = 0;
			int num = l.right( 9 ).to_int();
			raw.faces.resize( num );
			
		} else if ( raw.vpass ) {
			
			Vector<float> vs = l.split_floats(" ");
			if ( vs.size() >= 7 ) {
				raw.verts[index] = vs; ++index;
			} else {
				std::cout << "Skin::parse, failed to decompress line " << i << std::endl;
				return;
			}
			
		} else if ( raw.epass ) {
			
			Vector<int> vs = l.split_ints(" ");
			if ( vs.size() >= 3 ) {
				raw.edges[index] = vs; ++index;
			} else {
				std::cout << "Skin::parse, failed to decompress line " << i << std::endl;
				return;
			}
			
		} else if ( raw.fpass ) {
			
			Vector<int> vs = l.split_ints(" ");
			if ( vs.size() >= 3 ) {
				raw.faces[index] = vs; ++index;
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
		raw.verts.empty() ||
		raw.edges.empty() ||
		raw.faces.empty()
		) {
		std::cout << "Skin::parse, inconsistent data: " <<
		"verts: " << raw.verts.size() << ". " <<
		"edges: " << raw.edges.size() << ". " <<
		"faces: " << raw.faces.size() << ". " <<
		std::endl;
		return;
	}
	
	raw.vpass = false;
	raw.epass = false;
	raw.fpass = false;
	
	generate();
	
}

AABB Skin::get_aabb() const {
	
	return aabb;
	
}

PoolVector<Face3> Skin::get_faces(uint32_t p_usage_flags) const {
	
	return PoolVector<Face3>();
	
}

void Skin::_notification(int p_what) {
	
	if (p_what == NOTIFICATION_PROCESS ) {
// 		_update();
	}
	
}

void Skin::update( const float& delta ) {

	std::cout << "Skin::_update()" << std::endl;
	
// 	if ( imm == 0 || dots == 0 ) {
	if ( mesh == 0 || dots == 0 ) {
// 		std::cout << "Skin::update, object not ready for update!" << std::endl;
		return;
	}
	
	for( uint32_t i = 0; i < dots_num; ++i ) {
		
// 		dots[i].update( delta );
		
	}
	
	for( uint32_t i = 0; i < fibers_num; ++i ) {
		
		fibers[i].update( delta );
		
	}
	
// 	PoolVector<Vector3> points;
// 	PoolVector<Vector3> normals;
// 	PoolVector<int> indices;
// 	
// 	for( uint32_t i = 0; i < dots_num; ++i ) {
// 		Vector<float>& vert = raw.verts[i];
// 		points.push_back( Vector3( vert[1], vert[2], vert[3] ) );
// 		normals.push_back( Vector3( vert[4], vert[5], vert[6] ) );
// 		
// 	}
// 	
// 	for ( uint32_t i = 0; i < faces_num; ++i ) {
// 		Vector<int>& fids = raw.faces[i];
// 		int j = fids.size() - 1;
// 		while( j >= 0 ) {
// 			indices.push_back(fids[j]);
// 			--j;
// 		}
// 	}
// 	
// 	mesh_arr.resize(VS::ARRAY_MAX);
// 	mesh_arr[VS::ARRAY_VERTEX] = points;
// 	mesh_arr[VS::ARRAY_NORMAL] = normals;
// 	mesh_arr[VS::ARRAY_INDEX] = indices;
// 	
// 	VisualServer::get_singleton()->mesh_clear( mesh_rid );
// 	VisualServer::get_singleton()->mesh_add_surface_from_arrays(
// 		mesh_rid,
// 		(VisualServer::PrimitiveType) Mesh::PRIMITIVE_TRIANGLES, 
// 		mesh_arr
// 	);
	
// 	VisualServer::get_singleton()->mesh_surface_set_material(
// 		mesh_rid, 0, 
// 		materials[0].is_null() ? RID() : materials[0]->get_rid()
// 	);
	
// 	RasterizerStorageGLES3::Surface* surf = mesh->surfaces[0];
// 	PoolVector<Vector3>::Read vr = mesh_arr[VS::ARRAY_VERTEX].read();
// 	uint32_t format = 0;
// 	for (int i = 0; i < mesh_arr.size(); i++) {
// 		if (mesh_arr[i].get_type() == Variant::NIL)
// 			continue;
// 		format |= (1 << i);
// 	}
// 	glBindBuffer(GL_ARRAY_BUFFER, surf->vertex_id);
// // 	glBufferData(GL_ARRAY_BUFFER, array_size, vr.ptr(), p_format & VS::ARRAY_FLAG_USE_DYNAMIC_UPDATE ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
// 	glBufferData(GL_ARRAY_BUFFER, dots_num * 3, vr.ptr(), format & VS::ARRAY_FLAG_USE_DYNAMIC_UPDATE ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
// 	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
// 	VisualServer::get_singleton()->mesh_surface_update_region(
// 		mesh_rid, 0, 
// 		0, mesh_arr[VS::ARRAY_VERTEX]
// 	);
	
// 	imm->instance_change_notify();
	
}

void Skin::set_surface_material(int p_surface, const Ref<Material> &p_material) {
		
// 	ERR_FAIL_INDEX(p_surface, materials.size());
// 	
// 	materials[p_surface] = p_material;
// 	
// 	if (materials[p_surface].is_valid()) {
// 		std::cout << "Skin::set_surface_material, materials is valid" << std::endl;
// 		VisualServer::get_singleton()->instance_set_surface_material(get_instance(), p_surface, materials[p_surface]->get_rid());
// 	} else {
// 		std::cout << "Skin::set_surface_material, materials is INvalid" << std::endl;
// 		VisualServer::get_singleton()->instance_set_surface_material(get_instance(), p_surface, RID());
// 	}
	
}

void Skin::_bind_methods() {
	
	ClassDB::bind_method(D_METHOD("render_skin", "delta"), &Skin::update);
	ClassDB::bind_method(D_METHOD("parse", "path"), &Skin::parse);
	ClassDB::bind_method(D_METHOD("set_surface_material", "p_surface", "p_material"), &Skin::set_surface_material);
	
}

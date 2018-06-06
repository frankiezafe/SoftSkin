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
// 	faces_num(0),
	dots(0),
	fibers(0),
	forces(0)
// 	faces(0)
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
// 	if ( faces != 0 ) {
// 		delete [] faces;
// 		faces = 0;
// 	}
	dots_num = 0;
	fibers_num = 0;
// 	faces_num = 0;
	
	VisualServer::get_singleton()->immediate_clear(im);
	
}

void Skin::generate( SkinRaw& raw ) {
	
	dots_num = raw.verts.size();
	fibers_num = raw.edges.size();
	uint32_t faces_num = raw.faces.size();
	
	VisualServer::get_singleton()->immediate_begin(
		im, 
		(VisualServer::PrimitiveType) Mesh::PRIMITIVE_TRIANGLES, 
		RID());
	
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
			} else {
				sd.register_vert( &vs[fvs] );
				sd.register_normal( &ns[fvs] );
			}
			
			--j;
			++fvs;
		
		}
		
	}
	
	fibers = new SkinFiber[fibers_num];
	for( int i = 0; i < fibers_num; ++i ) {
		Vector<int>& vs = raw.edges[i];
		fibers[i].init( &dots[vs[0]], &dots[vs[1]] );
		if ( vs[2] != 0 ) {
			fibers[i].musclise(
				fibers[i].init_rest_len() * 0.2,
				fibers[i].init_rest_len() * 1.4,
				0.5, 0
				);
		}
	}
		
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
	
	std::cout << "Skin::parse, successfull decompression:" << std::endl <<
		"\tdecompressed.verts " << decompressed.verts.size() << std::endl <<
		"\tdecompressed.edges " << decompressed.edges.size() << std::endl <<
		"\tdecompressed.faces " << decompressed.faces.size() << std::endl;
	
	generate( decompressed );
	
// 	dots = new SkinDot[dots_num];
// 	for( int i = 0; i < dots_num; ++i ) {
// 		Vector<float>& vs = decompressed.verts[i];
// 		dots[i].vert( vs[1], vs[2], vs[3] );
// 		dots[i].normal( vs[4], vs[5], vs[6] );
// 		dots[i].damping( 0.2f );
// 	}
// 	
// 	fibers = new SkinFiber[fibers_num];
// 	for( int i = 0; i < fibers_num; ++i ) {
// 		Vector<int>& vs = decompressed.edges[i];
// 		fibers[i].init( &dots[vs[0]], &dots[vs[1]] );
// 		if ( vs[2] != 0 ) {
// 			fibers[i].musclise(
// 				fibers[i].init_rest_len() * 0.2,
// 				fibers[i].init_rest_len() * 1.4,
// 				0.5, 0
// 				);
// 		}
// 	}
// 	
// 	faces = new uint32_t[faces_num];
// 	i = 0;
// 	int vmax = decompressed.faces.size();
// 	for( int v = 0; v < vmax; ++v ) {
// 		faces[i] = decompressed.faces[v][2]; ++i;
// 		faces[i] = decompressed.faces[v][1]; ++i;
// 		faces[i] = decompressed.faces[v][0]; ++i;
// 	}
	
// 	generate();
// 	std::cout << "Skin::parse lines: " << i << std::endl;
	
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
	
// 	RasterizerStorageGLES3::Immediate* imm = (RasterizerStorageGLES3::Immediate*) im.get_data();
// 	Vector<Vector3>& vs = imm->chunks[0].vertices;
// 	Vector<Vector3>& ns = imm->chunks[0].normals;
	
	for( uint32_t i = 0; i < dots_num; ++i ) {
		
		dots[i].update( delta );
		
	}
	
	for( uint32_t i = 0; i < fibers_num; ++i ) {
		
		fibers[i].update( delta );
		
	}
// 	for ( uint32_t i = 0; i < faces_num; ++i ) {
// 		SkinDot::set_v3( vs[i], dots[faces[i]].vert() );
// 		SkinDot::set_v3( ns[i], dots[faces[i]].normal() );
// 	}
	imm->instance_change_notify();
	
}

void Skin::_bind_methods() {
// 	ClassDB::bind_method(D_METHOD("cube"), &Skin::cube);
	ClassDB::bind_method(D_METHOD("render_skin", "delta"), &Skin::update);
	ClassDB::bind_method(D_METHOD("parse", "path"), &Skin::parse);
}

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

Skin::Skin() {
	
	im = VisualServer::get_singleton()->immediate_create();
	set_base(im);
	
	dots_num = 0;
	fibers_num = 0;
	faces_num = 0;
	
	dots = 0;
	fibers = 0;
	faces = 0;
	
}

Skin::~Skin() {
	
	VisualServer::get_singleton()->free(im);
	
	if ( dots != 0 ) delete [] dots;
	if ( fibers != 0 ) delete [] fibers;
	if ( faces != 0 ) delete [] faces;
	
}

void Skin::cube() {
	
	dots_num = 8;
	fibers_num = 12 * 2;
	faces_num = 12 * 3;
	uint32_t i =0;
	
	dots = new SkinDot[dots_num];
	
	dots[0].vert( -1,-1,-1 );
	dots[1].vert( 1,-1,-1 );
	dots[2].vert( 1,1,-1 );
	dots[3].vert( -1,1,-1 );
	
	dots[4].vert( -1,-1,1 );
	dots[5].vert( 1,-1,1 );
	dots[6].vert( 1,1,1 );
	dots[7].vert( -1,1,1 );
	
	fibers = new SkinFiber[fibers_num];
	i =0;
	// edges
	fibers[i].init( &dots[0], &dots[1] ); ++i;
	fibers[i].init( &dots[1], &dots[2] ); ++i;
	fibers[i].init( &dots[2], &dots[3] ); ++i;
	fibers[i].init( &dots[3], &dots[0] ); ++i;
	
	fibers[i].init( &dots[0], &dots[4] ); ++i;
	fibers[i].init( &dots[1], &dots[5] ); ++i;
	fibers[i].init( &dots[2], &dots[6] ); ++i;
	fibers[i].init( &dots[3], &dots[7] ); ++i;
	
	fibers[i].init( &dots[4], &dots[5] ); ++i;
	fibers[i].init( &dots[5], &dots[6] ); ++i;
	fibers[i].init( &dots[6], &dots[7] ); ++i;
	fibers[i].init( &dots[7], &dots[4] ); ++i;
	
	// faces median
	fibers[i].init( &dots[0], &dots[2] ); ++i;
	fibers[i].init( &dots[1], &dots[3] ); ++i;
	fibers[i].init( &dots[0], &dots[5] ); ++i;
	fibers[i].init( &dots[1], &dots[4] ); ++i;
	fibers[i].init( &dots[1], &dots[6] ); ++i;
	fibers[i].init( &dots[2], &dots[5] ); ++i;
	fibers[i].init( &dots[2], &dots[7] ); ++i;
	fibers[i].init( &dots[3], &dots[6] ); ++i;
	fibers[i].init( &dots[3], &dots[4] ); ++i;
	fibers[i].init( &dots[0], &dots[7] ); ++i;
	fibers[i].init( &dots[4], &dots[6] ); ++i;
	fibers[i].init( &dots[7], &dots[5] ); ++i;
	
	for( uint32_t i = 0; i < fibers_num; ++i ) {
		fibers[i].musclise( 0.9, 1.1, 0.5, i * 0.001 );
	}
	
	faces = new uint32_t[faces_num];
	i =0;
	faces[i] = 0; ++i;
	faces[i] = 1; ++i;
	faces[i] = 2; ++i;
	faces[i] = 2; ++i;
	faces[i] = 3; ++i;
	faces[i] = 0; ++i;
	
	faces[i] = 5; ++i;
	faces[i] = 1; ++i;
	faces[i] = 0; ++i;
	faces[i] = 0; ++i;
	faces[i] = 4; ++i;
	faces[i] = 5; ++i;
	
	faces[i] = 6; ++i;
	faces[i] = 2; ++i;
	faces[i] = 1; ++i;
	faces[i] = 1; ++i;
	faces[i] = 5; ++i;
	faces[i] = 6; ++i;
	
	faces[i] = 7; ++i;
	faces[i] = 3; ++i;
	faces[i] = 2; ++i;
	faces[i] = 2; ++i;
	faces[i] = 6; ++i;
	faces[i] = 7; ++i;
	
	faces[i] = 4; ++i;
	faces[i] = 0; ++i;
	faces[i] = 3; ++i;
	faces[i] = 3; ++i;
	faces[i] = 7; ++i;
	faces[i] = 4; ++i;
	
	faces[i] = 6; ++i;
	faces[i] = 5; ++i;
	faces[i] = 4; ++i;
	faces[i] = 4; ++i;
	faces[i] = 7; ++i;
	faces[i] = 6; ++i;
	
	VisualServer::get_singleton()->immediate_begin(
		im, 
		(VisualServer::PrimitiveType) Mesh::PRIMITIVE_TRIANGLES, 
		RID());
	
	for ( uint32_t i = 0; i < faces_num; ++i ) {
		const Vector3& vert = dots[faces[i]].vert();
		if ( i == 0 ) {
			aabb.position = vert;
			aabb.size = Vector3();
		} else {
			aabb.expand_to( vert );
		}
		VisualServer::get_singleton()->immediate_vertex(im, vert );
	}
		
	VisualServer::get_singleton()->immediate_end(im);
	
// 	std::cout << ((RasterizerStorageGLES3::Immediate*) im.get_data() )->chunks[0].vertices.size() << std::endl;
	
}

AABB Skin::get_aabb() const {
	
	return aabb;
}

PoolVector<Face3> Skin::get_faces(uint32_t p_usage_flags) const {
	
	return PoolVector<Face3>();
	
}

void Skin::update( float delta ) {
	
	RasterizerStorageGLES3::Immediate* imm = (RasterizerStorageGLES3::Immediate*) im.get_data();
	Vector<Vector3>& vs = imm->chunks[0].vertices;
	
	for( uint32_t i = 0; i < dots_num; ++i ) {
		dots[i].update( delta );
		SkinDot::set_v3( vs[i], dots[i].vert() );
	}
	for( uint32_t i = 0; i < fibers_num; ++i ) {
		fibers[i].update( delta );
	}
	
	VisualServer::get_singleton()->immediate_clear(im);
	VisualServer::get_singleton()->immediate_begin(
		im, 
		(VisualServer::PrimitiveType) Mesh::PRIMITIVE_TRIANGLES, 
		RID());
	
	for ( uint32_t i = 0; i < faces_num; ++i ) {
		const Vector3& vert = dots[faces[i]].vert();
		if ( i == 0 ) {
			aabb.position = vert;
			aabb.size = Vector3();
		} else {
			aabb.expand_to( vert );
		}
		VisualServer::get_singleton()->immediate_vertex(im, vert );
	}
	
// 	imm->building = true;
	VisualServer::get_singleton()->immediate_end(im);
	
// 	std::cout << ((RasterizerStorageGLES3::Immediate*) im.get_data() )->chunks.size() << std::endl;	
// 	std::cout << fibers[0].rest_len() << std::endl;
	
}

void Skin::_bind_methods() {
	ClassDB::bind_method(D_METHOD("cube"), &Skin::cube);
	ClassDB::bind_method(D_METHOD("render_skin", "delta"), &Skin::update);
}

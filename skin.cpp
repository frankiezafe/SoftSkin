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
	dots_num(0),
	fibers_num(0),
	dots(0),
	forces(0),
	ligaments_heads(0),
	fibers(0)
{}

Skin::~Skin() {
	
	purge();
	
}

void Skin::purge() {
	
	if (dots) memdelete_arr( dots );
	if (forces) memdelete_arr( forces );
	if (ligaments_heads) memdelete_arr( ligaments_heads );
	if (fibers) memdelete_arr( fibers );
	
	dots = 0;
	forces = 0;
	ligaments_heads = 0;
	fibers = 0;
	dots_num = 0;
	fibers_num = 0;
	
}

void Skin::generate( SkinRaw& raw ) {
	
	dots_num = raw.verts.size();
	fibers_num = raw.edges.size();
	uint32_t faces_num = raw.faces.size();
	
	unbind_root();
	
	surfaces.resize(3);
	uint8_t surfid = 0;
	
	// ********** MAIN MESH **********
	surfid = 0;
	surfaces[surfid].type = Mesh::PRIMITIVE_TRIANGLES;
	surfaces[surfid].material = main_material;
	for( uint32_t i = 0; i < dots_num; ++i ) {
		Vector<float>& vert = raw.verts[i];
		surfaces[surfid].vertices.push_back( Vector3( vert[1], vert[2], vert[3] ) );
		surfaces[surfid].normals.push_back( Vector3( vert[4], vert[5], vert[6] ) );
	}
	for ( uint32_t i = 0; i < faces_num; ++i ) {
		Vector<int>& fids = raw.faces[i];
		int j = fids.size() - 1;
		while( j >= 0 ) {
			surfaces[surfid].indices.push_back(fids[j]);
			--j;
		}
	}
	
	// ********** DEBUG FIBERS **********
	surfid = 1;
	surfaces[surfid].type = Mesh::PRIMITIVE_LINES;
	surfaces[surfid].material = fiber_material;
	for( uint32_t i = 0; i < dots_num; ++i ) {
		Vector<float>& vert = raw.verts[i];
		surfaces[surfid].vertices.push_back( Vector3( vert[1], vert[2], vert[3] ) );
		surfaces[surfid].normals.push_back( Vector3( vert[4], vert[5], vert[6] ) );
	}
	for ( uint32_t i = 0; i < fibers_num; ++i ) {
		Vector<int>& vs = raw.edges[i];
		surfaces[surfid].indices.push_back(vs[0]);
		surfaces[surfid].indices.push_back(vs[1]);
	}
	
	// ********** DEBUG LIGAMENTS **********
	surfid = 2;
	surfaces[surfid].type = Mesh::PRIMITIVE_LINES;
	surfaces[surfid].material = ligament_material;
	for( uint32_t i = 0; i < dots_num; ++i ) {
		Vector<float>& vert = raw.verts[i];
		surfaces[surfid].vertices.push_back( Vector3( vert[1], vert[2], vert[3] ) );
		surfaces[surfid].normals.push_back( Vector3( vert[4], vert[5], vert[6] ) );
		surfaces[surfid].vertices.push_back( Vector3( vert[1], vert[2], vert[3] ) );
		surfaces[surfid].normals.push_back( Vector3( vert[4], vert[5], vert[6] ) );
		surfaces[surfid].indices.push_back( ( i * 2 ) + 0 );
		surfaces[surfid].indices.push_back( ( i * 2 ) + 1 );
	}
	
	// linking all poolvector writters
	for ( uint32_t i = 0; i < surfaces.size(); ++i ) {
		surfaces[i].verticesw = surfaces[i].vertices.write();
		surfaces[i].normalsw = surfaces[i].normals.write();
		surfaces[i].uvsw = surfaces[i].uvs.write();
		surfaces[i].indicesw = surfaces[i].indices.write();
	}
	
	// ceation of skin objects
	dots = memnew_arr( SkinDot , dots_num );
	forces = memnew_arr( Vector3 , dots_num );
	
	for( uint32_t i = 0; i < dots_num; ++i ) {
		forces[i][0] = 0;
		forces[i][1] = 0;
		forces[i][2] = 0;
		dots[i].init(
			&surfaces[0].verticesw[i],
			&surfaces[0].normalsw[i],
			&forces[i]
		);
		dots[i].register_vert( 
			&surfaces[2].verticesw[ ( i * 2 ) + 1 ]
		);
	}
	
	fibers = memnew_arr( SkinFiber , fibers_num + dots_num );
	uint32_t fibid = 0;
	
	// generate fibers and tensors
	for( int i = 0; i < fibers_num; ++i, ++fibid ) {
		Vector<int>& vs = raw.edges[i];
		fibers[fibid].fiber( &dots[vs[0]], &dots[vs[1]] );
		if ( vs[2] != 0 ) {
			fibers[fibid].musclise(
				fibers[fibid].init_rest_len() * 0.2,
				fibers[fibid].init_rest_len() * 1.4,
				0.5, 0
			);
		}
	}
	// generate ligaments
	ligaments_heads = memnew_arr( Vector3* , dots_num );
	for( int i = 0; i < dots_num; ++i, ++fibid ) {
		ligaments_heads[i] = &surfaces[surfid].verticesw[ ( i * 2 ) ];
		fibers[fibid].ligament( ligaments_heads[i], &dots[i] );
	}
	
	bind_root();
	
	// data has been pushed in godot engine,
	// we can now generate custom object that 
	// will interacts with it

/*
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
	
	// debugging ligaments
	Vector<Vector3>& vs_liga = imm->chunks[2].vertices;
	for ( uint32_t i = 0; i < dots_num; ++i ) {
		dots[i].register_vert( &vs_liga[(i*2)+1] );
	}
	
	fibers = new SkinFiber[fibers_num + dots_num];
	uint32_t fibid = 0;
	
	fibers = new SkinFiber[fibers_num];
	for( int i = 0; i < fibers_num; ++i ) {
		Vector<int>& vs = raw.edges[i];
		fibers[fibid].init( &dots[vs[0]], &dots[vs[1]] );
		if ( vs[2] != 0 ) {
			fibers[fibid].musclise(
				fibers[fibid].init_rest_len() * 0.2,
				fibers[fibid].init_rest_len() * 1.4,
				0.5, 0
				);
		}
	}
	// generate ligaments
	for( int i = 0; i < dots_num; ++i, ++fibid ) {
		fibers[fibid].init( &( dots[i].vert().ref() ), &dots[i] );
	}
*/
		
}

void Skin::unbind_root() {

	set_base(RID());
	root_mesh.unref(); //byebye root mesh

}

void Skin::bind_root() {
	
	root_mesh.instance();
	
	for ( uint32_t i = 0; i < surfaces.size(); ++i ) {
		
		surfaces[i].verticesw = PoolVector<Vector3>::Write();
		surfaces[i].normalsw = PoolVector<Vector3>::Write();
		surfaces[i].uvsw = PoolVector<Vector2>::Write();
		surfaces[i].indicesw = PoolVector<int>::Write();
		
		int idx = root_mesh->get_surface_count();
		switch( surfaces[i].type ) {
			case Mesh::PRIMITIVE_TRIANGLES:
			{
				Array array;
				array.resize(Mesh::ARRAY_MAX);
				array[Mesh::ARRAY_VERTEX] = surfaces[i].vertices;
				array[Mesh::ARRAY_NORMAL] = surfaces[i].normals;
				//array[Mesh::ARRAY_TEX_UV] = surfaces[i].uvs;
				array[Mesh::ARRAY_INDEX] = surfaces[i].indices;
				root_mesh->add_surface_from_arrays( Mesh::PRIMITIVE_TRIANGLES, array);
			}
			break;
			case Mesh::PRIMITIVE_LINES:
			{
				Array array;
				array.resize(Mesh::ARRAY_MAX);
				array[Mesh::ARRAY_VERTEX] = surfaces[i].vertices;
				array[Mesh::ARRAY_INDEX] = surfaces[i].indices;
				root_mesh->add_surface_from_arrays( Mesh::PRIMITIVE_LINES, array);
			}
			break;
		}
		root_mesh->surface_set_material(idx, surfaces[i].material);
		
	}
	
	set_base(root_mesh->get_rid());
	
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
	
	unbind_root();
	
	for( uint32_t i = 0; i < dots_num; ++i ) {
		dots[i].update( delta );
	}
	
	for( uint32_t i = 0; i < fibers_num; ++i ) {
		fibers[i].update( delta );
	}
	
	bind_root();
	
/*	
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
*/
}

void Skin::set_main_material( const Ref<Material> &material ) {
	main_material = material;
}

Ref<Material> Skin::get_main_material() const {
	return main_material;
}

void Skin::set_fiber_material( const Ref<Material> &material ) {
	fiber_material = material;
}

Ref<Material> Skin::get_fiber_material() const {
	return fiber_material;
}

void Skin::set_ligament_material( const Ref<Material> &material ) {
	ligament_material = material;
}

Ref<Material> Skin::get_ligament_material() const {
	return ligament_material;
}

void Skin::_bind_methods() {
	
	ClassDB::bind_method(D_METHOD("render_skin", "delta"), &Skin::update);
	ClassDB::bind_method(D_METHOD("parse", "path"), &Skin::parse);
	
	ClassDB::bind_method(D_METHOD("set_main_material", "material"), &Skin::set_main_material);
	ClassDB::bind_method(D_METHOD("get_main_material"), &Skin::get_main_material);
	ClassDB::bind_method(D_METHOD("set_fiber_material", "material"), &Skin::set_fiber_material);
	ClassDB::bind_method(D_METHOD("get_fiber_material"), &Skin::get_fiber_material);
	ClassDB::bind_method(D_METHOD("set_ligament_material", "material"), &Skin::set_ligament_material);
	ClassDB::bind_method(D_METHOD("get_ligament_material"), &Skin::get_ligament_material);
	
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, 
			"main_material", PROPERTY_HINT_RESOURCE_TYPE, 
			"SpatialMaterial,ShaderMaterial"), 
			"set_main_material", "get_main_material");
	
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, 
			"fiber_material", PROPERTY_HINT_RESOURCE_TYPE, 
			"SpatialMaterial,ShaderMaterial"), 
			"set_fiber_material", "get_fiber_material");
	
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, 
			"ligament_material", PROPERTY_HINT_RESOURCE_TYPE, 
			"SpatialMaterial,ShaderMaterial"), 
			"set_ligament_material", "get_ligament_material");
	
}

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
 *  
 * 
 */

#ifndef SKINCOMMON_H
#define SKINCOMMON_H

#include "core/bind/core_bind.h"
#include "scene/3d/visual_instance.h"
#include "scene/resources/mesh.h"
#include "drivers/gles3/rasterizer_storage_gles3.h"

// decompression data
struct SkinRaw {
	bool vpass;
	bool epass;
	bool fpass;
	Vector< Vector<float> > verts;
	Vector< Vector<int> > edges;
	Vector< int > ligaments;
	Vector< Vector<int> > faces;
	SkinRaw() : vpass(false), epass(false), fpass(false) {}
};

struct ShapeUpdateSurface {
	int type;
	bool enabled;
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
	ShapeUpdateSurface() : type(0), enabled(false) {}
};

#endif // SKINCOMMON_H

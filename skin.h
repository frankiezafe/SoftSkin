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

#include "skindot.h"
#include "skinfiber.h"

#include "scene/3d/visual_instance.h"
#include "scene/resources/mesh.h"
#include "drivers/gles3/rasterizer_storage_gles3.h"

class Skin: public GeometryInstance {
	
	GDCLASS(Skin, GeometryInstance);
	
public:
	
	Skin();
	
	~Skin();
	
	void cube();
	
	void update( float delta );
	
	virtual AABB get_aabb() const;
	
	virtual PoolVector<Face3> get_faces(uint32_t p_usage_flags) const;
	
protected:
	
	static void _bind_methods();
	
	AABB aabb;
	
private:
	
	RID im;
	
	uint32_t dots_num;
	uint32_t fibers_num;
	uint32_t faces_num;
	SkinDot* dots;
	SkinFiber* fibers;
	uint32_t* faces;
	
};

#endif // SKIN_H
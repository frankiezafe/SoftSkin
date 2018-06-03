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
	
	dots_num = 0;
	fibers_num = 0;
	faces_num = 0;
	
	dots = 0;
	fibers = 0;
	faces = 0;
	
}

void Skin::cube() {
		
	dots_num = 8;
	faces_num = 12 * 3;
	
	dots = new SkinDot[dots_num];
	
	dots[0].vert( -1,-1,-1 );
	dots[1].vert( 1,-1,-1 );
	dots[2].vert( 1,1,-1 );
	dots[3].vert( -1,1,-1 );
	
	dots[4].vert( -1,-1,1 );
	dots[5].vert( 1,-1,1 );
	dots[6].vert( 1,1,1 );
	dots[7].vert( -1,1,1 );
	
	uint32_t i =0;
	faces = new uint32_t[faces_num];
	
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
	
	begin(Mesh::PRIMITIVE_TRIANGLES );
	for ( uint32_t i = 0; i < faces_num; ++i ) {
		add_vertex( dots[faces[i]].vert() );
	}
	end();
	
}

void Skin::_bind_methods() {
	ClassDB::bind_method(D_METHOD("cube"), &Skin::cube);
}

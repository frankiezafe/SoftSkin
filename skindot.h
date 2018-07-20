/*
 * 
 * 
 * _________ ____  .-. _________/ ____ .-. ____ 
 * __|__  (_)_/(_)(   )____<    \|    (   )  (_)
 *                 `-'                 `-'      
 * 
 * 
 * art & game engine
 * 
 * ____________________________________  ?   ____________________________________
 *                                     (._.)
 * 
 * 
 * This file is part of softskin library
 * For the latest info, see http://polymorph.cool/
 * 
 * Copyright (c) 2018 polymorph.cool
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * ___________________________________( ^3^)_____________________________________
 * 
 * ascii font: rotated by MikeChat & myflix
 * have fun and be cool :)
 * 
 */


#ifndef SKINDOT_H
#define SKINDOT_H

#include <iostream>
#include <ostream>
#include <vector>
#include "vector3.h"
#include "vectorptr.h"

#include "skincommon.h"

class SkinDot {
    // in case there is more than 1 usage of the vertice
    // in the mesh

    template <typename S, typename T>
    class SkinDotMirror {
    public:

        bool empty;
        S* src;
        typename std::vector< T* > vecs;
        typename std::vector< T* >::iterator vecs_end;

        SkinDotMirror() : empty(true), src(0) {
        }

        bool is_enabled() const {
            src != 0;
        }

        void add(T* v) {
            
            assert(src != 0);
            
            vecs.push_back(v);
            vecs_end = vecs.end();
            empty = false;

        }

        void sync() {

            if (empty) {
                return;
            }

            assert(src != 0);

            typename std::vector< T* >::iterator it = vecs.begin();
            for (; it < vecs_end; ++it) {
                (*src) >> (*it);
            }

        }

    };

public:

    // do not forget to initialised after calling this constructor!!
    SkinDot();

    SkinDot(const float& x, const float& y, const float& z);

    SkinDot(Vector3* vert, Vector3* normal, Vector3* force, Vector2* uv2 = 0);

    void init(const float& x, const float& y, const float& z);

    void init(Vector3* vert, Vector3* normal, Vector3* force, Vector2* uv2 = 0);

    void register_vert(Vector3* vert);

    void register_normal(Vector3* normal);

    void register_force(Vector3* force);
    
    void register_uv2(Vector2* uv2);

    void push(const Vector3& f);

    const VectorPtr< Vector3 >& vert() const;

    const VectorPtr< Vector3 >& force() const;

    const VectorPtr< Vector3 >& normal() const;
    
    const VectorPtr< Vector2 >& uv2() const;

    const float& damping() const;

    const float& kicks() const;

    void vert(const float& x, const float& y, const float& z);

    void normal(const float& x, const float& y, const float& z);

    void damping(const float& d);

    const Vector3& update(const float& delta_time);

    const bool is_initialised() const {

        return _inititalised;

    }

    void operator=(const SkinDot& src);

    friend std::ostream &operator<<(
            std::ostream &os,
            SkinDot const &sd
            ) {

        return os <<
                sd.vert()[0] << ", " <<
                sd.vert()[0] << ", " <<
                sd.vert()[0];

    }

    void ray_distance(
            const Vector3& origin,
            const Vector3& ray,
            SkinRayResult& result
            );

protected:

    // 	static void _bind_methods();

private:
    
    Vector3 _vert_origin;
    float _total_push_length;

    VectorPtr< Vector3 > _vert;
    VectorPtr< Vector3 > _normal;
    VectorPtr< Vector3 > _force;
    VectorPtr< Vector2 > _uv2;
    float _damping;
    uint16_t _kicks;

    SkinDotMirror<VectorPtr< Vector3 >, Vector3> mirror_verts;
    SkinDotMirror<VectorPtr< Vector3 >, Vector3> mirror_normals;
    SkinDotMirror<VectorPtr< Vector3 >, Vector3> mirror_forces;
    SkinDotMirror<VectorPtr< Vector2 >, Vector2> mirror_uv2s;

    bool _inititalised;

    void init_internal();

};

#endif // SKINDOT_H

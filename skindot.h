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
#include "vector3ptr.h"

#include "skincommon.h"

class SkinDot {
    
    // in case there is more than 1 usage of the vertice
    // in the mesh

    struct SkinDotMirror {
        bool empty;
        Vector3ptr* src;
        std::vector< Vector3* > vec3s;
        std::vector< Vector3* >::iterator vec3s_end;

        SkinDotMirror() : empty(true), src(0) {
        }

        bool is_enabled() const {
            src != 0;
        }

        void add(Vector3* v) {

            assert(src != 0);

            vec3s.push_back(v);
            vec3s_end = vec3s.end();
            empty = false;

        }

        void sync() {

            if (empty) {
                return;
            }

            assert(src != 0);

            std::vector< Vector3* >::iterator it = vec3s.begin();
            for (; it < vec3s_end; ++it) {
                (*src) >> (*it);
            }

        }

    };

public:

    // do not forget to initialised after calling this constructor!!
    SkinDot();

    SkinDot(const float& x, const float& y, const float& z);

    SkinDot(Vector3* vert, Vector3* normal, Vector3* force);

    void init(const float& x, const float& y, const float& z);

    void init(Vector3* vert, Vector3* normal, Vector3* force);

    void register_vert(Vector3* vert);

    void register_normal(Vector3* normal);

    void register_force(Vector3* force);

    void push(const Vector3& f);

    const Vector3ptr& vert() const;

    const Vector3ptr& force() const;

    const Vector3ptr& normal() const;

    const float& damping() const;

    const float& kicks() const;

    void vert(const float& x, const float& y, const float& z);

    void normal(const float& x, const float& y, const float& z);

    void damping(const float& d);

    void gravity(Vector3* g);

    void update(const float& delta_time);

    const bool is_initialised() const {

        return _inititalised;

    }

    void operator=(const SkinDot& src);

    friend std::ostream &operator<<(std::ostream &os, SkinDot const &sd) {

        return os <<
                sd.vert()[0] << ", " <<
                sd.vert()[0] << ", " <<
                sd.vert()[0];

    }
    
    void ray_distance( 
        const Vector3& origin, 
        const Vector3& ray,
        SkinRay& result
    );

protected:

    // 	static void _bind_methods();

private:

    Vector3ptr _vert;
    Vector3ptr _normal;
    Vector3ptr _force;
    Vector3* _gravity;
    float _damping;
    uint16_t _kicks;

    SkinDotMirror mirror_verts;
    SkinDotMirror mirror_normals;
    SkinDotMirror mirror_forces;

    bool _inititalised;

    void init_internal();

};

#endif // SKINDOT_H

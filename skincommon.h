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

#include <iostream>
#include <limits>
#include <map>

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
    Vector< Vector<float> > faces;

    SkinRaw() : vpass(false), epass(false), fpass(false) {
    }
};

struct ShapeUpdateSurface {
    int type;
    bool enabled;
    PoolVector<Vector3> vertices;
    PoolVector<Vector3> normals;
    PoolVector<int> indices;
    PoolVector<Vector2> uvs;
    PoolVector<Vector2> uvs2;
    Ref<Material> material;
    int last_added;
    PoolVector<Vector3>::Write verticesw;
    bool normal_enabled;
    PoolVector<Vector3>::Write normalsw;
    PoolVector<int>::Write indicesw;
    bool uv_enabled;
    PoolVector<Vector2>::Write uvsw;
    bool uv2_enabled;
    PoolVector<Vector2>::Write uvs2w;

    ShapeUpdateSurface() :
    type(0),
    enabled(false),
    normal_enabled(false),
    uv_enabled(false),
    uv2_enabled(false) {
    }
};

enum skin_notifier_t {
    sn_UNDEFINED,
    sn_SKIN,
    sn_SERVER
};

enum skin_notification_t {
    sno_UNDEFINED,
    sno_NOTIFIER_DOWN,
    sno_SKIN_CREATED,
    sno_SKIN_MOVED,
    sno_SKIN_DELETED,
    sno_SERVER_CREATED,
    sno_SERVER_MOVED,
    sno_SERVER_DELETED
};

class SkinNotifierListener {
public:

    SkinNotifierListener() :
    _skl_type(sn_UNDEFINED) {
    }

    const skin_notifier_t& skl_type() const {
        return _skl_type;
    }

    virtual void skin_notification(const skin_notification_t& what) {
        return;
    }

protected:

    skin_notifier_t _skl_type;

};

class SkinRayResult {
public:

    void* skin_ptr;
    uint32_t dot_index;
    real_t distance_to_origin;
    real_t dot_to_ray;
    real_t distance_to_ray;
    real_t influence;

    SkinRayResult() :
    skin_ptr(0),
    dot_index(std::numeric_limits<uint32_t>::max()),
    distance_to_origin(0),
    dot_to_ray(0),
    distance_to_ray(0),
    influence(0) {
    }

    void operator=(const SkinRayResult& src) {
        skin_ptr = src.skin_ptr;
        dot_index = src.dot_index;
        distance_to_origin = src.distance_to_origin;
        dot_to_ray = src.dot_to_ray;
        distance_to_ray = src.distance_to_ray;
        influence = src.influence;
    }

};

typedef std::vector<SkinRayResult> skinresult_vector;
typedef skinresult_vector::iterator skinresult_vector_iterator;
typedef std::map<void*, skinresult_vector> skinray_map;
typedef skinray_map::iterator skinray_map_iterator;

class SkinRay {
public:

    Vector3 world_position;
    float world_distance;
    bool success;

    /* This map stores skin dots tests per Skin 
     * It will be analysed by the server once all skins have been visited.
     */
    skinray_map results;

    SkinRayResult* closest_result;

    SkinRay() :
    success(false),
    closest_result(0) {
    }

    ~SkinRay() {
        reset();
    }

    void reset() {
        success = false;
        if (closest_result) {
            delete closest_result;
        }
        closest_result = 0;
        results.clear();
    }

};

#endif // SKINCOMMON_H

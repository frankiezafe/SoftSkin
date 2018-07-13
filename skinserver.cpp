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

/* 
 * File:   SkinServer.cpp
 * Author: frankiezafe
 * 
 * Created on July 10, 2018, 6:48 PM
 */

#include "skinserver.h"

static uint32_t UID_COUNTER = 0;

SkinServer::SkinServer() :
SkinNotifierListener(),
_UID(UID_COUNTER++),
_valid(true) {

    //    std::cout << "SkinServer created with UID: " << _UID << std::endl;
    _skl_type = sn_SERVER;
    SkinNotifier::subscribe(this);

}

SkinServer::~SkinServer() {

    SkinNotifier::unsubscribe(this);
    SkinNotifier::notify(sno_SERVER_DELETED);

}

bool SkinServer::register_skin(Skin* s) {

    if (!_valid) {
        return false;
    }

    if (std::find(_skins.begin(), _skins.end(), s) == _skins.end()) {
        _skins.push_back(s);
        return true;
    }

    return false;

}

bool SkinServer::unregister_skin(Skin* s) {

    std::vector< Skin* >::iterator it =
            std::find(_skins.begin(), _skins.end(), s);
    if (it == _skins.end()) {
        return false;
    }

    _skins.erase(it);
    return true;

}

void SkinServer::set_skin_num(int i) {
    return;
}

void SkinServer::set_UID(int i) {
    return;
}

int SkinServer::get_skin_num() const {
    return _skins.size();
}

int SkinServer::get_UID() const {
    return _UID;
}

void SkinServer::_bind_methods() {

    ClassDB::bind_method(D_METHOD("set_skin_num", "i"), &SkinServer::set_skin_num);
    ClassDB::bind_method(D_METHOD("get_skin_num"), &SkinServer::get_skin_num);

    ClassDB::bind_method(D_METHOD("set_UID", "i"), &SkinServer::set_UID);
    ClassDB::bind_method(D_METHOD("get_UID"), &SkinServer::get_UID);

    ClassDB::bind_method(D_METHOD("hit", "from", "to", "radius"), &SkinServer::hit);
    ClassDB::bind_method(D_METHOD("hit_point"), &SkinServer::hit_point);
    ClassDB::bind_method(D_METHOD("drag_point", "from", "to"), &SkinServer::drag_point);

    ADD_GROUP("SkinServer", "skinserver_");

    ADD_PROPERTY(PropertyInfo(Variant::INT,
            "skinserver_UID", PROPERTY_HINT_NONE),
            "set_UID",
            "get_UID");

    ADD_PROPERTY(PropertyInfo(Variant::INT,
            "skinserver_skin_num", PROPERTY_HINT_NONE),
            "set_skin_num",
            "get_skin_num");

}

void SkinServer::_notification(int p_what) {

    switch (p_what) {
        case NOTIFICATION_ENTER_TREE:
            SkinNotifier::notify(sno_SERVER_CREATED);
            break;
        case NOTIFICATION_PARENTED:
        case NOTIFICATION_PATH_CHANGED:
        case NOTIFICATION_MOVED_IN_PARENT:
        case NOTIFICATION_UNPARENTED:
            SkinNotifier::notify(sno_SERVER_MOVED);
            break;
        default:
            break;

    }

}

void SkinServer::seek_skins() {

    _skins.clear();

    int cc = get_child_count();

    for (int i = 0; i < cc; ++i) {

        Node* sn = get_child(i);

        if (sn->is_class("Skin")) {
            _skins.push_back((Skin*) sn);
        } else {
            std::cout << "Child " << i << " is not a Skin" << std::endl;
        }

    }

}

void SkinServer::skin_notification(const skin_notification_t& what) {

    switch (what) {

        case sno_NOTIFIER_DOWN:
            _skins.clear();
            break;

        case sno_SKIN_CREATED:
        case sno_SKIN_MOVED:
        case sno_SKIN_DELETED:
            seek_skins();
            break;

    }

}

const Vector3& SkinServer::hit_point() const {

    return _hit_point.world_position;

}

const Vector3& SkinServer::drag_point(const Vector3& from, const Vector3& to) {

    if (_hit_point.closest_result) {
        
        // rendering world position based on world_distance
        Vector3 dir = to - from;        
        _hit_point.world_position = 
                from + 
                dir.normalized() * _hit_point.world_distance;

        skinray_map_iterator itr = _hit_point.results.begin();
        skinray_map_iterator itre = _hit_point.results.end();

        for (; itr != itre; ++itr) {

            if (itr->second.empty()) {
                continue;
            }
            ((Skin*) itr->first)->drag(_hit_point);

        }

    }

    return _hit_point.world_position;

}

bool SkinServer::hit(
        const Vector3& from,
        const Vector3& to,
        const real_t& radius) {

    if (_skins.empty()) {
        return false;
    }

    std::vector< Skin* >::iterator it = _skins.begin();
    std::vector< Skin* >::iterator ite = _skins.end();

    _hit_point.reset();

    for (; it != ite; ++it) {
        (*it)->hit(from, to, radius, _hit_point);
    }

    _hit_point.success = _hit_point.closest_result != 0;

    if (_hit_point.success) {

        // removal of too far of the ray.closest_result
        skinray_map_iterator itr = _hit_point.results.begin();
        skinray_map_iterator itre = _hit_point.results.end();

        for (; itr != itre; ++itr) {

            skinresult_vector valids;
            skinresult_vector_iterator itrv = itr->second.begin();
            skinresult_vector_iterator itrve = itr->second.end();

            for (; itrv != itrve; ++itrv) {

                SkinRayResult& srr = (*itrv);

                std::cout << "testing point: " <<
                            srr.skin_ptr << " : " <<
                            srr.dot_index << ", " <<
                            srr.distance_to_origin << " <> " << 
                            _hit_point.closest_result->distance_to_origin << 
                            std::endl;

                if (
                        srr.distance_to_origin -
                        _hit_point.closest_result->distance_to_origin <=
                        radius
                        ) {

                    srr.influence = 1 - (
                            (srr.distance_to_ray / radius) *
                            (
                            srr.distance_to_origin -
                            _hit_point.closest_result->distance_to_origin
                            ) / radius
                            );
                    
                    

                    std::cout << "valid point: " <<
                            srr.skin_ptr << " : " <<
                            srr.dot_index << ", " <<
                            srr.influence << std::endl;

                    valids.push_back(srr);

                }

            }

            itr->second.swap(valids);

        }

    }

    return _hit_point.success;

}
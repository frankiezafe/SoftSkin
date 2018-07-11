/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
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
    
    ClassDB::bind_method(D_METHOD("hit","from","to","radius"), &SkinServer::hit);
    ClassDB::bind_method(D_METHOD("hit_point"), &SkinServer::hit_point);
    ClassDB::bind_method(D_METHOD("drag_point","from","to"), &SkinServer::drag_point);

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

const Vector3& SkinServer::drag_point( const Vector3& from, const Vector3& to ) {
    
    if ( !_hit_point.skin_ptr ) {
        return _hit_point.world_position;
    }
    
    ((Skin*) _hit_point.skin_ptr)->drag( from, to, _hit_point );
    return _hit_point.world_position;
    
}

bool SkinServer::hit(
        const Vector3& from,
        const Vector3& to,
        const real_t& radius) {
    
    if ( _skins.empty() ) {
        return false;
    }
    
    std::vector< Skin* >::iterator it = _skins.begin();
    std::vector< Skin* >::iterator ite = _skins.end();
    
    SkinRay ray;
    Skin* _sel = 0;
            
    for ( ; it != ite; ++it ) {
        ray.success = false;
        (*it)->hit( from, to, radius, ray );
        if ( ray.success ) {
            ray.skin_ptr = (*it);
        }
        
    }
    
    _hit_point = ray;
        
//        std::cout << "SkinServer::hit, success " << 
//                _sel << " at dot " <<
//                ray.dot_id << ", position: " <<
//                ray.world_position.x << ", " <<
//                ray.world_position.y << ", " <<
//                ray.world_position.z << 
//                std::endl;
    
    return _hit_point.skin_ptr != 0;
    
}
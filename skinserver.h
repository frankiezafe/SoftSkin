/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SkinServer.h
 * Author: frankiezafe
 *
 * Created on July 10, 2018, 6:48 PM
 */

#ifndef SKINSERVER_H
#define SKINSERVER_H

#include <iostream>
#include <algorithm>

#include "skincommon.h"
#include "skin.h"
#include "skinnotifier.h"

class SkinServer : public Node, public SkinNotifierListener {
    
    GDCLASS(SkinServer, Node);

public:

    SkinServer();

    virtual ~SkinServer();

    bool register_skin(Skin* s);

    bool unregister_skin(Skin* s);
    
    /*Has no effect!*/
    void set_skin_num( int i );
    
    /*Has no effect!*/
    void set_UID( int i );
    
    int get_skin_num() const;
    
    int get_UID() const;
    
    void skin_notification(const skin_notification_t& what);
    
    // interactions
    bool hit( const Vector3& from, const Vector3& to, const real_t& radius );
    
    const Vector3& hit_point() const;
    
    const Vector3& drag_point( const Vector3& from, const Vector3& to );
    
protected:

    void _notification(int p_what);
    
    static void _bind_methods();

private:

    uint32_t _UID;
    bool _valid;
    SkinRay _hit_point;
    
    std::vector< Skin* > _skins;
        
    void seek_skins();
    
    

};

#endif /* SKINSERVER_H */


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


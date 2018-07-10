/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SkinNotifier.h
 * Author: frankiezafe
 *
 * Created on July 10, 2018, 9:37 PM
 */

#ifndef SKINNOTIFIER_H
#define SKINNOTIFIER_H

#include <iostream>
#include <vector>
#include <algorithm>

#include "skincommon.h"

class SkinNotifier {
public:

    static void notify(const skin_notification_t& what);

    static bool subscribe(SkinNotifierListener* l);

    static bool unsubscribe(SkinNotifierListener* l);

private:

    static void check_instance();

    SkinNotifier();

    virtual ~SkinNotifier();

    void _notify(const skin_notification_t& what);

    bool _subscribe(SkinNotifierListener* l);

    bool _unsubscribe(SkinNotifierListener* l);

    std::vector< SkinNotifierListener* > _listeners;

};

#endif /* SKINNOTIFIER_H */


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SkinNotifier.cpp
 * Author: frankiezafe
 * 
 * Created on July 10, 2018, 9:37 PM
 */

#include "skinnotifier.h"

static SkinNotifier* _instance = 0;

void SkinNotifier::check_instance() {
    if (!_instance) {
        _instance = new SkinNotifier();
    }
}

void SkinNotifier::notify(const skin_notification_t& what) {

    check_instance();
    _instance->_notify(what);

}

bool SkinNotifier::subscribe(SkinNotifierListener* l) {

    check_instance();
    return _instance->_subscribe(l);

}

bool SkinNotifier::unsubscribe(SkinNotifierListener* l) {

    check_instance();
    return _instance->_unsubscribe(l);

}

SkinNotifier::SkinNotifier() {
}

SkinNotifier::~SkinNotifier() {

    _notify(sno_NOTIFIER_DOWN);

}

bool SkinNotifier::_subscribe(SkinNotifierListener* l) {

    if (std::find(_listeners.begin(), _listeners.end(), l) ==
            _listeners.end()) {
        _listeners.push_back(l);
        return true;
    }

    return false;

}

bool SkinNotifier::_unsubscribe(SkinNotifierListener* l) {

    std::vector< SkinNotifierListener* >::iterator it =
            std::find(_listeners.begin(), _listeners.end(), l);
    if (it == _listeners.end()) {
        return false;
    }

    _listeners.erase(it);

    return true;

}

void SkinNotifier::_notify(const skin_notification_t& what) {

    std::vector< SkinNotifierListener* >::iterator it =
            _listeners.begin();
    std::vector< SkinNotifierListener* >::iterator ite =
            _listeners.end();

    for (; it != ite; ++it) {
        (*it)->skin_notification(what);
    }

}

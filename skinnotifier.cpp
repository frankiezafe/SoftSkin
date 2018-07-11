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

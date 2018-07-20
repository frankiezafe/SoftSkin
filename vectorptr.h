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

#ifndef VECTORPTR_H
#define VECTORPTR_H

#include "vector3.h"
#include <assert.h>

template< class T > 
class VectorPtr {
public:

    VectorPtr() :
    _vecptr(0),
    _local(false),
    _inititalised(false) {
    }

    VectorPtr(T* src) :
    _vecptr(src),
    _local(false),
    _inititalised(false) {
    }

    VectorPtr(const T& src) :
    _local(true),
    _inititalised(false) {

        _vecptr = new T();
        (*_vecptr) = src;

    }

    ~VectorPtr() {
        purge();
    }

    void init(T* src) {

        purge();

        if (src) {
            _local = false;
            _vecptr = src;
            _inititalised = true;
        }

    }

    void init(const T& src) {

        purge();
        
        _local = true;
        _vecptr = new T();
        (*_vecptr) = src;
        _inititalised = true;

    }

    // operators

    _FORCE_INLINE_ void operator=(const T& src) {
        assert(_inititalised);
        (*_vecptr) = src;
    }

    _FORCE_INLINE_ void operator=(const VectorPtr& srcptr) {
        assert(_inititalised);
        (*_vecptr) = srcptr.ref();
    }

    _FORCE_INLINE_ void operator+=(const T& src) {
        assert(_inititalised);
        (*_vecptr) += src;
    }

    _FORCE_INLINE_ void operator-=(const T& src) {
        assert(_inititalised);
        (*_vecptr) -= src;
    }

    _FORCE_INLINE_ const float& operator[](const uint8_t& i) const {
        assert(_inititalised);
        return (*_vecptr)[i];
    }

    _FORCE_INLINE_ void operator>>(T* src) const {
        assert(_inititalised);
        (*src) = (*_vecptr);
    }

    // getters

    _FORCE_INLINE_ T* ptr() const {
        return _vecptr;
    }

    _FORCE_INLINE_ T& ref() const {
        return (*_vecptr);
    }

    _FORCE_INLINE_ const bool is_initialised() const {
        return _inititalised;
    }

    _FORCE_INLINE_ bool is_local() const {
        return _local;
    }

protected:

    T* _vecptr;
    uint8_t _dlen;
    bool _local;
    bool _inititalised;

    void purge() {

        if (_local) {
            delete _vecptr;
        }
        
        _vecptr = 0;
        _local = false;
        _inititalised = false;

    }

};

#endif // VECTORPTR_H

#ifndef _DYNAMIC_HASHTABLE_H
#define _DYNAMIC_HASHTABLE_H

#include <memory>
#include <iostream>
#include <cassert>
#include <math.h>

#include "./course_files/library2.h"
#include "./DynamicArray.h"

/*
To ADD:
    -Exists(Key key) method, checks if player exists in hashtable
*/

namespace DHash{
    template <class Key, class Data>
    class DynamicHashtable{
        
        public:
        void Add(const Key& key, const Data& data){}
        bool Exists(const Key& key){}
    };
}

#endif
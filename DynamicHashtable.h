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

namespace DH{

    template <class Data>
    class DynamicHashtable{
        

        DA:DynamicArray table;

        int& Rehash(int current_size);

        public:

        DynamicHashtable(): table(){};
        void Add(const int& key, const Data& data){}
        bool Exists(const int& key){}

        typedef DynamicHashtable<Data> DHash;
    };

    
    
    




}

#endif
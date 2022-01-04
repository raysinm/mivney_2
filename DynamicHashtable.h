#ifndef _DYNAMIC_HASHTABLE_H
#define _DYNAMIC_HASHTABLE_H

#include <math.h>

#include <cassert>
#include <iostream>
#include <memory>

#include "./DynamicArray.h"

/*
To ADD:
    -Exists(Key key) method, checks if player exists in hashtable
*/

namespace DH {

template <class Data>
class DynamicHashtable {
    DA::DynamicArray<Data> table;

    int &Rehash(int current_size);

   public:
    DynamicHashtable() : table() {}
    void Add(const int &key, const Data &data) {}
    bool Exists(const int &key) {}

    typedef DynamicHashtable<Data> DHash;
};

}  // namespace DH

#endif
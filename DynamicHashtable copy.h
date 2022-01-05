#include <math.h>

#include <cassert>
#include <iostream>
#include <memory>

#include "./DynamicArray.h"

#ifndef _DYNAMIC_HASHTABLE_H
#define _DYNAMIC_HASHTABLE_H

/*
To ADD:
    -Exists(Key key) method, checks if player exists in hashtable
*/

namespace DH {

template <class Data>
class DynamicHashtable {
    DA::DynamicArray<int, Data> table;

    //int& Rehash(int current_size);
    int& Hash(int& x) {
        return (x)mod(table.Cap());
        //return xmod( CURRENT caacity of array)
    }

   public:
    DynamicHashtable() : table() {}
    void Add(const int& key, const Data& data);
    bool Exists(const int& key);

    typedef DynamicHashtable<Data> DHash;
};

template <class Data>
bool DynamicHashtable<Data>::Exists(const int& key) {
    return (this->Find(Hash(key),key) != nullptr);
}

template <class Data>
Data& DynamicHashtable<Data>::Find(const int& key) {
    return table.Find(Hash(key), key);
}

template <class Data>
void DynamicHashtable<Data>::Add(const int& key, const Data& data) {
    if(table.Add(Hash(key), key, data)){
        size_t new_capcity = (table.Cap())*2;

        DynamicArray<int,Data> new_table(new_capcity);
        for(int i = 0, i < table.Cap(); i++){
            //TODO: move all player elsewheer
        }
    }

}

template <class Data>
void DynamicHashtable<Data>::Remove(const int& key) {
    table.Remove(Hash(key), key);

}

}  // namespace DH

#endif
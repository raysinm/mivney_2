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
    DA::DynamicArray<Data> table;

    int &Rehash(int current_size);
    int& Hash(int x){
        //TODO:
        //return xmod( CURRENT caacity of array)
    }

    public:
    DynamicHashtable() : table() {}
    void Add(const int &key, const Data &data);
    bool Exists(const int &key);

    typedef DynamicHashtable<Data> DHash;
};

template <class Data>
bool DynamicHashtable<Data>::Find(const int& key){
    int index = Hash(key);
    while(table[index].isDeleted()){
        index = Hash(index);    //How to do double hashing?????????????????????????
    }
    return !(table[index] == nullptr);

}

template <class Data>
void DynamicHashtable<Data>::Add(const int& key, const Data&data){
    
}

}  // namespace DH

#endif
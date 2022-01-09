#include <math.h>

#include <iostream>
#include <memory>
#include <cstring>

#ifndef _DYNAMIC_HASHTABLE_H
#define _DYNAMIC_HASHTABLE_H

namespace DH {

typedef enum{
    SUCCESS,
    NOT_FOUND,
    ALREADY_EXIST
}DH_RESULT;

template <class Data>
class DynamicHashtable {

    class NotFound : public std::exception{};

    class Cell {
        friend DynamicHashtable;
        
        int key;
        Data data;
        Cell* next;

       public:
        Cell(const int& key, const Data& data) : key(key), data(data), next(nullptr){};

    };

    Cell** table;
    int used_size;
    int capacity;  //we want to keep: used_size = O(capacity)

    //PRIVATE METHODS
    void Rehash() {
        try{
        int old_cap = capacity;
        capacity *= 2;

        Cell** old_table = new Cell*[old_cap];
        memcpy(old_table, this->table, sizeof(old_table));

        delete[] table;
        table = new Cell* [capacity] { nullptr };

        for (int i = 0; i < old_cap; i++) {
            Cell* old_cell = old_table[i];

            while (old_cell) {
                this->Insert(old_cell->key, old_cell->data);
                old_cell = old_cell->next;
            }
        }

        delete[] old_table;
        }
        catch(std::bad_alloc &e){
            throw e;
        }
    }
    
    int Hash(int key) {
        int hash_key = key % capacity;
        return hash_key;
    }

   public:
    DynamicHashtable() : used_size(0), capacity(10) {
        try{
            table = new Cell* [10] { nullptr };
        }
        catch(std::bad_alloc &e){
            throw e;
        }
    }
    explicit DynamicHashtable(int cap) : used_size(0), capacity(cap) {
        try{
            table = new Cell* [cap] { nullptr };
        }
        catch(std::bad_alloc &e){
            throw e;
        }
    }

    DynamicHashtable(const DynamicHashtable& other) = delete;

    ~DynamicHashtable() {
        delete[] table;
    }



    DH_RESULT Insert(int key, Data data) {
        if(Exists(key)){
            return ALREADY_EXIST;
        }
        int index = Hash(key);

        Cell* new_cell = new Cell(key, data);

        if (table[index] == nullptr) {  //! remember to return to nullptr when deleting all players in chain
            table[index] = new_cell;
        } else {
            Cell* cell = table[index];
            new_cell->next = cell;
            table[index] = new_cell;
        }

        used_size++;
        if (used_size == capacity) {
            Rehash();  //TODO
        }
        return SUCCESS;
    }

    Data* Find(int key) {              //*return this after big bug test to Data&
        int index = Hash(key);

        Cell* cell = table[index];

        while (cell) {
            if (cell->key == key) {
                return &(cell->data);
            }
            cell = cell->next;
        }
        return nullptr;

        //throw NotFound(); //*return after big bug test
    }

    DH_RESULT Remove(int key) {
        if(!Exists(key)){
            return NOT_FOUND;
        }

        int index = Hash(key);
        //table[index] should exist
        Cell* cell = table[index];
        Cell* prev_cell = table[index];
        int chain_length = 0;
        while (cell) {
            ++chain_length;

            if (cell->key == key) {
                prev_cell->next = cell->next;
                delete cell;
                //not breaking to count number of cells in chain
            }

            prev_cell = cell;
            cell = cell->next;
        }

        if (chain_length <= 1) {
            table[index] = nullptr;
        }
        used_size--;

        return SUCCESS;
    }

    bool Exists(int key) {
        if(Find(key) == nullptr){
            return false;
        }
        return true;
/*         try{         //*return this after big bug test
            Find(key);
            return true;
        }
        catch(NotFound& e){
            return false;
        } */
    }

    void Print(){
        for(int i = 0; i < capacity; i++){
            Cell* cell = table[i];
            while (cell){

                std::cout << cell->data;

                cell = cell->next;
            }
        }
    }
};

}  // namespace DH

#endif
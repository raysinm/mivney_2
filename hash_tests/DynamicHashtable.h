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
    void Rehash(int new_capacity) {
        try{
        int old_cap = capacity;
        

        Cell** old_table = new Cell*[old_cap];
        //! doesnt work: memcpy(old_table, this->table, sizeof(this->table));

        for(int i = 0; i< old_cap; i++){
            old_table[i] = this->table[i];
        }

        deleteTable();

        capacity = new_capacity;
        used_size = 0;
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

    void deleteTable(){
        for(int i = 0; i<capacity; i++){
            Cell* cell = table[i];
            Cell* temp_cell = table[i];
            
            
            while(cell){
                temp_cell = cell->next;
                delete cell;
                cell =  temp_cell;
            }
        }
        delete table;
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
        deleteTable();
    }



    DH_RESULT Insert(int key, Data data) {
        if(Exists(key)){
            return ALREADY_EXIST;
        }
        int index = Hash(key);

        Cell* new_cell = new Cell(key, data);
        //Print();
        if (table[index] == nullptr) {  //! remember to return to nullptr when deleting all players in chain
            table[index] = new_cell;
        } else {
            Cell* cell = table[index];
            new_cell->next = cell;
            table[index] = new_cell;
        }
        //Print();
        used_size++;
        if (used_size == capacity) {
            Rehash(capacity*2);  
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
        //int chain_length = 0;
        /* while (cell) {
            ++chain_length;
            cell = cell->next;
        } */
        
        cell = table[index];

        while(cell){

            if (cell->key == key) {
                if(cell == table[index]){
                    table[index] = cell->next;
                }
                else{
                    prev_cell->next = cell->next;
                }
                delete cell;
                break;
            }

            

            prev_cell = cell;
            cell = cell->next;
        }

        used_size--;

        if(capacity > 10 && used_size == (capacity/4)){
            Rehash(capacity/2);
        }
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
        std::cout << std::endl;
        std::cout << "Used size: " << used_size << std::endl;
        std::cout << "Capacity: " << capacity << std::endl;
        
        
        for(int i = 0; i < capacity; i++){
            Cell* cell = table[i];
            int count = 0;
            
            std::cout << "row " << i << ": ";

            if(cell == nullptr){
                std::cout << " is empty";
            }
            while (cell){
                count++;
                std::cout << " cell " << count << ": " << cell->data << "  -->  "; 

                cell = cell->next;
            }
            std::cout << std::endl;
        }
    }
};

}  // namespace DH

#endif
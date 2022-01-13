#include <math.h>

#include <cstring>
#include <iostream>
#include <memory>

#ifndef _DYNAMIC_HASHTABLE_H
#define _DYNAMIC_HASHTABLE_H

namespace DH {

typedef enum {
    SUCCESS,
    NOT_FOUND,
    ALREADY_EXIST
} DH_RESULT;

template <class Data>
class DynamicHashtable {

    class NotFound : public std::exception {};
    class AlreadyExists : public std::exception {};

    class Cell {
        friend DynamicHashtable;

        int key;
        Data data;
        Cell* next;

       public:
        Cell(const int& key, const Data& data) : key(key), data(data), next(nullptr){};
    };

    class Head {
        Cell* first_cell;
        friend class DynamicHashtable;
       public:
        Head() : first_cell(nullptr){};
        Head(const Head& other):first_cell(other.first_cell){};
        Head& operator=(const Head& other)
        {   

            this->first_cell = nullptr;
            if(other.first_cell == nullptr){
                return *this;
            }

            first_cell = new Cell(other.first_cell->key, other.first_cell->data);
            Cell* other_cell = other.first_cell;
            Cell* this_cell = first_cell;

            while(other_cell->next){
                Cell* new_cell = new Cell(other_cell->next->key, other_cell->next->data);
                this_cell->next = new_cell;
                this_cell = this_cell ->next;

                other_cell = other_cell->next;
            }

            /* Cell* new_cell = new Cell(other.first_cell->key, other.first_cell->data);
            first_cell = new_cell;
            Cell* this_next_cell = first_cell->next;
            while(other_cell->next){
                Cell* new_next_cell = new Cell(other)
                this_next_cell = other_cell->next;
                if(this_next_cell){
                    this_next_cell = this_next_cell->next;
                }
                other_cell = other_cell->next;
            } */
            return *this;
        }
        ~Head() {
            Cell* cell = first_cell;
            Cell* temp = first_cell;
            while (cell) {
                temp = cell->next;
                delete cell;
                cell = temp;
            }
        }
        void Add(int key, Data data) {
            Cell* temp = first_cell;
            Cell* new_cell = new Cell(key, data);

            first_cell = new_cell;
            new_cell->next = temp;
        }

        void Remove(int key) {
            Cell* cell = first_cell;
            Cell* prev_cell = first_cell;
            while (cell) {
                if (cell->key == key) {
                    if (cell == first_cell) {
                        first_cell = cell->next;
                    } else {
                        prev_cell->next = cell->next;
                    }
                    delete cell;
                    break;
                }
                prev_cell = cell;
                cell = cell->next;
            }
        }

        Data& Find(int key){
            Cell* cell = first_cell;

            while (cell) {
                if (cell->key == key) {
                    return cell->data;
                    break;
                }
            cell = cell->next;
            }
            throw NotFound();
        }

        bool isEmpty(){
            return (first_cell == nullptr);
        }

        void Print(){
            int count = 0;
            Cell* cell = first_cell;
            while (cell) {
                count++;
                std::cout << " cell " << count << ": " << cell->data << "  -->  ";

                cell = cell->next;
            }
        }
    };
    
    Head* table;
    int used_size;
    int capacity;  //we want to keep: used_size = O(capacity)

    //PRIVATE METHODS
    void Rehash(int new_capacity) {
        try {
            int old_cap = capacity;

            Head* old_table = new Head[old_cap];
            //! doesnt work: memcpy(old_table, this->table, sizeof(this->table));

            for (int i = 0; i < old_cap; i++) {
                old_table[i] = table[i];
            }

            //deleteTable();
            delete[] table;
            capacity = new_capacity;
            used_size = 0;
            table = new Head[capacity];

            for (int i = 0; i < old_cap; i++) {
                Head& old_head = old_table[i];

                Cell* cell = old_head.first_cell;

                while (cell) {
                    this->Insert(cell->key, cell->data);
                    cell = cell->next;
                }
            }

            

            delete[] old_table;

        } catch (std::bad_alloc& e) {
            throw e;
        }
    }

    int Hash(int key) {
        int hash_key = key % capacity;
        return hash_key;
    }

    /* void deleteTable() {
        for (int i = 0; i < capacity; i++) {
            Cell* cell = table[i];
            Cell* temp_cell = table[i];

            while (cell) {
                temp_cell = cell->next;
                delete cell;
                cell = temp_cell;
            }
        }
        delete table;
    } */

   public:
    DynamicHashtable() : used_size(0), capacity(10) {
        try {
            table = new Head [10];
        } catch (std::bad_alloc& e) {
            throw e;
        }
    }
    explicit DynamicHashtable(int cap) : used_size(0), capacity(cap) {
        try {
            table = new Head[cap];
        } catch (std::bad_alloc& e) {
            throw e;
        }
    }

    DynamicHashtable(const DynamicHashtable& other) = delete;

    ~DynamicHashtable() {
        //deleteTable();
        delete[] table;
    }

    void Insert(int key, Data data) {
        if (Exists(key)) {
            return;
        }
        int index = Hash(key);

        Head& head = table[index];

        head.Add(key, data);

        //Print();
        /* if (table[index].isEmpty()) {  //! remember to return to nullptr when deleting all players in chain
            table[index] = new_cell;
        } else {
            Cell* cell = table[index];
            new_cell->next = cell;
            table[index] = new_cell;
        } */
        //Print();
        used_size++;
        if (used_size == capacity) {
            Rehash(capacity * 2);
        }
        //return SUCCESS;
    }

    Data& Find(int key) {  //*return this after big bug test to Data&
        int index = Hash(key);

        /* Cell* cell = table[index];

        while (cell) {
            if (cell->key == key) {
                return &(cell->data);
            }
            cell = cell->next;
        } */
        Head& head = table[index];
        try{
            return head.Find(key);
        }catch(...){ throw;}
        //throw NotFound(); //*return after big bug test
    }

    void Remove(int key) {
        if (!Exists(key)) {
            return;
        }

        int index = Hash(key);
        //table[index] should exist
       /*  Cell* cell = table[index];
        Cell* prev_cell = table[index];
        //int chain_length = 0;
        /* while (cell) {
            ++chain_length;
            cell = cell->next;
        } */
        /*
        cell = table[index];

        while (cell) {
            if (cell->key == key) {
                if (cell == table[index]) {
                    table[index] = cell->next;
                } else {
                    prev_cell->next = cell->next;
                }
                delete cell;
                break;
            }

            prev_cell = cell;
            cell = cell->next;
        } */

        Head& head = table[index];
        head.Remove(key);

        used_size--;

        if (capacity > 10 && used_size == (capacity / 4)) {
            Rehash(capacity / 2);
        }
    }

    bool Exists(int key) {
        /* if (Find(key) == nullptr) {
            return false;
        }
        return true; */
        try{         //*return this after big bug test
            Find(key);
            return true;
        }
        catch(NotFound& e){
            return false;
        }
    }

    void Print() {
        std::cout << std::endl;
        std::cout << "Used size: " << used_size << std::endl;
        std::cout << "Capacity: " << capacity << std::endl;

        for (int i = 0; i < capacity; i++) {
            Head& head = table[i];
            //int count = 0;

            std::cout << "row " << i << ": ";
            
            if (head.isEmpty()) {
                std::cout << " is empty";
            }
            else{
                head.Print();
            }
            

            std::cout << std::endl;
        }
    }
};

}  // namespace DH

#endif
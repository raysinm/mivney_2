#include <math.h>

#include <cassert>
#include <iostream>
#include <memory>

#ifndef _DYNAMIC_HASHTABLE_H
#define _DYNAMIC_HASHTABLE_H

namespace DH {

template <class Data>
class DynamicHashtable {
    class Cell {
        int key;
        Data data;
        Cell* next;

       public:
        Cell(const int& key, const Data& data) : key(key), data(data), next(nullptr){};

    }

    Cell** table;
    int used_size;
    int capacity;  //we want to keep: used_size = O(capacity)

   public:
    DynamicHashtable() : used_size(0), capacity(10) {
        table = new Cell* [10] { nullptr };
    }
    explicit DynamicHashtable(int cap) : used_size(0), capacity(cap) {
        table = new Cell* [cap] { nullptr };
    }

    DynamicHashtable& DynamicHashtable(const DynamicHashtable& other) = delete;

    ~DynamicHashtable() {
        delete[] table;
    }

    int& Hash(int key) {
        return (key)mod(capacity);
    }

    void Rehash() {
        int old_cap = capacity;
        capacity *= 2;

        Cell** old_table;
        memcpy(old_table, this->table);

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

    void Insert(int key, Data data) {
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
    }

    Data& Find(int key) {
        int index = Hash(key);

        Cell* cell = table[index];

        while (cell) {
            if (cell->key == key) {
                return cell->data;
            }
            cell = cell->next;
        }

        return nullptr;
    }

    void Remove(int key) {
        if (!Find(key)) {
            return;
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
    }

    bool Exists(int key) {
        return (Find(key != nullptr));
    }
}

}  // namespace DH

#endif
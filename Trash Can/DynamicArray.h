#ifndef _DYNAMIC_ARRAY_H
#define _DYNAMIC_ARRAY_H

namespace DA {

template <class Data>
class DynamicArray {

    class Cell {
        bool deleted;
        Data data;

       public:
        Cell() : deleted(false), data(nullptr){};

        Cell& opertaor = (const Data& other_data) {
            this->data(other_data);
            this->deleted = false;
        }

        bool operator!() {
            if (this->data == nullptr) {
                return true;
            }
            return false;
        }

        bool isDeleted(){
            return (this->deleted);
        }
        //destructor will call ~Data()
    };

    Cell* array;
    size_t size;
    size_t capacity;

    void Realloc(size_t new_capacity);

   public:
    DynamicArray() : size(0), capacity(7), array(nullptr) {
        array = new Cell[capacity];
    }
    const DynamicArray<Data>& DynamicArray(const DynamicArray& other) : DynamicArray() {  //! check validity
        if (this == &other) {
            return *this;
        }
        this->Realloc(other.capacity);
        for (size_t i = 0; i < other.capacity; i++) {
            this.array[i] = other.array[i];
        }
        return *this;
    }

    void Add(const Data& data);  //TODO: has to change delted to false, if changed size of array I have ti notify user (to do rehash)

    Cell& Operator[](size_t index) {
        if (index >= capacity || !array[index]) {
            return nullptr;
        }
        return array[index];
    }

    size_t cap(){
        return this->capacity;
    }
};

template <class Data>
void DynamicArray<Data>::Realloc(size_t new_capacity) {
    Cell* new_array = new Cell[new_capacity];  //* allocate new array

    for (size_t i = 0; i < this->capacity; i++) {  //* copy all previous elements
        new_array[i] = this->array[i];
    }

    //* delete old array
    delete[] this->array;
    this->array = new_array;
}

template <class Data>
void DynamicArray<Data>::Add(int index, const Data& data) {
    if (index >= capacity) {
        return;
    }
    if(!array[index]){
        array[index] = data; //*should work with operator= of class Cell
        array[index]->deleted = false;

        size++;
        if(size == capacity){
            size_t new_capacity = capacity*2 -1;
            Realloc(new_capacity);
            capacity = new_capacity;
        }
    }
}


// 1. how to choose initial size (7?) and do I double it each time, how to do the r(x)
// 2. do I have to shrink the dynamic array too?
template<class Data>
void DynamicArray<Data>::Remove(int index){
    if(index >= capacity){
        return;
    }
    array[index] = nullptr;
    array[index]->deleted = true;
    size--;

    //! Realloc with shrinking?????
}

template<class Data>
bool DynamicArray<Data>:: Find(size_t index)

}  // namespace DA

#endif

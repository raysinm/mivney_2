#ifndef _DYNAMIC_ARRAY_H
#define _DYNAMIC_ARRAY_H

namespace DA{

    template<class Data>
    class DynamicArray{

        class Cell{
            bool deleted;
            Data data;

            public:
            Cell():deleted(false), data(nullptr){}

            void Insert(const Data& in_data){
                if(data != nullptr){
                    data(in_data);  //will call copy constructor
                }
            }
            //destructor will call ~Data()
        };

        Cell* array;
        size_t size;
        size_t capacity;

        void Realloc(size_t new_capacity);
        public:
        DynamicArray():
        void Add(const Data& data);

    };

    template<class Data>
    void Realloc(size_t new_capacity){
        
        Cell* new_array = new Cell[new_capacity];   //* allocate new array

        for(size_t i = 0; i < this->capacity; i++){   //* copy all previous elements
            new_array[i] = this->array[i]
        }

        //* delete old array
        delete[] this->array;
        this->array = new_array;

    }


}

#endif

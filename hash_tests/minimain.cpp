

#include "./DynamicHashtable.h"

typedef DH::DynamicHashtable<int> Hash;

int main(){

    Hash hash1;
    Hash hash2;

    hash1.Insert(2, 2);
    hash1.Insert(4, 4);

    hash1.Remove(2);

    
    return 0;
}
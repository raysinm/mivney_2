

#include "./UnionFind.h"

typedef UF::UnionFind<int> Uf;

int main(){


    class Data{
        int k;
        public:
        Data(int k): k(k){};
    }

    Data data11(11);
    Data data12(12);
    
    Uf uf1(4);
    Uf uf2(3);

    int& find = uf1.Find(2);
    find = 5;
    if(uf1.Find(2) != 5){
        //test failed
    }
    << cout <<

    
    return 0;
}
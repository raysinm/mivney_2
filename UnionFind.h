#ifndef _UNION_FIND_H
#define _UNION_FIND_H

#include <memory>
#include <iostream>
#include <cassert>
#include <math.h>

/*
TO DO:
    -define the class generically with class T (T is int in PlayersManager) and class Data (GroupData in PlayersaManager)
    -constructor should create an array with size k

    -Union function, Find function
    -Find should return GroupData by reference
*/

namespace UF{

    template<class Data>
    class UnionFind{

        class GroupNode{
            friend class UnionFind;
            Data data; //*data holds AVL
            int size;
            public:
               GroupNode(Data data): data(data), size(1){}; //! data(data) could be dangerous- might copy the entire tree in O(n)

        };

        GroupNode* groups;

        public:
            explicit UnionFind(int k): size(k){
                   groups = new int[k+1];
                   for(int i=1; i<=k; i++){
                       Data data_k; //create empty AVL 
                       group_k = new GroupNode(data_k);
                       groups[i] = group_k;
                   }
               };

            void Union(int group1, int group2);

            //TODO: find function that recieved group id and return a reference to the data of the associated GroupNode. needed for messing with the tree inside
    };

    template<class Data>
    void Union(int group1, int group2){
        GroupNode g1 = groups[group1];
        GroupNode g2 = groups[group2];

        if(group1.group_size > group2.group_size)
        {
            //merge datas
            group1.size += group2.size;
            delete group2;
            //in case more than one place in array points to group, how to change all to point at new group

        }
    }
}

#endif
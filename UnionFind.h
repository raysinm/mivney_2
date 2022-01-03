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
            //needs to hold groupNum?
            GroupNode* father; //points to father if exists
            public:
               GroupNode(): father(nullptr){};

        };

        class UnionGroup{
            friend class UnionFind;
            Data data; //data holds GroupData with AVL
            int size; //holds num of groups
            public:
               UnionGroup(Data data): data(data), size(1){};
        };

        GroupNode* group_nodes;
        UnionGroup* groups;
        int num_of_groups;

        public:
               UnionFind(int k): num_of_groups(k){
                   group_nodes = new GroupNode[k+1]
                   groups = new UnionGroup[k+1];
                   for(int i=1; i<=k; i++){
                       groups[i].data(new Data); // ! legit?
                       group_node_i = new GroupNode();
                       group_nodes[i] = group_node_i;
                   }
               };

            void Union(int group1, int group2);
            UnionGroup Find(int group);
    };

    template<class Data>
    void Union(int group1, int group2){
        GroupNode g1 = group_nodes[group1];
        GroupNode g2 = group_nodes[group2];
       // UnionGroup g1 = groups[group1];
        //UnionGroup g2 = groups[group2];

        if(group1.group_size > group2.group_size)
        {
            //merge datas
            group1.size += group2.size;
            delete group2;
            //in case more than one place in array points to group, how to change all to point at new group

        }
    }

    template<class Data>
    UnionGroup Find(int group){
        GroupNode* node = group_nodes[group];
        while (node->father != nullptr)
        {
            node = node->father
        }
        

    }
}

#endif
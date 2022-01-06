#ifndef _UNION_FIND_H
#define _UNION_FIND_H

#include <math.h>

#include <cassert>
#include <iostream>
#include <memory>

/*
TO DO:
    -define the class generically with class T (T is int in PlayersManager) and class Data (GroupData in PlayersaManager)
    -constructor should create an array with size k

    -Union function, Find function
    -Find should return GroupData by reference
*/

namespace UF {

template <class Data>
class UnionFind {
    class GroupNode {
        friend class UnionFind;
        int group_num;
        //needs to hold groupNum?
        GroupNode* father;  //points to father if exists
       public:
        GroupNode(int group_num) : group_num(group_num), father(nullptr){};
    };

    class UnionGroup {
        friend class UnionFind;
        Data data;  //data holds GroupData with AVL
        int size;   //holds num of groups
       public:
        UnionGroup(Data data) : data(data), size(1){};
    };

    GroupNode* group_nodes;
    UnionGroup* groups;
    int num_of_groups;

   public:
    UnionFind(int k) : num_of_groups(k) {
        group_nodes = new GroupNode[k + 1] groups = new UnionGroup[k + 1];

        for (int i = 1; i <= k; i++) {
            groups[i].data(Data());  // ! legit?
            GroupNode group_node_i = new GroupNode(i);
            group_nodes[i] = group_node_i;
        }
    };

    void Union(int group1, int group2);
    void mergeIntoGroup(UnionGroup* g1, UnionGroup* g2);
    Data& Find(int group);
};

template <class Data>
void UnionFind<Data>::Union(int group1, int group2) {
    GroupNode* g1_node = group_nodes[group1];
    GroupNode* g2_node = group_nodes[group2];

    while (g1_node->father) {
        g1_node = g1_node->father;
    }
    while (g2_node->father) {
        g2_node = g1_node->father;
    }

    if (g1_node.group_num == g2_node.group_num) {
        return SUCCESS;  //TODO: check that case realy is success
    }

    UnionGroup* g1 = groups[g1_node.group_num];
    UnionGroup* g2 = groups[g2_node.group_num];

    if (g1.size > g2.size) {
        mergeIntoGroup(g1, g2);
    } else {
        mergeIntoGroup(g2, g1);
    }

    /* if(group1.group_size > group2.group_size)
        {
            //merge datas
            group1.size += group2.size;
            delete group2;
            //in case more than one place in array points to group, how to change all to point at new group

        } */
}

template <class Data>
void UnionFind<Data>::mergeIntoGroup(UnionGroup* g1, UnionGroup* g2) {
    g1.size += g2.size;
    //TODO: merge Data func (AVL + level 0)
}

template <class Data>
Data& UnionFind<Data>::Find(int group) {  //* notice that Find returns reference to group data
    GroupNode* node = group_nodes[group];
    while (node->father != nullptr) {
        node = node->father;
    }
    //UnionGroup*
}
}  // namespace UF

#endif
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
        GroupNode* father;  //points to father if exists
       public:
        GroupNode(): group_num(0), father(nullptr){};
        explicit GroupNode(int group_num) : group_num(group_num), father(nullptr){};
    };

    class UnionGroup {
        friend class UnionFind;
        Data data;  //data holds GroupData with AVL
        int size;   //holds num of groups
       public:
        UnionGroup(): data(0),size(1){};
        UnionGroup(const UnionGroup& other): data(other.data), size(other.size){};
        /* UnionGroup& operator =(const UnionGroup& other){
            size = other.size;
            data = (other.data);
        } */

        explicit UnionGroup(int scale): data(scale), size(1){};
        //explicit UnionGroup(Data data) : data(data), size(1){};
    };

    GroupNode* group_nodes;
    UnionGroup* groups;
    int num_of_groups;
    void mergeIntoGroup(UnionGroup* g1, UnionGroup* g2);
    

   public:
    UnionFind(int k, int scale) : num_of_groups(k) {
        group_nodes = new GroupNode[k + 1];
        groups = new UnionGroup[k + 1];

        for (int i = 1; i <= k; i++) {
            group_nodes[i].group_num = i;
            UnionGroup new_group(scale);
            groups[i].data =new_group.data; 
            
           /*  GroupNode group_node_i = new GroupNode(i);
            group_nodes[i] = group_node_i; */
        }
    };

    ~UnionFind(){
        delete[] group_nodes;
        delete[] groups;
    };

    void Union(int group1, int group2);
    Data& Find(int group);
};

template <class Data>
void UnionFind<Data>::Union(int group1, int group2) {
    GroupNode* g1_node = &group_nodes[group1];
    GroupNode* g2_node = &group_nodes[group2];

    while (g1_node->father) {
        g1_node = g1_node->father;
    }
    while (g2_node->father) {
        g2_node = g2_node->father;
    }

    if (g1_node->group_num == g2_node->group_num) {
        return;
    }

    UnionGroup* g1 = &groups[g1_node->group_num];
    UnionGroup* g2 = &groups[g2_node->group_num];

    if (g1->size > g2->size) {
        mergeIntoGroup(g1, g2);
        g2_node->father = g1_node;
    } else if( g1->size <= g2->size){
        mergeIntoGroup(g2, g1);
        g1_node->father = g2_node;
    }
}

template <class Data>
void UnionFind<Data>::mergeIntoGroup(UnionGroup* g1, UnionGroup* g2) {
    g1->size += g2->size;
    g1->data += g2->data;
    g2->size = 0;
    // call g2.data destructor
}

template <class Data>
Data& UnionFind<Data>::Find(int group) {  //* notice that Find returns reference to group data
    GroupNode* node = &group_nodes[group];
    GroupNode* next_node;
    GroupNode* root = &group_nodes[group];
    while (root->father != nullptr) {
        root = root->father;
    }
    
    while (node->father && node->father != root) {
        next_node = node->father;
        node->father = root;
        node = next_node;
    }
    UnionGroup* group_cell = &groups[root->group_num];
    return group_cell->data;
}
}  // namespace UF

#endif
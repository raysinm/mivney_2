#ifndef _AVL_RANK_TREE_H
#define _AVL_RANK_TREE_H

//stav shalom shalom
#include <math.h>

#include <cassert>
#include <iostream>
#include <memory>

//#include "./course_files/library2.h"

//TODO: add a seperate node to hold level 0 player scores. or should it be in PlayersManager?

namespace AVLRank {

template <class KeyElem, class Data, class Rank>
class AVLTree {
   private:
    class TNode {
        friend class AVLTree;
        KeyElem key;
        Data data;  //* will be num of players in level
        Rank rank;  // TODO: add GetRank(Key key) function. it returns a reference to the rank in the tree (needed for changing array)
        int BF;     //balance factor
        int height;

        TNode *father;
        TNode *left_son;   //smaller
        TNode *right_son;  //bigger

       public:
        // ! should rank be const? ---no, has to be updated constantly
        TNode(const KeyElem &key, const Data &data, const Rank &rank) : key(key), data(data), rank(rank) BF(0), height(0), father(nullptr), left_son(nullptr), right_son(nullptr){};

        TNode(const TNode &) = default;
        ~TNode() = default;

        bool operator<(const TNode &other_node) {
            if (other_node == nullptr || *this == other_node)
                return false;
            return (this->key < other_node.key);
        };
        bool operator==(const TNode &other_node) {
            return this->key == other_node.key;
        }
        bool isRightSon() {
            if (!this->father)
                return false;
            return (this->father->key < this->key);
        }
        bool isLeftSon() {
            if (!this->father)
                return false;
            return (this->key < this->father->key);
        }
        bool rightSonExists() {
            return this->right_son;
        }
        bool leftSonExists() {
            return this->left_son;
        }
        TNode *nextInOrder() {
            TNode *next_node = this;
            if (next_node->right_son) {
                next_node = next_node->right_son;
                while (next_node->left_son) {
                    next_node = next_node->left_son;
                }
                return next_node;
            }
            while (1) {
                if (next_node->father == nullptr) {
                    next_node = nullptr;
                    return next_node;
                }
                if (next_node->isLeftSon()) {
                    next_node = next_node->father;
                    return next_node;
                }
                next_node = next_node->father;
            }
        }
    };

    TNode *root;
    int tree_size;
    Key min_key;

    void AVLBalance(TNode *);
    void AVLRotate_LL(TNode *);
    void AVLRotate_RR(TNode *);
    void AVLRotate_LR(TNode *);
    void AVLRotate_RL(TNode *);

    TNode *AVLFind(const KeyElem &key) const;
    TNode *AVLFind_rec(TNode *current_node, const KeyElem &key_to_find, TNode **father_to_ret) const;
    void AVLNodeRefreshHeight(TNode *node) const;
    void AVLNodeRefreshBF(TNode *node) const;
    void AVLRemove_rec(TNode *node, const KeyElem &key);
    TNode *findReplacingNode(TNode *node) const;

    TNode *ArrayToAVLTree(TNode **array, int start, int end, TNode *father);
    void InOrderOutputTNodes_rec(TNode *node, TNode **arr, int &arr_index, const int arr_size);
    void MergeArray(AVLTree<KeyElem, Data, Rank>::TNode **arr1, const int arr1_size,
                    AVLTree<KeyElem, Data, Rank>::TNode **arr2, const int arr2_size,
                    AVLTree<KeyElem, Data, Rank>::TNode **merged_arr);
    TNode *AVLGetFirst() const;
    void printTree_rec(const std::string &prefix, const TNode *node, bool isLeft);
    void printTreeData_rec(const std::string &prefix, const TNode *node, bool isLeft);
    void AVLDestroy_rec(TNode *) const;

   public:
    AVLTree() : root(nullptr), tree_size(0){};
    AVLTree(Key min) : root(nullptr), tree_size(0), min_key(min){};

    AVLTree(const AVLTree &other) : AVLTree() {
        if (other.tree_size != 0)
            return;
        this->root = other.root;
        this->tree_size = other.tree_size;
    }
    ~AVLTree() {
        AVLDestroy_rec(root);
        root = nullptr;
    };

    class Iterator {
        TNode *current_node;

       public:
        Iterator(TNode *t) : current_node(t){};
        Data &operator*() {
            return current_node->data;
        }
        Iterator &operator++() {
            current_node = current_node->nextInOrder();
            return *this;
        }
        friend bool operator==(const Iterator &it1, const Iterator &it2) {
            return it1.current_node == it2.current_node;
        }
        friend bool operator!=(const Iterator &it1, const Iterator &it2) {
            return !(it1 == it2);
        }
    };
    class ConstIterator {
        TNode *current_node;

       public:
        ConstIterator(TNode *t) : current_node(t){};
        const Data &operator*() {
            return current_node->data;
        }
        Iterator &operator++() {
            current_node = current_node->nextInOrder();
            return *this;
        }
        friend bool operator==(const ConstIterator &it1, const ConstIterator &it2) {
            return it1.current_node == it2.current_node;
        }
        friend bool operator!=(const ConstIterator &it1, const ConstIterator &it2) {
            return !(it1 == it2);
        }
    };

    Iterator begin() {
        return Iterator(AVLGetFirst());
    }
    Iterator end() {
        return Iterator(nullptr);
    }

    ConstIterator begin() const {
        return ConstIterator(AVLGetFirst());
    }
    ConstIterator end() const {
        return ConstIterator(nullptr);
    }

    bool AVLExist(const KeyElem &) const;
    void AVLInsert(const KeyElem &, const Data &, const Rank &rank);
    void AVLRemove(const KeyElem &);
    void AVLMerge(AVLTree<KeyElem, Data, Rank> &other_tree);
    Data &AVLGet(const KeyElem &key) const;
    int size() const;
    KeyElem &AVLMin() const;

    void printTree();
    void printTreeData();
};

//___________***___PUBLIC FUNCTIONS IMPLEMENTATION___***___________//

template <class KeyElem, class Data, class Rank>
bool AVLTree<KeyElem, Data, Rank>::AVLExist(const KeyElem &key_to_find) const {
    return (AVLFind(key_to_find) != nullptr);
}

// ! should rank be const?
//TODO: update father rank and kids if needed
template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::AVLInsert(const KeyElem &key, const Data &data, const Rank &rank) {
    try {
        TNode *insert_node = new TNode(key, data, rank);

        if (!this->root) {
            this->root = insert_node;
            tree_size++;
            return;
        }

        TNode *insert_after_node = nullptr;
        AVLFind_rec(root, key, &insert_after_node);

        if (key < insert_after_node->key) {
            insert_after_node->left_son = insert_node;
        } else {
            insert_after_node->right_son = insert_node;
        }

        insert_node->father = insert_after_node;
        AVLBalance(insert_node->father);
        tree_size++;
    } catch (std::bad_alloc &e) {
        throw(e);
    }
}

// TODO: update father rank
template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::AVLRemove(const KeyElem &key) {
    AVLRemove_rec(this->root, key);
    tree_size--;
}

// TODO: we need a new merge because some levels should be the same and we do not want to re-add them
// * idea: add a merge method for TNodes in case their key is the same. should be called after the merged array is created.
// * (we have the O(n) needed for another run on the array)
template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::AVLMerge(AVLTree<KeyElem, Data, Rank> &other_tree) {
    //allocating two arrays for merging
    TNode **tree1_arr;
    TNode **other_tree_arr;
    TNode **merged_arr;
    try {
        tree1_arr = new TNode *[this->tree_size];
        other_tree_arr = new TNode *[other_tree.tree_size];
        merged_arr = new TNode *[this->tree_size + other_tree.tree_size];

        int i1 = 0, i2 = 0;

        InOrderOutputTNodes_rec(this->root, tree1_arr, i1, this->tree_size);
        InOrderOutputTNodes_rec(other_tree.root, other_tree_arr, i2, other_tree.tree_size);

        this->root = nullptr;
        other_tree.root = nullptr;
        MergeArray(tree1_arr, this->tree_size, other_tree_arr, other_tree.tree_size, merged_arr);

        this->root = this->ArrayToAVLTree(merged_arr, 0, (this->tree_size + other_tree.tree_size - 1), nullptr);
        this->tree_size += other_tree.tree_size;

        delete[] tree1_arr;
        delete[] other_tree_arr;
        delete[] merged_arr;
        tree1_arr = nullptr;
        other_tree_arr = nullptr;
        merged_arr = nullptr;
    } catch (std::bad_alloc &e) {
        delete[] tree1_arr;
        delete[] other_tree_arr;
        delete[] merged_arr;
        throw(e);
    }
}

template <class KeyElem, class Data, class Rank>
Data &AVLTree<KeyElem, Data, Rank>::AVLGet(const KeyElem &key) const {
    return AVLFind(key)->data;
}

template <class KeyElem, class Data, class Rank>
int AVLTree<KeyElem, Data, Rank>::size() const {
    return this->tree_size;
}

template <class KeyElem, class Data, class Rank>
KeyElem &AVLTree<KeyElem, Data, Rank>::AVLMin() const {
    auto *current = this->root;  //added *
    while (current->left_son) {
        current = current->left_son;
    }
    return current->key;
}

template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::printTree() {
    std::cout << "\n"
              << std::endl;
    this->printTree_rec("", this->root, false);
    std::cout << "\n"
              << std::endl;
}

template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::printTreeData() {
    std::cout << "\n"
              << std::endl;
    this->printTreeData_rec("", this->root, false);
    std::cout << "\n"
              << std::endl;
}

//___________***___PRIVATE FUNCTIONS IMPLEMENTATION___***___________//

// TODO: balance rank too with refresh rank
template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::AVLBalance(TNode *start) {
    auto current_node = start;
    while (current_node) {
        int prev_height = current_node->height;
        AVLNodeRefreshHeight(current_node);
        AVLNodeRefreshBF(current_node);

        if (current_node->BF == 2 && current_node->left_son->BF >= 0) {
            this->AVLRotate_LL(current_node);
        } else if (current_node->BF == 2 && current_node->left_son->BF == -1) {
            this->AVLRotate_LR(current_node);
        } else if (current_node->BF == -2 && current_node->right_son->BF == -1) {
            this->AVLRotate_RR(current_node);
        } else if (current_node->BF == -2 && current_node->right_son->BF >= 0) {
            this->AVLRotate_RL(current_node);
        }
        AVLNodeRefreshHeight(current_node->left_son);
        AVLNodeRefreshHeight(current_node->right_son);
        AVLNodeRefreshHeight(current_node);
        AVLNodeRefreshBF(current_node->left_son);
        AVLNodeRefreshBF(current_node->right_son);
        AVLNodeRefreshBF(current_node);
        if (current_node->right_son && abs(current_node->right_son->BF) > 1) {
            AVLBalance(current_node->right_son);
        }

        if (current_node->left_son && abs(current_node->left_son->BF) > 1) {
            AVLBalance(current_node->left_son);
        }
        if (current_node->height == prev_height) {
            break;
        }
        current_node = current_node->father;
    }
}

//******************_ROTATIONS_********************//

template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::AVLRotate_LL(TNode *node_uneven) {
    TNode *temp_left_right_son = node_uneven->left_son->right_son;
    TNode *left_son = node_uneven->left_son;

    left_son->father = node_uneven->father;
    left_son->right_son = node_uneven;
    if (temp_left_right_son)
        temp_left_right_son->father = node_uneven;
    node_uneven->left_son = temp_left_right_son;
    node_uneven->father = left_son;

    if (this->root == node_uneven) {
        this->root = left_son;
        return;
    }

    if (left_son->key < left_son->father->key) {
        left_son->father->left_son = left_son;
    } else {
        left_son->father->right_son = left_son;
    }
}

template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::AVLRotate_RR(TNode *node_uneven) {
    TNode *temp_right_left_son = (node_uneven->right_son->left_son);
    TNode *right_son = node_uneven->right_son;

    right_son->father = node_uneven->father;
    right_son->left_son = node_uneven;
    if (temp_right_left_son)
        temp_right_left_son->father = node_uneven;
    node_uneven->right_son = temp_right_left_son;
    node_uneven->father = right_son;

    if (this->root == node_uneven) {
        this->root = right_son;
        return;
    }

    if (right_son->father->key < right_son->key) {
        right_son->father->right_son = right_son;
    } else {
        right_son->father->left_son = right_son;
    }
}

template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::AVLRotate_LR(TNode *node_uneven) {
    AVLRotate_RR(node_uneven->left_son);
    AVLRotate_LL(node_uneven);
}

template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::AVLRotate_RL(TNode *node_uneven) {
    AVLRotate_LL(node_uneven->right_son);
    AVLRotate_RR(node_uneven);
}

template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::AVLNodeRefreshHeight(TNode *node) const {
    if (!node)
        return;

    if (!node->right_son && !node->left_son) {
        node->height = 0;
    } else if (!node->right_son) {
        node->height = node->left_son->height + 1;
    } else if (!node->left_son) {
        node->height = node->right_son->height + 1;
    } else {
        node->height = std::max(node->left_son->height, node->right_son->height) + 1;
    }
}

template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::AVLNodeRefreshBF(TNode *node) const {
    if (!node)
        return;
    if (!node->right_son && !node->left_son) {
        node->BF = 0;
    } else if (!node->right_son) {
        node->BF = node->left_son->height - (-1);
    } else if (!node->left_son) {
        node->BF = -1 - node->right_son->height;
    } else {
        node->BF = node->left_son->height - node->right_son->height;
    }
}

template <class KeyElem, class Data, class Rank>
typename AVLTree<KeyElem, Data, Rank>::TNode *AVLTree<KeyElem, Data, Rank>::AVLFind(const KeyElem &key_to_find) const {
    TNode *dummy_ptr;
    return AVLFind_rec(this->root, key_to_find, &dummy_ptr);
}

template <class KeyElem, class Data, class Rank>
typename AVLTree<KeyElem, Data, Rank>::TNode *AVLTree<KeyElem, Data, Rank>::AVLFind_rec(AVLTree<KeyElem, Data, Rank>::TNode *current_node,
                                                                                        const KeyElem &key_to_find, AVLTree<KeyElem, Data, Rank>::TNode **father_to_ret) const {
    if (!current_node) {
        return nullptr;
    }

    if (current_node->key == key_to_find) {
        return current_node;
    }

    if (key_to_find < current_node->key) {
        if (current_node->left_son == nullptr) {  //Knowledge for AVLInsert
            *father_to_ret = current_node;
        }
        return (AVLFind_rec(current_node->left_son, key_to_find, father_to_ret));
    } else {
        if (current_node->right_son == nullptr) {  //Knowledge for AVLInsert
            *father_to_ret = current_node;
        }
        return (AVLFind_rec(current_node->right_son, key_to_find, father_to_ret));
    }
}

template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::AVLRemove_rec(TNode *node, const KeyElem &key) {
    if (!node) {
        return;
    }
    if (node->key == key) {
        auto temp_father = node->father;
        if (!node->leftSonExists() && !node->rightSonExists()) {  //This is a leaf
            if (node == this->root) {
                this->root = nullptr;
            } else if (node->isLeftSon()) {
                node->father->left_son = nullptr;
            } else if (node->isRightSon()) {
                node->father->right_son = nullptr;
            }
            AVLBalance(temp_father);
            delete node;
            return;
        } else if (!node->leftSonExists() || !node->rightSonExists()) {  //Has only a right son or a left son

            TNode *node_son = node->leftSonExists() ? node->left_son : node->right_son;

            if (node == this->root) {
                this->root = node_son;
            } else if (node->isLeftSon()) {
                node->father->left_son = node_son;
            } else if (node->isRightSon()) {
                node->father->right_son = node_son;
            }
            node_son->father = node->father;
            AVLBalance(temp_father);
            delete node;
            return;
        } else if (node->leftSonExists() && node->rightSonExists()) {  //Has TWO sons
            auto replacer = findReplacingNode(node);                   //replacer is the biggest node that is smaller than our node

            if (node == this->root) {
                this->root = replacer;
            } else if (node->isLeftSon()) {
                node->father->left_son = replacer;
            } else if (node->isRightSon()) {
                node->father->right_son = replacer;
            }

            TNode *temp_node_left_son = node->left_son;
            TNode *temp_node_right_son = node->right_son;
            TNode *temp_replacer_left_son = replacer->left_son;

            if (node == replacer->father) {  // special case. They are direct relatives, and therfore point to eachother

                replacer->father = temp_father;
                node->father = replacer;
                replacer->left_son = node;
                temp_father = node->father;
            } else {
                auto temp_replacer_father = replacer->father;
                replacer->father = node->father;
                node->father = temp_replacer_father;
                replacer->left_son = temp_node_left_son;
            }

            node->left_son = temp_replacer_left_son;
            node->right_son = replacer->right_son;
            replacer->right_son = temp_node_right_son;
            replacer->right_son->father = replacer;
            replacer->left_son->father = replacer;
            replacer->BF = node->BF;
            replacer->height = node->height;
            AVLNodeRefreshHeight(node);
            AVLNodeRefreshBF(node);

            if (!node->leftSonExists() && !node->rightSonExists()) {  //This is a leaf

                if (node->father == replacer) {
                    node->father->left_son = nullptr;
                } else {
                    node->father->right_son = nullptr;
                }
                AVLBalance(node->father);
                delete node;
                return;
            }

            if (!node->leftSonExists() || !node->rightSonExists()) {  //Has only a right son or a left son

                TNode *node_son = node->leftSonExists() ? node->left_son : node->right_son;

                if (node->father == replacer) {
                    node->father->left_son = node_son;
                } else {
                    node->father->right_son = node_son;
                }
                node_son->father = node->father;
            }
            AVLBalance(node->father);
            delete node;
            return;
        }
    } else if (node->leftSonExists() && key < node->key) {
        AVLRemove_rec(node->left_son, key);
    } else if (node->rightSonExists() && node->key < key) {
        AVLRemove_rec(node->right_son, key);
    }
}

template <class KeyElem, class Data, class Rank>
typename AVLTree<KeyElem, Data, Rank>::TNode *AVLTree<KeyElem, Data, Rank>::findReplacingNode(AVLTree<KeyElem, Data, Rank>::TNode *node) const {
    auto replacer = node->left_son;
    while (replacer->right_son) {
        replacer = replacer->right_son;
    }
    return replacer;
}

template <class KeyElem, class Data, class Rank>
typename AVLTree<KeyElem, Data, Rank>::TNode *AVLTree<KeyElem, Data, Rank>::ArrayToAVLTree(AVLTree<KeyElem, Data, Rank>::TNode **array,
                                                                                           int start, int end, AVLTree<KeyElem, Data, Rank>::TNode *father) {
    if (end < start || start > end) {
        return nullptr;
    }
    int middle = (start + end) / 2;
    TNode *current_root = array[middle];
    current_root->father = father;
    current_root->left_son = ArrayToAVLTree(array, start, middle - 1, current_root);
    current_root->right_son = ArrayToAVLTree(array, middle + 1, end, current_root);
    AVLNodeRefreshHeight(current_root);
    AVLNodeRefreshBF(current_root);

    return current_root;
}

template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::InOrderOutputTNodes_rec(TNode *node, TNode **arr, int &arr_index, const int arr_size) {
    if (!node || arr_index == arr_size) {
        return;
    }
    InOrderOutputTNodes_rec(node->left_son, arr, arr_index, arr_size);

    arr[arr_index] = node;
    arr_index++;

    InOrderOutputTNodes_rec(node->right_son, arr, arr_index, arr_size);
    node = nullptr;
}

Normalize 

template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::MergeArray(typename AVLTree<KeyElem, Data, Rank>::TNode **arr1, const int arr1_size,
                                              typename AVLTree<KeyElem, Data, Rank>::TNode **arr2, const int arr2_size, typename AVLTree<KeyElem, Data, Rank>::TNode **merged_arr) {
    int index = 0;
    int index1 = 0;
    int index2 = 0;
    while ((index1 < arr1_size) && (index2 < arr2_size)) {
        if (arr1[index1]->key < arr2[index2]->key) {
            merged_arr[index] = arr1[index1];
            index1++;
        } else {
            merged_arr[index] = arr2[index2];
            index2++;
        }
        //std::cout << merged_arr[index]->key << std::endl;
        index++;

        //did not include arr1[index1] == arr2[index2] because not possible
    }
    if (index1 < arr1_size) {
        while (index1 < arr1_size) {
            merged_arr[index] = arr1[index1];
            index1++;
            index++;
        }
    }
    if (index2 < arr2_size) {
        while (index2 < arr2_size) {
            merged_arr[index] = arr2[index2];
            index2++;
            index++;
        }
    }
}

template <class KeyElem, class Data, class Rank>
typename AVLTree<KeyElem, Data, Rank>::TNode *AVLTree<KeyElem, Data, Rank>::AVLGetFirst() const {
    if (!this->root)
        return nullptr;
    auto *node = this->root;  //added *
    while (node->left_son) {
        node = node->left_son;
    }
    return node;
}

template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::printTree_rec(const std::string &prefix, const AVLTree<KeyElem, Data, Rank>::TNode *node, bool isLeft) {
    if (node != nullptr) {
        std::cout << prefix;

        std::cout << (isLeft ? "├──────" : "└──────");

        //         print the value of the node
        std::cout << "id: " << node->key << std::endl;

        //         enter the next tree level - left and right branch
        printTree_rec(prefix + (isLeft ? "│       " : "        "), node->right_son, true);
        printTree_rec(prefix + (isLeft ? "│       " : "        "), node->left_son, false);
    }
}

template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::printTreeData_rec(const std::string &prefix, const AVLTree<KeyElem, Data, Rank>::TNode *node, bool isLeft) {
    if (node != nullptr) {
        std::cout << prefix;

        std::cout << (isLeft ? "├──────" : "└──────");
        //         print the value of the node
        std::cout << "group id: " << node->data << ") " << std::endl;

        //         enter the next tree level - left and right branch
        printTreeData_rec(prefix + (isLeft ? "│       " : "        "), node->right_son, true);
        printTreeData_rec(prefix + (isLeft ? "│       " : "        "), node->left_son, false);
    }
}

template <class KeyElem, class Data, class Rank>
void AVLTree<KeyElem, Data, Rank>::AVLDestroy_rec(TNode *node) const {
    if (!node) {
        return;
    }

    if (node->left_son) {
        AVLDestroy_rec(node->left_son);
        //node->left_son = nullptr;
    }
    if (node->right_son) {
        AVLDestroy_rec(node->right_son);
        //node->right_son = nullptr;
    }
    delete node;
    return;
}

}  // namespace AVLRank

#endif
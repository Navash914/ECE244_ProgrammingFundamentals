//
// TreeDB.cpp
//
// Created by Naveed Ashfaq on 2018-11-19.
//

#include <iostream>
#include "TreeDB.h"

TreeDB::TreeDB() {
    root = nullptr;
}

TreeDB::~TreeDB() {
    clear();
}

bool TreeDB::insert(DBentry *newEntry) {
    if (root == nullptr) {
        // Inserting into empty tree
        root = new TreeNode(newEntry);
        return true;
    } else return insert_rec(root, newEntry);
}

DBentry *TreeDB::find(string name) {
    probesCount = 0;
    return find_rec(root, name);
}

bool TreeDB::remove(string name) {
    TreeNode* parent = nullptr;
    TreeNode* toDelete = find_delete_rec(root, name, parent);
    if (toDelete == nullptr)
        return false; // Node does not exist
    TreeNode *left = toDelete->getLeft(), *right = toDelete->getRight();
    TreeNode* replaceNode;
    if (left == nullptr && right == nullptr) {
        // Node to delete is a leaf
        replaceNode = nullptr;
    } else if (left == nullptr || right == nullptr) {
        // Node to delete has one subtree
        if (left == nullptr)
            replaceNode = right;
        else
            replaceNode = left;
    } else {
        // Node to delete has two subtrees
        replaceNode = left;
        if (left->getRight() != nullptr) {
            TreeNode* it = toDelete;
            while (replaceNode->getRight() != nullptr) {
                it = replaceNode;
                replaceNode = replaceNode->getRight();
            }
            it->setRight(replaceNode->getLeft());
            replaceNode->setLeft(left);
        }
        replaceNode->setRight(right);
    }

    // Check if deleting root
    if (parent != nullptr) {
        // Not deleting root
        if (parent->getLeft() == toDelete)
            parent->setLeft(replaceNode);
        else
            parent->setRight(replaceNode);
    }

    delete toDelete;
    return true;
}

void TreeDB::clear() {
    clear_rec(root);
}

void TreeDB::printProbes() const {
    std::cout << probesCount << std::endl;
}

void TreeDB::countActive() const {
    int count = countActive_rec(root);
    std::cout << count << std::endl;
}

ostream &operator<<(ostream &out, const TreeDB &rhs) { // Is a friend
    rhs.printAll_rec(out, rhs.root);
    return out;
}

//
// Recursive Helper Functions:
//

bool TreeDB::insert_rec(TreeNode *node, DBentry *entry) {
    // Assume not inserting at root, so node will never be null.
    if (node->getEntry()->getName() == entry->getName())
        return false; // Node already exists

    if (entry->getName() < node->getEntry()->getName()) {
        // Node goes in left subtree
        if (node->getLeft() == nullptr) {
            node->setLeft(new TreeNode(entry));
            return true;
        } else return insert_rec(node->getLeft(), entry);
    } else {
        // Node goes in right subtree
        if (node->getRight() == nullptr) {
            node->setRight(new TreeNode(entry));
            return true;
        } else return insert_rec(node->getRight(), entry);
    }
}

DBentry* TreeDB::find_rec(TreeNode *node, const string& name) {
    probesCount++;
    if (node == nullptr)
        return nullptr;
    if (node->getEntry()->getName() == name)
        return node->getEntry();
    if (name < node->getEntry()->getName())
        return find_rec(node->getLeft(), name);
    else
        return find_rec(node->getRight(), name);
}

TreeNode* TreeDB::find_delete_rec(TreeNode *node, const string &name, TreeNode *&prev) {
    if (node == nullptr)
        return nullptr;
    if (node->getEntry()->getName() == name)
        return node;
    prev = node;
    if (name < node->getEntry()->getName())
        return find_delete_rec(node->getLeft(), name, prev);
    else
        return find_delete_rec(node->getRight(), name, prev);
}

void TreeDB::clear_rec(TreeNode *node) {
    if (node == nullptr)
        return;
    clear_rec(node->getLeft());
    clear_rec(node->getRight());
    delete node;
}

int TreeDB::countActive_rec(TreeNode *node) const {
    if (node == nullptr)
        return 0;
    int count = node->getEntry()->getActive() ? 1 : 0;
    return count + countActive_rec(node->getLeft()) + countActive_rec(node->getRight());
}

void TreeDB::printAll_rec(ostream &out, TreeNode *node) const {
    if (node == nullptr)
        return;
    printAll_rec(out, node->getLeft());
    out << node;
    printAll_rec(out, node->getRight());
}

ostream &operator<<(ostream &out, TreeNode *rhs) { // Is not a friend
    out << rhs->getEntry();
    return out;
}

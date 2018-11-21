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
    probesCount = 0; // Reset probesCount
    return find_rec(root, name);
}

bool TreeDB::remove(string name) {
    TreeNode* parent = nullptr; // Parent of node to delete
    TreeNode* toDelete = find_delete_rec(root, name, parent); // Node to delete
    if (toDelete == nullptr)
        return false; // Node does not exist
    TreeNode* left = toDelete->getLeft(); // Left subtree of node to delete
    TreeNode* right = toDelete->getRight(); // Right subtree of node to delete
    TreeNode* replaceNode; // Node to replace the deleted node
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
        TreeNode* replaceParent = toDelete; // Parent node of replaceNode
        while (replaceNode->getRight() != nullptr) {
            replaceParent = replaceNode;
            replaceNode = replaceNode->getRight();
        }
        if (replaceParent != toDelete) {
            // Max on left subtree is not the root of that subtree
            // Left subtree of replaceNode is now right subtree of its parent
            replaceParent->setRight(replaceNode->getLeft());
            replaceNode->setLeft(left);
        }
        replaceNode->setRight(right);
    }

    // Check if deleting root
    if (parent == nullptr) {
        // Deleting root
        root = replaceNode;
    } else {
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
    root = nullptr;
}

void TreeDB::printProbes() const {
    std::cout << probesCount << std::endl;
}

void TreeDB::countActive() const {
    int count = countActive_rec(root);
    std::cout << count << std::endl;
}

// Print entire tree
// Is a friend
ostream &operator<<(ostream &out, const TreeDB &rhs) {
    rhs.printAll_rec(out, rhs.root);
    return out;
}

//
// Recursive Helper Functions:
//

// Insert new node recursively
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

// Find node in tree recursively
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

// Returns node to delete. Also sets pointer passed by ref to parent of node to delete.
TreeNode* TreeDB::find_delete_rec(TreeNode *node, const string &name, TreeNode *&parent) {
    if (node == nullptr)
        return nullptr;
    if (node->getEntry()->getName() == name)
        return node;
    parent = node;
    if (name < node->getEntry()->getName())
        return find_delete_rec(node->getLeft(), name, parent);
    else
        return find_delete_rec(node->getRight(), name, parent);
}

// Delete all nodes in tree recursively using post-order traversal
void TreeDB::clear_rec(TreeNode *node) {
    if (node == nullptr)
        return;
    clear_rec(node->getLeft());
    clear_rec(node->getRight());
    delete node;
}

// Counts active DBentries recursively
int TreeDB::countActive_rec(TreeNode *node) const {
    if (node == nullptr)
        return 0;
    int count = node->getEntry()->getActive() ? 1 : 0;
    return count + countActive_rec(node->getLeft()) + countActive_rec(node->getRight());
}

// Print all nodes alphabetically recursively using in-order traversal
void TreeDB::printAll_rec(ostream &out, TreeNode *node) const {
    if (node == nullptr)
        return;
    printAll_rec(out, node->getLeft());
    out << node;
    printAll_rec(out, node->getRight());
}

// Prints the entry in a tree node
// Is not a friend (?)
ostream &operator<<(ostream &out, TreeNode *rhs) {
    out << *rhs->getEntry();
    return out;
}

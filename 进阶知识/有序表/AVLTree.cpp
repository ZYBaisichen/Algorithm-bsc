//来自文心一言，仅做示意，不能运行。

#include <iostream>  
#include <climits>  
using namespace std;

struct AVLNode {  
    int key;  
    int height;  
    AVLNode* left;  
    AVLNode* right;  
  
    AVLNode(int k) : key(k), height(1), left(nullptr), right(nullptr) {}  
};  
  
class AVLTree {  
private:  
    AVLNode* root;  
  
    int getHeight(AVLNode* N) {  
        if (N == nullptr)  
            return 0;  
        return N->height;  
    }  
  
    int getBalance(AVLNode* N) {  
        if (N == nullptr)  
            return 0;  
        return getHeight(N->left) - getHeight(N->right);  
    }  
  
    AVLNode* rightRotate(AVLNode* y) {  
        AVLNode* x = y->left;  
        AVLNode* T2 = x->right;  
  
        x->right = y;  
        y->left = T2;  
  
        y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;  
        x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;  
  
        return x;  
    }  
  
    AVLNode* leftRotate(AVLNode* x) {  
        AVLNode* y = x->right;  
        AVLNode* T2 = y->left;  
  
        y->left = x;  
        x->right = T2;  
  
        x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;  
        y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;  
  
        return y;  
    }  
  
    AVLNode* insertNode(AVLNode* node, int key) {  
        if (node == nullptr)  
            return new AVLNode(key);  
  
        if (key < node->key)  
            node->left = insertNode(node->left, key);  
        else if (key > node->key)  
            node->right = insertNode(node->right, key);  
        else // Duplicate keys not allowed  
            return node;  
  
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));  

        //获取平衡因子，左树高度-右树高度
        int balance = getBalance(node);  
  
        // Left Left Case  LL型基于node左旋
        if (balance > 1 && key < node->left->key)  
            return rightRotate(node);  
  
        // Right Right Case   RR型基于node右旋
        if (balance < -1 && key > node->right->key)  
            return leftRotate(node);  
  
        // Left Right Case  LR，先左旋，再右旋
        if (balance > 1 && key > node->left->key) {  
            node->left = leftRotate(node->left);  
            return rightRotate(node);  
        }  
  
        // Right Left Case  RL，先右旋，再左旋
        if (balance < -1 && key < node->right->key) {  
            node->right = rightRotate(node->right);  
            return leftRotate(node);  
        }  
  
        return node;  
    }  
  
    AVLNode* minValueNode(AVLNode* node) {  
        AVLNode* current = node;  
  
        while (current->left != nullptr)  
            current = current->left;  
  
        return current;  
    }  
  
    AVLNode* deleteNode(AVLNode* root, int key) {  
        // Perform the normal BST delete  
        if (root == nullptr) return root;  
    
        if (key < root->key)  
            root->left = deleteNode(root->left, key);  
        else if (key > root->key)  
            root->right = deleteNode(root->right, key);  
        else {  
            // Node with only one child or no child  
            if (root->left == nullptr) {  
                AVLNode* temp = root->right;  
                delete root;  
                return temp;  
            } else if (root->right == nullptr) {  
                AVLNode* temp = root->left;  
                delete root;  
                return temp;  
            }  
    
            // Node with two children: Get the inorder successor (smallest in the right subtree)  
            AVLNode* temp = minValueNode(root->right);  
    
            // Copy the inorder successor's data to this node  
            root->key = temp->key;  
    
            // Delete the inorder successor  
            root->right = deleteNode(root->right, temp->key);  
        }  
    
        if (root == nullptr) return root;  
    
        // Update height of this ancestor node  
        root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));  
    
        // Get the balance factor of this ancestor node to check whether  
        // this node became unbalanced after the deletion  
        int balance = getBalance(root);  
    
        // If this node becomes unbalanced, then there are 4 cases  
    
        // Left Left Case  
        if (balance > 1 && getBalance(root->left) >= 0)  
            return rightRotate(root);  
    
        // Left Right Case  
        if (balance > 1 && getBalance(root->left) < 0) {  
            root->left = leftRotate(root->left);  
            return rightRotate(root);  
        }  
    
        // Right Right Case  
        if (balance < -1 && getBalance(root->right) <= 0)  
            return leftRotate(root);  
    
        // Right Left Case  
        if (balance < -1 && getBalance(root->right) > 0) {  
            root->right = rightRotate(root->right);  
            return leftRotate(root);  
        }  
    
        return root;  
    }
}
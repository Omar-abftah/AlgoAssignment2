#include <iostream>
#include <queue>
#include <limits>
using namespace std;

enum Color { RED, BLACK };

struct TreeNode {
    int value;
    Color color;
    TreeNode* left, * right, * parent;

    TreeNode(int value) {
        this->value = value;
        left = right = parent = nullptr;
        this->color = RED;
    }
};

class RedBlackTree {
private:
    TreeNode* root;

    // Helper function to insert a node in BST
    TreeNode* insertBST(TreeNode* root, TreeNode* newNode) {
        if (root == nullptr)
            return newNode;

        if (newNode->value < root->value) {
            root->left = insertBST(root->left, newNode);
            if (root->left != nullptr) root->left->parent = root;
        } else if (newNode->value > root->value) {
            root->right = insertBST(root->right, newNode);
            if (root->right != nullptr) root->right->parent = root;
        }

        return root;
    }

    // Rotation functions (Left and Right)
    void rotateLeft(TreeNode*& root, TreeNode*& node) {
        TreeNode* rightChild = node->right;

        node->right = rightChild->left;
        if (node->right != nullptr)
            node->right->parent = node;

        rightChild->parent = node->parent;
        if (node->parent == nullptr)
            root = rightChild;
        else if (node == node->parent->left)
            node->parent->left = rightChild;
        else
            node->parent->right = rightChild;

        rightChild->left = node;
        node->parent = rightChild;
    }

    void rotateRight(TreeNode*& root, TreeNode*& node) {
        TreeNode* leftChild = node->left;

        node->left = leftChild->right;
        if (node->left != nullptr)
            node->left->parent = node;

        leftChild->parent = node->parent;
        if (node->parent == nullptr)
            root = leftChild;
        else if (node == node->parent->left)
            node->parent->left = leftChild;
        else
            node->parent->right = leftChild;

        leftChild->right = node;
        node->parent = leftChild;
    }

    // Fix Red-Black Tree violations after insertion
    void fixViolation(TreeNode*& root, TreeNode*& node) {
        TreeNode* parent = nullptr;
        TreeNode* grandParent = nullptr;

        while ((node != root) && (node->color == RED) && (node->parent->color == RED)) {
            parent = node->parent;
            grandParent = node->parent->parent;

            if (parent == grandParent->left) {
                TreeNode* uncle = grandParent->right;

                if (uncle != nullptr && uncle->color == RED) {
                    grandParent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandParent;
                } else {
                    if (node == parent->right) {
                        rotateLeft(root, parent);
                        node = parent;
                        parent = node->parent;
                    }

                    rotateRight(root, grandParent);
                    swap(parent->color, grandParent->color);
                    node = parent;
                }
            } else {
                TreeNode* uncle = grandParent->left;

                if (uncle != nullptr && uncle->color == RED) {
                    grandParent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandParent;
                } else {
                    if (node == parent->left) {
                        rotateRight(root, parent);
                        node = parent;
                        parent = node->parent;
                    }

                    rotateLeft(root, grandParent);
                    swap(parent->color, grandParent->color);
                    node = parent;
                }
            }
        }

        root->color = BLACK;
    }

    // Fix Red-Black Tree violations after deletion
    void fixDeleteViolation(TreeNode*& root, TreeNode* x, TreeNode* xParent) {
        while (x != root && (x == nullptr || x->color == BLACK)) {
            if (x == xParent->left) {
                TreeNode* w = xParent->right;

                // Case 1: Sibling is red
                if (w->color == RED) {
                    w->color = BLACK;
                    xParent->color = RED;
                    rotateLeft(root, xParent);
                    w = xParent->right;
                }

                // Case 2: Sibling's children are black
                if ((w->left == nullptr || w->left->color == BLACK) &&
                    (w->right == nullptr || w->right->color == BLACK)) {
                    w->color = RED;
                    x = xParent;
                    xParent = x->parent;
                } else {
                    // Case 3: Sibling's right child is black
                    if (w->right == nullptr || w->right->color == BLACK) {
                        if (w->left != nullptr)
                            w->left->color = BLACK;
                        w->color = RED;
                        rotateRight(root, w);
                        w = xParent->right;
                    }

                    // Case 4: Sibling's right child is red
                    w->color = xParent->color;
                    xParent->color = BLACK;
                    if (w->right != nullptr)
                        w->right->color = BLACK;
                    rotateLeft(root, xParent);
                    x = root;
                    break;
                }
            } else {
                // Mirror cases for right child
                TreeNode* w = xParent->left;

                // Case 1: Sibling is red
                if (w->color == RED) {
                    w->color = BLACK;
                    xParent->color = RED;
                    rotateRight(root, xParent);
                    w = xParent->left;
                }

                // Case 2: Sibling's children are black
                if ((w->right == nullptr || w->right->color == BLACK) &&
                    (w->left == nullptr || w->left->color == BLACK)) {
                    w->color = RED;
                    x = xParent;
                    xParent = x->parent;
                } else {
                    // Case 3: Sibling's left child is black
                    if (w->left == nullptr || w->left->color == BLACK) {
                        if (w->right != nullptr)
                            w->right->color = BLACK;
                        w->color = RED;
                        rotateLeft(root, w);
                        w = xParent->left;
                    }

                    // Case 4: Sibling's left child is red
                    w->color = xParent->color;
                    xParent->color = BLACK;
                    if (w->left != nullptr)
                        w->left->color = BLACK;
                    rotateRight(root, xParent);
                    x = root;
                    break;
                }
            }
        }

        if (x != nullptr)
            x->color = BLACK;
    }

    // Find the minimum value node in a subtree
    TreeNode* minValueNode(TreeNode* node) {
        TreeNode* current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }

    // Inorder traversal for printing
    void inorderTraversal(TreeNode* root) {
        if (root == nullptr)
            return;

        inorderTraversal(root->left);
        cout << "Node: " << root->value << ", Color: " << (root->color == RED ? "RED" : "BLACK") << endl;
        inorderTraversal(root->right);
    }

    // Recursive tree deletion
    TreeNode* deleteBSTNode(TreeNode* root, int value) {
        if (root == nullptr) return root;

        if (value < root->value)
            root->left = deleteBSTNode(root->left, value);
        else if (value > root->value)
            root->right = deleteBSTNode(root->right, value);
        else {
            // Node to delete found
            TreeNode* replacement = nullptr;
            Color originalColor;

            // Case 1: Leaf node or node with one child
            if (root->left == nullptr || root->right == nullptr) {
                TreeNode* child = (root->left != nullptr) ? root->left : root->right;
                originalColor = root->color;

                if (child == nullptr) {
                    // Leaf node
                    replacement = root;
                    root = nullptr;
                } else {
                    // One child case
                    *root = *child;
                    replacement = root;
                    delete child;
                }

                // If we deleted a black node, we need to fix the violation
                if (originalColor == BLACK) {
                    fixDeleteViolation(this->root, replacement, replacement->parent);
                }
            } else {
                // Case 2: Node with two children
                TreeNode* successor = minValueNode(root->right);
                root->value = successor->value;
                originalColor = successor->color;

                // Delete the successor
                root->right = deleteBSTNode(root->right, successor->value);

                // If we deleted a black node, fix the violation
                if (originalColor == BLACK) {
                    TreeNode* replacementChild = (root->right != nullptr) ? root->right : nullptr;
                    fixDeleteViolation(this->root, replacementChild, root);
                }
            }
        }

        return root;
    }

    // Tree printing function for visualization
    void printTree(TreeNode* root, string indent = "", bool last = true) {
        if (root != nullptr) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "   ";
            } else {
                cout << "L----";
                indent += "|  ";
            }

            cout << root->value << "(" << (root->color == RED ? "RED" : "BLACK") << ")" << endl;

            printTree(root->left, indent, false);
            printTree(root->right, indent, true);
        }
    }

public:
    RedBlackTree() { root = nullptr; }

    bool search(TreeNode* root, int value) {
        if (root == nullptr)
            return false;

        if (value == root->value)
            return true;

        if (value < root->value)
            return search(root->left, value);
        else
            return search(root->right, value);
    }

    void insert(const int& value) {
        cout << "Inserting node with value: " << value << endl;

        if (search(root, value)) {
            cout << "Duplicate value detected! Node with value " << value << " already exists in the tree." << endl;
            return;
        }

        TreeNode* newNode = new TreeNode(value);
        root = insertBST(root, newNode);
        fixViolation(root, newNode);
        cout << "After violation fix:\n";
        printTree(root);
    }

    void deleteNode(int value) {
        if (root == nullptr) {
            cout << "Tree is empty. Nothing to delete.\n";
            return;
        }
        root = deleteBSTNode(root, value);
        cout << "After deletion of " << value << ":\n";
        printTree(root);
    }

    void inorder() {
        if (root == nullptr) {
            cout << "Tree is empty. Nothing to traverse.\n";
            return;
        }
        inorderTraversal(root);
    }
};

// Input validation function (unchanged from previous implementation)
int getValidInput() {
    int value;
    while (true) {
        if (cin >> value)
            return value;
        else {
            cout << "Invalid input! Please enter a valid integer: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int main() {
    RedBlackTree tree;
    int choice, value;

    do {
        cout << "\nRed-Black Tree Menu\n";
        cout << "1. Insert Node\n";
        cout << "2. Delete Node\n";
        cout << "3. Inorder Traversal\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        choice = getValidInput();

        switch (choice) {
            case 1:
                cout << "Enter the value to insert: ";
                value = getValidInput();
                tree.insert(value);
                break;
            case 2:
                cout << "Enter the value to delete: ";
                value = getValidInput();
                tree.deleteNode(value);
                break;
            case 3:
                cout << "Inorder Traversal of the tree:\n";
                tree.inorder();
                break;
            case 4:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}
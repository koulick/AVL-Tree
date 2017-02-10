#include <iostream>
using namespace std;

//The AVL Node class (generic)
template <class T> class AVLNode
{
    public:
        T key;
        int height;
        AVLNode *leftChild;
        AVLNode *rightChild;
        AVLNode *parent;

        AVLNode(T key, AVLNode *parent) :
            key(key), height(0), parent(parent), leftChild(NULL), rightChild(NULL) {}

        //destructor
        ~AVLNode() {
            delete leftChild;
            delete rightChild;
        }
};

//The AVL Tree class (generic)
template <class T> class AVLTree
{
    //public members
    public:
        AVLTree(void);
        ~AVLTree(void);
        void insertInAVLTree(T key);
        void inorderTraversalInAVLTree();

    //private members
    private:
        //the root element
        AVLNode<T> *root;

        //list of functions used
        AVLNode<T>* rotateLeftChild(AVLNode<T> *a);
        AVLNode<T>* rotateRightChild(AVLNode<T> *a);
        AVLNode<T>* rotateLeftChildThenRightChild(AVLNode<T> *n);
        AVLNode<T>* rotateRightChildThenLeftChild(AVLNode<T> *n);
        int height(AVLNode<T> *n);
        void adjustHeight(AVLNode<T> *n);
        void setHeight(AVLNode<T> *n);
        void inorderTraversalInAVLTree(AVLNode<T> *n);
};

//Root is set to NULL
template <class T> AVLTree<T>::AVLTree(void) : root(NULL) {}

//Function to rotate the right child and then the left child (after insertion)
template <class T> AVLNode<T>* AVLTree<T>::rotateRightChildThenLeftChild(AVLNode<T> *node)
{
    node->rightChild = rotateRightChild(node->rightChild);
    return rotateLeftChild(node);
}

//Function to rotate the left child (after insertion)
template <class T> AVLNode<T>* AVLTree<T>::rotateLeftChild(AVLNode<T> *node)
{
    AVLNode<T> *temp = node->rightChild;
    temp->parent = node->parent;
    node->rightChild = temp->leftChild;

    if (node->rightChild != NULL)
        node->rightChild->parent = node;

    temp->leftChild = node;
    node->parent = temp;

    if (temp->parent != NULL)
    {
        if (temp->parent->rightChild == node)
        {
            temp->parent->rightChild = temp;
        }
        else
        {
            temp->parent->leftChild = temp;
        }
    }

    setHeight(node);
    setHeight(temp);
    return temp;
}

//Function to rotate the left child and then the right child (after insertion)
template <class T> AVLNode<T>* AVLTree<T>::rotateLeftChildThenRightChild(AVLNode<T> *node)
{
    node->leftChild = rotateLeftChild(node->leftChild);
    return rotateRightChild(node);
}

//Function to rotate the right child (after insertion)
template <class T> AVLNode<T>* AVLTree<T>::rotateRightChild(AVLNode<T> *node)
{
    AVLNode<T> *temp = node->leftChild;
    temp->parent = node->parent;
    node->leftChild = temp->rightChild;

    if (node->leftChild != NULL)
        node->leftChild->parent = node;

    temp->rightChild = node;
    node->parent = temp;

    if (temp->parent != NULL)
    {
        if (temp->parent->rightChild == node)
        {
            temp->parent->rightChild = temp;
        }
        else
        {
            temp->parent->leftChild = temp;
        }
    }

    setHeight(node);
    setHeight(temp);
    return temp;
}

//Recursive function to determine the height (generic)
template <class T> int AVLTree<T>::height(AVLNode<T> *node)
{
    if (node == NULL)
        return -1;
    return 1 + std::max(height(node->leftChild), height(node->rightChild));
}

//Function to set the height (generic)
template <class T> void AVLTree<T>::setHeight(AVLNode<T> *node)
{
    //Height is the difference of right subtree and left subtree
    node->height = height(node->rightChild) - height(node->leftChild);
}

//Recursive function to adjust the height (generic)
template <class T> void AVLTree<T>::adjustHeight(AVLNode<T> *node)
{
    //Set the height of the AVL Tree
    setHeight(node);

    if (node->height == -2)
    {
        if (height(node->leftChild->leftChild) >= height(node->leftChild->rightChild))
            node = rotateRightChild(node);
        else
            node = rotateLeftChildThenRightChild(node);
    }
    else if (node->height == 2)
    {
        if (height(node->rightChild->rightChild) >= height(node->rightChild->leftChild))
            node = rotateLeftChild(node);
        else
            node = rotateRightChildThenLeftChild(node);
    }

    if (node->parent != NULL)
    {
        adjustHeight(node->parent);
    }
    else
    {
        root = node;
    }
}

//Recursive function for printing the in-order traversal of an AVL Tree
template <class T> void AVLTree<T>::inorderTraversalInAVLTree(AVLNode<T> *node)
{
   if(!node)
   {
       return;
   }
   inorderTraversalInAVLTree(node->leftChild);
   std::cout << ' ' << node->key;
   inorderTraversalInAVLTree(node->rightChild);
 }

// Destructor for the class
template <class T> AVLTree<T>::~AVLTree(void)
{
    delete root;
}

//Function to do in-order traversal in an AVL Tree
template <class T> void AVLTree<T>::inorderTraversalInAVLTree()
{
    inorderTraversalInAVLTree(root);
    std::cout << '\n';
}

//Function for inserting nodes in an AVL Tree
template <class T> void AVLTree<T>::insertInAVLTree(T key)
{
    if (root == NULL)
    {
        root = new AVLNode<T>(key, NULL);
    }
    else
    {
        AVLNode<T> *node = root, *parent;

        while (true)
        {
            if (node->key == key)
                return;

            parent = node;

            //check if the node will go to the left or the right
            bool goLeft = node->key > key;
            node = goLeft ? node->leftChild : node->rightChild;

            if (node == NULL)
            {
                if (goLeft)
                {
                    parent->leftChild = new AVLNode<T>(key, parent);
                }
                else
                {
                    parent->rightChild = new AVLNode<T>(key, parent);
                }

                //adjust the height (including insertion)
                adjustHeight(parent);
                break;
            }
        }
    }
}

//Main Function
int main(void)
{
    AVLTree<int> avlTree;

    std::cout << "10 numbers are inserted in an AVL Tree one by one and the in-order traversal is printed in each step below:" << '\n';
    //Numbers are given below:
    //7 9 13 21 34 36 45 54 65 87 (in the order given)
    std::cout << '\n' << "Step One:" << '\n';
    avlTree.insertInAVLTree(7);
    avlTree.inorderTraversalInAVLTree();

    std::cout << '\n' << "Step Two:" << '\n';
    avlTree.insertInAVLTree(9);
    avlTree.inorderTraversalInAVLTree();

    std::cout << '\n' << "Step Three:" << '\n';
    avlTree.insertInAVLTree(13);
    avlTree.inorderTraversalInAVLTree();

    std::cout << '\n' << "Step Four:" << '\n';
    avlTree.insertInAVLTree(21);
    avlTree.inorderTraversalInAVLTree();

    std::cout << '\n' << "Step Five:" << '\n';
    avlTree.insertInAVLTree(34);
    avlTree.inorderTraversalInAVLTree();

    std::cout << '\n' << "Step Six:" << '\n';
    avlTree.insertInAVLTree(36);
    avlTree.inorderTraversalInAVLTree();

    std::cout << '\n' << "Step Seven:" << '\n';
    avlTree.insertInAVLTree(45);
    avlTree.inorderTraversalInAVLTree();

    std::cout << '\n' << "Step Eight:" << '\n';
    avlTree.insertInAVLTree(54);
    avlTree.inorderTraversalInAVLTree();

    std::cout << '\n' << "Step Nine:" << '\n';
    avlTree.insertInAVLTree(65);
    avlTree.inorderTraversalInAVLTree();

    std::cout << '\n' << "Step Ten:" << '\n';
    avlTree.insertInAVLTree(87);
    avlTree.inorderTraversalInAVLTree();
}

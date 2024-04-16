#include <iostream>
#include <vector>
#include <limits>

using namespace std;

template <typename T>
struct node
{
    T el; // element
    node<T> *left;
    node<T> *right;
};

template <typename T>
class BinaryTree
{
protected:
    // const T MIN = numeric_limits<T>::lowest(); // oznaczenie pustego pola w tablicy jako najmniejsza mozliwa wartosc z danego typu
    node<T> *root = nullptr;

    vector<node<T> *> order;

    void print()
    {
        for (auto &element : order)
        {
            std::cout << element->el << " ";
        }
        cout << endl;
    }

    void preorder(node<T> *root) // PNode root
    {
        if (root == nullptr)
            return;

        order.push_back(root); // cout << t->info << endl;
        preorder(root->left);
        preorder(root->right);
    }

    void inorder(node<T> *root)
    {
        if (root == nullptr)
            return;

        inorder(root->left);
        order.push_back(root);
        inorder(root->right);
    }

    void postorder(node<T> *root)
    {
        if (root == nullptr)
            return;

        postorder(root->left);
        postorder(root->right);
        order.push_back(root);
    }

    void deleteBinaryTree(node<T> *&root)
    {
        if (root != nullptr)
        {
            deleteBinaryTree(root->left);
            deleteBinaryTree(root->right);
            delete root;
            root = nullptr;
        }
    }

public:
    BinaryTree(T element) // konstruktor
    {
        root = new node<T>;
        root->el = element;
    }

    ~BinaryTree() // destruktor
    {
        deleteBinaryTree(root);
    }

    node<T> *addToLeft(T element, node<T> *parent = nullptr) // gdy nie zostanie podany parent to zakladamy ze to root, metoda dodaje na lewo od parent nowy element lub go nadpisuje na nowy
    {
        if (parent == nullptr)
            parent = root;

        node<T> *buff = new node<T>;
        buff->el = element;
        if (parent->left != nullptr)
        {
            buff->left = parent->left->left;
            buff->right = parent->left->right;
            delete parent->left;
        }
        parent->left = buff;
        return parent->left;
    }
    node<T> *addToRight(T element, node<T> *parent = nullptr) // gdy nie zostanie podany parent to zakladamy ze to root, metoda dodaje na prawo od parent nowy element lub go nadpisuje na nowy
    {
        if (parent == nullptr)
            parent = root;

        node<T> *buff = new node<T>;
        buff->el = element;
        if (parent->right != nullptr)
        {
            buff->left = parent->right->left;
            buff->right = parent->right->right;
            delete parent->right;
        }
        parent->right = buff;
        return parent->right;
    }
    vector<node<T> *> getPreorder(node<T> *root = nullptr)
    {
        if (root == nullptr)
            root = this->root;

        order.clear();
        preorder(root);
        return order;
    }

    vector<node<T> *> getInorder(node<T> *root = nullptr)
    {
        if (root == nullptr)
            root = this->root;

        order.clear();
        inorder(root);
        return order;
    }

    vector<node<T> *> getPostorder(node<T> *root = nullptr)
    {
        if (root == nullptr)
            root = this->root;

        order.clear();
        postorder(root);
        return order;
    }

    void printPreorder(node<T> *root = nullptr)
    {
        getPreorder(root);
        print();
    }

    void printInorder(node<T> *root = nullptr)
    {
        getInorder(root);
        print();
    }

    void printPostorder(node<T> *root = nullptr)
    {
        getPostorder(root);
        print();
    }
};

int main()
{
    // w konstruktorze ustalamy root
    BinaryTree<int> test(5);                                 //                          5                           //
                                                             //                        /   \                         //
    node<int> *rootLeft = test.addToLeft(25);                //                       /     \                        //
    node<int> *rootRight = test.addToRight(30);              //                      25     30                       //
                                                             //                     /  \    /                        //
    node<int> *leftFrom25 = test.addToLeft(100, rootLeft);   //                   100  332 47                        //
    node<int> *rightFrom25 = test.addToRight(332, rootLeft); //                        /    \                        //
                                                             //                      8767   999                      //
    node<int> *leftFrom30 = test.addToLeft(47, rootRight);

    node<int> *leftFrom332 = test.addToLeft(8767, rightFrom25);

    node<int> *rightFrom47 = test.addToRight(999, leftFrom30);

    cout << "Preorder:  ";
    test.printPreorder();
    cout << endl;

    cout << "Inorder:   ";
    test.printInorder();
    cout << endl;

    cout << "Postorder: ";
    test.printPostorder();
    cout << endl;

    // test podmieniania elementu
    cout << endl;
    cout << "test podmieniania elemtnu 25 na 7000:" << endl;
    test.addToLeft(7000);
    cout << "Preorder:  ";
    test.printPreorder();
    cout << endl;

    cout << "Inorder:   ";
    test.printInorder();
    cout << endl;

    cout << "Postorder: ";
    test.printPostorder();
    cout << endl;

    return 0;
}

#include <iostream>
#include <vector>

using namespace std;

template <typename T>
struct node
{
    T el; // element
    node<T> *smaller = nullptr;
    node<T> *bigger = nullptr;
};

template <typename T>
class BST
{
protected:
    node<T> *root = nullptr;

    int size = 0;

    vector<node<T> *> order;

    void print()
    {
        for (auto &element : order)
        {
            std::cout << element->el << " ";
        }
        cout << endl;
    }

    bool putInPlace(T element, node<T> *&root)
    {
        if (root == nullptr)
        {
            root = new node<T>;
            root->el = element;
            return true;
        }

        if (element < root->el)
            return putInPlace(element, root->smaller);
        else if (element > root->el)
            return putInPlace(element, root->bigger);
        else
            return false;
    }

    bool removeNode(T element, node<T> *&root)
    {
        if (root == nullptr)
            return false;

        if (element < root->el)
            return removeNode(element, root->smaller);
        else if (element > root->el)
            return removeNode(element, root->bigger);
        else
        {
            if (root->smaller == nullptr && root->bigger == nullptr)
            {
                delete root;
                root = nullptr;
            }
            else if (root->smaller == nullptr)
            {
                node<T> *temp = root;
                root = root->bigger;
                delete temp;
            }
            else if (root->bigger == nullptr)
            {
                node<T> *temp = root;
                root = root->smaller;
                delete temp;
            }
            else
            {
                node<T> *temp = findMin(root->bigger);
                root->el = temp->el;
                removeNode(temp->el, root->bigger);
            }
            return true;
        }
    }

    node<T> *findMin(node<T> *root)
    {
        while (root->smaller != nullptr)
            root = root->smaller;
        return root;
    }

    void preorder(node<T> *root)
    {
        if (root == nullptr)
            return;

        order.push_back(root);
        preorder(root->smaller);
        preorder(root->bigger);
    }

    void inorder(node<T> *root)
    {
        if (root == nullptr)
            return;

        inorder(root->smaller);
        order.push_back(root);
        inorder(root->bigger);
    }

    void postorder(node<T> *root)
    {
        if (root == nullptr)
            return;

        postorder(root->smaller);
        postorder(root->bigger);
        order.push_back(root);
    }

    void deleteBST(node<T> *&root)
    {
        if (root != nullptr)
        {
            deleteBST(root->smaller);
            deleteBST(root->bigger);
            delete root;
            root = nullptr;
        }
    }

public:
    BST() {} // konstruktor

    ~BST() // destruktor
    {
        deleteBST(root);
    }

    bool add(T element) // zwraca true gdy uda sie dodac nowy element
    {
        bool statement = false;
        if (root == nullptr)
        {
            root = new node<T>;
            root->el = element;
            statement = true;
        }
        else
            statement = putInPlace(element, this->root);

        if (statement)
            size++;

        return statement;
    }

    bool remove(T element)
    {
        bool statement = removeNode(element, this->root);
        if (statement)
            size--;

        return statement;
    }

    int getSize()
    {
        return size;
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

    void printPreorder()
    {
        getPreorder();
        print();
    }

    void printInorder()
    {
        getInorder();
        print();
    }

    void printPostorder()
    {
        getPostorder();
        print();
    }
};

int main()
{
    BST<int> test;

    test.add(12); //          12            //
    test.add(6);  //         /  \           //
    test.add(3);  //        6   20          //
    test.add(7);  //       / \    \         //
    test.add(20); //      3   7   23        //
    test.add(4);  //       \                //
    test.add(23); //        4               //
    cout << "Test dla proby dodania tego samego elementu: " << test.add(7) << endl
         << endl;

    // cout << "Preorder:  ";
    // test.printPreorder();
    // cout << endl;

    cout << test.getSize() << endl;
    cout << "Inorder:   ";
    test.printInorder();
    cout << endl;

    test.remove(12);

    cout << test.getSize() << endl;
    cout << "Inorder:   ";
    test.printInorder();
    cout << endl;

    test.remove(6);

    cout << test.getSize() << endl;
    cout << "Inorder:   ";
    test.printInorder();
    cout << endl;

    test.remove(4);

    cout << test.getSize() << endl;
    cout << "Inorder:   ";
    test.printInorder();
    cout << endl;

    test.add(6);
    test.add(12);
    test.add(4);

    cout << test.getSize() << endl;
    cout << "Inorder:   ";
    test.printInorder();
    cout << endl;

    // cout << "Postorder: ";
    // test.printPostorder();
    // cout << endl;

    return 0;
}

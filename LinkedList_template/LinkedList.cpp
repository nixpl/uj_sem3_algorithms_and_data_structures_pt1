#include <iostream>

using namespace std;

template <typename T>
struct cell
{
    T el; // element
    cell<T> *next;
};

template <typename T>
class ListaJednokierunkowa
{
protected:
    cell<T> *header = nullptr; // głowa listy

public:
    ListaJednokierunkowa(){}; // konstruktor

    ~ListaJednokierunkowa() // destruktor
    {
        while (header != nullptr)
        {
            cell<T> *buff = header;
            header = header->next;
            delete buff;
        }
    };

    void Insert(T x, cell<T> *p) // wstawia element x na pozycję next komórki o wskaźniku p
    {                            // (lub w przypadku pustej listy tworzy komórkę z elementem x w miejscu głowy listy)
        if (p == nullptr || header == nullptr)
        {
            cell<T> *buffPtr = header;
            header = new cell<T>;
            header->el = x;
            header->next = buffPtr;
        }
        else
        {
            cell<T> *buffPtr = p->next;
            p->next = new cell<T>;
            (p->next)->el = x;
            (p->next)->next = buffPtr;
        }
    };

    bool Delete(cell<T> *p) // usuwa komórkę z pozycji next komórki o wskaźniku p, jak dostanie null to usuwa z head
    {                       // ma informować czy operacja się powiodła czy nie
        if (p == nullptr)
        {
            cell<T> *bufPtr = header->next;
            delete header;
            header = bufPtr;
            return true;
        }
        else if (p->next == nullptr)
        {
            return false;
        }
        else
        {
            cell<T> *buffPtr = (p->next)->next;
            delete (p->next);
            p->next = buffPtr;
            return true;
        }
    };

    T Retrieve(cell<T> *p) // zwraca element komórki o wskaźniku p
    {
        if (p == nullptr && header != nullptr)
            return header->el;
        else
            return p->el;
    };

    cell<T> *Locate(T x) // zwraca wskaźnik do pierwszej komórki z elementem x
    {
        bool found = false;
        cell<T> *location = header;
        while (location != nullptr && !found)
        {
            if (location->el == x)
                found = true;
            else
                location = location->next;
        }
        return location;
    };

    cell<T> *First() // zwraca nullptr tak jak Pani zaproponowala w mailu w przypadku pustej listy
    {
        return header;
    };

    cell<T> *Next(cell<T> *p) // zwraca wskaźnik do kolejnej komórki po komórce o wskaźniku p
    {
        return p->next;
    };

    cell<T> *Previous(cell<T> *p) // zwraca wskaźnik do komórki poprzedzającej komórkę o wskaźniku p
    {
        bool found = false;
        cell<T> *location = header;
        while (location != nullptr && !found)
        {
            if (location->next == p)
                found = true;
            else
                location = location->next;
        }
        return location;
    };

    cell<T> *Last() // zwraca wskaźnik do ostatniej komórki na liście
    {
        cell<T> *location = header;
        cell<T> *previousLocation;
        while (location != nullptr)
        {
            previousLocation = location;
            location = location->next;
        }
        return previousLocation;
    };

    void print() // wyświetla wszystkie elementy listy
    {
        cell<T> *location = header;
        int i = 0;
        while (location != nullptr)
        {
            cout << "[cell nr.:" << ++i << " | "
                 << "element: " << location->el << " ]" << endl;

            location = location->next;
        }
        cout << endl;
    };
};

template <typename T>
struct cell_with_prev
{
    T el; // element
    cell_with_prev<T> *next;
    cell_with_prev<T> *prev;
};

template <typename T>
class ListaDwukierunkowa
{
protected:
    cell_with_prev<T> *header = nullptr; // głowa listy

public:
    ListaDwukierunkowa(){}; // konstruktor

    ~ListaDwukierunkowa() // destruktor
    {
        while (header != nullptr)
        {
            cell_with_prev<T> *buff = header;
            header = header->next;
            delete buff;
        }
    };

    void Insert(T x, cell_with_prev<T> *p) // wstawia element x na pozycję next komórki o wskaźniku p
    {                                      // (lub w przypadku pustej listy tworzy komórkę z elementem x w miejscu głowy listy)
        if (p == nullptr || header == nullptr)
        {
            cell_with_prev<T> *buffPtr = header;
            header = new cell_with_prev<T>;
            header->el = x;
            header->next = buffPtr;
            if (buffPtr != nullptr)
                buffPtr->prev = header;
        }
        else
        {
            cell_with_prev<T> *buffPtr = p->next;
            p->next = new cell_with_prev<T>;
            (p->next)->el = x;
            (p->next)->next = buffPtr;
            (p->next)->prev = p;
            if (buffPtr != nullptr)
                buffPtr->prev = p->next;
        }
    };

    bool Delete(cell_with_prev<T> *p) // usuwa komórkę z pozycji next komórki o wskaźniku p, jak dostanie null to usuwa z head
    {                                 // ma informować czy operacja się powiodła czy nie
        if (p == nullptr)
        {
            cell_with_prev<T> *bufPtr = header->next;
            delete header;
            header = bufPtr;
            return true;
        }
        else if (p->next == nullptr)
        {
            return false;
        }
        else
        {
            cell_with_prev<T> *buffPtr = (p->next)->next;
            delete (p->next);
            p->next = buffPtr;
            if (buffPtr != nullptr)
                buffPtr->prev = p;
            return true;
        }
    };

    T Retrieve(cell_with_prev<T> *p) // zwraca element komórki o wskaźniku p
    {
        if (p == nullptr && header != nullptr)
            return header->el;
        else
            return p->el;
    };

    cell_with_prev<T> *Locate(T x) // zwraca wskaźnik do pierwszej komórki z elementem x
    {
        bool found = false;
        cell_with_prev<T> *location = header;
        while (location != nullptr && !found)
        {
            if (location->el == x)
                found = true;
            else
                location = location->next;
        }
        return location;
    };

    cell_with_prev<T> *First() // zwraca nullptr tak jak Pani zaproponowala w mailu w przypadku pustej listy
    {
        return header;
    };

    cell_with_prev<T> *Next(cell_with_prev<T> *p) // zwraca wskaźnik do kolejnej komórki po komórce o wskaźniku p
    {
        return p->next;
    };

    cell_with_prev<T> *Previous(cell_with_prev<T> *p) // zwraca wskaźnik do komórki poprzedzającej komórkę o wskaźniku p
    {
        return p->prev;
    };

    cell_with_prev<T> *Last() // zwraca wskaźnik do ostatniej komórki na liście
    {
        cell_with_prev<T> *location = header;
        cell_with_prev<T> *previousLocation;
        while (location != nullptr)
        {
            previousLocation = location;
            location = location->next;
        }
        return previousLocation;
    };

    void print() // wyświetla wszystkie elementy listy
    {
        cell_with_prev<T> *location = header;
        int i = 0;
        while (location != nullptr)
        {
            cout << "[cell nr.:" << ++i << " | "
                 << "element: " << location->el << " ]" << endl;

            location = location->next;
        }
        cout << endl;
    };
};

int main()
{
    // Test listy jednokierunkowej -------------------------------------------------------------------
    cout << "Test listy jednokierunkowej:" << endl;
    ListaJednokierunkowa<int> one_way;

    cout << "Dodawanie najpierw dwoch elementow nastepnie zapisanie wskaznika jako previous z one_way.Previous(one_way.Last()), a potem dodanie jeszcze jednego elementu na poczatek:" << endl;
    one_way.Insert(1, one_way.First());
    one_way.Insert(2, NULL);
    cell<int> *previous = one_way.Previous(one_way.Last());
    one_way.Insert(3, NULL);
    one_way.print();

    cout << "Test one_way.First()->el: ";
    cout << one_way.First()->el << endl
         << endl;

    cout << "Test one_way.Retrieve(previous):";
    cout << one_way.Retrieve(previous) << endl
         << endl;

    cout << "Dolaczenie na koniec nowego elementu:" << endl;
    one_way.Insert(100, one_way.Last());
    one_way.print();

    cout << "Dodanie elementu przy pomocy one_way.Insert(600, previous):" << endl;
    one_way.Insert(600, previous);
    one_way.print();

    cout << "Dodanie elementu przy pomocy one_way.Insert(2500, one_way.Next(previous)):" << endl;
    one_way.Insert(2500, one_way.Next(previous));
    one_way.print();

    cout << "Usuniecie previous: one_way.Delete(previous):" << endl;
    one_way.Delete(previous);
    one_way.print();

    cout << "Usuniecie pierwszego elementu: one_way.Delete(NULL):" << endl;
    one_way.Delete(NULL);
    one_way.print();

    cout << "Test one_way.First()->el: ";
    cout << one_way.First()->el << endl
         << endl;

    cout << "Test one_way.Locate(1): ";
    cout << one_way.Locate(1) << "->el: " << one_way.Locate(1)->el << endl;
    cout << "Test bledu one_way.Locate(80000): ";
    cout << one_way.Locate(80000) << endl
         << endl;

    // Test listy dwukierunkowej -------------------------------------------------------------------
    cout << "Test listy dwukierunkowej:" << endl;
    ListaDwukierunkowa<int> two_way;

    cout << "Dodawanie najpierw dwoch elementow nastepnie zapisanie wskaznika jako previous1 z two_way.Previous(two_way.Last()), a potem dodanie jeszcze jednego elementu na poczatek:" << endl;
    two_way.Insert(1, two_way.First());
    two_way.Insert(2, NULL);
    cell_with_prev<int> *previous1 = two_way.Previous(two_way.Last());
    two_way.Insert(3, NULL);
    two_way.print();

    cout << "Test two_way.First()->el: ";
    cout << two_way.First()->el << endl
         << endl;

    cout << "Test two_way.Retrieve(previous1):";
    cout << two_way.Retrieve(previous1) << endl
         << endl;

    cout << "Dolaczenie na koniec nowego elementu:" << endl;
    two_way.Insert(100, two_way.Last());
    two_way.print();

    cout << "Dodanie elementu przy pomocy two_way.Insert(600, previous1):" << endl;
    two_way.Insert(600, previous1);
    two_way.print();

    cout << "Dodanie elementu przy pomocy two_way.Insert(2500, two_way.Next(previous1)):" << endl;
    two_way.Insert(2500, two_way.Next(previous1));
    two_way.print();

    cout << "Usuniecie previous: two_way.Delete(previous1):" << endl;
    two_way.Delete(previous1);
    two_way.print();

    cout << "Usuniecie pierwszego elementu: two_way.Delete(NULL):" << endl;
    two_way.Delete(NULL);
    two_way.print();

    cout << "Test two_way.First()->el: ";
    cout << two_way.First()->el << endl
         << endl;

    cout << "Test two_way.Locate(1): ";
    cout << two_way.Locate(1) << "->el: " << two_way.Locate(1)->el << endl;
    cout << "Test bledu two_way.Locate(80000): ";
    cout << two_way.Locate(80000) << endl
         << endl;

    cout << "Przed zakonczeniem dzialania uruchomia sie destruktory usuwajace listy" << endl;
    return 0;
}
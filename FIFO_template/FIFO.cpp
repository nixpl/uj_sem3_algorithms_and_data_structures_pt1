#include <iostream>
#include <limits>

using namespace std;

template <typename T>
struct cell
{
    T el; // element
    cell<T> *next;
};

template <typename T>
class FIFO
{
protected:
    const T MIN = numeric_limits<T>::lowest(); // oznaczenie pustego pola w tablicy jako najmniejsza mozliwa wartosc z danego typu
    cell<T> *header = nullptr;                 // głowa listy

    void clear()
    {
        while (header != nullptr)
        {
            cell<T> *buff = header;
            header = header->next;
            delete buff;
        }
    }

public:
    FIFO(){}; // konstruktor

    ~FIFO() // destruktor
    {
        clear();
    };

private:
    bool Insert(T x, cell<T> *p) // wstawia element x na pozycję next komórki o wskaźniku p
    {                            // (lub w przypadku pustej listy tworzy komórkę z elementem x w miejscu głowy listy)
        if (x > MIN)             // zwraca true gdy operacja sie uda i false gdy sie nie uda (nie uda sie gdy x = MIN)
        {
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
            return true;
        }
        else
        {
            return false;
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
        cell<T> *previousLocation = nullptr;
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

public:
    bool put(T x) //- dodaje element do kolejki,
    {             // gdy x = MIN nwraca false
        return Insert(x, Last());
    }
    T get() //- pobiera (i usuwa) pierwszy element z kolejki,
    {       // gdy kolejka jest pusta get() zwraca MIN (element który na pewno nie wystąpi w kolejce)
        if (!empty())
        {
            T buff = Retrieve(nullptr);
            Delete(nullptr);
            return buff;
        }
        else
        {
            return MIN;
        }
    }
    T front() //- zwraca element znajdujący się na początku kolejki (bez usuwania),
    {
        if (!empty())
            return Retrieve(nullptr);
        else
            return MIN;
    }
    void makenull() //- usuwa wszystkie elementy z kolejki,
    {
        clear();
    }
    bool empty() //- sprawdza, czy kolejka jest pusta.
    {
        // cout << header << endl;
        if (header == nullptr)
            return true;
        else
            return false;
    }
};

template <typename T>
class ArrayFIFO
{
private:
    const T MIN = numeric_limits<T>::lowest(); // oznaczenie pustego pola w tablicy jako najmniejsza mozliwa wartosc z danego typu
    int array_size = 5;
    T *array;
    int last = 0;
    int first = 0;

public:
    ArrayFIFO()
    {
        ArrayFIFO(this->array_size);
    }
    ArrayFIFO(unsigned int array_size)
    {
        this->array_size = array_size + 1; // +1 bo w tej implementacji jedno pole (przed first) pozostaje puste
        array = new T[this->array_size];
    }
    ~ArrayFIFO()
    {
        delete[] array;
    }

    bool put(T x) //- dodaje element do kolejki, jeśli jest miejsce i x > MIN,
    {
        if (x > MIN)
        {
            if ((last + 1) % array_size != first)
            {
                array[last] = x;
                last = (last + 1) % array_size;
                return true;
            }
            else
                return false;
        }
        else
        {
            return false;
        }
    }
    T get() //- pobiera (i usuwa) pierwszy element z kolejki, zwraca MIN gdy kolejka jest pusta
    {
        if (first != last)
        {
            T buff = array[first];
            first = (first + 1) % array_size;
            return buff;
        }
        else
            return MIN;
    }
    T front() //- zwraca element znajdujący się na początku kolejki (bez usuwania),
    {
        if (!empty())
            return array[first];
        else
            return MIN;
    }
    void makenull() //- usuwa wszystkie elementy z kolejki,
    {
        first = last;
    }
    bool empty() //- sprawdza, czy kolejka jest pusta.
    {
        return first == last;
    }
};

int main()
{
    // test FIFO ze wskaznikami ------------------------------------------------------------------------------
    cout << "TEST FIFO ze wskaznikami: " << endl;
    FIFO<int> test;

    cout << "czy kolejka jest pusta: " << test.empty() << endl;

    cout << "dodaje 10 do kolejki" << endl;
    test.put(10);
    cout << "dodaje 20 do kolejki" << endl;
    test.put(20);
    cout << "czy kolejka jest pusta: " << test.empty() << endl;

    cout << "co jest na przedzie kolejki: " << test.front() << endl;
    cout << "pierwszy element opuszcza kolejkę i jest nim: " << test.get() << endl;
    cout << "co jest na przedzie kolejki: " << test.front() << endl;
    cout << "pierwszy element opuszcza kolejkę i jest nim: " << test.get() << endl;
    cout << "czy kolejka jest pusta: " << test.empty() << endl;

    cout << "uzycie get() na pustej kolejce zwroci najmniejsza mozliwa wartosc danego typu, ktora sie nie moze znalezc w kolejce: " << test.get() << endl;
    cout << "test front() dla pustej kolejki: " << test.front() << endl;

    for (int i = 1; i < 7; i++)
    {
        test.put(i);
        cout << "dodano element: " << i << " do kolejki" << endl;
    }
    cout << "czy kolejka jest pusta: " << test.empty() << endl;
    cout << "co jest na przedzie kolejki: " << test.front() << endl;

    cout << "usuwam cala kolejke przy pomocy makenull()" << endl;
    test.makenull();
    cout << "czy kolejka jest pusta: " << test.empty() << endl
         << endl;

    // test FIFO tablicowego cyklicznego ------------------------------------------------------------------------------

    cout << "TEST FIFO tablicowego cyklicznego: " << endl;
    ArrayFIFO<int> test1(10); // dziesiec elementow chcemy aby miescila ta lista (w srodku zrobi sie tablica z 11 elementami ze wzgledu na jedno puste pole ktorego wymaga ta implementacja)

    cout << "czy kolejka jest pusta: " << test1.empty() << endl;
    for (int i = 30; i < 35; i++)
    {
        cout << "Wstawiam element: " << i << " do listy, czy powodilo sie: " << test1.put(i) << endl;
    }
    cout << "czy kolejka jest pusta: " << test1.empty() << endl;
    cout << "co jest na przedzie kolejki: " << test1.front() << endl;
    cout << "pierwszy element opuszcza kolejkę i jest nim: " << test1.get() << endl;
    cout << "czy kolejka jest pusta: " << test1.empty() << endl;
    cout << "co jest na przedzie kolejki: " << test1.front() << endl;
    cout << "usuwam cala kolejke przy pomocy makenull()" << endl;
    test1.makenull();
    cout << "czy kolejka jest pusta: " << test1.empty() << endl;

    cout << "uzycie get() na pustej kolejce zwroci najmniejsza mozliwa wartosc danego typu, ktora sie nie moze znalezc w kolejce: " << test.get() << endl;
    cout << "test front() dla pustej kolejki: " << test1.front() << endl;

    for (int i = 1; i < 13; i++)
    {
        cout << "Wstawiam element: " << i << " do listy, czy powodilo sie: " << test1.put(i) << endl;
    }
    cout << "czy kolejka jest pusta: " << test1.empty() << endl;
    cout << "co jest na przedzie kolejki: " << test1.front() << endl;
    cout << "pierwszy element opuszcza kolejkę i jest nim: " << test1.get() << endl;
    cout << "co jest na przedzie kolejki: " << test1.front() << endl;
    cout << "usuwam cala kolejke przy pomocy makenull()" << endl;
    test1.makenull();
    cout << "czy kolejka jest pusta: " << test1.empty() << endl;

    return 0;
}
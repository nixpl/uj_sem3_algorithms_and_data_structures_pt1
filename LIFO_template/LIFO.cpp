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
class LIFO
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
    LIFO(){}; // konstruktor

    ~LIFO() // destruktor
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
    bool push(T x) //- dodaje element do kolejki,
    {              // gdy x = MIN nwraca false
        return Insert(x, nullptr);
    }
    T pop() //- pobiera (i usuwa) element z gory stosu,
    {       // gdy stos jest pusty pop() zwraca MIN (element który na pewno nie wystąpi w stosie)
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
    T top() //- zwraca element znajdujący się na gorze stosu (bez usuwania),
    {
        if (!empty())
        {
            return Retrieve(nullptr);
        }
        else
        {
            return MIN;
        }
    }
    void makenull() //- usuwa wszystkie elementy ze stosu,
    {
        clear();
    }
    bool empty() //- sprawdza, czy stos jest pusty.
    {
        // cout << header << endl;
        if (header == nullptr)
            return true;
        else
            return false;
    }
};

template <typename T>
class ArrayLIFO
{
private:
    const T MIN = numeric_limits<T>::lowest(); // oznaczenie pustego pola w tablicy jako najmniejsza mozliwa wartosc z danego typu
    int array_size = 5;
    T *array;
    int currentTop = -1;

public:
    ArrayLIFO()
    {
        ArrayLIFO(this->array_size);
    }
    ArrayLIFO(unsigned int array_size)
    {
        this->array_size = array_size;
        array = new T[this->array_size];
    }
    ~ArrayLIFO()
    {
        delete[] array;
    }

    bool push(T x) //- dodaje element do stosu, jeśli jest miejsce i x > MIN,
    {
        if (x > MIN)
        {
            if (currentTop + 1 < array_size)
            {
                currentTop++;
                array[currentTop] = x;
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
    T pop() //- pobiera (i usuwa) element ze stosu, zwraca MIN gdy stos jest pusty
    {
        if (currentTop >= 0)
        {
            currentTop--;
            return array[currentTop + 1];
        }
        else
            return MIN;
    }
    T top() //- zwraca element znajdujący się na szczycie stosu (bez usuwania),
    {
        if (!empty())
        {
            return array[currentTop];
        }
        else
        {
            return MIN;
        }
    }
    void makenull() //- usuwa wszystkie elementy ze stosu,
    {
        currentTop = -1;
    }
    bool empty() //- sprawdza, czy stos jest pusty.
    {
        return currentTop == -1;
    }
};

int main()
{
    // test LIFO ze wskaznikami ------------------------------------------------------------------------------
    cout << "TEST LIFO ze wskaznikami: " << endl;
    LIFO<int> test;

    cout << "czy stos jest pusty: " << test.empty() << endl;

    cout << "dodaje 10 do stosu" << endl;
    test.push(10);
    cout << "dodaje 20 do stosu" << endl;
    test.push(20);
    cout << "czy stos jest pusty: " << test.empty() << endl;

    cout << "co jest na gorze stosu: " << test.top() << endl;
    cout << "element opuszcza stos i jest nim: " << test.pop() << endl;
    cout << "co jest na gorze stosu: " << test.top() << endl;
    cout << "element opuszcza stos i jest nim: " << test.pop() << endl;
    cout << "czy stos jest pusty: " << test.empty() << endl;

    cout << "uzycie pop() na pustym stosie zwroci najmniejsza mozliwa wartosc danego typu, ktora sie nie moze znalezc w stosie: " << test.pop() << endl;

    for (int i = 1; i < 7; i++)
    {
        test.push(i);
        cout << "dodano element: " << i << " do stosu" << endl;
    }
    cout << "czy stos jest pusty: " << test.empty() << endl;
    cout << "co jest na gorze stosu: " << test.top() << endl;

    cout << "usuwam caly stos przy pomocy makenull()" << endl;
    test.makenull();
    cout << "czy stos jest pusty: " << test.empty() << endl;

    cout << "co jest na gorze stosu: " << test.top() << endl;

    cout << "Test dla pop dla pustego stosu: " << test.pop() << endl
         << endl;

    // test LIFO tablicowego ------------------------------------------------------------------------------

    cout << "TEST LIFO tablicowego: " << endl;
    ArrayLIFO<int> test1(10);

    cout << "czy stos jest pusty: " << test1.empty() << endl;
    for (int i = 30; i < 35; i++)
    {
        cout << "Wstawiam element: " << i << " do stosu, czy powodilo sie: " << test1.push(i) << endl;
    }
    cout << "czy stos jest pusty: " << test1.empty() << endl;
    cout << "co jest na gorze stosu: " << test1.top() << endl;
    cout << "element opuszcza stos i jest nim: " << test1.pop() << endl;
    cout << "czy stos jest pusty: " << test1.empty() << endl;
    cout << "co jest na gorze stosu: " << test1.top() << endl;
    cout << "usuwam caly stos przy pomocy makenull()" << endl;
    test1.makenull();
    cout << "czy stos jest pusty: " << test1.empty() << endl;

    cout << "uzycie pop() na pustym stosie zwroci najmniejsza mozliwa wartosc danego typu, ktora sie nie moze znalezc w stosie: " << test.pop() << endl;

    for (int i = 1; i < 13; i++)
    {
        cout << "Wstawiam element: " << i << " do stosu, czy powodilo sie: " << test1.push(i) << endl;
    }
    for (int i = 1; i < 13; i++)
    {
        cout << "usuwam element ze szczytu i jest nim: " << test1.pop() << endl;
    }
    cout << "czy stos jest pusty: " << test1.empty() << endl;
    cout << "co jest na gorze stosu: " << test1.top() << endl;
    cout << "element opuszcza stos i jest nim: " << test1.pop() << endl;

    return 0;
}

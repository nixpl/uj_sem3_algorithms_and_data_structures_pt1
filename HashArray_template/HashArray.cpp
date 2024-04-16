#include <iostream>
#include <limits>

using namespace std;

template <typename T>
struct hashPair
{
    int key;
    T value;
    bool empty = true;
    bool removed = false;
};

template <typename T>
class HashArray
{
protected:
    int arraySize{};
    int freeSpace{};
    hashPair<T> *hashArray;

    //---flagi-programowe----------------------------------------------------------------------
    const T MIN = numeric_limits<T>::lowest(); // value nie mogaca wystapic w HashArray (uzywane do informowania w metodzie get() o braku takiego klucza)
    const hashPair<T> defaultPair = {-1, MIN, true, false};
    //-----------------------------------------------------------------------------------------

    int hashMethod(int key) // metoda hashujaca -> zwraca numer indeksu tablicy
    {
        return key % arraySize;
    }

    int findIndexOfFreeCell(int key) // zwraca -1 gdy nie znajdzie wolnego miejsca, zwraca -2 gdy juz jest taki klucz, -3 gdy nie ma miejsca na nowy element
    {                                // metoda iteruje po tablicy zaczynając od indexu wyznaczonego przez hashMethod(kedy), gdy znajdzie puste miejsce to zwraca jego indeks,
        if (isFull())                // w przeciwnym wypadku patrzy czy juz nie ma takiego elementu, jesli to nie ten sam element to wyznacza nowy indeks przy pomocy
            return -3;               // hashMethod(keyCopy++), maksymalna ilosc iteracji to rozmiar tablicy, jesli petla sie rozpoczela to znaczy ze tablica nie jest pelna
                                     // i na pewno funkcja znajdzie wolne miejsce
        int keyCopy = key;
        int searchedIndex = hashMethod(key);

        for (int i = 0; i < arraySize; i++)
        {
            if (hashArray[searchedIndex].empty == true)
                break;

            if (hashArray[searchedIndex].key == key)
                return -2;

            searchedIndex = hashMethod(keyCopy++);
        }

        return searchedIndex;
    }

    int findKeyIndex(int key) // zwraca indeks szukanego klucza, gdy nie ma pary z takim kluczem zwroci -1
    {                         // funkcja szuka indeksu szukanego elementu o kluczu key, iteracja zaczy na sie od pozycji hashMethod(key), jesli w tablicy na aktualnym indeksie jest element
        int keyCopy = key;    // o parametrach empty = true i removed = false. Oznacza to ze nie warto juz dalej szukac bo na tym polu jeszcze nie powstal node od momentu calkowicie pustej tablicy. Oznacza to koniec mozliwego przesunieca indeksu elementu o kluczu key
        int searchedIndex = hashMethod(key);

        for (int i = 0; i < arraySize; i++)
        {
            if (hashArray[searchedIndex].empty == true && hashArray[searchedIndex].removed == false)
                break;

            if (hashArray[searchedIndex].key == key)
                return searchedIndex;

            searchedIndex = hashMethod(keyCopy++);
        }

        return -1;
    }

public:
    HashArray(unsigned int arraySize = 10000) // konstruktor
    {
        this->arraySize = arraySize;
        freeSpace = arraySize;
        this->hashArray = new hashPair<T>[arraySize]
        { defaultPair };
    }

    ~HashArray() // destruktor
    {
        delete[] hashArray;
    }

    int put(unsigned int key, T value) // zwraca 1 gdy operacja jest udana ,-1 gdy nie znajdzie wolnego miejsca, -2 gdy juz jest taki klucz, -3 gdy nie ma miejsca na dodanie nowego elementu , -4 proba dodania MIN, ktorego nie mozna dodac (uzywane do informowania w metodzie get() o braku takiego klucza)
    {
        if (value == MIN)
            return -4;

        int index = findIndexOfFreeCell(key);
        if (index < 0)
            return index;

        hashArray[index].key = key;
        hashArray[index].value = value;
        hashArray[index].empty = false;
        freeSpace--;
        return 1;
    }

    T get(unsigned int key) // zwraca wartosc z hashPair o podanym kluczu, w przypadku braku pary o takim kluczu zwraca MIN
    {
        int index = findKeyIndex(key);
        if (index < 0)
            return MIN;
        else
            return hashArray[index].value;
    }

    T getMIN() // zwraca wartosc MIN, aby uzytkonik wiedzial jaka ona jest
    {
        return MIN;
    }

    bool remove(unsigned int key)
    {
        int index = findKeyIndex(key);
        if (index < 0)
            return false;

        hashPair<T> removedPair = defaultPair;
        removedPair.removed = true; // zaznaczamy ze na tym polu doszlo do usuwania elementu
        hashArray[index] = removedPair;
        freeSpace++;

        if (isEmpty())
        {
            empty();
        }

        return true;
    }

    void empty()
    {
        delete[] hashArray;
        this->hashArray = new hashPair<T>[arraySize]
        { defaultPair };

        freeSpace = arraySize;
    }

    bool isFull()
    {
        if (freeSpace <= 0)
            return true;
        else
            return false;
    }

    bool isEmpty()
    {
        if (freeSpace == arraySize)
            return true;
        else
            return false;
    }

    void print()
    {
        if (isEmpty())
            cout << "this array is empty" << endl;
        for (int i = 0; i < arraySize; i++)
        {
            if (hashArray[i].empty == false)
                cout << "[key: " << hashArray[i].key << ", value: " << hashArray[i].value << "]" << endl;
        }
    }
};

int main()
{
    HashArray<int> test(3);
    cout << "test.print():" << endl;
    test.print();
    cout << endl;

    cout << "test.isFull(): " << test.isFull() << endl;

    cout << "test.isEmpty(): " << test.isEmpty() << endl;

    cout << "test.put(31, 10): " << test.put(31, 10) << endl;

    cout << "test.print():" << endl;
    test.print();
    cout << endl;

    cout << "test.put(31, 200): " << test.put(31, 200) << endl;

    cout << "test.put(1, 350): " << test.put(1, 350) << endl;

    cout << "test.print():" << endl;
    test.print();
    cout << endl;

    cout << "test.put(5, 76): " << test.put(5, 76) << endl;

    cout << "test.put(9, 80): " << test.put(9, 80) << endl;

    cout << "test.isFull(): " << test.isFull() << endl;

    cout << "test.print():" << endl;
    test.print();
    cout << endl;

    cout << "test.get(16785):" << test.get(16785) << endl;

    cout << "test.get(31):" << test.get(31) << endl;

    cout << "test.get(1):" << test.get(1) << endl;

    cout << "test.remove(31): " << test.remove(31) << endl;

    cout << "test.print():" << endl;
    test.print();
    cout << endl;

    cout << "test.get(31):" << test.get(31) << endl;

    cout << "test.isFull(): " << test.isFull() << endl;

    cout << "test.remove(5): " << test.remove(5) << endl;

    cout << "test.print():" << endl;
    test.print();
    cout << endl;

    cout << "test.remove(1): " << test.remove(1) << endl;

    cout << "test.print():" << endl;
    test.print();
    cout << endl;

    cout << "test.isEmpty(): " << test.isEmpty() << endl;

    cout << "test.put(99, 21): " << test.put(99, 21) << endl;

    cout << "test.put(99, 21): " << test.put(99, 21) << endl;

    cout << "test.put(56, 38): " << test.put(56, 38) << endl;

    cout << "test.put(322, 11): " << test.put(322, 11) << endl;

    cout << "test.put(800, -10): " << test.put(800, -10) << endl;

    cout << "test.print():" << endl;
    test.print();
    cout << endl;

    cout << "test.empty()" << endl;
    test.empty();

    cout << "test.isEmpty(): " << test.isEmpty() << endl;
    cout << "test.print():" << endl;
    test.print();
    cout << endl;

    cout << "test.put(800, -10): " << test.put(800, -10) << endl;

    cout << "test.print():" << endl;
    test.print();
    cout << endl;

    return 0;
}

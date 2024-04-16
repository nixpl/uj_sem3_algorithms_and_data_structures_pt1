#include <iostream>
#include <limits>

using namespace std;

template <typename T>
class ArrayList
{
private:
    const T MIN = numeric_limits<T>::lowest(); // oznaczenie pustego pola w tablicy jako najmniejsza mozliwa wartosc z danego typu
    int array_size{100000};
    T *array;
    int last{-1}; // trzyma indeks tablicowy ostatniego elementu w liscie

    bool R_Shift_array_from(int p)
    {
        if (p >= 0 && last + 1 < array_size)
        {
            for (int i = last; i >= p; i--)
                array[i + 1] = array[i];
            last++;
            return true;
        }
        else
            return false;
    }

public:
    ArrayList()
    {
        array = new T[this->array_size];

        for (int i = 0; i < array_size; i++)
            array[i] = MIN;
    }

    ArrayList(unsigned int array_size)
    {
        this->array_size = array_size;
        array = new T[array_size];

        for (int i = 0; i < this->array_size; i++)
            array[i] = MIN;
    }

    int First()
    {
        if (last == -1)
            return -1;
        else
            return 0;
    }

    int END()
    {
        return last + 1;
    }

    int Next(int p)
    {
        if (p + 1 <= last && p >= 0)
            return p + 1;
        else
            return -1;
    }

    int Previous(int p)
    {
        if (p - 1 >= 0 && array[p - 1] != MIN)
            return p - 1;
        else
            return -1;
    }

    bool Insert(int x, int p)
    {
        if (x <= MIN)
            return false;

        if (p >= 0 && p <= last)
        {
            if (R_Shift_array_from(p))
            {
                array[p] = x;
                return true;
            }
        }
        if (p > last && p < array_size)
        {
            array[++last] = x;
            return true;
        }
        return false;
    }

    bool Delete(int p)
    {
        if (p >= 0 && p <= last)
        {
            for (int i = p; i < last; i++)
                array[i] = array[i + 1];

            array[last] = MIN;
            last--;
            return true;
        }
        else
            return false;
    }

    int Locate(int x)
    {
        for (int i = 0; i <= last; i++)
            if (array[i] == x)
                return i;
        return END();
    }

    T Retrive(int p)
    {
        if (p >= 0 && p <= last)
            return array[p];
        else
            return MIN;
    }

    bool Remove_duplicates()
    {
        if (last == -1)
            return false;

        for (int i = 0; i <= last; i++)
            for (int k = i + 1; k <= last; k++)
                if (array[i] == array[k])
                    Delete(k);
        return true;
    }

    void Write_out() // dodatkowa metoda wyswietlajaca w konsoli stan listy
    {
        if (last == -1)
        {
            cout << "List is EMPTY" << endl;
        }

        for (int i = 0; i <= last; i++)
            cout << "position[" << i << "] contains " << array[i] << endl;
        cout << endl;
    }

    ~ArrayList()
    {
        delete[] array;
    }
};

int main()
{
    ArrayList<double> list; // tworzę obiekt z klasy "ArrayList" w tym przypadku akurat typu double

    cout << "Test bledu First(): " << list.First() << endl;

    for (int i = 0; i < 20; i++) // wypelnienie poczatkowe listy na potrzeby testow
        list.Insert(i % 12, i);

    cout << "Lista poczotkowa: " << endl;
    list.Write_out();

    //-----------------------------------------------------
    cout << "Test Insert(1000, 9)" << endl;
    list.Insert(1000, 9);
    list.Write_out();

    //-----------------------------------------------------
    cout << "Test Insert(2000, 40)" << endl;
    list.Insert(2000, 40);
    list.Write_out();

    //-----------------------------------------------------
    cout << "Test bledu Insert(5000, -1)" << endl;
    list.Insert(5000, -1);
    list.Write_out();

    //-----------------------------------------------------
    cout << "Test First(): " << list.First() << endl;
    cout << "Test END(): " << list.END() << endl;
    cout << "Test Next(7): " << list.Next(7) << endl;
    cout << "Test bledu Next(21): " << list.Next(21) << endl;
    cout << "Test Previous(18): " << list.Previous(18) << endl;
    cout << "Test bledu Previous(0): " << list.Previous(0) << endl
         << endl;

    //-----------------------------------------------------
    cout << "Test Delete(4)" << endl;
    list.Delete(4);
    list.Write_out();

    //-----------------------------------------------------
    cout << "Test bledu Delete(30)" << endl;
    list.Delete(30);
    list.Write_out();

    //-----------------------------------------------------
    cout << "Test Locate(6): " << list.Locate(6) << endl;
    cout << "Test bledu Locate(20000): " << list.Locate(20000) << endl;

    //-----------------------------------------------------
    cout << "Test Retrive(8): " << list.Retrive(8) << endl;
    cout << "Test bledu Retrive(300): " << list.Retrive(300) << endl
         << endl;

    //-----------------------------------------------------
    cout << "Test Remove_duplicates()" << endl;
    list.Remove_duplicates();
    list.Write_out();

    return 0;
}

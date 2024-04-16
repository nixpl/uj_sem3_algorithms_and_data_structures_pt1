#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <random>
#include <chrono>
#include <type_traits>

using namespace std;

//--Klasa-KDTree--------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class KDTree
{
private:
    struct Node
    {
        vector<T> point;
        Node *left = nullptr;
        Node *right = nullptr;
    };

    Node *root = nullptr;
    int numberOfDimensions = 0;

    void quickSort(vector<vector<T>> &points, int axisIndex, int firstIndex, int lastIndex)
    {
        if (firstIndex < lastIndex)
        {
            int i = firstIndex;
            for (int j = firstIndex; j < lastIndex; j++)
            {
                if (points.at(j).at(axisIndex) <= points.at(lastIndex).at(axisIndex))
                {
                    if (j > i)
                    {
                        swap(points.at(i), points.at(j));
                    }
                    i++;
                }
            }
            swap(points.at(i), points.at(lastIndex));

            int pivotIndex = i;
            quickSort(points, axisIndex, firstIndex, pivotIndex - 1);
            quickSort(points, axisIndex, pivotIndex + 1, lastIndex);
        }
    }

    void createKDTree(vector<vector<T>> &points, Node *&root, int axisIndex, int firstIndex, int lastIndex)
    {
        if (firstIndex > lastIndex)
        {
            return;
        }

        root = new Node;

        quickSort(points, axisIndex, firstIndex, lastIndex);
        int indexOfMiddlePoint = (firstIndex + lastIndex) / 2;
        root->point = points.at(indexOfMiddlePoint);

        int newAxisIndex = (axisIndex + 1) % numberOfDimensions;

        createKDTree(points, root->left, newAxisIndex, firstIndex, indexOfMiddlePoint - 1);

        createKDTree(points, root->right, newAxisIndex, indexOfMiddlePoint + 1, lastIndex);
    }

    void deleteKDTree(Node *&root)
    {
        if (root != nullptr)
        {
            deleteKDTree(root->left);
            deleteKDTree(root->right);
            delete root;
            root = nullptr;
        }
    }

    Node *findNearestPoint(vector<T> &point, Node *root, int axisIndex = 0)
    {
        if (root == nullptr)
        {
            return root;
        }
        int newAxisIndex = (axisIndex + 1) % numberOfDimensions;

        Node *nextBranch = root->right;
        Node *otherBranch = root->left;
        if (point.at(axisIndex) < root->point.at(axisIndex))
        {
            nextBranch = root->left;
            otherBranch = root->right;
        }

        T rootDistanceSquared = getDistanceSquared(point, root->point);

        Node *bestNode = root;
        T distanceSquared = rootDistanceSquared;

        Node *fromNextBranchSubTree = findNearestPoint(point, nextBranch, newAxisIndex);
        if (fromNextBranchSubTree != nullptr)
        {
            T fromNextBranchSubTreeDistanceSquared = getDistanceSquared(point, fromNextBranchSubTree->point);

            if (fromNextBranchSubTreeDistanceSquared < rootDistanceSquared)
            {
                bestNode = fromNextBranchSubTree;
                distanceSquared = fromNextBranchSubTreeDistanceSquared;
            }
        }

        T axisDistanceSquared = pow(point.at(axisIndex) - root->point.at(axisIndex), 2); // obliczam kwadrat odleglosci punktu od osi podzialu tworzacej przez root bo jesli ta wartosc jest mniejsza badz rowna niz distanceSquared to po drugiej stronie moze kryc sie lepszy Node
        if (distanceSquared >= axisDistanceSquared)
        {
            Node *fromOtherBranchSubTree = findNearestPoint(point, otherBranch, newAxisIndex);
            if (fromOtherBranchSubTree != nullptr)
            {
                T fromOtherBranchSubTreeDistanceSquared = getDistanceSquared(point, fromOtherBranchSubTree->point);
                if (fromOtherBranchSubTreeDistanceSquared < distanceSquared)
                {
                    bestNode = fromOtherBranchSubTree;
                    distanceSquared = fromOtherBranchSubTreeDistanceSquared; // ta operacja jest nie potrzebna ale zdecydowalem sie ja zamiescic dla spojnosci kodu
                }
            }
        }

        return bestNode;
    }

    int addPoint(vector<T> &point, Node *&root, int axisIndex = 0)
    {
        if (root == nullptr)
        {
            root = new Node;
            root->point = point;
            return 1;
        }
        if (point == root->point)
        {
            return -2;
        }

        int newAxisIndex = (axisIndex + 1) % numberOfDimensions;
        if (point.at(axisIndex) < root->point.at(axisIndex))
        {
            return addPoint(point, root->left, newAxisIndex);
        }
        else
        {

            return addPoint(point, root->right, newAxisIndex);
        }
    }

    void inorder(vector<vector<T>> &points, Node *root) const
    {
        if (root == nullptr)
        {
            return;
        }

        inorder(points, root->left);
        points.push_back(root->point);
        inorder(points, root->right);
    }

public:
    KDTree(unsigned int numberOfDimensions) : numberOfDimensions(static_cast<int>(numberOfDimensions)) {}

    KDTree(vector<vector<T>> points)
    {
        if (!points.empty())
        {
            numberOfDimensions = points.at(0).size();
            int axisIndex = 0;
            int firstIndex = 0;
            int lastIndex = points.size() - 1;

            createKDTree(points, root, axisIndex, firstIndex, lastIndex);
        }
    }

    T getDistanceSquared(vector<T> pointA, vector<T> pointB)
    {
        T distanceSquared = 0;

        for (int i = 0; i < numberOfDimensions; i++)
        {
            distanceSquared += pow(pointA.at(i) - pointB.at(i), 2);
        }

        return distanceSquared;
    }

    vector<T> findNearestPoint(vector<T> point) // gdy nie znajdzie najblizszego punktu lub punkt ma inna liczbe wymiarow niz punkty w drzewie to zwróci pusty wektor
    {
        vector<T> nearestPoint;
        if (numberOfDimensions == 0)
            return nearestPoint;

        if (root != nullptr && static_cast<int>(point.size()) == numberOfDimensions)
            nearestPoint = findNearestPoint(point, root)->point;

        return nearestPoint;
    }

    int addPoint(vector<T> point) // zwraca informacje czy udalo sie dodac element. 1-> gdy udalo sie dodac, -1 gdy dodawany punkt ma inna liczbe wymiarow niz oczekiwana, -2 gdy juz istnieje taki punkt wiec nie zostaje on dodany
    {
        if (static_cast<int>(point.size()) != numberOfDimensions)
        {
            return -1;
        }

        return addPoint(point, root);
    }

    void rebuild()
    {
        vector<vector<T>> points;
        inorder(points, root);

        deleteKDTree(root);

        int axisIndex = 0;
        int firstIndex = 0;
        int lastIndex = points.size() - 1;
        createKDTree(points, root, axisIndex, firstIndex, lastIndex);
    }

    int getNumberOfDimensions() const
    {
        return numberOfDimensions;
    }

    vector<vector<T>> getPoints() const
    {
        vector<vector<T>> points;
        inorder(points, root);
        return points;
    }

    ~KDTree()
    {
        deleteKDTree(root);
    }
};

//--Programy-Pomocnicze--------------------------------------------------------------------------------------------------------------------------------

template <typename T>
void printPoint(const vector<T> &point)
{
    int numberOfDimensions = point.size();
    cout << "{";
    for (int i = 0; i < numberOfDimensions - 1; i++)
        cout << point.at(i) << ", ";
    if (!point.empty())
        cout << point.back();
    cout << "}";
}

template <typename T>
void printPoints(const vector<vector<T>> &points)
{
    int numberOfPoints = points.size();
    cout << "{";
    for (int i = 0; i < numberOfPoints - 1; i++)
    {
        printPoint(points.at(i));
        cout << ", ";
    }
    printPoint(points.back());
    cout << "}";
}

//--Programy-do-przeprowadzania-testow---------------------------------------------------------------------------------------------------------
template <typename T>
bool testerKDTreeFindNearestPoint(KDTree<T> &testObject, const vector<T> &point)
{
    vector<vector<T>> points = testObject.getPoints();

    if (points.empty())
    {
        cout << "there are no points" << endl;
        return false;
    }
    if (static_cast<int>(point.size()) != testObject.getNumberOfDimensions())
    {
        cout << "missing coordinates for chosen point" << endl;
        return false;
    }

    cout << "Checking nearest point for ";
    printPoint(point);
    cout << ":" << endl;

    vector<T> nearestPointBruteForce = points.front();
    T minimalDistanceSquared = testObject.getDistanceSquared(point, nearestPointBruteForce);
    for (auto pt : points)
    {
        T newDistanceSquared = testObject.getDistanceSquared(point, pt);
        if (newDistanceSquared < minimalDistanceSquared)
        {
            nearestPointBruteForce = pt;
            minimalDistanceSquared = newDistanceSquared;
        }
    }

    vector<T> nearestPointKDTree = testObject.findNearestPoint(point);

    cout << "realNearestPoint =?= KDTree.findNearestPoint(): ";

    bool passedTest = minimalDistanceSquared == testObject.getDistanceSquared(point, nearestPointKDTree) ? true : false;
    if (passedTest)
    {
        if (nearestPointKDTree == nearestPointBruteForce)
        {
            cout << "same point -> ";
            printPoint(nearestPointKDTree);
            cout << endl;
        }
        else
        {
            cout << "same distanceSquared (<<" << minimalDistanceSquared << ") for points -> ";
            printPoint(nearestPointBruteForce);
            cout << ", ";
            printPoint(nearestPointKDTree);
            cout << endl;
        }
    }
    else
    {
        cout << "different distance for points -> ";
        printPoint(nearestPointBruteForce);
        cout << " with distanceSquared: " << minimalDistanceSquared << ", ";
        printPoint(nearestPointKDTree);
        cout << " with distanceSquared: " << testObject.getDistanceSquared(point, nearestPointKDTree) << endl;

        cout << "points: ";
        printPoints(points);
        cout << endl;
    }

    return passedTest;
}

template <typename T>
T generateRandom(T maxValue)
{
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 generator(static_cast<unsigned int>(seed));

    if constexpr (is_same<T, int>::value)
    {
        uniform_int_distribution<int> distribution(1, static_cast<int>(maxValue));
        return distribution(generator);
    }
    else
    {
        uniform_real_distribution<T> distribution(-maxValue, maxValue);
        return distribution(generator);
    }
}

template <typename T>
vector<T> generateRandomPoint(int numberOfDimensions, T maxDistance)
{
    vector<T> point;
    for (int i = 0; i < numberOfDimensions; i++)
    {
        point.push_back(generateRandom<T>(maxDistance));
    }
    return point;
}

template <typename T>
vector<vector<T>> generateVectorOfRandomPoints(int numberOfPoints, int numberOfDimensions, T maxDistanceForPoints)
{
    vector<vector<T>> points;
    for (int j = 0; j < numberOfPoints; j++)
    {
        points.push_back(generateRandomPoint<T>(numberOfDimensions, maxDistanceForPoints));
    }
    return points;
}

template <typename T>
bool automaticTester(int numberOfTests, int maxNumofDimensions, int maxNumofPoints, T maxDistanceForPoints)
{
    bool passedTest = true;
    int passedTestScore = 0;
    cout << "Automatic tests for finding nearest point:" << endl;

    for (int i = 0; i < numberOfTests; i++)
    {
        int numberOfDimensions = generateRandom<int>(maxNumofDimensions);
        int numberOfPoints = generateRandom<int>(maxNumofPoints);

        vector<vector<T>> points = generateVectorOfRandomPoints(numberOfPoints, numberOfDimensions, maxDistanceForPoints);

        KDTree<T> testTree(points);
        cout << "test " << i + 1 << "/" << numberOfTests << ":" << endl;
        if (!testerKDTreeFindNearestPoint(testTree, generateRandomPoint<T>(numberOfDimensions, maxDistanceForPoints)))
        {
            passedTest = false;
            // break;
        }
        else
        {
            passedTestScore++;
        }

        cout << endl
             << endl;
    }
    cout << "Automatic testing finished with score " << passedTestScore << "/" << numberOfTests << " passed tests for finding the nearest point" << endl;

    return passedTest;
}

//--Main---------------------------------------------------------------------------------------------------------------------------------------

int main()
{
    // automaticTester<double>(5000, 20, 50, 50);

    cout << "start testu zerowego wymiaru" << endl;

    KDTree<double> zeroTree(0);

    cout << zeroTree.addPoint({}) << endl;

    printPoint(zeroTree.findNearestPoint({}));
    cout << endl;

    printPoints(zeroTree.getPoints());
    cout << endl;

    cout << "koniec testu zerowego wymiaru" << endl
         << endl;

    vector<vector<double>> points = generateVectorOfRandomPoints<double>(10, 3, 50); // ile pumnktow maksymalnie moze wygenerowac, ile wymiarow, jaka maksymalna odleglosc od srodka ukladu
    KDTree<double> testTree(points);

    cout << "testTree.getPoints():" << endl;
    printPoints<double>(testTree.getPoints());
    cout << endl
         << endl;

    vector<double> point = generateRandomPoint<double>(3, 50); // ile wymiarow, jaka maksymalna odleglosc
    cout << "testTree.addPoint(";
    printPoint(point);
    cout << ")" << endl;
    cout << "return value: " << testTree.addPoint(point) << endl;

    cout << "testTree.addPoint(";
    printPoint(point);
    cout << ")" << endl;
    cout << "return value: " << testTree.addPoint(point) << endl;

    cout << "testTree.getPoints():" << endl;
    printPoints<double>(testTree.getPoints());
    cout << endl
         << endl;

    cout << "testTree.findNearestPoint(";
    printPoint(point);
    cout << "):" << endl;
    printPoint(testTree.findNearestPoint(point));
    cout << endl
         << endl;

    cout << "testTree.rebuild():" << endl;
    testTree.rebuild();

    cout << "testTree.getPoints():" << endl;
    printPoints<double>(testTree.getPoints());
    cout << endl
         << endl;

    cout << "testTree.findNearestPoint(";
    printPoint(point);
    cout << "):" << endl;
    printPoint(testTree.findNearestPoint(point));
    cout << endl
         << endl;

    return 0;
}
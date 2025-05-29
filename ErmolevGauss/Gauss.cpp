#include <iostream>
#include <stdlib.h>
#include <time.h>

/*

things to test
copying a vector with different types

*/

template <typename T> class Vector
{
private:
    T* objects;
    size_t size;
public:
    Vector(size_t n = 0)
    {
        size = n;
        if (size < 0) throw std::invalid_argument("vector size must be positive");
        else if (size == 0)
        {
            objects = nullptr;
        }
        else
        {
            objects = new T[n]();
        }
    }
    
    ~Vector()
    {
        delete[] objects;
    }

    Vector(const Vector<T>& v)
    {
        size = v.length();
        if (size == 0)
        {
            objects = nullptr;
        }
        else
        {
            objects = new T[size]();
            for (int i = 0; i < size; i++) objects[i] = v[i];
        }
    }

    size_t length() const
    {
        return size;
    }

    void swap(int a, int b)
    {
        std::swap(objects[a], objects[b]);
    }

//needed operations: * / *= /= scalar, + - += -= = vector, [],

    T& operator[] (int index) const
    {
        if(index<0 ||index>=length()) throw std::invalid_argument("invalid index");
        return objects[index];
    }

    void operator+= (const Vector<T>& v)
    {
        if(v.size!=size) throw std::invalid_argument("vectors are of different sizes");
        for (int i = 0; i < size; i++)
        {
            objects[i] += v.objects[i];
        }
    }

    void operator-= (const Vector<T>& v)
    {
        if (v.size != size) throw std::invalid_argument("vectors are of different sizes");
        for (int i = 0; i < size; i++)
        {
            objects[i] -= v.objects[i];
        }
    }

    Vector<T> operator+ (const Vector<T>& v)
    {
        Vector<T> ret(*this);
        ret += v;
        return ret;
    }

    Vector<T> operator- (const Vector<T>& v)
    {
        Vector<T> ret(*this);
        ret -= v;
        return ret;
    }

    Vector<T>& operator=(const Vector<T>& v)
    {
        if (this == &v) return *this;
        delete[] objects;
        size = v.length();
        if (size == 0) {
            objects = nullptr;
        }
        else {
            objects = new T[size];
            for (size_t i = 0; i < size; ++i)
                objects[i] = v.objects[i];
        }
        return *this;
    }

    void operator*= (T val)
    {
        for (int i = 0; i < size; i++)
        {
            objects[i] *= val;
        }
    }

    void operator/= (T val)
    {
        if(val == 0) throw std::invalid_argument("can't divide by 0");
        for (int i = 0; i < size; i++)
        {
            objects[i] /= val;
        }
    }

    Vector<T> operator* (T val)
    {
        Vector<T> ret(*this);
        ret *= val;
        return ret;
    }

    Vector<T> operator/ (T val)
    {
        Vector<T> ret(*this);
        ret /= val;
        return ret;
    }

};

template <typename T> class Matrix : public Vector<Vector<T>>
{
protected:
    size_t columns;
public:
    Matrix(size_t rows, size_t col): Vector<Vector<T>>(rows)
    {
        if (col <= 0) { throw std::invalid_argument("can't have <=0 columns"); }
        columns = col;
        for (int i = 0; i < rows; i++)
        {
            (*this)[i] = Vector<T>(columns);
        }
    }
};

template <typename T> void Gauss(Vector<T>& b, Matrix<T>& A)
{
    size_t m = A.length();
    size_t n = A[0].length();

    if (b.length() != m)
    {
        throw std::invalid_argument("b's count doesn't match A's row count");
    }

    size_t lead = 0;
    size_t currow = 0;
    while (currow < m)
    {
        if (lead >= n) { break; }

        size_t i = currow;
        while (i < m && A[i][lead] == T(0)) { i++; }
        if (i == m)
        {
            lead++;
            continue;
        }
        if (i!=currow)
        {
            A.swap(i, currow);
            b.swap(i, currow);
        }

        T div = A[currow][lead];
        A[currow] /= div;
        b[currow] /= div;

        for (int k = 0; k < m; k++)
        {
            if (k == currow || A[k][lead] == 0) { continue; }
            T factor = A[k][lead];
            A[k]-= (A[currow]* factor);
            A[k][lead] = 0;
            b[k] -= factor * b[currow];
        }
        lead++;
        currow++;
    }

    bool complete = true;
    int dim = 0;
    for (int i = 0; i < m; i++)
    {
        bool allZero = true;
        for (int j = 0; j < n; j++)
        {
            if (A[i][j] != T(0) && !allZero) { complete = false; }
            if (A[i][j] != T(0)) { allZero = false; }
        }
        if (allZero && b[i] != T(0)) { throw std::runtime_error("system has no solution"); }
        if (!allZero) dim++;
    }
    if (n == m) std::cout << "\n\n\nDimension is: " << dim;
    printMat(A, b);
}

template <typename T> void printMat(Matrix<T>& A, Vector<T>& b)
{
    std::cout << "\n";
    for (int i = 0; i < A.length(); i++)
    {
        for (int j = 0; j < A[0].length(); j++)
        {
            std::cout << A[i][j] << " ";
        }
        std::cout << "| " << b[i];
        std::cout << "\n";
    }
}

int main()
{
    srand(time(NULL));
    int choice;
    int n, m;
    std::cout << "input the amount of rows and columns: ";
    std::cin >> m >> n;
    Matrix<double> A(m, n);
    Vector<double> b(m);
    std::cout << "\n1. Random Matrix \n2. Input Matrix.\n";

    
    
    std::cin >> choice;
    if (choice == 1)
    {
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                A[i][j] = rand() % 2000 - 1000;
            }
            b[i] = rand() % 2000 - 1000;
        }
    }
    else
    {
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                std::cin >> A[i][j];
            }
        }
        std::cout << "\n\ninput vector b:" << "\n\n";
        for (int i = 0; i < m; i++)
        {
            std::cin >> b[i];
        }
    }
    printMat(A, b);
    Gauss(b, A);
    std::cin >> choice;
    return 1;
}
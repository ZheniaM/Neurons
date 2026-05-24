#include <iostream>
#include <string>

struct B
{
    std::size_t size;
    int *data;
    B(std::initializer_list<int> l)
        : data(new int[l.size()]), size(l.size())
    {
        printf("B(list)\n");
        std::size_t i = 0;
        for (auto x = l.begin(); x != l.end(); ++x)
        {
            data[++i] = *x;
        }
    }

    B(B const &b) : size(b.size), data(new int[b.size])
    {
        printf("B(&)\n");
        for (std::size_t i = 0; i < size; ++i)
        {
            data[i] = b.data[i];
        }
    }

    B(B &&b) : size(b.size) 
    {
        printf("B(&&)\n");
        data = b.data;
        b.size = 0;
        b.data = nullptr;
    }

    ~B()
    {
        delete[] data;
    }

    B &operator=(B &&b)
    {
        printf("operator=(&&)\n");
        if (this == &b)
            return *this;

        this->~B();
        data = b.data;
        b.size = 0;
        b.data = nullptr;
        return *this;
    }

    B &operator=(B const &b)
    {
        printf("operator=(&)\n");
        if (this == &b)
            return *this;

        this->~B();
        size = b.size;
        for (std::size_t i = 0; i < size; ++i)
        {
            data[i] = b.data[i];
        }
        return *this;

    }
};

std::ostream &operator<<(std::ostream &os, B const &b)
{
    os << "{";
    for (std::size_t i = 0; i + 1 < b.size; ++i)
    {
        os << b.data[i];
        os << ", ";
    }
    os << b.data[b.size - 1];
    os << "}";
    return os;
}

struct A
{
    int a;
    B b;
    double c;
};

int main(void)
{
    A AA{123, {1, 2, 3}, 0.123};
    auto &&[aa, bb, cc] = AA;
    std::cout << aa << " " << bb << " " << cc << std::endl;
    std::cout << AA.a << " " << AA.b << " " << AA.c << std::endl;
}
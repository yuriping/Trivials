#include <iostream>

using namespace std;

class A {
public:
    void Set(int value)
    {
        a = value;
    }
    void Print()
    {
        cout << a << endl;
    }
private:
    int a;
};

void fun(A & ra, int value)
{
    ra.Set(value);
}

int main(void)
{
    A a;
    
    fun(a, 3);

    a.Print();

    return 0;
}



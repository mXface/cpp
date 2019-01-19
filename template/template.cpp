//"template <class T>" or "template <typename T>";

#include <iostream>

using namespace std;

//template function example
template <class T>
T getSmaller(T a, T b) {
    return a < b ? a : b;
}

//class template example
template <class T>
class Calculator {
public:
    T add(T n1, T n2) {
        return n1 + n2;
    }

    T subtract(T n1, T n2) {
        return n1 - n2;
    }

    T multiply(T n1, T n2) {
        return n1 * n2;
    }
};

//template specialization
template <class T>
class Cipher {
public:
    T encode(T n1) {
        return 2 * n1;
    }
};

template <>
class Cipher<char> {
public:
    char encode(char c) {
        char newCode = c + 2;
        if (newCode > 'z') //start again
            newCode -= 26;
        return newCode;
    }
};

//Template Metaprogramming (TMP) example
//It is interesting concept, It does computation in compile time
template<int n>
class Fibonacci {
public:
    static int getValue() {
        return Fibonacci<n-1>::getValue() + Fibonacci<n-2>::getValue();
    }
};

template<>
class Fibonacci<1> {
public:
    static int getValue() {
        return 1;
    }
};

template<>
class Fibonacci<0>
{
public:
    static int getValue() {
        return 0;
    }
};

int main()
{
    //Function template example
    int x = 8, y = 6;
    cout << "Smaller is " << getSmaller(x, y) << endl;
    string s1 = "Earth", s2 = "Saturn";
    cout << "Smaller is " << getSmaller(s1, s2) << endl;

    //Class template example
    Calculator<int> integerCalculator;
    Calculator<double> doubleCalculator;
    cout << "3 plus 5 is equal to " << integerCalculator.add(3,5) << endl;
    cout << "2.1 times 2.0 is equal to " << doubleCalculator.multiply(2.1, 2.0) << endl;

    //Template specialization example
    Cipher<int> integerCipher;
    Cipher<char> characterCipher;
    cout << "Encoded value of 3 is " << integerCipher.encode(3) << endl;
    cout << "Encoded value of 'b' is " << characterCipher.encode('b') << endl;

    //Template Metaprogramming (TMP) example
    cout << Fibonacci<8>::getValue() << endl;

    return 0;
}
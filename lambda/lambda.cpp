#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

//Syntax for the lambda expressions
//[captures] (parameters) -> return { statements; }

//basic lambda function with no capture, no variable, no return
void test01() {
    auto lambda = []() { cout << "Basic lambda expression is interesting!" << endl; };
    lambda();
}

//basic lambda function with no capture, two parameters, and return
void test02(string s1, string s2) {
    auto lambda = [](string x, string y) { return x + " " + y; };
    cout << lambda(s1, s2) << endl;
}

//lambda using as a predicate
void test03(vector<string>& words) {
    auto wordCount = count_if(words.begin(), words.end(), [](string s) { return (s.length() > 4); });
    cout << "Number of words: " << wordCount << endl;
}

//capture copy variable and using predicate
void test04(vector<string>& words) {
    int maxSize = 4;
    for_each(words.rbegin(), words.rend(), [maxSize] (string s) {
        if (s.length() > maxSize) {
            cout << s << " ";
        }
    });
    cout << endl;
}

//capture all variables and using predicate
void test05(vector<string>& words) {
    int maxSize = 4;
    for_each(words.rbegin(), words.rend(), [=] (string s) {
        if (s.length() > maxSize) {
            cout << s << " ";
        }
    });
    cout << endl;
}

//capture reference variables
void test06(vector<string>& words) {
    string all = "";
    for_each(words.rbegin(), words.rend(), [&all] (string s) {
        all += (s + " ");
    });
    cout << all << endl;
}

int main() {
    vector<string> words = {"place", "biggest", "the", "is", "world"};
    test01();
    test02(words[1], words[4]);
    test03(words);
    test04(words);
    test05(words);
    test06(words);
}
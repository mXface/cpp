//Smart pointers are crucial for the RAII.
//Pointers can create a lot of problems (dangling pointers, memory leaks), if they are not handled properly
//So smart pointers are implemented to handle these. Header file is <memory> and namespace is std.
//
//auto_ptr=======> It was first implementation of the smart pointers
//              => It is replaced with unique_ptr due to some design flaws
//              => It can not be used with standard containers (vector, map, etc.)
//              => It can not be used with array of objects
//              => It transfers the ownership when it is assigned to another pointer (passing as a function parameter)
//
//shared_ptr=====> It is a reference counted smart pointer, multiple shared pointers can refer to a single object,
//              => and memory is released automatically when latest memory goes out of scope
//
//weak_ptr=======> Provides access to an object that is owned by one or more shared_ptr instances,
//              => but not involve in reference counting. It is useful when want to observe an instance
//
//unique_ptr=====> Replacement for the auto_ptr, allows one owner for the pointer.
//              => It can be moved to a new owner, but not copied or shared.

#include <iostream>
#include <memory>
#include <fstream>

using namespace std;

//Test RAII class for the smart pointers test
class SPTest01 {
private:
    ofstream fileHandler;

public:
    SPTest01(string fileName) {
        fileHandler.open(fileName);
    }

    ~SPTest01() {
        if(fileHandler.is_open()) {
            fileHandler.close();
        }
    }
};

int main()
{
    //auto_ptr example, resource is relinquished after definition goes out of the scope
    auto_ptr<SPTest01> autoPointer(new SPTest01("test01.txt"));

    //shared_ptr example, all pointers point same instance
    //use_count() returns the reference count which shows number of pointers point the same instance
    shared_ptr<SPTest01> sharedPointer1(new SPTest01("test02.txt"));
    cout << "Strong reference count = " << sharedPointer1.use_count() << endl;  //reference count
    shared_ptr<SPTest01> sharedPointer2 = sharedPointer1;
    cout << "Strong reference count = " << sharedPointer1.use_count() << endl;
    shared_ptr<SPTest01> sharedPointer3 = sharedPointer2;
    cout << "Strong reference count = " << sharedPointer3.use_count() << endl; //other pointer also give instance total pointer count

    //weak_ptr example
    weak_ptr<SPTest01> weakPointer = sharedPointer1;
    cout << "Strong reference count = " << weakPointer.use_count() << endl; // doesn't effect shared pointer (strong ref) count, it is kept with weak ref

    //unique_ptr example, resource is relinquished after definition goes out of the scope
    unique_ptr<SPTest01> uniquePointer(new SPTest01("test03.txt"));

    return 0;
}
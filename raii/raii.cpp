//RAII : Resource Acquisition Is Initialization
//- Acquire a resource (file handle, network socket, allocating memory, acquiring lock etc.) in the constructor
//- Instances are allocated on the stack
//- Resource is relinquished in the destructor
//- It is guarantee that the destructor of objects on the stack will be called even if in case of exception

#include <iostream>
#include <fstream>

using namespace std;

class Logger {
private:
    ofstream fileHandler;
public:
    //resource is allocated on the stack in constructor
    Logger(string filename) {
        fileHandler.open(filename);
    }

    //Relinquish resource
    ~Logger() {
        if(fileHandler.is_open()) {
            fileHandler.close();
        }
    }

    void addLog(string log) {
        if(fileHandler.is_open()) {
            fileHandler << log.c_str() << endl;
        }
    }
};

int main() {
    Logger logger("log.txt");
    logger.addLog("app crashed!");
    return 0;
}
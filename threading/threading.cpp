#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <future>

using namespace std;

//basic thread function
void threadTest01(string name) {
    for (int i=0; i<1000; ++i) {
        cout << "Message from " << name << " (id=" << this_thread::get_id() << ") => " << i << endl;
    }
}

//class operator thread
class ThreadTest02 {
public:
    void operator()() {
        for(int i = 0; i < 1000; i++) {
            cout << "Message from thread (id=" << this_thread::get_id() << ") => " << i << endl;
        }
    }
};

//basic producer reducer implementation with mutexes
class Buffer {
private:
    int dataSize;
    mutex dataMutex;
public:
    Buffer() {
        dataSize = 0;
    }

    bool isEmpty() { //it is atomic, no need to lock
        return (dataSize == 0) ? true : false;
    }

    int getSize() {
        return dataSize;
    }

    void produceData(int size) {
        for (int i=0; i<size; ++i) {
            dataMutex.lock();
            dataSize += 1;
            dataMutex.unlock();
            this_thread::sleep_for(chrono::milliseconds(1));
        }
    }

    void reduceData(int size) {
        int count = 0; //never give a chance to size is less than 0
        while (count < size) {
            if (!isEmpty()) {
                dataMutex.lock();
                dataSize -= 1;
                ++count;
                dataMutex.unlock();
            }
            this_thread::sleep_for(chrono::milliseconds(1));
        }
    }
};

//basic producer reducer implementation with mutexes and lock_guard
class LockGuardBuffer {
private:
    int dataSize;
    mutex dataMutex;
public:
    LockGuardBuffer() {
        dataSize = 0;
    }

    bool isEmpty() { //it is atomic, no need to lock
        return (dataSize == 0) ? true : false;
    }

    int getSize() {
        return dataSize;
    }

    void produceData(int size) {
        for (int i=0; i<size; ++i) {
            lock_guard<mutex> lockGuard(dataMutex);
            dataSize += 1;
            this_thread::sleep_for(chrono::milliseconds(1));
        }
    }

    void reduceData(int size) {
        int count = 0; //never give a chance to size is less than 0
        while (count < size) {
            if (!isEmpty()) {
                lock_guard<mutex> lockGuard(dataMutex);
                dataSize -= 1;
                ++count;
            }
            this_thread::sleep_for(chrono::milliseconds(1));
        }
    }
};

//wonderful example for the handshaking between threads
class DataFactory {
private:
    bool ready;
    condition_variable dataCondition;
    mutex dataMutex;
public:
    DataFactory() {
        ready = false;
    }

    bool isReady() {
        return ready;
    }

    void prepareData() {
        cout << "Preparing Data.." << endl;
        this_thread::sleep_for(chrono::milliseconds(3000));
        cout << "Data prepared" << endl;
        lock_guard<mutex> lockGuard(dataMutex);
        ready = true;
        dataCondition.notify_one(); //or notify_all()
    }

    void processData() {
        cout << "Waiting for the data.." << endl;
        unique_lock<mutex> uniqueLock(dataMutex);
        dataCondition.wait(uniqueLock, bind(&DataFactory::isReady, this));
        cout << "Processing Data..." << endl;
        this_thread::sleep_for(chrono::milliseconds(2000));
        cout << "Data Processed!" << endl;
    }
};

//example for the future and promise
void calculate(std::promise<int>* promObj, int x, int y) {
    this_thread::sleep_for(chrono::milliseconds(2000));
    promObj->set_value(x+y);
}

int main() {
    //thread calling with basic function pointer
    thread thread1(threadTest01, "thread");

    for (int i=0; i<1000; ++i) {
        cout << "Message from main (id=" << this_thread::get_id() << ") => " << i << endl;
    }
    thread1.join();

    //thread calling with function objects
    thread thread2((ThreadTest02()));

    for (int i=0; i<1000; ++i) {
        cout << "Message from main (id=" << this_thread::get_id() << ") => " << i << endl;
    }
    thread2.join();

    //thread calling with lambda functions
    thread thread3([]{
        for(int i = 0; i < 1000; i++)
            cout << "Message from thread (id=" << this_thread::get_id() << ") => " << i << endl;
    });
    for(int i = 0; i < 1000; i++) {
        cout << "Message from main (id=" << this_thread::get_id() << ") => " << i << endl;
    }

    if (thread4.joinable()) { // check thread joinable or not
        thread4.join();
    }

    //detach test
    thread thread4(threadTest01, "thread");
    if (thread4.joinable()) { // check thread joinable or not
        //we can not access thread after detaching
        //we create Daemon / Background threads
        thread4.detach();
    }

    //producer and consumer thread test 
    //in that reducer never consumes when no data
    Buffer buffer;
    thread thread5(&Buffer::produceData, &buffer, 1000);
    thread thread6(&Buffer::reduceData, &buffer, 1000);
    thread6.join();
    thread5.join();
    cout << buffer.getSize() << endl;

    //producer and consumer thread test with lock_guard
    //in that reducer never consumes when no data
    LockGuardBuffer lockGuardBuffer;
    thread thread7(&LockGuardBuffer::produceData, &lockGuardBuffer, 1000);
    thread thread8(&LockGuardBuffer::reduceData, &lockGuardBuffer, 1000);
    thread8.join();
    thread7.join();
    cout << lockGuardBuffer.getSize() << endl;

    //condition_variable test
    DataFactory dataFactory;
    thread thread9(&DataFactory::prepareData, &dataFactory);
    thread thread10(&DataFactory::processData, &dataFactory);
    thread10.join();
    thread9.join();

    //future and promise test
    promise<int> promiseObject;
    future<int> futureObject = promiseObject.get_future();
    thread thread11(calculate, &promiseObject, 12, 33);
    cout << futureObject.get() << endl; //will be blocked until promise set future value
    thread11.join();

    return 0;
}
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex stationMutex;

void trainArrival(char trainName, int travelTime) {
    std::this_thread::sleep_for(std::chrono::seconds(travelTime));
    std::cout << "Train " << trainName << " has arrived at the station.\n";

    stationMutex.lock();
    std::cout << "Train " << trainName << " is at the station.\n";

    std::string command;
    do {
        std::cout << "Enter 'depart' for train " << trainName << ": ";
        std::cin >> command;
    } while (command != "depart");

    std::cout << "Train " << trainName << " has departed from the station.\n";
    stationMutex.unlock();
}

int main() {
    int timeA, timeB, timeC;

    std::cout << "Enter travel time for train A: ";
    std::cin >> timeA;

    std::cout << "Enter travel time for train B: ";
    std::cin >> timeB;

    std::cout << "Enter travel time for train C: ";
    std::cin >> timeC;

    std::thread trainA(trainArrival, 'A', timeA);
    std::thread trainB(trainArrival, 'B', timeB);
    std::thread trainC(trainArrival, 'C', timeC);

    trainA.join();
    trainB.join();
    trainC.join();

    std::cout << "All trains have departed, program ends.\n";

    return 0;
}

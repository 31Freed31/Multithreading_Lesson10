#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <chrono>

std::mutex kitchenMutex;
std::queue<std::string> ordersQueue;
std::queue<std::string> readyOrdersQueue;
int completedDeliveries = 0;

std::string getRandomDish() {
    std::string dishes[] = { "Pizza", "Soup", "Steak", "Salad", "Sushi" };
    return dishes[rand() % 5];
}

void waiter() {
    while (completedDeliveries < 10) {
        std::this_thread::sleep_for(std::chrono::seconds(5 + rand() % 6));
        std::string order = getRandomDish();
        std::lock_guard<std::mutex> lock(kitchenMutex);
        ordersQueue.push(order);
        std::cout << "Waiter: New order received for " << order << ".\n";
    }
}

void kitchen() {
    while (completedDeliveries < 10) {
        std::string order;
        {
            std::lock_guard<std::mutex> lock(kitchenMutex);
            if (!ordersQueue.empty()) {
                order = ordersQueue.front();
                ordersQueue.pop();
                std::cout << "Kitchen: Started preparing " << order << ".\n";
            }
            else {
                continue;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(5 + rand() % 11));
        std::lock_guard<std::mutex> lock(kitchenMutex);
        readyOrdersQueue.push(order);
        std::cout << "Kitchen: " << order << " is ready for delivery.\n";
    }
}

void courier() {
    while (completedDeliveries < 10) {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        std::lock_guard<std::mutex> lock(kitchenMutex);
        while (!readyOrdersQueue.empty()) {
            std::string order = readyOrdersQueue.front();
            readyOrdersQueue.pop();
            std::cout << "Courier: Delivered " << order << ".\n";
            completedDeliveries++;
        }
    }
}

int main() {
    srand(time(0));

    std::thread waiterThread(waiter);
    std::thread kitchenThread(kitchen);
    std::thread courierThread(courier);

    waiterThread.join();
    kitchenThread.join();
    courierThread.join();

    std::cout << "All deliveries completed. Restaurant closing.\n";
    return 0;
}

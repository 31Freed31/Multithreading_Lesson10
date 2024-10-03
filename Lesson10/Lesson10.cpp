#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <mutex>

struct Swimmer {
    std::string name;
    double speed;
    double distanceCovered = 0.0;
    int finishTime = 0;
};

std::mutex mtx;

void swim(Swimmer& swimmer, int& timeCounter, bool& raceFinished) {
    while (!raceFinished) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::lock_guard<std::mutex> lock(mtx);
        swimmer.distanceCovered += swimmer.speed;
        timeCounter++;

        std::cout << swimmer.name << " проплыл " << swimmer.distanceCovered << " метров\n";

        if (swimmer.distanceCovered >= 100.0) {
            swimmer.finishTime = timeCounter;
            break;
        }
    }
}

int main() {
    setlocale(LC_CTYPE, "rus");
    const int numSwimmers = 6;
    std::vector<Swimmer> swimmers(numSwimmers);
    std::vector<std::thread> threads(numSwimmers);
    int timeCounter = 0;
    bool raceFinished = false;

    std::cout << "Введите имена и скорости пловцов (метры в секунду):\n";
    for (int i = 0; i < numSwimmers; ++i) {
        std::cout << "Пловец " << i + 1 << " имя: ";
        std::cin >> swimmers[i].name;
        std::cout << "Скорость (м/с): ";
        std::cin >> swimmers[i].speed;
    }

    for (int i = 0; i < numSwimmers; ++i) {
        threads[i] = std::thread(swim, std::ref(swimmers[i]), std::ref(timeCounter), std::ref(raceFinished));
    }

    for (int i = 0; i < numSwimmers; ++i) {
        threads[i].join();
    }

    std::sort(swimmers.begin(), swimmers.end(), [](const Swimmer& a, const Swimmer& b) {
        return a.finishTime < b.finishTime;
        });

    std::cout << "\nРезультаты заплыва:\n";
    for (int i = 0; i < numSwimmers; ++i) {
        std::cout << i + 1 << ". " << swimmers[i].name << " - время: " << swimmers[i].finishTime << " секунд\n";
    }

    return 0;
}

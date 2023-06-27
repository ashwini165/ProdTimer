// ConsoleTests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "..\ProdTimerLib\Interface\Timer.h"
#include "..\ProdTimerLib\Src\Helpers.h"

int main()
{
    Timer& testTimer = Timer::instance();

    testTimer.Start();

    /*testTimer.SetTimer(10000, []() {
        std::cout << "At now+10s" << std::endl;
        });*/
    testTimer.SetDelayedTask(GetNowTimestamp() + 10000, []() {
        std::cout << "At now+10s" << std::endl;
        });

    testTimer.SetTimer(500, []() {
        std::cout << "Every 0.5s" << std::endl;
        }, -1);

    std::cout << timetoStr() << std::endl;
    auto timer_id = testTimer.SetTimer(5000, []() {
        std::cout << "Every 5s: " << timetoStr() << std::endl;
        }, 10);

    testTimer.SetTimer(30 * 1000, []() {
        std::cout << "Every 30s: " << timetoStr() << std::endl;
        }, 2);

    testTimer.SetTimer(60 * 1000, []() {
        std::cout << "Every 1m: " << timetoStr() << std::endl;
        }, 3);

    testTimer.SetTimer(90 * 1000, []() {
        std::cout << "Every 1.5m: " << timetoStr() << std::endl;
        }, 4);

    testTimer.SetTimer(120 * 1000, []() {
        std::cout << "Every 2m: " << timetoStr() << std::endl;
        }, 5);

    std::this_thread::sleep_for(std::chrono::seconds(30));
    testTimer.CancelTimer(timer_id);

    std::this_thread::sleep_for(std::chrono::minutes(20));
    testTimer.Stop();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#include "pch.h"
#include "CppUnitTest.h"
#include "../ProdTimerLib/Src/Helpers.h"
#include "../ProdTimerLib/Interface/Timer.h"

#include <iostream>
#include <unordered_map>
#include <vector>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TimerTests
{
	TEST_CLASS(TimerTests)
	{
	public:
		
		TEST_METHOD(TestTimer)
		{
			Timer& testTimer = Timer::instance();
            testTimer.Start();

            std::unordered_map<uint32_t, std::vector<int64_t>> outputs;

            uint32_t idHalfSecond = testTimer.SetTimer(500, [&]() {
                outputs[idHalfSecond].push_back(GetNowTimestamp());
                }); // continue repetition

            std::cout << timetoStr() << std::endl;
            uint32_t idSecond_1 = testTimer.SetTimer(1000, [&]() {
                outputs[idSecond_1].push_back(GetNowTimestamp());
                }, 17); // repetition = 10

            uint32_t idSecond_2 = testTimer.SetTimer(2000, [&]() {
                outputs[idSecond_2].push_back(GetNowTimestamp());
                }, 10);

            std::this_thread::sleep_for(std::chrono::milliseconds(10000));
            testTimer.CancelTimer(idSecond_2);
            Assert::AreEqual(3, (int)outputs.size());
            Assert::IsTrue(abs(20 - (int)outputs[idHalfSecond].size()) <= 1);
            Assert::IsTrue(abs(10 - (int)outputs[idSecond_1].size()) <= 1);
            Assert::IsTrue(abs(5 - (int)outputs[idSecond_2].size()) <= 1);
 

            std::this_thread::sleep_for(std::chrono::milliseconds(10000));
            Assert::AreEqual(3, (int)outputs.size());
            Assert::IsTrue(abs(40 - (int)outputs[idHalfSecond].size()) <= 1);
            Assert::AreEqual(17, (int)outputs[idSecond_1].size());
            Assert::IsTrue(abs(5 - (int)outputs[idSecond_2].size()) <= 1);

            //std::this_thread::sleep_for(std::chrono::minutes(20));
            testTimer.Stop();
		}

        TEST_METHOD(TestTimerLongRunning)
        {
            Timer& testTimer = Timer::instance();
            testTimer.Start();

            std::unordered_map<uint32_t, std::vector<int64_t>> outputs;

            uint32_t idMilliSecond_5 = testTimer.SetTimer(500, [&]() {
                outputs[idMilliSecond_5].push_back(GetNowTimestamp());
                });

            std::cout << timetoStr() << std::endl;
            uint32_t idSecond_5 = testTimer.SetTimer(5000, [&]() {
                outputs[idSecond_5].push_back(GetNowTimestamp());
                });

            uint32_t idSecond_30 = testTimer.SetTimer(30 * 1000, [&]() {
                outputs[idSecond_30].push_back(GetNowTimestamp());
                });

            uint32_t idMinute_1 = testTimer.SetTimer(60 * 1000, [&]() {
                outputs[idMinute_1].push_back(GetNowTimestamp());
                });

            uint32_t idMinute_2 = testTimer.SetTimer(120 * 1000, [&]() {
                outputs[idMinute_2].push_back(GetNowTimestamp());
                });


            std::this_thread::sleep_for(std::chrono::seconds(200));
            testTimer.CancelTimer(idSecond_5);

            Assert::AreEqual(5, (int)outputs.size());

            //std::this_thread::sleep_for(std::chrono::minutes(20));
            testTimer.Stop();
        }
	};
}

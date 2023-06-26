#include "Helpers.h"
#include <thread>

std::string timetoStr() {
    char tmp[64] = { 0 };
    time_t t = time(NULL);
    tm tm;
    localtime_s(&tm, &t);
    snprintf(tmp, 64, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
    return tmp;
}

int64_t GetNowTimestamp() {
    using namespace std::chrono;
    auto now = steady_clock::now().time_since_epoch();
    return duration_cast<milliseconds>(now).count();
}
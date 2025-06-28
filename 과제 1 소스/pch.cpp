// pch.cpp: 미리 컴파일된 헤더에 해당하는 소스 파일

#include "pch.h"

using namespace std;
using namespace chrono;
// 미리 컴파일된 헤더를 사용하는 경우 컴파일이 성공하려면 이 소스 파일이 필요합니다.
std::vector<int> generateUniqueRandom(int min, int max)
{
    std::vector<int> values;

    // 값 채우기
    for (int i = min; i <= max; ++i)
        values.push_back(i);

    // 셔플
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(values.begin(), values.end(), gen);

    return values;
}

// 경과 시간 표시
std::chrono::milliseconds computeOverTime(std::chrono::time_point<std::chrono::steady_clock> timeBegin, const char* pchText)
{
    auto end = std::chrono::steady_clock::now();
    auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(end - timeBegin);

    /*
    // 절대 현재 시스템 시간 출력
    auto sys_now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(sys_now);
    char buffer[255] = { 0 };
    ctime_s(buffer, 255, &now_c);
    std::cout << "현재 시간:  " << buffer << std::endl;
    */

    std::cout << pchText << millisec.count() << " ms" << std::endl;
    return millisec;
}
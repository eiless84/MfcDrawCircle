// pch.cpp: 미리 컴파일된 헤더에 해당하는 소스 파일

#include "pch.h"

using namespace std;
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

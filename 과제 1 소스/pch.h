﻿// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
#include <format>
#include "iostream"
#include <fstream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>
#include <ctime>

/// <summary>
/// #enum
/// </summary>
enum ePARENT {
	FULL_WIDTH = 1280,
	FULL_HEIGHT = 800,
};

enum eCHILD 
{
	WINDOW_WIDTH = 1024,
	WINDOW_HEIGHT = 768,
};

enum ePOINT
{
	ERROR_PT = -9999,
	CIRCLE_ARRAY = 3,
	POINT_MAX = 10000,
};

/// <summary>
/// #define
/// </summary>
#define COLOR_BLACK	RGB(0, 0, 0)
#define COLOR_RED	RGB(0xff, 0, 0)
#define COLOR_GREEN RGB(0, 0xff, 0)
#define COLOR_BLUE	RGB(0, 0, 0xff)
#define COLOR_WHITE	RGB(0xff, 0xff, 0xff)
#define BLACK_COLOR	0
#define GRAY_COLOR	128
#define WHITE_COLOR	255

/// <summary>
/// struct
/// </summary>
struct stDoublePos {
	stDoublePos(double CX = 0, double CY = 0) : dX(CX), dY(CY) {};
	double dX;
	double dY;
};

/// <summary>
/// util Function
/// </summary>
std::vector<int> generateUniqueRandom(int min, int max);
std::chrono::milliseconds computeOverTime(std::chrono::time_point<std::chrono::steady_clock> timeBegin, const char* pchText = "경과시간");
#endif //PCH_H

#pragma once

enum eCIRCLE
{
	RADIUS = 4,	// 반지름 미설정시 디폴트값
	THICKNESS = 2,	// 큰원의 두께 설정
};

// CProcess 명령 대상
class CProcess : public CObject
{
public:
	CProcess();
	virtual ~CProcess();
	
private:	// 내부 함수는 함수명이 소문자로 시작합니다.
	int getPointSize();
	CPoint getPointData(int arrayNum);
	int getThickness(int Radius);
	

public:		// 외부 참조 함수는 함수명이 대문자로 시작합니다.
	void ProcessData(CRect rect);
	int GetRadiusSize();
	void SetRadiusSize(int size);
	void SetPointData(CPoint pos);
	CPoint IsPointCheck(CPoint pos, int nRadiusSize);
	void SetImageSize(int width, int height, int bpp);
	void ClearData();
	void UpdateRandomPos();

	const char* GetFrameBuffer();

protected:
	bool isInCircle(int nX, int nY, int cx, int cy, int nRadius, int nThickness = THICKNESS);
	bool calcCircle(const CPoint& p1, const CPoint& p2, const CPoint& p3, double& cx, double& cy, double& radius);
	bool isIntersectRect(CRect Rect1, CRect Rect2);
	void updateCircle(int x, int y, int nRadius, int nCoror, RECT rectArea, int nThickness = THICKNESS);

private:	// copy image Data
	std::vector<CPoint> m_vPointData;
	char m_frameBuffer[eCHILD::WINDOW_WIDTH * eCHILD::WINDOW_HEIGHT] = {0};
	int m_nRadiusSize = eCIRCLE::RADIUS;
	int m_nPitch = 0;
	int m_nWidth = 0;
	int m_nHeight = 0;
	int m_nBpp = 0;

	std::vector<CPoint> m_vComputeCirclePos;
	std::atomic<int> m_nComputeRadius;

	void processPoint(CRect rect);
	void processCombine(CRect rect);
};
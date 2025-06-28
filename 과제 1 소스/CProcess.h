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
	std::vector<CPoint> m_vPointData;
	unsigned char* m_pImageData = NULL;	// DlgImage를 참조한다.

	void ClearPoint();
	void setPointData(CPoint pos);
	int getPointSize();
	CPoint getPointData(int arrayNum);
	CPoint isPointCheck(CPoint pos, int nRadiusSize);
	void SetRadiusSize(int size);
	int GetRadiusSize();
	int GetThickness(int Radius);

	void UpdateRandomPos();
	void processPoint(CRect rect);
	void processCombine(CRect rect);
	void processImage(CRect rect);

	void setPitch(int pitch);
	void SetImageData(unsigned char* pImageData);
	void clearDate();

protected:
	bool isInCircle(int nX, int nY, int cx, int cy, int nRadius, int nThickness = THICKNESS);
	bool calcCircle(const CPoint& p1, const CPoint& p2, const CPoint& p3, double& cx, double& cy, double& radius);
	bool isIntersectRect(CRect Rect1, CRect Rect2);
	void UpdateCircle(unsigned char* fm, int x, int y, int nRadius, int nCoror, RECT rectArea, int nThickness = THICKNESS);

private:
	int m_nRadiusSize = eCIRCLE::RADIUS;
	int m_nPitch;

	std::vector<CPoint> m_vComputeCirclePos;
	std::atomic<int> m_nComputeRadius;
};
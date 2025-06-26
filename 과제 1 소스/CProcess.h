#pragma once

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
	void UpdateRandomPos();
	void processPoint(CRect rect);
	void processCircle(CRect rect);
	void processImage(CRect rect);
	void setPitch(int pitch);
	void SetImageData(unsigned char* pImageData);

protected:
	bool isInCircle(int nX, int nY, int cx, int cy, int nRadius);
	bool isInCombinedCircle(int nX, int nY, int cx, int cy, int nRadius);
	bool calcCircle(const CPoint& p1, const CPoint& p2, const CPoint& p3, double& cx, double& cy, double& r);
	void UpdateCircle(unsigned char* fm, int x, int y, int nRadius, int nCoror, RECT rectArea);
	void UpdateCombinedCircle(unsigned char* fm, int x, int y, int nRadius, int nCoror, RECT rectArea);

	

private:
	int m_nRadiusSize = eCIRCLE::RADIUS;
	int m_nPitch;
};
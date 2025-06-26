// CProcess.cpp: 구현 파일
//

#include "pch.h"
#include "MfcDrawCircle.h"
#include "CProcess.h"

using namespace std;

// CProcess
CProcess::CProcess()
{
}

CProcess::~CProcess()
{
}


void CProcess::SetRadiusSize(int size)
{
	m_nRadiusSize = size;
}

int CProcess::GetRadiusSize()
{
	if (m_nRadiusSize == 0)
	{
		return eCIRCLE::RADIUS;
	}
	return m_nRadiusSize;
}

void CProcess::SetImageData(unsigned char* pImageData)
{
	m_pImageData = pImageData;
}

void CProcess::setPitch(int pitch)
{
	m_nPitch = pitch;
}


void CProcess::processPoint(CRect rect)
{
	for (int i = 0; i < ePOINT::CIRCLE_ARRAY; i++)
	{
		CPoint CP = getPointData(i);
		if (CP.x != ePOINT::ERROR_PT)
		{
			UpdateCircle(m_pImageData, CP.x - GetRadiusSize(), CP.y - GetRadiusSize(), GetRadiusSize(), COR_GRAY/2, rect);
		}
	}
}

void CProcess::processCircle(CRect rect)
{
	// 3점이 모두 찍힌 상태
	if (getPointSize() >= ePOINT::CIRCLE_ARRAY)
	{
		CPoint CP1 = getPointData(0);
		CPoint CP2 = getPointData(1);
		CPoint CP3 = getPointData(2);
		stDoublePos dCombinedCirclePos(0, 0);
		double dCombinedRadius = 0;

		if (calcCircle(CP1, CP2, CP3, dCombinedCirclePos.dX, dCombinedCirclePos.dY, dCombinedRadius))
		{
			UpdateCombinedCircle(m_pImageData, static_cast<int>(dCombinedCirclePos.dX - dCombinedRadius), static_cast<int>(dCombinedCirclePos.dY - dCombinedRadius), static_cast<int>(dCombinedRadius), COR_GRAY, rect);
		}
		// 세점이 일직선 상이라 원으로 그리기 불가능합니다.
		else
		{
			AfxMessageBox(L"그리기 불가능한 포인트들입니다");
			// 초기화
		}
	}
}

void CProcess::processImage(CRect rect)
{
	// 점 정보를 계산하여 갱신합니다.
	processPoint(rect);
	
	processCircle(rect);
}

void CProcess::UpdateCircle(unsigned char* fm, int x, int y, int nRadius, int nCoror, RECT rectArea)
{
	int nCenterX = x + nRadius;
	int nCenterY = y + nRadius;

	int nImageheight = eCHILD::WINDOW_HEIGHT;
	
	int maxY = max(y, rectArea.top);
	int maxX = max(x, rectArea.left);

	int minY = min(y + nRadius * 2, rectArea.bottom);
	int minX = min(x + nRadius * 2, rectArea.right);

	for (int j = max(y, rectArea.top); j < min(y + nRadius * 2, rectArea.bottom); j++) {
		for (int i = max(x, rectArea.left); i < min(x + nRadius * 2, rectArea.right); i++) {
			if (isInCircle(i, j, nCenterX, nCenterY, nRadius)) {
				int totalArray = j * m_nPitch + i;
				if (totalArray >= 0 && totalArray < nImageheight * m_nPitch) {
					fm[totalArray] = nCoror;
				}
			}
		}
	}
}

void CProcess::UpdateCombinedCircle(unsigned char* fm, int x, int y, int nRadius, int nCoror, RECT rectArea)
{
	int nCenterX = x + nRadius;
	int nCenterY = y + nRadius;

	int nWidth = rectArea.right;
	int nHeight = rectArea.bottom;

	int nImageheight = eCHILD::WINDOW_HEIGHT;

	for (int j = max(y, rectArea.top); j < min(y + nRadius * 2, rectArea.bottom); j++) {
		for (int i = max(x, rectArea.left); i < min(x + nRadius * 2, rectArea.right); i++) {
			if (isInCombinedCircle(i, j, nCenterX, nCenterY, nRadius)) {
				int totalArray = j * m_nPitch + i;
				if (totalArray >= 0 && totalArray < nImageheight * m_nPitch) {
					fm[totalArray] = nCoror;
				}
			}
		}
	}
}

void CProcess::UpdateRandomPos()
{
	vector<int> vRandomX = generateUniqueRandom(0, eCHILD::WINDOW_WIDTH - GetRadiusSize());
	vector<int> vRandomY = generateUniqueRandom(0, eCHILD::WINDOW_HEIGHT - GetRadiusSize());

	for (int i = 0; i < ePOINT::CIRCLE_ARRAY; i++)
	{
		setPointData(CPoint(vRandomX[i], vRandomY[i]));
	}
}

// CProcess 멤버 함수
void CProcess::setPointData(CPoint pos)
{
	if (m_vPointData.size() > ePOINT::CIRCLE_ARRAY)
		return;

	m_vPointData.push_back(pos);

	cout << m_vPointData.size() << endl;
}

int CProcess::getPointSize()
{
	return static_cast<int>(m_vPointData.size());
}

CPoint CProcess::getPointData(int arrayNum)
{	
	CPoint errorPoint = CPoint(ePOINT::ERROR_PT, ePOINT::ERROR_PT);
	if ( arrayNum >= ePOINT::CIRCLE_ARRAY)
		return errorPoint;

	if (m_vPointData.size() <= arrayNum)
		return errorPoint;

	//cout << "m_vPointData [ " << arrayNum << " ] " << m_vPointData[arrayNum].x << " , " << m_vPointData[arrayNum].y << endl;

	return m_vPointData[arrayNum];
}

CPoint CProcess::isPointCheck(CPoint pos, int nRadiusSize)
{
	CPoint errorPoint = CPoint(ePOINT::ERROR_PT, ePOINT::ERROR_PT);

	if (m_vPointData.size() < ePOINT::CIRCLE_ARRAY)
		return errorPoint;

	for (auto &pointData : m_vPointData)
	{
		if (isInCircle(pointData.x, pointData.y, pos.x, pos.y, nRadiusSize+2))
		{
			//cout << "isPointCheck TRUE : " << pointData.x << " , " << pointData.y << endl;
			// 기존 값을 갱신해 줍니다.
			pointData = pos;
			return pointData;
		}
	}

	cout << "isPointCheck FALSE !!! " << endl;
	return errorPoint;
}

void CProcess::ClearPoint()
{
	m_vPointData.clear();
}

bool CProcess::isInCircle(int nX, int nY, int cx, int cy, int nRadius)
{
	double dX = nX - cx;
	double dY = nY - cy;
	double dDist = dX * dX + dY * dY;

	if (dDist <= nRadius * nRadius) {
		return true;
	}
	return false;
}

bool CProcess::isInCombinedCircle(int nX, int nY, int cx, int cy, int nRadius)
{
	double dX = nX - cx;
	double dY = nY - cy;
	double dDist = dX * dX + dY * dY;

	// 원 안의 색을 비워주기 위해서 두께의 분포도를 조절했습니다.
	if (dDist <= nRadius * nRadius && dDist >= (nRadius * nRadius- (nRadius*2)))
	{
		return true;
	}
	return false;
}

bool CProcess::calcCircle(const CPoint& p1, const CPoint& p2, const CPoint& p3, double& cx, double& cy, double& r) {
	double x1 = p1.x, y1 = p1.y;
	double x2 = p2.x, y2 = p2.y;
	double x3 = p3.x, y3 = p3.y;

	double D = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
	if (fabs(D) < 1e-10) return false;  // 세 점이 일직선상에 있음
	
	double x1sq = x1 * x1 + y1 * y1;
	double x2sq = x2 * x2 + y2 * y2;
	double x3sq = x3 * x3 + y3 * y3;

	cx = ((x1sq) * (y2 - y3) + (x2sq) * (y3 - y1) + (x3sq) * (y1 - y2)) / D;
	cy = ((x1sq) * (x3 - x2) + (x2sq) * (x1 - x3) + (x3sq) * (x2 - x1)) / D;

	r = sqrt((x1 - cx) * (x1 - cx) + (y1 - cy) * (y1 - cy));

	return true;
}
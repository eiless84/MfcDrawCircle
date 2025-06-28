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

int CProcess::GetThickness(int Radius)
{
	int nRadius = Radius - eCIRCLE::THICKNESS;
	if (nRadius <0)
	{
		return 0; // return 0일 경우 전부 그린다.
	}
	return nRadius;
}

void CProcess::SetImageData(unsigned char* pImageData)
{
	m_pImageData = pImageData;
}

void CProcess::setPitch(int pitch)
{
	m_nPitch = pitch;
}

void CProcess::clearDate()
{
	m_vComputeCirclePos.clear();
	m_nComputeRadius = 0;
}

void CProcess::processPoint(CRect rect)
{
	//std::cout << "==== CProcess::processPoint :: 업데이트 =====\n";
	for (int i = 0; i < ePOINT::CIRCLE_ARRAY; i++)
	{
		CPoint CP = getPointData(i);
		CPoint CPSize = CPoint(GetRadiusSize()*2, GetRadiusSize() * 2);
		CRect pointRect(CP.x, CP.y, CP.x + CPSize.x, CP.y + CPSize.y);
		if (isIntersectRect(rect, pointRect))
		{
			int nInnerThickness = GetThickness(0);	// 원이 작기 때문에 0부터 전부 그립니다.
			UpdateCircle(m_pImageData, CP.x, CP.y, GetRadiusSize(), COR_GRAY/2, rect, nInnerThickness);
		}
	}
}

void CProcess::processCombine(CRect rect)
{
	std::cout << "==== CProcess::processCombine :: 업데이트 =====\n";
	// update Combined Circle Pos
	if (m_vComputeCirclePos.size() == 0 && m_nComputeRadius ==0)
	{		
		if (getPointSize() >= ePOINT::CIRCLE_ARRAY)
		{
			CPoint CP1 = getPointData(0);
			CPoint CP2 = getPointData(1);
			CPoint CP3 = getPointData(2);
			
			stDoublePos dCombinedCirclePos(0, 0);
			double dCombinedRadius = 0;
			calcCircle(CP1, CP2, CP3, dCombinedCirclePos.dX, dCombinedCirclePos.dY, dCombinedRadius);
			int nX = static_cast<int>(dCombinedCirclePos.dX - dCombinedRadius + GetRadiusSize());
			int nY = static_cast<int>(dCombinedCirclePos.dY - dCombinedRadius + GetRadiusSize());
			m_vComputeCirclePos.push_back(CPoint(nX, nY));
			m_nComputeRadius = static_cast<int>(dCombinedRadius);
			std::cout << "==== processCombine :: 큰원 정보 만들기 =====" << "dCombinedCirclePos  X : " << dCombinedCirclePos.dX << "Y : " << dCombinedCirclePos.dY << "Radius : " << m_nComputeRadius;
		}
	}
		
	// 3점이 모두 찍힌 상태
	if (m_vComputeCirclePos.size() > 0)
	{
		// 큰 원의 영역에 들어온 경우에만 업데이트 합니다.
		CPoint CP = CPoint(m_vComputeCirclePos[0].x, m_vComputeCirclePos[0].y);
		CPoint CPSize = CPoint(m_nComputeRadius * 2, m_nComputeRadius * 2);
		CRect pointRect(CP.x, CP.y, CP.x + CPSize.x, CP.y + CPSize.y);
		if (isIntersectRect(rect, pointRect))
		{
			int nInnerThickness = GetThickness(m_nComputeRadius - THICKNESS);	// 원이 작기 때문에 0부터 전부 그립니다.
			UpdateCircle(m_pImageData, m_vComputeCirclePos[0].x, m_vComputeCirclePos[0].y, m_nComputeRadius, COR_GRAY, rect, nInnerThickness);
		}
	}
}

void CProcess::processImage(CRect rect)
{
	std::cout << "==== CProcess::processImage :: 업데이트 =====\n";

	// 세개의 점 정보를 이어서 큰 원을 만듭니다.
	processCombine(rect);

	// 세개의 점 정보를 계산하여 갱신합니다. 점을 위에 그리기 위해서 뒤에 뒵니다.
	processPoint(rect);
}

void CProcess::UpdateRandomPos()
{
	vector<int> vRandomX = generateUniqueRandom(GetRadiusSize(), eCHILD::WINDOW_WIDTH - GetRadiusSize());
	vector<int> vRandomY = generateUniqueRandom(GetRadiusSize(), eCHILD::WINDOW_HEIGHT - GetRadiusSize());

	for (int i = 0; i < ePOINT::CIRCLE_ARRAY; i++)
	{
		setPointData(CPoint(vRandomX[i] - GetRadiusSize()/2, vRandomY[i] - GetRadiusSize()/2));
	}

	std::cout << "==== CProcess::UpdateRandomPos :: 완료 =====\n";
}

// CProcess 멤버 함수
void CProcess::setPointData(CPoint pos)
{
	if (m_vPointData.size() > ePOINT::CIRCLE_ARRAY)
		return;

	m_vPointData.push_back(pos);

	//cout << m_vPointData.size() << endl;
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

	cout << "m_vPointData [ " << arrayNum << " ] " << m_vPointData[arrayNum].x << " , " << m_vPointData[arrayNum].y << endl;

	return m_vPointData[arrayNum];
}

CPoint CProcess::isPointCheck(CPoint pos, int nRadiusSize)
{
	CPoint errorPoint = CPoint(ePOINT::ERROR_PT, ePOINT::ERROR_PT);

	if (m_vPointData.size() < ePOINT::CIRCLE_ARRAY)
		return errorPoint;

	for (auto &pointData : m_vPointData)
	{
		int nInnerThickness = GetThickness(nRadiusSize);
		int nCollisizeSize = nRadiusSize + 2;
		if (isInCircle(pointData.x, pointData.y, pos.x, pos.y, nCollisizeSize, nInnerThickness))
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
bool CProcess::isIntersectRect(CRect Rect1, CRect Rect2)
{
	CRect IntersectRect;
	if (IntersectRect.IntersectRect(Rect1, Rect2))
	{
		return true;
	}
	return false;
}

bool CProcess::isInCircle(int nX, int nY, int cx, int cy, int nRadius, int nthickness)
{
	double dX = (nX - cx);
	double dY = (nY - cy);

	double dDist = dX * dX + dY * dY; // 여기는 거리의 제곱 값이에요!

	// 원 안의 색을 비워주기 위해서 두께의 분포도를 조절했습니다.
	int nInnerRadiusAdjusted = nthickness;
	if (nInnerRadiusAdjusted < 0) {
		nInnerRadiusAdjusted = 0; // 만약 0보다 작아지면 0으로 만들어줘요!
	}

	double dOuterRadiusSquared = (double)nRadius * nRadius; // nRadius가 바깥 원의 반지름
	double dInnerRadiusSquared = (double)nInnerRadiusAdjusted * nInnerRadiusAdjusted; // nRadius가 바깥 원의 반지름

	// 이제 이 안전한 안쪽 반지름으로 범위를 체크해요!
	//if (dDist <= (double)nRadius * nRadius && dDist >= (double)nInnerRadiusAdjusted * nInnerRadiusAdjusted)

	
	if (dDist <= dOuterRadiusSquared && dDist >= dInnerRadiusSquared)
	//if (dDist <= nRadius * nRadius )
	{
		return true; // 도넛 모양 영역 안에 있다면 참!
	}

	return false;
}

bool CProcess::calcCircle(const CPoint& p1, const CPoint& p2, const CPoint& p3, double& cx, double& cy, double& radius)
{
	std::cout << "=========== calcCircle START ===========" << std::endl;

	double x1 = p1.x, y1 = p1.y;
	double x2 = p2.x, y2 = p2.y;
	double x3 = p3.x, y3 = p3.y;

	double A = x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2;

	if (fabs(A) < 1e-6) {
		// 세 점이 거의 일직선
		return false;
	}

	double B = (x1 * x1 + y1 * y1) * (y3 - y2) +
		(x2 * x2 + y2 * y2) * (y1 - y3) +
		(x3 * x3 + y3 * y3) * (y2 - y1);

	double C = (x1 * x1 + y1 * y1) * (x2 - x3) +
		(x2 * x2 + y2 * y2) * (x3 - x1) +
		(x3 * x3 + y3 * y3) * (x1 - x2);

	cx = -B / (2 * A);
	cy = -C / (2 * A);

	// 반지름 계산
	radius = sqrt((x1 - cx) * (x1 - cx) + (y1 - cy) * (y1 - cy));

	std::cout << "결과 :: 중심(cx, cy): (" << cx << ", " << cy << "), radius: " << radius << std::endl;
	return true;
}


void CProcess::UpdateCircle(unsigned char* fm, int x, int y, int nRadius, int nCoror, RECT rectArea, int nThickness)
{
	//std::cout << "=========== UpdateCircle START ===========" << endl;
	int fullHeight = rectArea.bottom;
	
	int nCenterX = x + nRadius;
	int nCenterY = y + nRadius;

	int circleLeft = nCenterX - nRadius;
	int circleRight = nCenterX + nRadius;
	int circleTop = nCenterY - nRadius;
	int circleBottom = nCenterY + nRadius;

	int minX = max(circleLeft, rectArea.left);
	int maxX = min(circleRight, rectArea.right);
	int minY = max(circleTop, rectArea.top);
	int maxY = min(circleBottom, rectArea.bottom);

	for (int j = minY; j < maxY; j++) {
		for (int i = minX; i < maxX; i++) {
			if (isInCircle(i, j, nCenterX, nCenterY, nRadius, nThickness)) {
				int totalArray = j * m_nPitch + i;
				if (totalArray >= 0 && totalArray < fullHeight * m_nPitch) {
					fm[totalArray] = nCoror;
				}
			}
		}
	}
}

//4사 분면의 x y 값입니다.
//
//(0, 0) ~(512, 384)
//(512, 0) ~(1024, 384)
//(0, 384) ~(512, 768)
//(512, 384) ~(1024, 768)
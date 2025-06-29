// CDlgImage.cpp: 구현 파일
//

#include "pch.h"
#include "MfcDrawCircle.h"
#include "afxdialogex.h"
#include "CDlgImage.h"
#include "MfcDrawCircleDlg.h"

using namespace std;

// CDlgImage 대화 상자
IMPLEMENT_DYNAMIC(CDlgImage, CDialogEx)

CDlgImage::CDlgImage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgImageChild, pParent)
{
	m_pParent = pParent;

	// 이미지 생성
	int Width = eCHILD::WINDOW_WIDTH;
	int Height = eCHILD::WINDOW_HEIGHT;
	
	if (m_Image.IsNull())
	{
		m_Image.Create(Width, -Height, m_Bpp);  // top-down

		if (m_Bpp == 8) {
			static RGBQUAD rgb[256];
			for (int i = 0; i < 256; i++)
				rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
			m_Image.SetColorTable(0, 256, rgb);
		}

		int pitch = m_Image.GetPitch();
		// 배경을 하얗게 초기화 (직접 픽셀 접근)
		unsigned char* pBits = (unsigned char*)m_Image.GetBits();
		memset(pBits, WHITE_COLOR, pitch * Height);
	}
}

CDlgImage::~CDlgImage()
{
}

void CDlgImage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgImage, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgImage 메시지 처리기
BOOL CDlgImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	MoveWindow(0, 0, eCHILD::WINDOW_WIDTH, eCHILD::WINDOW_HEIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgImage::OnPaint()
{
	CPaintDC dc(this); // 화면 DC

	CRect rect;
	GetClientRect(&rect);

	int width = rect.Width();
	int height = rect.Height();
	int pitch = m_Image.GetPitch();
	
	CImage backBuffer;
	backBuffer.Create(width, -height, 8);	// bpp = 8
	if (m_Bpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		backBuffer.SetColorTable(0, 256, rgb);
	}

	CDC* pMemDC = CDC::FromHandle(backBuffer.GetDC());	// 백퍼버 이미지의 DC 얻기
	
	pMemDC->FillSolidRect(rect, COLOR_WHITE);  // 배경 초기화

	if (!m_Image.IsNull())
	{
		unsigned char* fm = (unsigned char*)m_Image.GetBits();
		unsigned char* frameBuffer = (unsigned char*)m_process.GetFrameBuffer();

		if (frameBuffer[0] != NULL)
			memcpy(fm, frameBuffer, pitch * height);
		
		m_Image.Draw(*pMemDC, 0, 0);
	}

	// 백 버퍼를 화면에 복사
	backBuffer.ReleaseDC();  // DC 사용 종료
	backBuffer.BitBlt(dc.m_hDC, 0, 0); // 전체를 한 번에 출력

	backBuffer.Destroy();  // 백 버퍼 제거 (필수)	// 자주 호출될시 문제 생길수 있다.
}

void CDlgImage::SetImageSize(int width, int height)
{
	// set Image
	m_process.SetImageSize(width, height, m_Bpp);
}

void CDlgImage::ClearData()
{
	m_process.ClearData();
}

void CDlgImage::UpdateRandomPos()
{
	m_process.UpdateRandomPos();
}

void CDlgImage::ProcessData(CRect rect)
{
	m_process.ProcessData(rect);
}

void CDlgImage::SetRadiusSize(int size)
{
	m_process.SetRadiusSize(size);
}

void CDlgImage::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 마우스 클릭시 이동값을 저장합니다.
	CStringA strPoint;
	strPoint.Format(("마우스 클릭 위치: X = %d, Y = %d"), point.x, point.y);
	cout << strPoint.GetString() << endl;

	int nWidth = m_Image.GetWidth();
	int nHeight = m_Image.GetHeight();

	m_bDragging = true;

	m_process.SetPointData(point);

	CRect rect(0, 0, nWidth, nHeight);
	m_process.ProcessData(rect);

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CDlgImage::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bDragging = false;

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CDlgImage::OnMouseMove(UINT nFlags, CPoint point)
{
	// 마우스 이동입니다.
	if (m_bDragging)
	{
		// 이 함수 내부에서 point값을 갱신해줍니다.
		CPoint dragPoint = m_process.IsPointCheck(point, m_process.GetRadiusSize());

		// 영역에 들어온 경우
		if (dragPoint.x != ePOINT::ERROR_PT)
		{
			//cout << "영역에 들어왔다!! 마우스 값 갱신해주기 " << endl;
			int nWidth = m_Image.GetWidth();
			int nHeight = m_Image.GetHeight();
			CRect rect(0, 0, nWidth, nHeight);
			m_process.ProcessData(rect);
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

BOOL CDlgImage::OnEraseBkgnd(CDC* pDC)
{
	// 기본 배경 지우기 막음
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}

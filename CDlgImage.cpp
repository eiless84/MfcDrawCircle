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
END_MESSAGE_MAP()


// CDlgImage 메시지 처리기
BOOL CDlgImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	MoveWindow(0, 0, eCHILD::WINDOW_WIDTH, eCHILD::WINDOW_HEIGHT);

	InitImage();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgImage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	if (m_Image) {
		m_Image.Draw(dc, 0, 0);
	}
}

void CDlgImage::InitImage()
{
	int nWidth = eCHILD::WINDOW_WIDTH;	// 4096 * 5;
	int nHeight = eCHILD::WINDOW_HEIGHT;	// 4096 * 5;
	int nBpp = 8;

	m_Image.Create(nWidth, -nHeight, nBpp);

	if (nBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_Image.SetColorTable(0, 256, rgb);
	}

	ClearImage();
}

void CDlgImage::ClearImage(bool bIsClearPoint)
{
	std::cout << "CELAR IMAGE \n";
	int nWidth = m_Image.GetWidth();
	int nHeight = m_Image.GetHeight();
	int nBpp = 8;

	int nPitch = m_Image.GetPitch();
	unsigned char* fm = (unsigned char*)m_Image.GetBits();

	memset(fm, 0xff, nWidth * nHeight);	// 0xff WHITE

	if(bIsClearPoint)
		m_process.ClearPoint();

	m_process.SetImageData(fm);
	m_process.setPitch(nPitch);
}

void CDlgImage::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CStringA strPoint;
	strPoint.Format(("마우스 클릭 위치: X = %d, Y = %d"), point.x, point.y);
	cout << strPoint.GetString() << endl;

	int nWidth = m_Image.GetWidth();
	int nHeight = m_Image.GetHeight();

	m_bDragging = true;

	m_process.setPointData(point);

	CRect rect(0, 0, nWidth, nHeight);
	m_process.processImage(rect);

	Invalidate();

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CDlgImage::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bDragging = false;

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CDlgImage::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bDragging)
	{
		CPoint dragPoint = m_process.isPointCheck(point, m_process.GetRadiusSize());

		// 영역에 들어온 경우
		if (dragPoint.x != ePOINT::ERROR_PT)
		{
			cout << "영역에 들어왔다!! 마우스 값 갱신해주기 " << endl;
			ClearImage(false);
			int nWidth = m_Image.GetWidth();
			int nHeight = m_Image.GetHeight();
			CRect rect(0, 0, nWidth, nHeight);
			m_process.processImage(rect);
			Invalidate();
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

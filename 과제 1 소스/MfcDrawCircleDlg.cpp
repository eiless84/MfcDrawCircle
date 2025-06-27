
// MfcDrawCircleDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MfcDrawCircle.h"
#include "MfcDrawCircleDlg.h"
#include "afxdialogex.h"

using namespace std;
using namespace chrono;

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new DEBUG_NEW
#ifdef UNICODE // 유니코드일 경우
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else // 멀티바이트일 경우
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CMfcDrawCircleDlg 대화 상자
CMfcDrawCircleDlg::CMfcDrawCircleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCDRAWCIRCLE_DIALOG, pParent)
	, nRadiusSize(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMfcDrawCircleDlg::Clear()
{
	if (m_pDlgImage)
		delete m_pDlgImage;
	m_pDlgImage = NULL;
}

void CMfcDrawCircleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, nRadiusSize);
}

BEGIN_MESSAGE_MAP(CMfcDrawCircleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_INIT_CIRCLE, &CMfcDrawCircleDlg::OnBnClickedBtnInitCircle)
	ON_BN_CLICKED(IDC_BTN_INIT_RANDOM, &CMfcDrawCircleDlg::OnBnClickedBtnInitRandom)
	ON_BN_CLICKED(IDC_BTN_INIT_RANDOM_THREAD, &CMfcDrawCircleDlg::OnBnClickedBtnInitRandomThread)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_RADIUS_OK, &CMfcDrawCircleDlg::OnBnClickedBtnRadiusOk)
END_MESSAGE_MAP()

BOOL WINAPI CMfcDrawCircleDlg::ConsoleHandler(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		std::cout << "Cleaning up resources...\n";
		// 🔑 리소스 해제 코드 작성
		// 예: delete, free, CloseHandle, GDI DeleteObject 등
		FreeConsole();
		return TRUE;
	default:
		return FALSE;
	}
}

// CMfcDrawCircleDlg 메시지 처리기
BOOL CMfcDrawCircleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// 초기화
	MoveWindow(0, 0, ePARENT::FULL_WIDTH, ePARENT::FULL_HEIGHT);

	m_pDlgImage = new CDlgImage;
	m_pDlgImage->Create(IDD_DlgImageChild, this);
	m_pDlgImage->ShowWindow(SW_SHOW);
	m_pDlgImage->MoveWindow(0, 0, eCHILD::WINDOW_WIDTH, eCHILD::WINDOW_HEIGHT);

	// 콘솔창 릭 수정
	SetConsoleCtrlHandler(ConsoleHandler, TRUE);

	
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMfcDrawCircleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMfcDrawCircleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMfcDrawCircleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMfcDrawCircleDlg::OnBnClickedBtnRadiusOk()
{
	// 반지름값 적용하기
	UpdateData();

	m_pDlgImage->m_process.SetRadiusSize(nRadiusSize);

	m_pDlgImage->ClearImage(false);
	
	m_pDlgImage->m_process.processImage(CRect(0, 0, eCHILD::WINDOW_WIDTH, eCHILD::WINDOW_HEIGHT));

	Invalidate();
}

void CMfcDrawCircleDlg::OnBnClickedBtnInitCircle()
{
	DestroyThread();

	// 정보를 초기화합니다.
	if (m_pDlgImage)
	{
		m_pDlgImage->ClearImage();
		m_pDlgImage->Invalidate();
	}
}

void CMfcDrawCircleDlg::OnBnClickedBtnInitRandom()
{
	DestroyThread();

	// 랜덤한 위치로이동합니다.
	if (m_pDlgImage)
	{
		m_pDlgImage->ClearImage();
		m_pDlgImage->m_process.UpdateRandomPos();
		m_pDlgImage->m_process.processImage(CRect(0, 0, eCHILD::WINDOW_WIDTH, eCHILD::WINDOW_HEIGHT));
		m_pDlgImage->Invalidate();
	}
}


int CMfcDrawCircleDlg::UpdateImageData(CRect rect, int index)
{	
	std::cout << "Thread " << index << "m_nEndThreads : " << m_nEndThreads << " 시작\n";
	while(m_nPhaseCount< PHASE_COUNT * THREAD_COUNT)
	{
		if (m_bStop_threads)
			break;
		{
			// 쓰레드 작업
			std::lock_guard<std::mutex> lock(m_threadMtx);
			if (m_nClearCount == 0)
			{
				m_pDlgImage->ClearImage();
				m_pDlgImage->m_process.UpdateRandomPos();
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));  // 초기화 시간 예시		

		if (m_bStop_threads)
			break;
		{
			// 쓰레드 작업
			std::lock_guard<std::mutex> lock(m_threadMtx);
			std::cout << "Thread " << " m_nPhaseCount : " << m_nPhaseCount << " nClear : " << m_nClearCount << " 시작\n";
			m_pDlgImage->m_process.processImage(rect);
			showOverTime(m_timeBegin);
			++m_nClearCount;
			if (m_nClearCount >= THREAD_COUNT)
			{
				m_nClearCount = 0;
				Invalidate();
				UpdateWindow();
			}

			m_nPhaseCount++;
		}
	}

	m_nEndThreads++;
	std::cout << "Thread " << index << "m_nEndThreads : " << m_nEndThreads << " 종료\n";
	return 0;
}

void threadProcess(CWnd* pParent, CRect rect, int* nRet, int index)
{
	CMfcDrawCircleDlg* pWnd = (CMfcDrawCircleDlg*)pParent;
	*nRet = pWnd->UpdateImageData(rect, index);
}

void CMfcDrawCircleDlg::OnBnClickedBtnInitRandomThread()
{
	// 쓰레드로 자동 생성합니다.
	// 랜덤 생성을 초당 2회, 총 10번 자동으로 반복합니다.
	cout << "THREAD START!! " << endl;
	
	DestroyThread();

	std::lock_guard<std::mutex> lock(m_threadMtx);

	m_bStop_threads = false;
	m_nClearCount = 0;
	m_nPhaseCount = 0;

	int nImgSizeW = eCHILD::WINDOW_WIDTH / 2;
	int nImgSizeH = eCHILD::WINDOW_HEIGHT / 2;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_timeBegin = system_clock::now();
	CRect rect(0, 0, nImgSizeW, nImgSizeH);

	CRect rt[THREAD_COUNT];
	int nRet[THREAD_COUNT];

	for (int k = 0; k < THREAD_COUNT; k++) {
		rt[k] = rect;
		rt[k].OffsetRect(nImgSizeW * (k % 2), nImgSizeH * int(k / 2));
	}

	// 모든 쓰레드가 아직 종료되지 않으면.. 아직 쓰레드가 사용중이면..
	if (m_vthreads.size() > 0 && m_nEndThreads < THREAD_COUNT)
	{
		m_nEndThreads = 0;
		cout << "THREAD continue" << endl;
	}
	// 이미지 드로우가 끝났으면.
	else
	{
		cout << "THREAD restart " << endl;
		m_nEndThreads = 0;
		m_vthreads.clear();

		// 모든 쓰레드 생성
		for (int i = 0; i < THREAD_COUNT; i++) {
			m_vthreads.emplace_back(threadProcess, this, rt[i], &nRet[i], i);
		}

		for (auto& th : m_vthreads) {
			th.detach();
		}
	}

	int nSum = 0;
	for (int k = 0; k < THREAD_COUNT; k++) {
		nSum += nRet[k];
	}

	showOverTime(m_timeBegin);
	cout << "THREAD END!! " << endl;
}

void CMfcDrawCircleDlg::DestroyThread()
{
	std::lock_guard<std::mutex> lock(m_threadMtx);
	m_bStop_threads = true;

	// 쓰레드 종료 대기
	while (m_vthreads.size() > 0 && m_nEndThreads < THREAD_COUNT) {
		Sleep(10);
	}

	m_vthreads.clear();
}

void CMfcDrawCircleDlg::OnDestroy()
{	
	DestroyThread();
	
	Clear();

	CDialogEx::OnDestroy();
}
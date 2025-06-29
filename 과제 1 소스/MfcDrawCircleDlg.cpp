
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
	ON_WM_UPDATEUISTATE()
	ON_WM_TIMER()
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

	//	OnPaint 주기적으로 그려주기
	SetTimer(1, 33, NULL); // 33ms마다 호출 ≒ 약 30FPS
	

	// 이미지 정보를 초기 설정합니다.
	if (m_pDlgImage)
		m_pDlgImage->SetImageSize(eCHILD::WINDOW_WIDTH, eCHILD::WINDOW_HEIGHT);
	
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

	m_pDlgImage->SetRadiusSize(nRadiusSize);

	//m_pDlgImage->ProcessData(CRect(0, 0, eCHILD::WINDOW_WIDTH, eCHILD::WINDOW_HEIGHT));
}

void CMfcDrawCircleDlg::OnBnClickedBtnInitCircle()
{
	DestroyThread();

	// 정보를 초기화합니다.
	if (m_pDlgImage)
	{
		m_pDlgImage->SetImageSize(eCHILD::WINDOW_WIDTH, eCHILD::WINDOW_HEIGHT);
	}
}

void threadProcess(CWnd* pParent, CRect rect, int index)
{
	CMfcDrawCircleDlg* pWnd = (CMfcDrawCircleDlg*)pParent;
	pWnd->UpdateImageData(rect, index);
}

void CMfcDrawCircleDlg::RequestRandomCircle(int nPhaseCount, int nPhaseMaximum, int nWaitTimeMS)
{
	// 쓰레드로 자동 생성합니다.
	// 랜덤 생성을 초당 2회, 총 10번 자동으로 반복합니다.
	// 중복 실행 방지 (lock-free 방식)
	cout << "THREAD ENTER " << endl;
	
	m_nWaitTimeMS = nWaitTimeMS;
		
	// 시간 체크 (입력 시간 제한)
	if (computeOverTime(m_timeBegin).count() < m_nWaitTimeMS) {
		std::cout << "THREAD TOO FAST - SKIP 너무 빠른 입력입니다. - SKIP\n";
		//AfxMessageBox(_T("너무 빠른 입력입니다."));
		m_flagStopThread.clear();
		return;
	}
	
	std::cout << "THREAD INIT SUCCESS \n";
	m_timeBegin = std::chrono::steady_clock::now();

	cout << "THREAD START!!! " << endl;
	m_nPhaseMaximum = nPhaseMaximum;
	
	// 종료되지 않거나 사용중이면.. // 처음부터 다시 반복
	if (m_vthreads.size() > 0 && m_nEndThreads < THREAD_COUNT)
	{
		m_nPhaseCount = nPhaseCount;
		std::cout << "THREAD INIT m_nPhaseCount 111 " << m_nPhaseCount << " 초기화\n";
	}
	// 새로 생성
	else
	{
		m_nPhaseCount = nPhaseCount;
		std::cout << "THREAD INIT m_nPhaseCount 222 " << m_nPhaseCount << " 초기화\n";
		
		DestroyThread();		

		CreateThread();
	}
	
	computeOverTime(m_timeBegin);
	cout << "THREAD END!! " << endl;
}

void CMfcDrawCircleDlg::UpdateImageData(CRect rect, int index)
{
	static std::chrono::steady_clock::time_point tpLastWakeTime = std::chrono::steady_clock::now();

	//std::cout << "Thread " << index << " 시작\n";
	while (m_nPhaseCount < m_nPhaseMaximum * THREAD_COUNT)
	{
		//std::cout << "Thread " << index << "UpdateImageData m_nPhaseCount :" << m_nPhaseCount << " 시작\n";
		// 쓰레드 종료 요청 확인
		if (m_flagStopThread.test())
			break;
		{
			{
				std::lock_guard<std::mutex> lock(m_threadMtx);
				// 쓰레드 종료 요청 확인
				if (m_flagStopThread.test())
					break;
				// PHASE 초기 처리
				if (m_nPhaseCount % THREAD_COUNT == 0)
				{
					//std::cout << "Thread " << index << "m_nPhaseCount : " << m_nPhaseCount << " 이미지 초기화\n";
					m_pDlgImage->ClearData();
					m_pDlgImage->UpdateRandomPos();
					computeOverTime(tpLastWakeTime);
					tpLastWakeTime = std::chrono::steady_clock::now();
				}
			}
			

			{
				std::lock_guard<std::mutex> lock(m_threadMtx);
				// 쓰레드 종료 요청 확인
				if (m_flagStopThread.test())
					break;
				m_pDlgImage->ProcessData(rect);
			}
			
			{
				std::lock_guard<std::mutex> lock(m_threadMtx);
				// 쓰레드 종료 요청 확인
				if (m_flagStopThread.test())
					break;
				m_nPhaseCount++;
				if (m_nPhaseCount % THREAD_COUNT == 0)
				{
					//Invalidate(FALSE);
					computeOverTime(m_timeBegin, "총 누적시간");
				}
			}
		}
		
		// 주기 맞추기
		auto expected_time = tpLastWakeTime + std::chrono::milliseconds(m_nWaitTimeMS);
		auto now = std::chrono::steady_clock::now();
		if (expected_time > now)
			std::this_thread::sleep_until(expected_time);
	}

	m_nEndThreads++;
	std::cout << "Thread " << index << "m_nEndThreads : " << m_nEndThreads << " 종료\n";
}

void CMfcDrawCircleDlg::OnBnClickedBtnInitRandom()
{
	RequestRandomCircle(0, 1, 50);	// 0부터 1까지 한번실행.. 500ms
}

void CMfcDrawCircleDlg::OnBnClickedBtnInitRandomThread()
{
	RequestRandomCircle(0,10,500);	// 0부터 10까지 10번 실행 500ms
}

void CMfcDrawCircleDlg::CreateThread()
{
	std::cout << "CreateThread: threads start...\n";
	
	std::lock_guard<std::mutex> lock(m_threadMtx);
	//std::cout << "CreateThread START!!!\n";

	CRect rt[THREAD_COUNT];
	int nImgSizeW = eCHILD::WINDOW_WIDTH / 2;
	int nImgSizeH = eCHILD::WINDOW_HEIGHT / 2;
	CRect rect(0, 0, nImgSizeW, nImgSizeH);

	// 모든 쓰레드 생성
	
	for (int i = 0; i < THREAD_COUNT; i++) {
		rt[i] = rect;
		int resultX = nImgSizeW * (i % 2);
		int resultY = nImgSizeH * (i / 2);
		rt[i].OffsetRect(resultX, resultY);
		//std::cout << i << "사분면 : " << resultX << " ," << resultY << endl;
		m_vthreads.emplace_back(threadProcess, this, rt[i], i);
	}

	for (auto& th : m_vthreads) {
		th.detach();
	}

	std::cout << "CreateThread: threads end...\n";

	// 끝났으면 다음 진입 허용
	m_flagStopThread.clear();
}

void CMfcDrawCircleDlg::DestroyThread()
{
	std::cout << "DestroyThread: Stopping threads...\n";
	// 이 함수의 거의 실행되지 않아야 합니다. 반복 종료시 1회 또는 실행 종료시만 실행됩니다.

	{	
		std::lock_guard<std::mutex> lock(m_threadMtx);
		m_flagStopThread.test_and_set();	// 종료 플래그 설정
	}

	// 메모리 릭을 잡기 위해 추가합니다.
	{
		const int MAX_WAIT_MS = 2000; // 무한 루프에 빠지지 않도록 탈출 시간 추가
		auto start = std::chrono::steady_clock::now();
		std::lock_guard<std::mutex> lock(m_threadMtx);
		while (m_vthreads.size() > 0 && m_nEndThreads < THREAD_COUNT)
		{
			// 현재까지 경과 시간 계산
			auto now = std::chrono::steady_clock::now();
			auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

			if (elapsed_ms >= MAX_WAIT_MS)
			{
				std::cout << "DestroyThread wait timeout exceeded. Forcing exit.\n";
				break;
			}

			if (m_vthreads.size() > 0 && m_nEndThreads < THREAD_COUNT)
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			else
				break;
		}
	}
	

	{
		std::lock_guard<std::mutex> lock(m_threadMtx);
		// 상태 초기화
		m_vthreads.clear();
		m_nEndThreads = 0;
		m_flagStopThread.clear();
		std::cout << "DestroyThread: All threads stopped.\n";
	}
}

void CMfcDrawCircleDlg::OnDestroy()
{	
	DestroyThread();
	
	Clear();

	CDialogEx::OnDestroy();
}

void CMfcDrawCircleDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1)
	{
		//std::cout << "OnTimer Invalidate(false) OnPaint Update \n";
		Invalidate(FALSE); // → WM_PAINT 발생 → OnPaint 호출
	}

	CDialogEx::OnTimer(nIDEvent);
}

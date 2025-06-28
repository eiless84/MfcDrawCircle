
// MfcDrawCircleDlg.h: 헤더 파일
//

#pragma once

#include "CDlgImage.h"
#include "CProcess.h"


#include <thread>
#include <mutex>
#include <condition_variable>

const int THREAD_COUNT = 4;			// 스레드 갯수 4사 분면을 그리기 위해서 4개

// CMfcDrawCircleDlg 대화 상자
class CMfcDrawCircleDlg : public CDialogEx
{
// 생성입니다.
public:
	CMfcDrawCircleDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	void UpdateImageData(CRect rect, int index);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCDRAWCIRCLE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CDlgImage* m_pDlgImage;
	std::vector<std::thread> m_vthreads;
	std::mutex m_threadMtx;
	std::atomic_flag m_flagStopThread = ATOMIC_FLAG_INIT;
	std::atomic<bool> m_bStop_threads = false;
	std::atomic<int> m_nEndThreads = 0;
	std::atomic<int> m_nPhaseCount = 0;		// 쓰레드 실행 횟수 시작값
	std::atomic<int> m_nPhaseMaximum = 10;			// 쓰레드 실행 반복 횟수
	int m_nWaitTimeMS = 500;				// 쓰레드 1회 실행 속도 500ms
	std::chrono::time_point<std::chrono::steady_clock> m_timeBegin;
	static BOOL WINAPI ConsoleHandler(DWORD dwCtrlType);
	void Clear();
	void CreateThread();				// 비동기 쓰레드 생성
	void DestroyThread();
	void RequestRandomCircle(int nPhaseCount=0, int nPhaseMaximum=10, int nWaitTimeMS=500);
public:
	int nRadiusSize;
	afx_msg void OnBnClickedBtnInitCircle();
	afx_msg void OnBnClickedBtnInitRandom();
	afx_msg void OnBnClickedBtnInitRandomThread();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnRadiusOk();
	afx_msg void OnUpdateUIState(UINT /*nAction*/, UINT /*nUIElement*/);
};

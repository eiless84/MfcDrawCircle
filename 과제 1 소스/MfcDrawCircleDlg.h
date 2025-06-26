
// MfcDrawCircleDlg.h: 헤더 파일
//

#pragma once

#include "CDlgImage.h"
#include "CProcess.h"

// CMfcDrawCircleDlg 대화 상자
class CMfcDrawCircleDlg : public CDialogEx
{
// 생성입니다.
public:
	CMfcDrawCircleDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	int UpdateImageData(CRect rect, int index);

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
	volatile bool m_bStop_threads = false;
	volatile int m_nClearCount = 0;
	volatile int m_nPhaseCount = 0;
	std::chrono::time_point<std::chrono::system_clock> m_timeBegin;
	static BOOL WINAPI ConsoleHandler(DWORD dwCtrlType);
	void Clear();

public:
	int nRadiusSize;
	afx_msg void OnBnClickedBtnInitCircle();
	afx_msg void OnBnClickedBtnInitRandom();
	afx_msg void OnBnClickedBtnInitRandomThread();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnRadiusOk();
};

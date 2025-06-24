#pragma once
#include "afxdialogex.h"

#include "CProcess.h"

// CDlgImage 대화 상자
class CDlgImage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImage)

public:
	CDlgImage(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgImage();
	void ClearImage(bool bIsClearPoint = true);
	CImage m_Image;
	CProcess m_process;

private:	
	CWnd* m_pParent;
	bool m_bDragging = false;	
	void InitImage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgImageChild
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

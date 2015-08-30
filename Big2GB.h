#if !defined(AFX_BIG2GB_H__86048A20_E1BD_11D4_9003_E889A6A0A42E__INCLUDED_)
#define AFX_BIG2GB_H__86048A20_E1BD_11D4_9003_E889A6A0A42E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Big2GB.h : header file
//
#include "StringEx.h"
#include "BtnST.h" //平面按钮类
#include "EllepticalButton.h" //椭圆按钮类
/////////////////////////////////////////////////////////////////////////////
// Big2GB dialog

class Big2GB : public CDialog
{
// Construction
public:
	Big2GB(CWnd* pParent = NULL);   // standard constructor

	HRSRC hRes;
	HGLOBAL hGBTable;
	HGLOBAL hBIGTable;
	LPWORD pGBTable;
	LPWORD pBIGTable;

	CStringEx m_text;
	CButtonST m_SourceFile,m_DescFile; //定义扁平按钮变量
	//建立椭圆按钮
	CEllepticalButton m_Ok,m_Cancel,m_View;
// Dialog Data
	//{{AFX_DATA(Big2GB)
	enum { IDD = IDD_BIG2GB };
	CString strOpenTXTName;
    CString strSaveTXTName;
	CString	m_SourceFileName;
	CString	m_DescFileName;
	int		m_Result;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Big2GB)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CStringArray strLine;
	long GetLineNumber();
	long CalLineNumber();
	int GetTextLength();
	CString GetText();
	CString Range(long start,long end);
	void BigtoGB() ;
    void GBtoBig();
// Attributes
private:
	long m_lineNumber;
	BOOL bBigToGB;
protected:

	// Generated message map functions
	//{{AFX_MSG(Big2GB)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDirbrowser();
	afx_msg void OnFilebrowser();
	virtual void OnOK();
	afx_msg void OnButton1();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BIG2GB_H__86048A20_E1BD_11D4_9003_E889A6A0A42E__INCLUDED_)

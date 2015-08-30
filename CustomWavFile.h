#if !defined(AFX_CUSTOMWAVFILE_H__30CAE880_CFC8_11D4_9000_A2AEA3B32EA7__INCLUDED_)
#define AFX_CUSTOMWAVFILE_H__30CAE880_CFC8_11D4_9000_A2AEA3B32EA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomWavFile.h : header file
//
#include "BtnST.h" //平面按钮类
#include "EllepticalButton.h" //椭圆按钮类
#include "FlatEdit.h" //扁平编辑框类
/////////////////////////////////////////////////////////////////////////////
// CCustomWavFile dialog

class CCustomWavFile : public CDialog
{
// Construction
public:
	CCustomWavFile(CWnd* pParent = NULL);   // standard constructor

	
	CString	m_FilePathString1;

	CString GetWaveFile();
	bool GetOriginPlayFlag();

	CButtonST m_SourceFile; //定义扁平按钮变量
	//建立椭圆按钮
	CEllepticalButton m_OK,m_Cancel;

// Dialog Data
	//{{AFX_DATA(CCustomWavFile)
	enum { IDD = IDD_CUSTOMWAVE };
	CFlatEdit	m_TextDescriptionEdit; //显示为扁平编辑框
	CString	m_TextDescription;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomWavFile)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCustomWavFile)
	afx_msg void OnFilebrowser1();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMWAVFILE_H__30CAE880_CFC8_11D4_9000_A2AEA3B32EA7__INCLUDED_)

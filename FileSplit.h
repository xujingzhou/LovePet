#if !defined(AFX_FILESPLIT_H__3FB0B940_0C24_11D5_9005_E172D79D66C0__INCLUDED_)
#define AFX_FILESPLIT_H__3FB0B940_0C24_11D5_9005_E172D79D66C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileSplit.h : header file
//
//Modify by 徐景周 ，2001.2.13
//功能：文件分割为多种格式的大小，并将其合并在一起
//
#include "FlatComboBox.h" //扁平组合框显示类
#include "BtnST.h" //平面按钮类
#include "EllepticalButton.h" //椭圆按钮类
#include "GradientProgressCtrl.h" //渐变色进度条类
/////////////////////////////////////////////////////////////////////////////
// CFileSplit dialog

class CFileSplit : public CDialog
{
// Construction
public:
	CFileSplit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileSplit)
	enum { IDD = IDD_FILESPLIT };
	CFlatComboBox	m_FileSize; //显示为扁平组合框
	CGradientProgressCtrl  pProgress;
    CEllepticalButton m_buttMerge;  
	CEllepticalButton m_buttExit;
	CEllepticalButton m_buttSplit;
	CButtonST m_Browse;
    CButtonST m_Browse2;
	CButton	m_RadioSplit;
	CEdit	m_edit;
	CString	m_parts;
	CString	m_path;
	CString	m_targetpath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileSplit)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_split;
	CString m_filename;
	CFile m_SourceFile;
	HICON m_hIcon;
	int SplitMe();   //分割文件
	int MergeMe();  //合并文件
	UINT GetSplitFileSize(); //获取文件分割大小

	// Generated message map functions
	//{{AFX_MSG(CFileSplit)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	afx_msg void OnBrowse2();
	afx_msg void OnRadio4();
	afx_msg void OnRadio3();
	afx_msg void OnOk();
	afx_msg void OnButtonMerge();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILESPLIT_H__3FB0B940_0C24_11D5_9005_E172D79D66C0__INCLUDED_)

/*
 *  StaticFilespec.h
 *
 *功能：静态标签扩展类，用来显示长路径名
 * 修改人：徐景周 　
 *日期：2001年7月18日
 *
 */

#ifndef _StaticFilespec_h
#define _StaticFilespec_h

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CStaticFilespec window

class CStaticFilespec : public CWnd
{
// Construction
public:
	CStaticFilespec
    (DWORD  dwFormat = DT_LEFT | DT_NOPREFIX | DT_VCENTER,
     BOOL   bPathEllipsis = FALSE);

// Attributes
public:

// Operations
public:
  BOOL    IsPath() { return m_bPathEllipsis; }
  void    SetPath (BOOL bPathEllipsis)  { m_bPathEllipsis = bPathEllipsis; } 
  DWORD   GetFormat() { return m_dwFormat; } 
  void    SetFormat (DWORD dwFormat) { m_dwFormat = dwFormat; } 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticFilespec)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStaticFilespec();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticFilespec)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetText (WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  BOOL    m_bPathEllipsis;    // flag: draw text as path
  DWORD   m_dwFormat;         // text format
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif

// End StaticFilespec.h

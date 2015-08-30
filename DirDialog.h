////////////////////////////////////////////////////////////////////////
// DirDialog.h: interface for the CDirDialog class.
//
//////////////////////////////////////////////////////////////////////
//
//Modify by 徐景周 2000.10
//功能：调用系统目录浏览对话框
//
#if !defined(AFX_DIRDIALOG_H__62FFAC92_1DEE_11D1_B87A_0060979CDF6D__INCLUDED_)
#define AFX_DIRDIALOG_H__62FFAC92_1DEE_11D1_B87A_0060979CDF6D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDirDialog
{
  public:
    CDirDialog();
    virtual ~CDirDialog();
    int DoBrowse(CWnd *pwndParent);

    CString m_strPath;
    CString m_strInitDir;
    CString m_strSelDir;
    CString m_strWindowTitle;
    int  m_iImageIndex;
};

#endif // !defined(AFX_DIRDIALOG_H__62FFAC92_1DEE_11D1_B87A_0060979CDF6D__INCLUDED_)



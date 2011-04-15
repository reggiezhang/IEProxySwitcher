// IEProxySwitcherDlg.h : header file
// $Id: IEProxySwitcherDlg.h 238 2007-12-10 17:37:21Z ezhenzh $

#pragma once


// CIEProxySwitcherDlg dialog
class CIEProxySwitcherDlg : public CDialog
{
// Construction
public:
	CIEProxySwitcherDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_IEPROXYSWITCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    CString m_proxy;
    CString m_autoUrl;
    CString m_bypass;
    int m_proxyChkBtn;
    DWORD m_connFlags;
    DWORD m_autoDiscoveryFlags;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();

    BOOL QueryProxy();
    BOOL SetProxy();
    
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
    
public:
    afx_msg void OnBnClickedOk();
};

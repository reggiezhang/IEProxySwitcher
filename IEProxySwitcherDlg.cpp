// IEProxySwitcherDlg.cpp : implementation file
// $Id: IEProxySwitcherDlg.cpp 238 2007-12-10 17:37:21Z ezhenzh $

#include "stdafx.h"
#include "IEProxySwitcher.h"
#include "IEProxySwitcherDlg.h"
#include <Wininet.h>
#pragma comment(lib, "Wininet.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CIEProxySwitcherDlg dialog




CIEProxySwitcherDlg::CIEProxySwitcherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIEProxySwitcherDlg::IDD, pParent)
    , m_proxy("")
    , m_autoUrl("")
    , m_bypass("")
    , m_proxyChkBtn(BST_UNCHECKED)
    , m_connFlags(0)
    , m_autoDiscoveryFlags(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIEProxySwitcherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_ADDR, m_proxy);
    DDX_Check(pDX, IDC_CHECK_PROXY, m_proxyChkBtn);
}

BEGIN_MESSAGE_MAP(CIEProxySwitcherDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, &CIEProxySwitcherDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CIEProxySwitcherDlg message handlers

BOOL CIEProxySwitcherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    QueryProxy();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIEProxySwitcherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIEProxySwitcherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIEProxySwitcherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CIEProxySwitcherDlg::OnBnClickedOk()
{
    UpdateData(TRUE);
    theApp.WriteProfileString("", "address", m_proxy);
    SetProxy();
    OnOK();
}

BOOL CIEProxySwitcherDlg::QueryProxy()
{
    INTERNET_PER_CONN_OPTION_LIST    List;
    INTERNET_PER_CONN_OPTION         Option[5];
    unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);

    Option[0].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;
    Option[1].dwOption = INTERNET_PER_CONN_AUTODISCOVERY_FLAGS;
    Option[2].dwOption = INTERNET_PER_CONN_FLAGS;
    Option[3].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
    Option[4].dwOption = INTERNET_PER_CONN_PROXY_SERVER;

    List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
    List.pszConnection = NULL;
    List.dwOptionCount = 5;
    List.dwOptionError = 0;
    List.pOptions = Option;

    InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, &nSize);
    m_autoUrl = Option[0].Value.pszValue;
    m_autoDiscoveryFlags = Option[1].Value.dwValue;
    m_connFlags = Option[2].Value.dwValue;
    m_bypass = Option[3].Value.pszValue;
    m_proxy = Option[4].Value.pszValue;
    
    if (Option[2].Value.dwValue & PROXY_TYPE_PROXY)
    {
        m_proxyChkBtn = BST_CHECKED;
    }
    else if (Option[2].Value.dwValue & PROXY_TYPE_DIRECT)
    {
        m_proxyChkBtn = BST_UNCHECKED;
    }
    if (m_proxy.IsEmpty())
    {
        m_proxy = theApp.GetProfileString("", "address", NULL);
    }


    if(Option[0].Value.pszValue != NULL)
        GlobalFree(Option[0].Value.pszValue);

    if(Option[3].Value.pszValue != NULL)
        GlobalFree(Option[3].Value.pszValue);

    if(Option[4].Value.pszValue != NULL)
        GlobalFree(Option[4].Value.pszValue);

    UpdateData(FALSE);
    return TRUE;
}

BOOL CIEProxySwitcherDlg::SetProxy()
{
    INTERNET_PER_CONN_OPTION_LIST list;
    BOOL    bReturn;
    DWORD   dwBufSize = sizeof(list);

    // Fill the list structure.
    list.dwSize = sizeof(list);

    // NULL == LAN, otherwise connectoid name.
    list.pszConnection = NULL;

    // Set three options.
    list.dwOptionCount = 3;
    list.pOptions = new INTERNET_PER_CONN_OPTION[5];

    // Ensure that the memory was allocated.
    if(NULL == list.pOptions)
    {
        // Return FALSE if the memory wasn't allocated.
        return FALSE;
    }

    // Set flags.
    if (m_proxyChkBtn) // proxy
    {
        m_connFlags = m_connFlags ^ PROXY_TYPE_DIRECT;
        m_connFlags = m_connFlags | PROXY_TYPE_PROXY;
    }
    else // none proxy
    {
        m_connFlags = m_connFlags ^ PROXY_TYPE_PROXY;
        m_connFlags = m_connFlags | PROXY_TYPE_DIRECT;
    }
    list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
    list.pOptions[0].Value.dwValue = m_connFlags;

    // Set proxy name.
    list.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
    list.pOptions[1].Value.pszValue = (LPSTR)(LPCSTR)m_proxy;

    // Set auto config url
    list.pOptions[2].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;
    list.pOptions[2].Value.pszValue = (LPSTR)(LPCSTR)m_autoUrl;

    // Set proxy override.
    list.pOptions[3].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
    list.pOptions[3].Value.pszValue = (LPSTR)(LPCSTR)m_bypass;

    // Set auto discovery flag
    list.pOptions[4].dwOption = INTERNET_PER_CONN_AUTODISCOVERY_FLAGS;
    list.pOptions[4].Value.dwValue 
        = m_autoDiscoveryFlags;


    // Set the options on the connection.
    bReturn = InternetSetOption(NULL,
        INTERNET_OPTION_PER_CONNECTION_OPTION, &list, dwBufSize);

    bReturn = InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);

    // InternetSetOption(nil, INTERNET_OPTION_REFRESH, nil, 0);
    // Sendmessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, 0); 

    // Free the allocated memory.
    delete [] list.pOptions;
    return bReturn;
}
BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    CString str("Built on ");
    str +=  __DATE__;
    str +=  " ";
    str +=  __TIME__;

    GetDlgItem(IDC_STATIC_BUILT_ON)->SetWindowText(str);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


// OpenCVImageTesterDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "OpenCVImageTester.h"
#include "OpenCVImageTesterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COpenCVImageTesterDlg 대화 상자



COpenCVImageTesterDlg::COpenCVImageTesterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPENCVIMAGETESTER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpenCVImageTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COpenCVImageTesterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BN_OPEN, &COpenCVImageTesterDlg::OnBnClickedBnOpen)
END_MESSAGE_MAP()


// COpenCVImageTesterDlg 메시지 처리기

BOOL COpenCVImageTesterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void COpenCVImageTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void COpenCVImageTesterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if (!m_matOrigin.empty())
		{
			cv::bitwise_or(m_matOrigin, m_matOverlay, m_matDisplay);
			m_fn_DrawcvImage(m_matDisplay, IDC_IMAGE);
		}
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR COpenCVImageTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COpenCVImageTesterDlg::m_fn_OpenImage(std::string strFile)
{
	if(!m_matOrigin.empty())	m_matOrigin.release();

	m_matOrigin = cv::imread(strFile, cv::ImreadModes::IMREAD_UNCHANGED);

	if (!m_matOrigin.empty())
	{
		if (!m_matOverlay.empty())	m_matOverlay.release();

		m_matOverlay = cv::Mat::zeros(m_matOrigin.size(), CV_8UC3);
	}
	OnPaint();
}

void COpenCVImageTesterDlg::m_fn_DrawcvImage(cv::Mat matImage, DWORD id)
{
	RECT r;
	GetDlgItem(id)->GetClientRect(&r);
	cv::Size winSize(r.right, r.bottom);
	int bpp = matImage.channels() * 8;
	m_cImg.Create(winSize.width, winSize.height, bpp);
	HDC hdc = m_cImg.GetDC();
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = bpp;
	bitInfo.bmiHeader.biWidth = matImage.cols;
	bitInfo.bmiHeader.biHeight = -matImage.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;

	SetStretchBltMode(hdc, HALFTONE);
	StretchDIBits(hdc, 0, 0, winSize.width, winSize.height, 0, 0, matImage.cols, matImage.rows, matImage.data, &bitInfo, DIB_RGB_COLORS, SRCCOPY);

	HDC dc = ::GetDC(GetDlgItem(id)->m_hWnd);
	m_cImg.BitBlt(dc, 0, 0);
	::ReleaseDC(GetDlgItem(id)->m_hWnd, dc);

	m_cImg.ReleaseDC();
	m_cImg.Destroy();
}

void COpenCVImageTesterDlg::OnBnClickedBnOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString filter = _T("Image Files (*.bmp;*.jpg;*.png) |*.bmp;*.jpg;*.png|");
	std::string tmp;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, filter, NULL);

	if (dlg.DoModal() == IDOK)
	{
		CString strFilepath = dlg.GetFolderPath() + _T("\\") + dlg.GetFileName();
		this->SetWindowTextW(strFilepath);
		CT2CA pszCntAnsi(strFilepath);
		tmp = pszCntAnsi;

		m_fn_OpenImage(tmp);
	}
}

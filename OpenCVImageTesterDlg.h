
// OpenCVImageTesterDlg.h: 헤더 파일
//

#pragma once


// COpenCVImageTesterDlg 대화 상자
class COpenCVImageTesterDlg : public CDialogEx
{
// 생성입니다.
public:
	COpenCVImageTesterDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCVIMAGETESTER_DIALOG };
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
	cv::Mat m_matOrigin;	/// 이미지 원본 객체
	cv::Mat m_matOverlay;   /// 이미지 오버레이 객체
	cv::Mat m_matDisplay;   /// 실제 보여지는 객체 (원본 + 오버레이)

	ATL::CImage m_cImg;		/// DIB 비트맵 출력 객체

	void m_fn_OpenImage(std::string strFile);
	void m_fn_DrawcvImage(cv::Mat matImage, DWORD id);
public:
	afx_msg void OnBnClickedBnOpen();
};

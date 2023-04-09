
// WinPE_WDSCaptureDlg.h : fichier d'en-tête
//

#pragma once
#pragma warning (disable:4996)

DWORD GetVolumeInfo(LPCTSTR pDriveLetter, LPTSTR pDriveInfoBuffer, DWORD nDriveInfoSize);
VOID GetDiskSpaces(LPCTSTR pDriveLetter, LPTSTR pSpaceInfoBuffer, DWORD nSpaceInfoBufferSize);
CHAR* GetDate();

class CWinPEWDSCaptureDlg : public CDialog{
public:
	CWinPEWDSCaptureDlg(CWnd* pParent = nullptr);	
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINPE_WDSCAPTURE_DIALOG };
#endif
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void EnumeratiLecteurs();
	afx_msg void OnBnClickedAboutbox();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeLecteurs();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton3();
	CProgressCtrl progress1;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnNMClickButton3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickButton5(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickButton1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickButton6(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickButton2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickButton4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickButton7(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickAboutbox(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickCancel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickOk(NMHDR* pNMHDR, LRESULT* pResult);
};

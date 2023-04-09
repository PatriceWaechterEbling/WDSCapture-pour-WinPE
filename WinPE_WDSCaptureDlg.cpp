
// WinPE_WDSCaptureDlg.cpp : fichier d'implémentation
//

#include "pch.h"
#include "framework.h"
#include "WinPE_WDSCapture.h"
#include "WinPE_WDSCaptureDlg.h"
#include "afxdialogex.h"
#include "CONFIGURATION_WDS.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString strAboutMenu;
CHAR appPath[0x104];
TCHAR volumeName[MAX_PATH + 1];
TCHAR fileSysName[MAX_PATH + 1];
TCHAR driveType[MAX_PATH];
TCHAR driveLetter[16];
unsigned __int64 i64NumberOfBytesUsed;
INT mnu = 1;
	int Range, Pos;
UINT iDrvType = 0;
HWND hWnd= (HWND)AfxGetApp() ->m_pMainWnd;
HINSTANCE hMainMod = AfxGetApp()->m_hInstance;
BOOL isW7 = FALSE;
CONFIGURATION_WDS configWDS;
char Reponse[2][4] = { "No","Yes" };
char jours[7][10] = { "dimanche", "lundi","mardi","mercredi","jeudi","vendredi","samedi" };
char mois[12][10] = { "janvier", "fevrier","mars", "avril", "mai", "juin","juillet","aout","septembre", "octobre", "novembre", "decembre" };

DWORD GetVolumeInfo(LPCTSTR pDriveLetter, LPTSTR pDriveInfoBuffer, DWORD nDriveInfoSize) {
	CHAR InfoLecteur[80];
	DWORD serialNumber = 0;
	DWORD maxCompLength = 0;
	DWORD fileSysFlags = 0;
	DWORD lastError = 0;
	BOOL bGetVolInf = FALSE;
	typedef enum tagGetVolumeInfoResult { RESULTS_SUCCESS = 0, RESULTS_GETVOLUMEINFORMATION_FAILED = 1 };
	size_t size = sizeof(driveType) / sizeof(TCHAR);
	bGetVolInf = GetVolumeInformation(pDriveLetter, volumeName, sizeof(volumeName) / sizeof(TCHAR), &serialNumber, &maxCompLength, &fileSysFlags, fileSysName, sizeof(fileSysName) / sizeof(TCHAR));
	if (bGetVolInf == 0) {
		lastError = GetLastError();
		_stprintf_s(pDriveInfoBuffer, nDriveInfoSize, TEXT(" ** Erreur %d lors de la requete d'information sur le lecteur %.1s **"), lastError, pDriveLetter);
		return RESULTS_GETVOLUMEINFORMATION_FAILED;
	}
	iDrvType = GetDriveType(pDriveLetter);
	if (_tcslen(volumeName) == 0) { _stprintf_s(volumeName, sizeof(volumeName) / sizeof(TCHAR), TEXT("%s"), TEXT("Nom indefini")); }
	if (iDrvType == DRIVE_RAMDISK) { _stprintf_s(driveType, size, TEXT("%s"), TEXT("RAM-Disk")); }
	if (iDrvType == DRIVE_CDROM) { _stprintf_s(driveType, size, TEXT("%s"), TEXT("CD-ROM")); return 0; }
	if (iDrvType == DRIVE_FIXED || iDrvType == DRIVE_REMOTE) {
		_stprintf_s(driveType, size, TEXT("%s"), TEXT("Disque Dur"));
		wsprintf(InfoLecteur,"%s [%s] %s %s", pDriveLetter, volumeName,driveType, fileSysName);
		SendDlgItemMessage(hWnd,IDC_LECTEURS, CB_ADDSTRING, 0, (LPARAM)(char *)InfoLecteur);
	}
	return RESULTS_SUCCESS;
}
VOID GetDiskSpaces(LPCTSTR pDriveLetter, LPTSTR pSpaceInfoBuffer, DWORD nSpaceInfoBufferSize) {
	unsigned __int64 i64TotalNumberOfBytes, i64TotalNumberOfFreeBytes, i64FreeBytesAvailableToCaller;
	BOOL bGetDiskFreeSpaceEx = FALSE;
	bGetDiskFreeSpaceEx = GetDiskFreeSpaceEx(pDriveLetter, (PULARGE_INTEGER)&i64FreeBytesAvailableToCaller, (PULARGE_INTEGER)&i64TotalNumberOfBytes, (PULARGE_INTEGER)&i64TotalNumberOfFreeBytes);
	if (bGetDiskFreeSpaceEx == TRUE) {
		wsprintf(pSpaceInfoBuffer, "Utilise: %I64uGo\nDisponible: %I64uGo\nCapacite: %I64uGo",(i64TotalNumberOfBytes- i64TotalNumberOfFreeBytes) / (static_cast<unsigned long long>(1024 * 1024) * 1024), i64TotalNumberOfFreeBytes / (static_cast<unsigned long long>(1024 * 1024) * 1024), i64TotalNumberOfBytes / (static_cast<unsigned long long>(1024 * 1024) * 1024));
		i64NumberOfBytesUsed = (i64TotalNumberOfBytes - i64TotalNumberOfFreeBytes) ;
		Range = (i64TotalNumberOfBytes ) / (static_cast<unsigned long long>(1024 * 1024) * 1024);
		Pos = (i64TotalNumberOfBytes - i64TotalNumberOfFreeBytes) / (static_cast<unsigned long long>(1024 * 1024) * 1024);
	}
}
CHAR* GetDate() {
	SYSTEMTIME st;
	CHAR date[0x09];
	GetLocalTime(&st);
	sprintf(date, "%4d%.2d%.2d", st.wYear, st.wMonth - 1, st.wDay);
	return date;
}
class CAboutDlg : public CDialog{
public:
	CAboutDlg();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX){}
void CAboutDlg::DoDataExchange(CDataExchange* pDX){	CDialog::DoDataExchange(pDX);}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()
CWinPEWDSCaptureDlg::CWinPEWDSCaptureDlg(CWnd* pParent /*=nullptr*/): CDialog(IDD_WINPE_WDSCAPTURE_DIALOG, pParent){	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);}
void CWinPEWDSCaptureDlg::DoDataExchange(CDataExchange* pDX){	CDialog::DoDataExchange(pDX);	DDX_Control(pDX, IDC_PROGRESS1, progress1);}
BEGIN_MESSAGE_MAP(CWinPEWDSCaptureDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDM_ABOUTBOX, &CWinPEWDSCaptureDlg::OnBnClickedAboutbox)
	ON_BN_CLICKED(IDC_BUTTON1, &CWinPEWDSCaptureDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_LECTEURS, &CWinPEWDSCaptureDlg::OnCbnSelchangeLecteurs)
	ON_BN_CLICKED(IDOK, &CWinPEWDSCaptureDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON3, &CWinPEWDSCaptureDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CWinPEWDSCaptureDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CWinPEWDSCaptureDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CWinPEWDSCaptureDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON7, &CWinPEWDSCaptureDlg::OnBnClickedButton7)
	ON_NOTIFY(NM_CLICK, IDC_BUTTON3, &CWinPEWDSCaptureDlg::OnNMClickButton3)
	ON_NOTIFY(NM_CLICK, IDC_BUTTON5, &CWinPEWDSCaptureDlg::OnNMClickButton5)
	ON_NOTIFY(NM_CLICK, IDC_BUTTON1, &CWinPEWDSCaptureDlg::OnNMClickButton1)
	ON_NOTIFY(NM_CLICK, IDC_BUTTON6, &CWinPEWDSCaptureDlg::OnNMClickButton6)
	ON_NOTIFY(NM_CLICK, IDC_BUTTON2, &CWinPEWDSCaptureDlg::OnNMClickButton2)
	ON_NOTIFY(NM_CLICK, IDC_BUTTON4, &CWinPEWDSCaptureDlg::OnNMClickButton4)
	ON_NOTIFY(NM_CLICK, IDC_BUTTON7, &CWinPEWDSCaptureDlg::OnNMClickButton7)
	ON_NOTIFY(NM_CLICK, IDM_ABOUTBOX, &CWinPEWDSCaptureDlg::OnNMClickAboutbox)
	ON_NOTIFY(NM_CLICK, IDCANCEL, &CWinPEWDSCaptureDlg::OnNMClickCancel)
	ON_NOTIFY(NM_CLICK, IDOK, &CWinPEWDSCaptureDlg::OnNMClickOk)
END_MESSAGE_MAP()
BOOL CWinPEWDSCaptureDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr){
		BOOL bNameValid;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty()){pSysMenu->AppendMenu(MF_SEPARATOR);pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);}
	}
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	GetCurrentDirectory(0x100, appPath);
	hWnd = this->m_hWnd;	
	CreateStatusWindowA(WS_CHILD | WS_VISIBLE, appPath,hWnd , 6000);
	isW7= AfxGetApp()->IsWindows7();
	if (!isW7) { MessageBox("Veuillez contacter l'auteur car cette version ne coresspond pas a votre systeme d'exploitation\nRequiere au minimum Windows7", strAboutMenu, MB_ICONHAND); PostQuitMessage(isW7); }
	EnumeratiLecteurs();
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf(configWDS.capture.ImageDescription,"Bienvenue en ce %s,%d %s %d\n", jours[st.wDayOfWeek], st.wDay, mois[st.wMonth - 1], st.wYear);
	SetDlgItemText(6000, configWDS.capture.ImageDescription);
	OnBnClickedButton4();
	OnBnClickedButton3();
	return TRUE;
}
void CWinPEWDSCaptureDlg::OnSysCommand(UINT nID, LPARAM lParam){if ((nID & 0xFFF0) == IDM_ABOUTBOX){CAboutDlg dlgAbout;dlgAbout.DoModal();}else{CDialog::OnSysCommand(nID, lParam);}}
void CWinPEWDSCaptureDlg::OnPaint(){
	if (IsIconic()){
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}else{CDialog::OnPaint();}
}
HCURSOR CWinPEWDSCaptureDlg::OnQueryDragIcon(){return static_cast<HCURSOR>(m_hIcon);}
void CAboutDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized){	CDialog::OnActivate(nState, pWndOther, bMinimized);	CreateStatusWindowA(WS_CHILD | WS_VISIBLE, "A propos de WinPE_WDSCapture...", this->m_hWnd, 6000);}
void CWinPEWDSCaptureDlg::EnumeratiLecteurs(){
	SendDlgItemMessage(IDC_LECTEURS, CB_RESETCONTENT, (WPARAM)0x00, (LPARAM)0x00);
	INT i = 0;
	mnu = 0x01;
	TCHAR driveInfo[MAX_PATH + 1];
	TCHAR spaceInfo[MAX_PATH + 1];
	DWORD dwGetVolInf = 0;
	DWORD drives = GetLogicalDrives();
	CHAR windir[100];
	GetWindowsDirectory(windir, 100);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x9F);
	SetDlgItemText(6000,"Analyse des lecteurs possibles");
	CHAR InfoLecteur[80];
	for (i = 0; i < 26; i++) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
		if ((drives & (1 << i)) != 0) {
			_stprintf_s(driveLetter, sizeof(driveLetter) / sizeof(TCHAR), TEXT("%c:\\"), TEXT('A') + i);
			if (driveLetter[0] == windir[0]) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x8E); }
			if (GetDriveType(driveLetter) == DRIVE_FIXED) {
				GetDiskSpaces(driveLetter, spaceInfo, MAX_PATH + 1);
				dwGetVolInf = GetVolumeInfo(driveLetter, driveInfo, MAX_PATH + 1);
				mnu++;
			}
		}
	}
}
void CWinPEWDSCaptureDlg::OnBnClickedAboutbox(){	CAboutDlg dlgAbout;	dlgAbout.DoModal();}
void CWinPEWDSCaptureDlg::OnBnClickedButton1(){	EnumeratiLecteurs();}
void CWinPEWDSCaptureDlg::OnCbnSelchangeLecteurs(){
	char drv[0x50];
	TCHAR spaceInfo[MAX_PATH + 1];
	SendDlgItemMessage(IDC_LECTEURS, CB_GETLBTEXT, (WPARAM)SendDlgItemMessage(IDC_LECTEURS,CB_GETCURSEL,0x00,0x00), (LPARAM)drv);
	sprintf(configWDS.capture.VolumeToCapture,"%.3s",drv);
	SetDlgItemText(IDC_RACINE, configWDS.capture.VolumeToCapture);
	GetDiskSpaces(configWDS.capture.VolumeToCapture, spaceInfo, MAX_PATH + 1);
	SetDlgItemText(IDC_STAT_HDD, spaceInfo);
	CString tmp = drv;
	tmp.Replace(" ", "_");
	tmp.Replace("[", "");
	tmp.Replace("]", "");
	sprintf(configWDS.capture.DestinationFile, "%s_%s.wim", tmp.GetString(),GetDate());
	SetDlgItemText(IDC_IMAGE_TEMPORAIRE, configWDS.capture.DestinationFile);
	tmp = tmp + GetDate();
	SetDlgItemText(IDC_NOM_IMAGE,	tmp.Right(tmp.GetLength() - 4));
	sprintf(configWDS.capture.ImageName, "%s", tmp.Right(tmp.GetLength() - 4));
	progress1.SetBkColor(RGB(0xCC, 0xCC, 0xEC));
	if (Pos >= (Range * 0.90)) { progress1.SetBarColor(RGB(0xFF, 0x00, 0x00)); }
	if (Pos >= (Range * 0.50)) { progress1.SetBarColor(RGB(0xFF, 0xff, 0x00)); }
	else{
		progress1.SetBarColor(RGB( 0xFF, 0x00,0xFF));
	}
	progress1.SetRange32(0, Range);
	progress1.SetPos(Pos);
	float M_PI = static_cast<float>(100) / Range * Pos;
	sprintf(drv, "%.2f%%", M_PI);
	SetDlgItemText(IDC_STAT_HDD2, drv);
	OnBnClickedButton7();
}
void CWinPEWDSCaptureDlg::OnBnClickedOk(){
	configWDS.capture.Unattended = true;
	configWDS.capture.Overwrite = true;
	configWDS.wds.UploadToWDSServer = true;
	OnBnClickedButton5();
	GetDlgItemText(IDC_RACINE, configWDS.capture.VolumeToCapture, MAX_PATH+1);
	GetDlgItemText(IDC_IMAGE_TEMPORAIRE, configWDS.capture.DestinationFile, MAX_PATH + 1);
	GetDlgItemText(IDC_NOM_IMAGE, configWDS.capture.ImageName, MAX_PATH + 1);
	GetDlgItemText(IDC_DESCRIPTION_IMAGE, configWDS.capture.ImageDescription, MAX_PATH + 1);
	GetDlgItemText(IDC_NOM_SERVEUR_WDS, configWDS.wds.WDSServerName, MAX_PATH + 1);
	GetDlgItemText(IDC_MOT_PASSE_WDS, configWDS.wds.Password, MAX_PATH + 1);
	GetDlgItemText(IDC_NOM_UTILISATEUR_WDS, configWDS.wds.Username, MAX_PATH + 1);
	GetDlgItemText(IDC_GROUPE_IMAGE, configWDS.wds.WDSImageGroup, MAX_PATH + 1);
	configWDS.wds.UploadToWDSServer = (bool)IsDlgButtonChecked(IDC_CHECK2); // true par defaut
	configWDS.capture.Unattended=true; // true par defaut
	configWDS.wds.DeleteLocalWimOnSuccess = (bool)IsDlgButtonChecked(IDC_CHECK1); // false par defaut
	configWDS.capture.Overwrite = true; //Remplace l'image existante (economie d'espace)
	CDialog::OnOK();
}
void CWinPEWDSCaptureDlg::OnBnClickedButton3(){
	strcpy(configWDS.wds.Password, "Password01$");
	strcpy(configWDS.wds.Username, "Jackson5\\Administrator");
	strcpy(configWDS.wds.WDSServerName, "WinAD.jackson5.intra");
	strcpy(configWDS.wds.WDSImageGroup, "ImageGroup1");
	SendDlgItemMessage(IDC_CHECK1, BM_SETCHECK, BST_CHECKED, 0);
	SendDlgItemMessage(IDC_CHECK2, BM_SETCHECK, BST_CHECKED, 0);
	SetDlgItemText(IDC_MOT_PASSE_WDS, configWDS.wds.Password);
	SetDlgItemText(IDC_NOM_UTILISATEUR_WDS, configWDS.wds.Username);
	SetDlgItemText(IDC_NOM_SERVEUR_WDS, configWDS.wds.WDSServerName);
	SetDlgItemText(IDC_GROUPE_IMAGE, configWDS.wds.WDSImageGroup);
	configWDS.wds.DeleteLocalWimOnSuccess, (bool)IsDlgButtonChecked(IDC_CHECK1);
	configWDS.wds.UploadToWDSServer, (bool)IsDlgButtonChecked(IDC_CHECK2);
}
void CWinPEWDSCaptureDlg::OnBnClickedButton2(){
	CHAR szPath[MAX_PATH];
	CHAR szFile[MAX_PATH];
	GetCurrentDirectory(0x103, szPath);
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = this->m_hWnd;
	sprintf(szFile, "%s.wim", configWDS.capture.ImageDescription);
	ofn.lpstrFile =strupr(szFile);
	ofn.nMaxFile = 0x104;
	ofn.lpstrInitialDir = szPath;
	ofn.lpstrFilter = "Fichiers Images\0*.wim\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrTitle = "Enregister l'image sous";
	ofn.lpstrFileTitle = (LPSTR)configWDS.capture.ImageName;
	ofn.nMaxFileTitle = 0x104;
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_EXPLORER | OFN_OVERWRITEPROMPT;
	if (GetSaveFileName(&ofn) == TRUE) {
		HANDLE hf = CreateFile(ofn.lpstrFile, GENERIC_WRITE, 0, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL); //acces au fichier
		SetDlgItemText(IDC_IMAGE_TEMPORAIRE, ofn.lpstrFile);
	}
}
void CWinPEWDSCaptureDlg::OnBnClickedButton4(){
	DWORD lpsys = 80;
	GetComputerName(fileSysName, &lpsys);
	SetDlgItemText(IDC_EDIT2, fileSysName);
	sprintf(configWDS.capture.ImageDescription, "%s_%s_%s", fileSysName, volumeName, GetDate());
	SetDlgItemText(IDC_DESCRIPTION_IMAGE, configWDS.capture.ImageDescription);
}
void CWinPEWDSCaptureDlg::OnBnClickedButton5(){
	GetDlgItemText(IDC_NOM_SERVEUR_WDS, configWDS.wds.WDSServerName, strlen(configWDS.wds.WDSServerName));
	GetDlgItemText(IDC_GROUPE_IMAGE, configWDS.wds.WDSImageGroup, strlen(configWDS.wds.WDSImageGroup));
	GetDlgItemText(IDC_NOM_UTILISATEUR_WDS, configWDS.wds.Username, strlen(configWDS.wds.Username));
	GetDlgItemText(IDC_MOT_PASSE_WDS, configWDS.wds.Password, strlen(configWDS.wds.Password));
	configWDS.wds.DeleteLocalWimOnSuccess = (bool)IsDlgButtonChecked(IDC_CHECK1);
	configWDS.wds.UploadToWDSServer = (bool)IsDlgButtonChecked(IDC_CHECK2);
	MessageBox("Modifitcation des parametres WDS appliquee", "Configuration Serveur WDS", MB_DEFBUTTON1 | MB_ICONEXCLAMATION | MB_OK);
}
void CWinPEWDSCaptureDlg::OnBnClickedButton7(){
	DWORD lpsys = 80;
	GetComputerName(fileSysName, &lpsys);
	SetDlgItemText(IDC_EDIT2, fileSysName);
	sprintf(configWDS.capture.ImageDescription, "CaptureWDS_%s_%s", fileSysName, configWDS.capture.ImageName);
	SetDlgItemText(IDC_DESCRIPTION_IMAGE, configWDS.capture.ImageDescription);
}
void CWinPEWDSCaptureDlg::OnNMClickButton3(NMHDR* pNMHDR, LRESULT* pResult){OnBnClickedButton3();		*pResult = 0;}
void CWinPEWDSCaptureDlg::OnNMClickButton5(NMHDR* pNMHDR, LRESULT* pResult){	OnBnClickedButton5();	*pResult = 0;}
void CWinPEWDSCaptureDlg::OnNMClickButton1(NMHDR* pNMHDR, LRESULT* pResult){	OnBnClickedButton1();	*pResult = 0;}
void CWinPEWDSCaptureDlg::OnNMClickButton6(NMHDR* pNMHDR, LRESULT* pResult){	*pResult = 0;}
void CWinPEWDSCaptureDlg::OnNMClickButton2(NMHDR* pNMHDR, LRESULT* pResult){	OnBnClickedButton2();	*pResult = 0;}
void CWinPEWDSCaptureDlg::OnNMClickButton4(NMHDR* pNMHDR, LRESULT* pResult){	OnBnClickedButton4();	*pResult = 0;}
void CWinPEWDSCaptureDlg::OnNMClickButton7(NMHDR* pNMHDR, LRESULT* pResult){	OnBnClickedButton7();	*pResult = 0;}
void CWinPEWDSCaptureDlg::OnNMClickAboutbox(NMHDR* pNMHDR, LRESULT* pResult){	OnBnClickedAboutbox();	*pResult = 0;}
void CWinPEWDSCaptureDlg::OnNMClickCancel(NMHDR* pNMHDR, LRESULT* pResult){
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	::LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1; 
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	::AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);	
	::ExitWindowsEx(EWX_REBOOT  |EWX_FORCE, 0);
	*pResult = 0;
}
void CWinPEWDSCaptureDlg::OnNMClickOk(NMHDR* pNMHDR, LRESULT* pResult){
	OnBnClickedOk();
	strcat(appPath, "\\wdscapture.inf");
	WritePrivateProfileString("Capture", "Unattended", Reponse[configWDS.capture.Unattended], appPath);
	WritePrivateProfileString("Capture", "VolumeToCapture", configWDS.capture.VolumeToCapture, appPath);
	WritePrivateProfileString("Capture", "SystemRoot","Windows",appPath); //Outre-passer OSCluster (chiant sous Win11)
	WritePrivateProfileString("Capture", "ImageName", configWDS.capture.ImageName, appPath);
	WritePrivateProfileString("Capture", "ImageDescription", configWDS.capture.ImageDescription, appPath);
	WritePrivateProfileString("Capture", "DestinationFile", configWDS.capture.DestinationFile, appPath);
	WritePrivateProfileString("Capture", "Overwrite", Reponse[configWDS.capture.Overwrite], appPath);
	WritePrivateProfileString("WDS", "UploadToWDSServer", Reponse[configWDS.wds.UploadToWDSServer], appPath);
	WritePrivateProfileString("WDS", "WDSServerName", configWDS.wds.WDSServerName, appPath);
	WritePrivateProfileString("WDS", "WDSImageGroup", configWDS.wds.WDSImageGroup, appPath);
	WritePrivateProfileString("WDS", "Username", configWDS.wds.Username, appPath);
	WritePrivateProfileString("WDS", "Password", configWDS.wds.Password, appPath);
	WritePrivateProfileString("WDS", "DeleteLocalWimOnSuccess", Reponse[configWDS.wds.DeleteLocalWimOnSuccess], appPath);
	MessageBox("Configuration ecrite", AfxGetAppName(), 0);
	*pResult = ShellAbout(hWnd, AfxGetAppName(), "Conception: Louis Provencher & Patrice Waechter-Ebling\nDeveloppe pour le cours de Serveur afin de faciliter les captures", AfxGetApp()->LoadIconA(IDI_BLUE_SHIELD));
}

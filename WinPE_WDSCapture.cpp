
// WinPE_WDSCapture.cpp : définit les comportements de classe de l'application.
//

#include "pch.h"
#include "framework.h"
#include "WinPE_WDSCapture.h"
#include "WinPE_WDSCaptureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CWinPEWDSCaptureApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()
CWinPEWDSCaptureApp::CWinPEWDSCaptureApp(){	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;}
CWinPEWDSCaptureApp theApp;
BOOL CWinPEWDSCaptureApp::InitInstance(){
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinApp::InitInstance();
	AfxEnableControlContainer();
	SetRegistryKey(_T("Patrice Waechter-Ebling\\Developpement\\WinPE\\10"));
	CWinPEWDSCaptureDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK){
	}else if (nResponse == IDCANCEL){
	}
	else if (nResponse == -1){
		TRACE(traceAppMsg, 0, "Avertissement : échec de création de la boîte de dialogue, par conséquent, l'application s'arrête de manière inattendue.\n");
	}
#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif
	return FALSE;
}
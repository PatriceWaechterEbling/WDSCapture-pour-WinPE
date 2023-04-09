
// WinPE_WDSCapture.h : fichier d'en-tête principal de l'application PROJECT_NAME
//

#pragma once
#ifndef __AFXWIN_H__
	#error "incluez 'pch.h' avant d'inclure ce fichier pour PCH"
#endif

#include "resource.h"		// symboles principaux
class CWinPEWDSCaptureApp : public CWinApp{
public:
	CWinPEWDSCaptureApp();
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};
extern CWinPEWDSCaptureApp theApp;

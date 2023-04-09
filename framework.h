#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclure les en-têtes Windows rarement utilisés
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // certains constructeurs CString seront explicites
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // supprimer la prise en charge des contrôles MFC dans les boîtes de dialogue
#define _AFX_ALL_WARNINGS
#include <afxwin.h>
#include <afxext.h>
#include <afxdisp.h>
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>  
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxcontrolbars.h>
#include <stdio.h>
#include <math.h>

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
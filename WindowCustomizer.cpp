#include "stdafx.h"
#include "WindowCustomizer.h"
#include "WindowCustomizerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(linker, "/SECTION:.shr,RWS")
#pragma data_seg(".shr")
HWND g_hWnd = NULL;
#pragma data_seg()

BEGIN_MESSAGE_MAP(CWindowCustomizerApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CWindowCustomizerApp::CWindowCustomizerApp()
{
}

CWindowCustomizerApp theApp;

static BOOL bClassRegistered = FALSE;

BOOL CWindowCustomizerApp::InitInstance()
{
	InitCommonControls();

	CWinApp::InitInstance();

	bool AlreadyRunning;

	HANDLE hMutexOneInstance = ::CreateMutex( NULL, TRUE, _T("WindowCustomizerApp-34273420-F00F-44e9-862B-306C8C38DDD2"));

	AlreadyRunning = (GetLastError() == ERROR_ALREADY_EXISTS);

	if (hMutexOneInstance != NULL) 
	{
		::ReleaseMutex(hMutexOneInstance);
	}

	if ( AlreadyRunning )
	{ 
		HWND hOther = g_hWnd;
		if (hOther != NULL)
		{ 
			::SetForegroundWindow(hOther);
			if (IsIconic(hOther))
			{ 
				::ShowWindow(hOther, SW_RESTORE);
			} 
		} 
		return FALSE;
	} 

	CWindowCustomizerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	return FALSE;
}
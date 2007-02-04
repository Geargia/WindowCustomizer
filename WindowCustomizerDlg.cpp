#include "stdafx.h"
#include <stdio.h>
#include "WindowCustomizer.h"
#include "WindowCustomizerDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern HWND g_hWnd;

CWindow::CWindow()
{
	alreadyOnTop = FALSE;
}
void CWindow::Set(HWND hwnd, char * oldName, char * newName, BOOL changeName, BYTE oldTransparency, BYTE newTransparency, BOOL changeTransparency, BOOL oldOnTop, BOOL newOnTop, BOOL changeOnTop, HICON oldSmallIcon, HICON newSmallIcon, BOOL changeSmallIcon, HICON oldBigIcon, HICON newBigIcon, BOOL changeBigIcon, BOOL enforce)
{
	this->hwnd = hwnd;
	this->oldName.Format("%s",oldName);
	this->newName.Format("%s",newName);
	this->changeName = changeName;
	this->oldTransparency = oldTransparency;
	this->newTransparency = newTransparency;
	this->changeTransparency = changeTransparency;
	this->oldOnTop = oldOnTop;
	this->newOnTop = newOnTop;
	this->changeOnTop = changeOnTop;
	this->oldSmallIcon = oldSmallIcon;
	this->newSmallIcon = newSmallIcon;
	this->changeSmallIcon = changeSmallIcon;
	this->oldBigIcon = oldBigIcon;
	this->newBigIcon = newBigIcon;
	this->changeBigIcon = changeBigIcon;
	this->enforce = enforce;
}

BOOL CWindow::IsOnTop(void)
{
	return this->newOnTop;
}
void CWindow::SetNew(char * newName, BOOL changeName, BYTE newTransparency, BOOL changeTransparency, BOOL newOnTop, BOOL changeOnTop, HICON newSmallIcon, BOOL changeSmallIcon, HICON newBigIcon, BOOL changeBigIcon, BOOL enforce)
{
	this->newName.Format("%s",newName);
	this->changeName = changeName;
	this->newTransparency = newTransparency;
	this->changeTransparency = changeTransparency;
	this->newOnTop = newOnTop;
	this->changeOnTop = changeOnTop;
	this->newSmallIcon = newSmallIcon;
	this->changeSmallIcon = changeSmallIcon;
	this->newBigIcon = newBigIcon;
	this->changeBigIcon = changeBigIcon;
	this->enforce = enforce;
}
BOOL CWindow::IsEnforce()
{
	return enforce;
}

void CWindow::StopEnforce()
{
	enforce = FALSE;
}

BOOL CWindow::IsHWND(HWND hwnd)
{
	return (this->hwnd == hwnd);
}

void CWindow::ResetDefaults(void)
{
	newName = oldName;
	newTransparency = oldTransparency;
	newOnTop = oldOnTop;
	changeName = FALSE;
	changeTransparency = FALSE;
	changeOnTop=FALSE;
	newSmallIcon = oldSmallIcon;
	newBigIcon = oldBigIcon;
	changeSmallIcon=FALSE;
	changeBigIcon=FALSE;
	enforce = FALSE;
	this->Change(TRUE);
}

void CWindow::GetFullConfig(HWND * hwnd, char * oldName, char * newName, BOOL * changeName, BYTE * oldTransparency, BYTE * newTransparency, BOOL *changeTransparency, BOOL * oldOnTop, BOOL * newOnTop, BOOL * changeOnTop, BOOL * enforce)
{
	*hwnd = this->hwnd;
	strcpy(oldName,this->oldName);
	strcpy(newName,this->newName);
	*changeName = this->changeName;
	*oldTransparency = this->oldTransparency;
	*newTransparency = this->newTransparency;
	*changeTransparency = this->changeTransparency;
	*oldOnTop = this->oldOnTop;
	*newOnTop = this->newOnTop;
	*changeOnTop = this->changeOnTop;
	*enforce = this->enforce;
}

void CWindow::GetNewConfig(HWND * hwnd, char * newName, BOOL * changeName, BYTE * newTransparency, BOOL *changeTransparency, BOOL * newOnTop, BOOL * changeOnTop, HICON * newSmallIcon, BOOL * changeSmallIcon, HICON *newBigIcon, BOOL *changeBigIcon,BOOL * enforce)
{
	*hwnd = this->hwnd;
	strcpy(newName,this->newName);
	*changeName = this->changeName;
	*newTransparency = this->newTransparency;
	*changeTransparency = this->changeTransparency;
	*newOnTop = this->newOnTop;
	*changeOnTop = this->changeOnTop;
	*enforce = this->enforce;
	*newSmallIcon = this->newSmallIcon;
	*changeSmallIcon = this->changeSmallIcon;
	*newBigIcon = this->newBigIcon;
	*changeBigIcon = this->changeBigIcon;
}


void CWindow::Change(BOOL override)
{
	CWnd *target;
	target = CWnd::FromHandle(hwnd);
	if (::IsWindow(hwnd)) {
		if (changeName || override) {
			target->SendMessage(WM_SETTEXT,0,(LPARAM)(char *) (LPCTSTR) newName);
		}
		if (changeTransparency || override) {
			if (newTransparency == 0xFF) {
				SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
				RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME |
        RDW_ALLCHILDREN);

			} else {
				SetWindowLong(hwnd,GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
				SetLayeredWindowAttributes(hwnd,RGB(0,0,0),newTransparency,LWA_ALPHA);
			}
		}
		if (changeOnTop || override) {
			if (newOnTop && !alreadyOnTop) {
				SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
				alreadyOnTop = TRUE;
			} else if (!newOnTop) {
				SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
			}
		}
		/* FIXME: The icon is destroyed when this window shuts down */
		if (changeSmallIcon || override) {
			target->SendMessage(WM_SETICON,ICON_SMALL,(LPARAM) newSmallIcon);
		}
		if (changeBigIcon || override) {
			target->SendMessage(WM_SETICON,ICON_BIG,(LPARAM) newBigIcon);
		}
	}
	return;
}


void CWindows::Add(HWND hwnd, char * oldName, BYTE oldTransparency, BOOL oldOnTop, HICON oldSmallIcon, HICON oldBigIcon)
{
	for (int iii=0; iii<arr.GetSize();iii++)
	{
		if (((CWindow *)(arr[iii]))->IsHWND(hwnd))
		{ // Already exists; don't overwrite startup "defaults"
			return;
		}
	}
	INT_PTR pos=arr.Add((void *) new CWindow);
	((CWindow *)(arr[pos]))->Set(hwnd,oldName,oldName,FALSE,oldTransparency,oldTransparency,FALSE,oldOnTop,oldOnTop,FALSE,oldSmallIcon,oldSmallIcon,FALSE,oldBigIcon,oldBigIcon,FALSE,FALSE);
	return;
}

void CWindows::ChangeAll(void)
{
	for (int iii=0;iii<arr.GetSize();iii++)
	{
		if (((CWindow *)arr[iii])->IsEnforce())
		{
			((CWindow *)arr[iii])->Change();
		}
	}
	return;
}

int CWindows::GetEnforcedSize(void)
{
	int count = 0;
	for (int iii=0;iii<arr.GetSize();iii++)
	{
		if (((CWindow *)arr[iii])->IsEnforce())
		{
			count++;
		}
	}
	return count;
}

bool CWindows::Find(HWND hwnd)
{
	for (int iii=0; iii<arr.GetSize();iii++)
	{
		if (((CWindow *)(arr[iii]))->IsHWND(hwnd))
		{
			return true;
		}
	}
	return false;
}

CWindows::CWindows()
{
}

int CWindows::GetSize()
{
	return (int) arr.GetSize();
}

void CWindows::DeleteAll()
{
	while(arr.GetSize())
	{
		delete (CWindow *)(arr[0]);
		arr.RemoveAt(0);
	}
	return;
}

CWindow * CWindows::FindChange(HWND hwnd)
{
	for (int iii=0; iii<arr.GetSize();iii++)
	{
		if (((CWindow *)(arr[iii]))->IsHWND(hwnd))
		{
			return (CWindow *) (arr[iii]);
		}
	}
	return NULL;
}

void CWindows::Delete(HWND hwnd)
{
	for (int iii=0; iii<arr.GetSize();iii++)
	{
		if (((CWindow *)(arr[iii]))->IsHWND(hwnd))
		{
			delete (CWindow *)(arr[iii]);
			arr.RemoveAt(iii);
			return;
		}
	}
	return;
}

void CWindow::SetEnforce(void)
{
	enforce = TRUE;
}

// CWindowCustomizerDlg dialog



CWindowCustomizerDlg::CWindowCustomizerDlg(CWnd* pParent) : CDialog(CWindowCustomizerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	imageList = new CImageList;
}

void CWindowCustomizerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NEW_TITLE, m_NewTitle);
	DDX_Control(pDX, IDC_WINDOWS, m_Windows);
	DDX_Control(pDX, IDC_BUTTON1, m_ResetDefaults);
	DDX_Control(pDX, IDC_HIGHLIGHT_WINDOW, m_HighlightWindow);
	DDX_Control(pDX, IDC_AutoChange, m_AutoChange);
	DDX_Control(pDX, IDC_SLIDER1, m_Transparency);
	DDX_Control(pDX, IDC_CHECK3, m_WindowCustomizer);
	DDX_Control(pDX, IDC_EDIT1, m_TransparencyValue);
	DDX_Control(pDX, IDC_TRANSPARENCY, m_ChangeTransparency);
	DDX_Control(pDX, IDC_BUTTON3, m_BigIcon);
	DDX_Control(pDX, IDC_BUTTON4, m_SmallIcon);
	DDX_Control(pDX, IDC_CHECK4, m_ChangeOnTop);
	DDX_Control(pDX, IDC_ONTOPALWAYS, m_OnTopAlways);
	DDX_Control(pDX, IDC_ONTOPREGULAR, m_OnTopRegular);
	DDX_Control(pDX, IDC_SMALL, m_ChangeSmallIcon);
	DDX_Control(pDX, IDC_BIG, m_ChangeBigIcon);
}

BEGIN_MESSAGE_MAP(CWindowCustomizerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedResetDefaults)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedRefreshWindows)
	ON_BN_CLICKED(IDC_HIGHLIGHT_WINDOW, OnBnClickedHighlightWindow)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_WINDOWS, OnLvnItemchangedWindows)
	ON_MESSAGE(CT_TRAY_MESSAGE,OnTrayNotify)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_HELPINFO()
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_NEW_TITLE, OnEnChangeNewTitle)
	ON_BN_CLICKED(IDC_AutoChange, OnBnClickedAutochange)
	ON_BN_CLICKED(IDC_TRANSPARENCY, OnBnClickedTransparency)
	ON_BN_CLICKED(IDC_CHECK3, OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_ONTOPALWAYS, OnBnClickedOntopalways)
	ON_BN_CLICKED(IDC_ONTOPREGULAR, OnBnClickedOntopregular)
	ON_BN_CLICKED(IDC_BIG, OnBnClickedBig)
	ON_BN_CLICKED(IDC_SMALL, OnBnClickedSmall)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CWindowCustomizerDlg message handlers

BOOL CWindowCustomizerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE); SetIcon(m_hIcon, FALSE);

	g_hWnd = this->GetSafeHwnd();

	notUser = true;
	m_NewTitle.SetWindowText("New Title Bar Text");
	m_Windows.InsertColumn(0,"Window",LVCFMT_LEFT,347,-1);
	m_Windows.InsertColumn(1,"Opacity",LVCFMT_LEFT,50,-1);
	m_Windows.InsertColumn(2,"OnTop",LVCFMT_LEFT,45,-1);
	m_Windows.InsertItem(0,"Loading...");
	//m_Windows.SetColumnWidth(0,LVSCW_AUTOSIZE);
	m_Transparency.SetRange(1,0xFF); /* Setting transparency to 0 appears to render the window non clickable! */
	m_Transparency.SetPos(0xFF);
	m_TransparencyValue.SetWindowText("100%");
	UpdateData(true);
	notUser=false;
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
	SetTimer(CT_TIMER, 500, NULL); 
	SetupMinimizeToTray();
	return TRUE;  
}

void CWindowCustomizerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

void CWindowCustomizerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CWindowCustomizerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWindowCustomizerDlg::OnBnClickedResetDefaults()
{
	windows.FindChange(currentHwnd)->ResetDefaults();
	OnBnClickedRefreshWindows();
	FindSelection();
}

void CWindowCustomizerDlg::OnBnClickedRefreshWindows()
{
	m_HighlightWindow.EnableWindow(false);
	m_ResetDefaults.EnableWindow(false);
	m_WindowCustomizer.EnableWindow(false);
	m_NewTitle.EnableWindow(false);
	m_ChangeTransparency.EnableWindow(false);
	m_Transparency.EnableWindow(false);
	m_TransparencyValue.EnableWindow(true);
	m_OnTopAlways.EnableWindow(false);
	m_OnTopRegular.EnableWindow(false);
	m_ChangeOnTop.EnableWindow(false);
	m_SmallIcon.EnableWindow(false);
	m_ChangeSmallIcon.EnableWindow(false);
	m_BigIcon.EnableWindow(false);
	m_ChangeBigIcon.EnableWindow(false);
	m_AutoChange.EnableWindow(false);
	m_Windows.DeleteAllItems();
	imageList->DeleteImageList();
	imageList->Create(16,16, ILC_MASK | ILC_COLOR32, 0,0);
	EnumWindows(EnumCB,(LPARAM) this);
	//m_Windows.SetColumnWidth(0,LVSCW_AUTOSIZE);
	m_Windows.SetImageList(imageList, LVSIL_SMALL);
	m_Windows.SetFocus();
	UpdateData(true);
}

BOOL CALLBACK CWindowCustomizerDlg::EnumCB(HWND hwnd, LPARAM lParam)
{

	CWindowCustomizerDlg * me = (CWindowCustomizerDlg *)lParam;
	return me->EnumCB(CWnd::FromHandle(hwnd));
}

BOOL CWindowCustomizerDlg::EnumCB(CWnd *wnd)
{
	char title[TITLE_BAR_SIZE+2];
	wnd->GetWindowText(title,TITLE_BAR_SIZE);
	if (wnd->IsWindowVisible() && (title[0]))
	{
		LVITEM item;
		::ZeroMemory(&item, sizeof(LVITEM));
		item.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;

		HICON smallIcon=NULL;
		HICON bigIcon=NULL;
		HICON icon = NULL;
		int iImage;
		smallIcon = CopyIcon((HICON) wnd->SendMessage(WM_GETICON,ICON_SMALL,0));
		if (!smallIcon)
			smallIcon = CopyIcon((HICON) (INT_PTR) GetClassLongPtr (wnd->GetSafeHwnd(),GCLP_HICONSM));
		if (smallIcon)
			icon = smallIcon;
		
		bigIcon = CopyIcon((HICON) wnd->SendMessage(WM_GETICON,ICON_BIG,0));
		if (!bigIcon)
			bigIcon =CopyIcon((HICON) (INT_PTR) GetClassLongPtr(wnd->GetSafeHwnd(),GCLP_HICON));

		if (!smallIcon)
			smallIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
		if (!bigIcon)
			bigIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));

		if (!icon)
			icon = bigIcon;

		iImage = imageList->Add(icon);


		BYTE transparency;
		BOOL onTop;
		if (!(::GetLayeredWindowAttributes(wnd->GetSafeHwnd(),NULL,&transparency,NULL)))
			transparency = 0xFF;
		char transparencyStr[5];
		sprintf(transparencyStr,"%d%%",transparency * 100 / 0xFF);
		onTop = (WS_EX_TOPMOST & GetWindowLong(wnd->GetSafeHwnd(),GWL_EXSTYLE));

		item.pszText = title;
		item.lParam = (LPARAM) wnd->GetSafeHwnd();
		item.iImage = iImage;
		windows.Add(wnd->GetSafeHwnd(),title,transparency,onTop,smallIcon,bigIcon);
		int iItem = m_Windows.InsertItem(&item);
		m_Windows.SetItemText(iItem,1,transparencyStr);
		m_Windows.SetItemText(iItem,2,onTop ? "Top" : "");
	}
	return true;
}


void CWindowCustomizerDlg::OnBnClickedHighlightWindow()
{
	CWnd *target;
	UpdateData(false);
	int iItem = m_Windows.GetNextItem(-1, LVNI_SELECTED);
	if (iItem == 0xFFFFFFFF)
	{
		MessageBox("Hrmm... that should not have happened after the last bug fix.\n\nPlease email me!","Weird",MB_ICONEXCLAMATION);
		return;
	}

	LVITEM item;
	::ZeroMemory(&item, sizeof(LVITEM));
	item.iItem = iItem;
	item.iSubItem = 0;
	item.mask = LVIF_PARAM;
	BOOL stat = m_Windows.GetItem(&item);
	if (item.lParam) {
		HWND hwnd = (HWND) item.lParam;
		target = CWnd::FromHandle(hwnd);
		if (!::IsWindow(hwnd))
		{
			MessageBox("The window appears to have disappeared!\nRefreshing Window List...","Missing Window",MB_ICONEXCLAMATION);
			OnBnClickedRefreshWindows();
			return;
		}
		WINDOWPLACEMENT placement;
		target->GetWindowPlacement(&placement);
		if (placement.showCmd == SW_SHOWMAXIMIZED)
			target->ShowWindow(SW_SHOW);
		else 
			target->ShowWindow(SW_SHOWNORMAL);
		target->FlashWindow(true);
		target->BringWindowToTop();
	} else  {
		MessageBox("Target Not Found!\nRefreshing Window List...","Missing Window",MB_ICONEXCLAMATION);
		OnBnClickedRefreshWindows();
	}
}


void CWindowCustomizerDlg::OnLvnItemchangedWindows(NMHDR *pNMHDR, LRESULT *pResult)
{
	HWND hwnd;
	char title[TITLE_BAR_SIZE];
	BOOL WindowCustomizer;
	BYTE transparency;
	BOOL changeTransparency;
	BOOL onTop;
	BOOL changeOnTop;
	HICON smallIcon;
	BOOL changeSmallIcon;
	HICON bigIcon;
	BOOL changeBigIcon;
	BOOL enforce;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if ((pNMListView->uOldState == LVIS_SELECTED) && (pNMListView->uNewState == 0)) {
		if (m_Windows.GetItemState(pNMListView->iItem, LVIS_FOCUSED)) {
			m_Windows.SetItemState(pNMListView->iItem, LVIS_SELECTED, LVIS_SELECTED);
		}
	}

	int iItem = m_Windows.GetNextItem(-1, LVNI_SELECTED);
	if (iItem == 0xFFFFFFFF)
	{
		return;
	}
	LVITEM item;
	::ZeroMemory(&item, sizeof(LVITEM));
	item.iItem = iItem;
	item.iSubItem = 0;
	item.mask = LVIF_PARAM | LVIF_IMAGE;
	BOOL stat = m_Windows.GetItem(&item);

	if (item.lParam) {
		hwnd = (HWND) item.lParam;
		if (!::IsWindow(hwnd))
		{
			MessageBox("The window appears to have disappeared!\nRefreshing Window List...","Missing Window",MB_ICONEXCLAMATION);
			OnBnClickedRefreshWindows();
			return;
		}
		windows.FindChange(hwnd)->GetNewConfig(&hwnd,title,&WindowCustomizer,&transparency,&changeTransparency,&onTop,&changeOnTop,&smallIcon, &changeSmallIcon,&bigIcon,&changeBigIcon,&enforce);
	} else  {
		MessageBox("Target Not Found!\nRefreshing Window List...","Missing Window",MB_ICONEXCLAMATION);
		OnBnClickedRefreshWindows();
		return;
	}

	m_Transparency.SetPos(transparency);
	CString str;
	str.Format("%d%%",transparency*100/255);
	m_TransparencyValue.SetWindowText(str);

	m_ChangeTransparency.SetCheck( changeTransparency ? BST_CHECKED : BST_UNCHECKED);

	m_AutoChange.SetCheck( enforce ? BST_CHECKED : BST_UNCHECKED);
	
	m_WindowCustomizer.SetCheck(WindowCustomizer ? BST_CHECKED : BST_UNCHECKED);

	m_OnTopAlways.SetCheck(onTop ? BST_CHECKED : BST_UNCHECKED);
	m_OnTopRegular.SetCheck(!onTop ? BST_CHECKED : BST_UNCHECKED);

	m_ChangeOnTop.SetCheck(changeOnTop ? BST_CHECKED : BST_UNCHECKED);

	m_SmallIcon.SetIcon(smallIcon);
	m_ChangeSmallIcon.SetCheck(changeSmallIcon ? BST_CHECKED : BST_UNCHECKED);

	m_BigIcon.SetIcon(bigIcon);
	m_ChangeBigIcon.SetCheck(changeBigIcon ? BST_CHECKED : BST_UNCHECKED);

	notUser=true;
	m_NewTitle.SetWindowText(title);
	m_HighlightWindow.EnableWindow(true);
	m_ResetDefaults.EnableWindow(true);
	m_WindowCustomizer.EnableWindow(true);
	m_NewTitle.EnableWindow(true);
	m_ChangeTransparency.EnableWindow(true);
	m_Transparency.EnableWindow(true);
	m_TransparencyValue.EnableWindow(true);
	m_OnTopAlways.EnableWindow(true);
	m_OnTopRegular.EnableWindow(true);
	m_ChangeOnTop.EnableWindow(true);
	m_SmallIcon.EnableWindow(true);
	m_ChangeSmallIcon.EnableWindow(true);
	m_BigIcon.EnableWindow(true);
	m_ChangeBigIcon.EnableWindow(true);
	m_AutoChange.EnableWindow(true);
	UpdateData(true);
	notUser=false;
	currentHwnd = hwnd;
	currentIdx = iItem;
	*pResult = 0;
}

void CWindowCustomizerDlg::FindSelection(void)
{
	LVFINDINFO item;
	item.flags = LVFI_PARAM;
	item.lParam = (LPARAM) currentHwnd;
	int pos = m_Windows.FindItem(&item);
	if (pos != 0xFFFFFFFF)  {
		m_Windows.SetItemState(pos, LVIS_SELECTED, LVIS_SELECTED);
		m_Windows.SetItemState(pos, LVIS_FOCUSED, LVIS_FOCUSED);
	}
	return;
}
void CWindowCustomizerDlg::OnTimer(UINT id)
{
	static bool first_time = false;
	if(first_time == false) {
		OnBnClickedRefreshWindows();
		first_time = true;
		return;
	}
	windows.ChangeAll();
}

afx_msg LRESULT CWindowCustomizerDlg::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	UINT uID; 
	UINT uMsg; 

	uID = (UINT) wParam;
	uMsg = (UINT) lParam; 

	if (uID != 1)
		return 1;

	switch (uMsg ) 
	{  
	case WM_LBUTTONUP:
		MaximizeFromTray();
		break;
	} 
	return 0; 
}

void CWindowCustomizerDlg::SetupMinimizeToTray()
{
	m_TrayData.cbSize = sizeof(NOTIFYICONDATA);
	m_TrayData.hWnd  = this->m_hWnd; 
	m_TrayData.uID = 1;
	m_TrayData.uCallbackMessage  = CT_TRAY_MESSAGE;
	m_TrayData.hIcon = this->m_hIcon;
	m_TrayData.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;

	m_Minimized = false;
}


void CWindowCustomizerDlg::MinimizeToTray()
{
	_snprintf(m_TrayData.szTip, sizeof(m_TrayData.szTip),"Window Customizer Running...(%d window%s being monitored and changed)",windows.GetEnforcedSize(), (windows.GetEnforcedSize() == 1) ? "" : "s");
	BOOL stat = Shell_NotifyIcon(NIM_ADD, &m_TrayData);
	if (!(stat))
	{
		MessageBox("Unable to set tray icon", "Error");
	}

	this->ShowWindow(SW_MINIMIZE);
	this->ShowWindow(SW_HIDE);

	m_Minimized = true;
}

void CWindowCustomizerDlg::MaximizeFromTray()
{
	this->ShowWindow(SW_SHOW);
	this->ShowWindow(SW_RESTORE);

	Shell_NotifyIcon(NIM_DELETE, &m_TrayData);

	m_Minimized = false;
}
void CWindowCustomizerDlg::OnSize(UINT nType, int cx, int cy)
{
	if (nType == SIZE_MINIMIZED)
		MinimizeToTray();
	else
		CDialog::OnSize(nType, cx, cy);
}

void CWindowCustomizerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_Minimized == true)
		Shell_NotifyIcon(NIM_DELETE, &m_TrayData);
}


BOOL CWindowCustomizerDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	static bool message_open = false;
	if (!message_open)
	{
		message_open = true;
		MessageBox("Window Customizer allows you to customize many aspects of any open window; Title Bar texts, opacity (Alpha transparency), Always-on-Top and both the big (Alt-Tab) and small (titlebar/taskbar) icons can all be changed.\n\nTo use, select a window, enter the new parameters, and make sure the \"Change ___\" checkbox is set.\n\nBy selecting the \"Enforce\" option, Window Customizer will monitor the window and ensure that the change is persistant, even if the application attempts to reset its settings.\n\nWhen minimized, the application will be in the system tray, and not in the taskbar.\n\nWindow Customizer was written by Mike Miller (software@mikeage.net)","Window Customizer Quick Start",MB_OK | MB_ICONINFORMATION);
		message_open = false;
	}
	return false;
}

void CWindowCustomizerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CString str;
	str.Format("%d%%",m_Transparency.GetPos()*100/255);
	m_TransparencyValue.SetWindowText(str);
	m_ChangeTransparency.SetCheck(BST_CHECKED);
	SaveCfg();
	UpdateList();
	windows.FindChange(currentHwnd)->Change();
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}



void CWindowCustomizerDlg::OnEnChangeNewTitle()
{
	if (!notUser) {
		m_WindowCustomizer.SetCheck(BST_CHECKED);
		int startPos,endPos;
		m_NewTitle.GetSel(startPos,endPos);
		SaveCfg();
		UpdateList();
		m_NewTitle.SetSel(startPos,endPos,TRUE);
		windows.FindChange(currentHwnd)->Change();
	}
}
void CWindowCustomizerDlg::OnBnClickedAutochange()
{
	SaveCfg();
	UpdateList();
	windows.FindChange(currentHwnd)->Change();
	return;
}

void CWindowCustomizerDlg::OnBnClickedTransparency()
{
	SaveCfg();
	UpdateList();
	windows.FindChange(currentHwnd)->Change();
	m_Transparency.SetFocus();
}

void CWindowCustomizerDlg::OnBnClickedCheck3()
{
	SaveCfg();
	UpdateList();
	windows.FindChange(currentHwnd)->Change();
	m_NewTitle.SetFocus();
}

void CWindowCustomizerDlg::SaveCfg(void)
{
	UpdateData(false);
	char title[TITLE_BAR_SIZE+2];
	m_NewTitle.GetWindowText(title,sizeof(title));
	HICON smallIcon = NULL;
	HICON bigIcon = NULL;
	smallIcon = m_SmallIcon.GetIcon();
	bigIcon = m_BigIcon.GetIcon();
	windows.FindChange(currentHwnd)->SetNew(
		title,
		(m_WindowCustomizer.GetCheck() == BST_CHECKED),
		m_Transparency.GetPos(),
		(m_ChangeTransparency.GetCheck() == BST_CHECKED),
		(m_OnTopAlways.GetCheck() == BST_CHECKED),
		(m_ChangeOnTop.GetCheck() == BST_CHECKED),
		(HICON) smallIcon,
		(m_ChangeSmallIcon.GetCheck() == BST_CHECKED), 
		(HICON) bigIcon,
		(m_ChangeBigIcon.GetCheck() == BST_CHECKED), 
		(m_AutoChange.GetCheck() == BST_CHECKED)
		);
}

CWindowCustomizerDlg::~CWindowCustomizerDlg()
{
	windows.DeleteAll();
	imageList->DeleteImageList();
	delete imageList;
}

void CWindowCustomizerDlg::UpdateList()
{
	char title[TITLE_BAR_SIZE];
	char transparencyStr[5];
	UpdateData(false);
	m_NewTitle.GetWindowText(title,sizeof(title));
	m_TransparencyValue.GetWindowText(transparencyStr,sizeof(transparencyStr));
	

	m_Windows.SetItemText(currentIdx,0,title);
	m_Windows.SetItemText(currentIdx,1,transparencyStr);
	m_Windows.SetItemText(currentIdx,2,(windows.FindChange(currentHwnd)->IsOnTop()) ? "Top" : "");

	UpdateData(true);
}


void CWindowCustomizerDlg::OnBnClickedCheck4()
{
	SaveCfg();
	UpdateList();
	windows.FindChange(currentHwnd)->Change();
	m_OnTopAlways.SetFocus();

}

void CWindowCustomizerDlg::OnBnClickedOntopalways()
{
	if (m_OnTopAlways.GetCheck() == BST_CHECKED)
		m_ChangeOnTop.SetCheck(BST_CHECKED);
	SaveCfg();
	UpdateList();
	windows.FindChange(currentHwnd)->Change();
}

void CWindowCustomizerDlg::OnBnClickedOntopregular()
{
	m_ChangeOnTop.SetCheck(BST_CHECKED);
	SaveCfg();
	UpdateList();
	windows.FindChange(currentHwnd)->Change();
}

void CWindowCustomizerDlg::OnBnClickedBig()
{
	SaveCfg();
//	UpdateList();
	windows.FindChange(currentHwnd)->Change();
}

void CWindowCustomizerDlg::OnBnClickedSmall()
{
	SaveCfg();
//	UpdateList();
	windows.FindChange(currentHwnd)->Change();
}

void CWindowCustomizerDlg::OnBnClickedButton3()
{
	char szFilters[]="Icons (*.ico)|*.ico|All Files (*.*)|*.*||";

   // Create an Open dialog; the default file name extension is ".my".
   CFileDialog fd(TRUE, "ico", "*.ico", OFN_FILEMUSTEXIST| OFN_DONTADDTORECENT, szFilters, this);

	if (IDCANCEL == fd.DoModal())
		return;

	m_ChangeBigIcon.SetCheck(BST_CHECKED);
	CString str = fd.GetPathName();
	HICON icon = (HICON) LoadImage(NULL, str, IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	m_BigIcon.SetIcon(icon);
	SaveCfg();
	windows.FindChange(currentHwnd)->Change();
}

void CWindowCustomizerDlg::OnBnClickedButton4()
{
	char szFilters[]="Icons (*.ico)|*.ico|All Files (*.*)|*.*||";

   // Create an Open dialog; the default file name extension is ".my".
   CFileDialog fd(TRUE, "ico", "*.ico", OFN_FILEMUSTEXIST| OFN_DONTADDTORECENT, szFilters, this);

	if (IDCANCEL == fd.DoModal())
		return;

	m_ChangeSmallIcon.SetCheck(BST_CHECKED);
	CString str = fd.GetPathName();
	HICON icon = (HICON) LoadImage(NULL, str, IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
	m_SmallIcon.SetIcon(icon);
	SaveCfg();
	windows.FindChange(currentHwnd)->Change();
}

void CWindowCustomizerDlg::OnClose()
{
	int numWindows = windows.GetEnforcedSize();
	if (numWindows > 0) {
		CString str;
		str.Format("Are you sure you want to close WindowCustomizer?\n%d window%s currently being monitored",numWindows,numWindows == 1 ? " is" : "s are");
		if (IDNO == MessageBox(str,"Confirm Close", MB_YESNO | MB_ICONSTOP)) {
			return;
		}
	}
	CDialog::OnClose();
}

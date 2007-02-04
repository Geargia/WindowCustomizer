#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#define TITLE_BAR_SIZE 260
#define CT_TIMER (WM_USER + 1)
#define CT_TRAY_MESSAGE (WM_USER + 2)

class CWindow : public CObject {
public:
	CWindow();
	void Set(HWND hwnd, char * oldName, char * newName, BOOL changeName, BYTE oldTransparency, BYTE newTransparency, BOOL changeTransparency, BOOL oldOnTop, BOOL newOnTop, BOOL changeOnTop, HICON oldSmallIcon, HICON newSmallIcon, BOOL changeSmallIcon, HICON oldBigIcon, HICON newBigIcon, BOOL changeBigIcon, BOOL enforce);
	void SetNew(char * newName, BOOL changeName, BYTE newTransparency, BOOL changeTransparency, BOOL newOnTop, BOOL changeOnTop, HICON newSmallIcon, BOOL changeSmallIcon, HICON newBigIcon, BOOL changeBigIcon, BOOL enforce);
	void ResetDefaults();
	void Change(BOOL override = FALSE);
	BOOL IsEnforce();
	BOOL IsHWND(HWND hwnd);
	void GetFullConfig(HWND * hwnd, char * oldName, char * newName, BOOL * changeName, BYTE * oldTransparency, BYTE * newTransparency, BOOL *changeTransparency, BOOL * oldOnTop, BOOL * newOnTop, BOOL * changeOnTop, BOOL * enforce);
	void CWindow::GetNewConfig(HWND * hwnd, char * newName, BOOL * changeName, BYTE * newTransparency, BOOL *changeTransparency, BOOL * newOnTop, BOOL * changeOnTop, HICON * newSmallIcon, BOOL * changeSmallIcon, HICON *newBigIcon, BOOL *changeBigIcon,BOOL * enforce);
	void StopEnforce(void);
	void SetEnforce(void);
	BOOL IsOnTop(void);
private:
	HWND hwnd;
	CString oldName;
	CString newName;
	BYTE oldTransparency; 
	BYTE newTransparency; 
	BOOL oldOnTop;
	BOOL newOnTop;
	BOOL enforce;
	BOOL changeName;
	BOOL changeTransparency;
	BOOL changeOnTop;
	BOOL alreadyOnTop;
	HICON oldSmallIcon;
	HICON newSmallIcon;
	BOOL changeSmallIcon;
	HICON oldBigIcon;
	HICON newBigIcon;
	BOOL changeBigIcon;
};


class CWindows {
public:
	CWindows();
	void Add(HWND hwnd, char * oldName, BYTE oldTransparency, BOOL oldOnTop, HICON oldSmallIcon, HICON oldBigIcon);
	void Set(HWND hwnd, char * newName, BOOL changeName, BYTE newTransparency, BOOL changeTransparency, BOOL newOnTop, BOOL changeOnTop, HICON newSmallIcon, BOOL changeSmallIcon, HICON newBigIcon, BOOL changeBigIcon, BOOL enforce);
	void ChangeAll(void);
	void Delete(HWND hwnd);
	bool Find(HWND hwnd);
	CWindow *  FindChange(HWND hwnd);
	int GetSize(void);
	int GetEnforcedSize(void);
	void DeleteAll(void);

private:
	CPtrArray arr;
};

class CWindowCustomizerDlg : public CDialog
{
public:
	CWindowCustomizerDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_WindowCustomizer_DIALOG };
	static BOOL CALLBACK EnumCB(HWND hwnd, LPARAM lParam);
	BOOL EnumCB(CWnd * wnd);
	afx_msg void OnBnClickedResetDefaults();
	afx_msg void OnBnClickedRefreshWindows();
	afx_msg void OnBnClickedHighlightWindow();
	afx_msg void OnLvnItemchangedWindows(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	CEdit m_NewTitle;
	CListCtrl m_Windows;
	CImageList * imageList;
	CWindows windows;
	CButton m_ResetDefaults;
	CButton m_HighlightWindow;
	NOTIFYICONDATA m_TrayData;
	bool m_Minimized;
	void MaximizeFromTray();
	void MinimizeToTray();
	void SetupMinimizeToTray();
	CButton m_AutoChange;
	bool notUser;
	HWND currentHwnd;
	int currentIdx;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	HICON m_hIcon;

	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT id);
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_Transparency;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CButton m_ChangeTransparency;
	CEdit m_TransparencyValue;
	CButton m_WindowCustomizer;
	afx_msg void OnEnChangeNewTitle();
	CButton m_BigIcon;
	CButton m_SmallIcon;
	void FindSelection();
	afx_msg void OnBnClickedAutochange();
	afx_msg void OnBnClickedTransparency();
	afx_msg void OnBnClickedCheck3();
	void SaveCfg();
	void UpdateList();
	~CWindowCustomizerDlg();
	CButton m_ChangeOnTop;
	CButton m_OnTopAlways;
	afx_msg void OnBnClickedCheck4();
	CButton m_OnTopRegular;
	afx_msg void OnBnClickedOntopalways();
	afx_msg void OnBnClickedOntopregular();
	CButton m_ChangeSmallIcon;
	CButton m_ChangeBigIcon;
	afx_msg void OnBnClickedBig();
	afx_msg void OnBnClickedSmall();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnClose();
};


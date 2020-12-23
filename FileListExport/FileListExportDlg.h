
// FileListExportDlg.h: 헤더 파일
//

#pragma once


// CFileListExportDlg 대화 상자
class CFileListExportDlg : public CDialogEx
{
// 생성입니다.
public:
	CFileListExportDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILELISTEXPORT_DIALOG };
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

public:
	CString		m_strFolerPath;

public:
	void GetFileList(CString strPath);
	void InsertItemDataToListCtrl();
	void initListControl();

public:
	CListCtrl	m_List_File;
	CEdit		m_Edit_Path;

public:
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonExport();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonCurrent();
};

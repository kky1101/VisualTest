
// FileListExportDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "FileListExport.h"
#include "FileListExportDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileListExportDlg 대화 상자

CFileListExportDlg::CFileListExportDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILELISTEXPORT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileListExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE, m_List_File);
	DDX_Control(pDX, IDC_EDIT_PATH, m_Edit_Path);
}

BEGIN_MESSAGE_MAP(CFileListExportDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CFileListExportDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CFileListExportDlg::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CFileListExportDlg::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_CURRENT, &CFileListExportDlg::OnBnClickedButtonCurrent)
END_MESSAGE_MAP()


// CFileListExportDlg 메시지 처리기

BOOL CFileListExportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CFileListExportDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFileListExportDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CFileListExportDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFileListExportDlg::GetFileList(CString strPath)
{
	//검색 클래스
	CFileFind finder;
	int iCount = 0;
	strPath = strPath + _T("\\*.*");
	//CFileFind는 파일, 디렉터리가 존재하면 TRUE 를 반환함
	BOOL bWorking = finder.FindFile(strPath);

	CString strFileName;
	CString strDirName;
	CString strNo;

	InsertItemDataToListCtrl();

	while (bWorking)
	{
		//다음 파일 / 폴더 가 존재하면다면 TRUE 반환
		bWorking = finder.FindNextFile();
		//파일 일때

		if (finder.IsArchived())
		{
			//파일의 이름
			CString _fileName = finder.GetFileName();

			// 현재폴더 상위폴더 썸네일파일은 제외
			if (_fileName == _T(".") || _fileName == _T("..") || _fileName == _T("Thumbs.db"))
			{
				continue;
			}

			strFileName = finder.GetFileName();

			strNo.Format(_T("%d"), iCount + 1);

			m_List_File.InsertItem(iCount, _T(" "));
			m_List_File.SetItemText(iCount, 1, strNo);
			m_List_File.SetItemText(iCount, 2, strFileName);
			iCount++;
		}

		if (finder.IsDirectory())
		{
			strDirName = finder.GetFileName();
		}
	}
}

void CFileListExportDlg::InsertItemDataToListCtrl()
{
	int nCount = m_List_File.GetHeaderCtrl()->GetItemCount();
	CRect rect;

	m_List_File.DeleteAllItems();
	m_List_File.GetClientRect(&rect);

	for (int i = 0; i < nCount; i++)
	{
		m_List_File.DeleteColumn(0);
	}

	initListControl();

	m_List_File.InsertColumn(0, _T(""), LVCFMT_CENTER, 0);
	m_List_File.InsertColumn(1, _T("No"), LVCFMT_CENTER, 50, -1);
	m_List_File.InsertColumn(2, _T("File Name"), LVCFMT_LEFT, 500, -1);
}

void CFileListExportDlg::initListControl()
{
	CRect rect;
	CImageList m_Image;

	m_Image.Create(1, 30, ILC_COLORDDB, 1, 0);
	m_List_File.SetImageList(&m_Image, LVSIL_SMALL);
	m_List_File.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_List_File.GetClientRect(&rect);

	HFONT hNewFont;
	hNewFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_MODERN, _T("Verdana"));
	m_List_File.SendMessage(WM_SETFONT, (WPARAM)hNewFont, (LPARAM)TRUE);
}

void CFileListExportDlg::OnBnClickedButtonOpen()
{
	BROWSEINFO	BrInfo;
	TCHAR		szBuffer[512];                                      // 경로저장 버퍼 
	CString		strPath;

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = _T("파일이 저장될 폴더를 선택하세요");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);

	if (pItemIdList != NULL)
	{
		::SHGetPathFromIDList(pItemIdList, szBuffer);				// 파일경로 읽어오기

		strPath.Format(_T("%s"), szBuffer);
		m_Edit_Path.SetWindowText(strPath);
		GetFileList(strPath);
		m_strFolerPath = strPath;
	}
	else
	{
		return;
	}	
}


void CFileListExportDlg::OnBnClickedButtonRefresh()
{
	GetFileList(m_strFolerPath);
}


void CFileListExportDlg::OnBnClickedButtonExport()
{
	CString dirdir;
	CString strFilename;
	CString strFileDir;
	CString strNo;
	CString strFListName;
	CString strFullPath;
	CString strMsg;

	wchar_t szFileName[128];
	wchar_t szCSVLine[1024];
	char	szFullLine[1024];
	int iLCount = 0;
	int iSize = 0;

	memset(szFileName, NULL, 128);
	memset(szCSVLine, NULL, 1024);

	CTime cTime = CTime::GetCurrentTime(); // 현재 시스템으로부터 날짜 및 시간을 얻어 온다.
	wsprintf(szFileName, _T("Export-%04d-%02d-%02d-%02d%02d%02d"),
		cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(),
		cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

	CFileDialog fileDlg(FALSE, NULL, szFileName, OFN_OVERWRITEPROMPT, _T("CSV (*.csv, *.CSV) | *.csv; *.CSV"));

	int iRtn = fileDlg.DoModal();

	if (iRtn == IDOK)
	{
		iLCount = m_List_File.GetItemCount();

		if (iLCount == 0)
		{
			AfxMessageBox(_T("저장할 파일 리스트가 없습니다."));
			return;
		}
		else
		{
			strFileDir = fileDlg.GetPathName();								// 경로 받기
			strFilename = fileDlg.GetFileName();							// 파일명 받기				
			strFileDir = strFileDir.Left(strFileDir.ReverseFind('\\'));		// 파일 경로 수정
			strFilename = strFilename + (_T(".csv"));							// 파일명 수정
			strFullPath = strFileDir + ("\\") + strFilename;

			LPCTSTR szTemp = (LPCTSTR)strFullPath;
			
			FILE* fp;
			_wfopen_s(&fp, szTemp, _T("w+"));
			wsprintf(szCSVLine, _T("No.,File Name,\n"));

			//입력받은 wchar_t 변수의 길이를 구함
			iSize = WideCharToMultiByte(CP_ACP, 0, szCSVLine, -1, NULL, 0, NULL, NULL);
			//형 변환
			WideCharToMultiByte(CP_ACP, 0, szCSVLine, -1, szFullLine, iSize, 0, 0);
			fprintf_s(fp, szFullLine);

			memset(szCSVLine, NULL, 1024);
			memset(szFullLine, NULL, 1024);

			for (int i = 0; i < iLCount; i++)
			{
				strNo = m_List_File.GetItemText(i, 1);
				strFListName = m_List_File.GetItemText(i, 2);

				wsprintf(szCSVLine, _T("%s,%s,\n"), (LPCTSTR)strNo, (LPCTSTR)strFListName);
				//입력받은 wchar_t 변수의 길이를 구함
				iSize = WideCharToMultiByte(CP_ACP, 0, szCSVLine, -1, NULL, 0, NULL, NULL);
				//형 변환
				WideCharToMultiByte(CP_ACP, 0, szCSVLine, -1, szFullLine, iSize, 0, 0);
				fprintf_s(fp, szFullLine);
				memset(szCSVLine, NULL, 1024);
				memset(szFullLine, NULL, 1024);
			}
			fclose(fp);
			strMsg.Format(_T("저장된 파일을 확인하시겠습니까?"));
			if (IDYES == AfxMessageBox(strMsg, MB_YESNO))
			{
				ShellExecute(NULL, _T("open"), strFileDir, NULL, NULL, SW_SHOWNORMAL);
			}
			GetFileList(m_strFolerPath);
		}		
	}	
}

void CFileListExportDlg::OnBnClickedButtonCurrent()
{
	CString strPath;
	m_Edit_Path.GetWindowText(strPath);

	if (strPath == _T(""))
	{
		return;
	}
	else
	{
		ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOWNORMAL);
	}	
}


BOOL CFileListExportDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F5) 
	{
		GetFileList(m_strFolerPath);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}




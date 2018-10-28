
// MFCApplicationDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "MFCApplication.h"
#include "MFCApplicationDlg.h"
#include "afxdialogex.h"
#include "DlgPersonInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplicationDlg 对话框

CMFCApplicationDlg::CMFCApplicationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplicationDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMFCApplicationDlg::~CMFCApplicationDlg()
{
}

void CMFCApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATA_LIST, _dataList);
	DDX_Control(pDX, IDC_STATIC_ADMIN, _lblAdmin);
	DDX_Control(pDX, IDC_STATIC_PASSWORD, _lblPassword);
	DDX_Control(pDX, IDC_BUTTON_LOGIN, _btnLogin);
	DDX_Control(pDX, IDC_EDIT_ADMIN, _editAdmin);
	DDX_Control(pDX, IDC_EDIT_PASSWAORD, _editPassword);
	DDX_Control(pDX, IDC_BUTTON_ADD, _btnAdd);
	DDX_Control(pDX, IDC_BUTTON_DELETE, _btnDel);
	DDX_Control(pDX, IDC_BUTTON_QUERY, _btnQuery);
	DDX_Control(pDX, IDC_BUTTON_ALTER, _btnAlter);
	DDX_Control(pDX, IDC_BUTTON_SELECTPATH, _btnSelPath);
}

BEGIN_MESSAGE_MAP(CMFCApplicationDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CMFCApplicationDlg::OnBnClickedButtonLogin)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DATA_LIST, &CMFCApplicationDlg::OnLvnItemchangedDataList)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMFCApplicationDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CMFCApplicationDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CMFCApplicationDlg::OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_ALTER, &CMFCApplicationDlg::OnBnClickedButtonAlter)
	ON_BN_CLICKED(IDC_BUTTON_SELECTPATH, &CMFCApplicationDlg::OnBnClickedButtonSelectpath)
END_MESSAGE_MAP()


// CMFCApplicationDlg 消息处理程序

BOOL CMFCApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	HideListView(TRUE);
	CheckDatabaseFile();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CMFCApplicationDlg::PreTranslateMessag(MSG* pMsg)
{
	/*
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}
	CDialogEx::PreTranslateMessage(pMsg);
	*/
	return TRUE;
}
void CMFCApplicationDlg::OnOK()
{

}
void CMFCApplicationDlg::CheckDatabaseFile()
{
	CFileFind pFind;
	::CoInitialize(NULL);
	//如果没找到数据库文件
	if (!pFind.FindFile(DATABASE_PATH))
	{
		//判断文件夹是否存在
		if (!PathIsDirectory(DATABASE_DIRECTORY))
		{
			//没有则创建文件夹
			SECURITY_ATTRIBUTES attrib;
			attrib.bInheritHandle = FALSE;
			attrib.lpSecurityDescriptor = NULL;
			attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
			CreateDirectory(DATABASE_DIRECTORY, &attrib);
		}

		//开始创建数据库
		CString cstrcon;
		cstrcon.Format(DATABASE_LINK_STR, DATABASE_PATH);
		_bstr_t strcnn(cstrcon);
		try
		{
			ADOX::_CatalogPtr pCatalog = NULL;
			_ConnectionPtr pConnection;
			HRESULT hr = pCatalog.CreateInstance(__uuidof(ADOX::Catalog));
			if (FAILED(hr))
			{
				_com_issue_error(hr);
			}
			else
			{
				pCatalog->Create(strcnn);
			}
			//创建表
			hr = pConnection.CreateInstance(__uuidof(Connection));
			if (FAILED(hr))
			{
				_com_issue_error(hr);
			}
			pConnection->Open(strcnn, "", "", adModeUnknown);
			CString sqlCmd;
			//sqlCmd.Format(L"create table employee (firstname varchar(15),lastname varchar(20),age number(3),address varchar(30),city varchar(20));");			
			//字段和对应的类型在CommonSetting.h中声明
			sqlCmd.Format(L"create table %s (", TABLE_NAME);
			int length = sizeof(colNameArray) / sizeof(colNameArray[0]);
			for (int i = 0; i < length; i++)
			{
				CString tmpCstr;
				tmpCstr.Format(L"%s %s", colNameArray[i], colTypeArray[i]);
				sqlCmd += tmpCstr;
				if (i != length - 1)
				{
					sqlCmd += L",";
				}
			}
			sqlCmd += CString(");");
			_variant_t RecordsAffected;
			pConnection->Execute((_bstr_t)sqlCmd, &RecordsAffected, adCmdText);
		}
		catch (_com_error &e)
		{
			ErrorMessage(L"CheckDatabaseFile", e.Description());
		}
	}
	else
	{

	}
}
void CMFCApplicationDlg::HideListView(bool hide)
{
	hide = !hide;
	_dataList.ShowWindow(hide);
	_btnAdd.ShowWindow(hide);
	_btnDel.ShowWindow(hide);
	_btnQuery.ShowWindow(hide);
	_btnAlter.ShowWindow(hide);
}

void CMFCApplicationDlg::HideLoginView(bool hide)
{
	hide = !hide;
	_editAdmin.ShowWindow(hide);
	_editPassword.ShowWindow(hide);
	_lblAdmin.ShowWindow(hide);
	_lblPassword.ShowWindow(hide);
	_btnLogin.ShowWindow(hide);
	_btnSelPath.ShowWindow(hide);
}

void CMFCApplicationDlg::Connect2DataBase()
{
	try
	{
		CString strcnn;
		strcnn.Format(DATABASE_LINK_STR, DATABASE_PATH);
		HRESULT hr;
		hr = m_pConnection.CreateInstance(__uuidof(Connection));
		hr = m_pConnection->Open((_bstr_t)strcnn, "", "", adConnectUnspecified);
		if (SUCCEEDED(hr))
		{
			InitDataListView();			
		}
	}
	catch (_com_error &e)
	{
		ErrorMessage(L"Connect2DataBase", e.Description());
	}
}

void CMFCApplicationDlg::InitDataListView()
{
	_dataList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	// 获取编程语言列表视图控件的位置和大小   
	_dataList.GetClientRect(&rect);
	for (int i = 0; i < 3; i++)
	{
		_dataList.InsertColumn(i, colNameArray[i], LVCFMT_LEFT, rect.Width()/3);
	}
	OnDatabaseUpdate();
}

void CMFCApplicationDlg::OnBnClickedButtonLogin()
{
	// TODO:  在此添加控件通知处理程序代码
	_editAdmin.GetWindowTextW(this->_name);
	/*
	CString cstrAdmin;
	CString cstrPsswd;
	_editAdmin.GetWindowTextW(cstrAdmin);
	_editPassword.GetWindowTextW(cstrPsswd);
	if (cstrAdmin != ADMIN)
	{
	AfxMessageBox(L"账号不过正确");
	return;
	}
	if (cstrPsswd != PASSWORD)
	{
	AfxMessageBox(L"密码不正确");
	return;
	}
	*/
	HideLoginView(TRUE);
	HideListView(FALSE);
	Connect2DataBase();
}


void CMFCApplicationDlg::OnLvnItemchangedDataList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

void CMFCApplicationDlg::ShowMyName()
{
	AfxMessageBox(this->_name);
}

void CMFCApplicationDlg::OnDatabaseUpdate()
{
	_dataList.DeleteAllItems();
	try
	{
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		CString sqlCmd;
		sqlCmd.Format(L"select * from %s;", TABLE_NAME);
		m_pRecordset = m_pConnection->Execute(sqlCmd.AllocSysString(), NULL, adCmdText);
		//HRESULT hr = m_pRecordset->Open(sqlCmd.AllocSysString(), _variant_t((IDispatch *)m_pConnection, true), adOpenStatic, adLockOptimistic, adCmdText);
		while (!m_pRecordset->adoEOF && m_pRecordset!=NULL)
		{
			CString word;
			word = m_pRecordset->GetCollect(colNameArray[0].AllocSysString()).bstrVal;
			_dataList.InsertItem(0, word);
			word = m_pRecordset->GetCollect(colNameArray[1].AllocSysString()).bstrVal;
			_dataList.SetItemText(0, 1, word);
			word = m_pRecordset->GetCollect(colNameArray[2].AllocSysString()).bstrVal;
			_dataList.SetItemText(0, 2, word);
			m_pRecordset->MoveNext();
		}
		m_pRecordset->Close();
	}
	catch (_com_error &e)
	{
		ErrorMessage(L"ProcessSqlCmd", e.ErrorMessage());
	}
}

void CMFCApplicationDlg::OnBnClickedButtonAdd()
{
	// TODO:  在此添加控件通知处理程序代码
	DlgPersonInfo infoDlg;
	infoDlg.SetReadMode(_INFO_CREATE_, m_pConnection);
	infoDlg.DoModal();
}


void CMFCApplicationDlg::OnBnClickedButtonDelete()
{
	// TODO:  在此添加控件通知处理程序代码
	int selectIndex = _dataList.GetSelectionMark();
	if (selectIndex < 0)return;
	CString key = _dataList.GetItemText(selectIndex, 0);
	CString sqlCmd;
	sqlCmd.Format(L"delete from %s where %s=\'%s\'", TABLE_NAME, colNameArray[0], key);
	try{
		HRESULT hr=m_pConnection->Execute((_bstr_t)sqlCmd, NULL, adCmdText);
		if (SUCCEEDED(hr))
		{
			OnDatabaseUpdate();
		}
	}
	catch (_com_error &e)
	{
		ErrorMessage(L"OnBnClickedButtonDelete", e.Description());
	}
	_dataList.SetSelectionMark(-1);
}


void CMFCApplicationDlg::OnBnClickedButtonQuery()
{
	int selectIndex = _dataList.GetSelectionMark();
	if (selectIndex < 0)return;
	// TODO:  在此添加控件通知处理程序代码
	try{
		DlgPersonInfo dlg;		
		dlg.SetReadMode(_INFO_CHECK_, m_pConnection, _dataList.GetItemText(selectIndex, 0));
		int hr = dlg.DoModal();		
	}
	catch (_com_error e)
	{
		ErrorMessage(L"OnBnClickedButtonQuery", e.ErrorMessage());
	}
	_dataList.SetSelectionMark(-1);
}


void CMFCApplicationDlg::OnBnClickedButtonAlter()
{
	int selectIndex = _dataList.GetSelectionMark();
	if (selectIndex < 0)return;

	// TODO:  在此添加控件通知处理程序代码
	try{
		DlgPersonInfo dlg;
		dlg.SetReadMode(_INFO_ALTER_, m_pConnection, _dataList.GetItemText(selectIndex, 0));
		int hr = dlg.DoModal();
	}
	catch (_com_error e)
	{
		ErrorMessage(L"OnBnClickedButtonAlter", e.ErrorMessage());
	}
	_dataList.SetSelectionMark(-1);
}

//能够选择保存路径
void CMFCApplicationDlg::OnBnClickedButtonSelectpath()
{
	// TODO:  在此添加控件通知处理程序代码
	TCHAR szFolderpath[MAX_PATH] = { 0 };
	CString strFolderPath = L"";
	BROWSEINFO brwInfo;
	::ZeroMemory(&brwInfo, sizeof(BROWSEINFO));
	brwInfo.pidlRoot = 0;
	brwInfo.lpszTitle = L"选择保存数据库的路径";
	brwInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_DONTGOBELOWDOMAIN;
	brwInfo.lpfn = NULL;
	LPITEMIDLIST ipidBrowse = ::SHBrowseForFolder(&brwInfo);
	if (ipidBrowse != NULL)
	{
		if (::SHGetPathFromIDList(ipidBrowse, szFolderpath))
		{
			strFolderPath = szFolderpath;
		}
	}
	if (ipidBrowse != NULL)
	{
		::CoTaskMemFree(ipidBrowse);
	}

	AfxMessageBox(strFolderPath);
}

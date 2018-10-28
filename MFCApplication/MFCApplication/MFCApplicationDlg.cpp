
// MFCApplicationDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "MFCApplication.h"
#include "MFCApplicationDlg.h"
#include "afxdialogex.h"
#include "DlgPersonInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplicationDlg �Ի���

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


// CMFCApplicationDlg ��Ϣ�������

BOOL CMFCApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	HideListView(TRUE);
	CheckDatabaseFile();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	//���û�ҵ����ݿ��ļ�
	if (!pFind.FindFile(DATABASE_PATH))
	{
		//�ж��ļ����Ƿ����
		if (!PathIsDirectory(DATABASE_DIRECTORY))
		{
			//û���򴴽��ļ���
			SECURITY_ATTRIBUTES attrib;
			attrib.bInheritHandle = FALSE;
			attrib.lpSecurityDescriptor = NULL;
			attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
			CreateDirectory(DATABASE_DIRECTORY, &attrib);
		}

		//��ʼ�������ݿ�
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
			//������
			hr = pConnection.CreateInstance(__uuidof(Connection));
			if (FAILED(hr))
			{
				_com_issue_error(hr);
			}
			pConnection->Open(strcnn, "", "", adModeUnknown);
			CString sqlCmd;
			//sqlCmd.Format(L"create table employee (firstname varchar(15),lastname varchar(20),age number(3),address varchar(30),city varchar(20));");			
			//�ֶκͶ�Ӧ��������CommonSetting.h������
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
	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	_dataList.GetClientRect(&rect);
	for (int i = 0; i < 3; i++)
	{
		_dataList.InsertColumn(i, colNameArray[i], LVCFMT_LEFT, rect.Width()/3);
	}
	OnDatabaseUpdate();
}

void CMFCApplicationDlg::OnBnClickedButtonLogin()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	_editAdmin.GetWindowTextW(this->_name);
	/*
	CString cstrAdmin;
	CString cstrPsswd;
	_editAdmin.GetWindowTextW(cstrAdmin);
	_editPassword.GetWindowTextW(cstrPsswd);
	if (cstrAdmin != ADMIN)
	{
	AfxMessageBox(L"�˺Ų�����ȷ");
	return;
	}
	if (cstrPsswd != PASSWORD)
	{
	AfxMessageBox(L"���벻��ȷ");
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	DlgPersonInfo infoDlg;
	infoDlg.SetReadMode(_INFO_CREATE_, m_pConnection);
	infoDlg.DoModal();
}


void CMFCApplicationDlg::OnBnClickedButtonDelete()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

//�ܹ�ѡ�񱣴�·��
void CMFCApplicationDlg::OnBnClickedButtonSelectpath()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TCHAR szFolderpath[MAX_PATH] = { 0 };
	CString strFolderPath = L"";
	BROWSEINFO brwInfo;
	::ZeroMemory(&brwInfo, sizeof(BROWSEINFO));
	brwInfo.pidlRoot = 0;
	brwInfo.lpszTitle = L"ѡ�񱣴����ݿ��·��";
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

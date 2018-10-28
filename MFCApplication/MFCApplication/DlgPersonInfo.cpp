// DlgPersonInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication.h"
#include "DlgPersonInfo.h"
#include "afxdialogex.h"
#include "MFCApplicationDlg.h"

// DlgPersonInfo 对话框

IMPLEMENT_DYNAMIC(DlgPersonInfo, CDialogEx)

DlgPersonInfo::DlgPersonInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgPersonInfo::IDD, pParent)
{
	_readMode = _INFO_CHECK_;
	_key = L"";
	InitCtrlData();
}

DlgPersonInfo::~DlgPersonInfo()
{

}

BOOL DlgPersonInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitTextLimit();
	SetReadOnlyMode(_readMode==_INFO_CHECK_);
	UpdatePersonInfo();
	LoadHeadImage();
	return TRUE; 
}


void DlgPersonInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_HEADIMG, _imgHead);
	DDX_Control(pDX, IDC_RICHEDIT_NAME, _editName);
	DDX_Control(pDX, IDC_RICHEDIT_HEIGHT, _editHeight);
	DDX_Control(pDX, IDC_RICHEDIT_PRO, _editPro);
	DDX_Control(pDX, IDC_RICHEDIT_WEIGHT, _editWeight);
	DDX_Control(pDX, IDC_RICHEDIT_BIRTH, _editBirth);
	DDX_Control(pDX, IDC_RICHEDIT_ADDRESS, _editAdress);
	DDX_Control(pDX, IDC_RICHEDIT_STUATION, _editStuation);
	DDX_Control(pDX, IDC_RICHEDIT_SPRIT, _editSprit);
	DDX_Control(pDX, IDC_RICHEDIT_ATTENTION, _editAttention);
	DDX_Control(pDX, IDC_RICHEDIT_TELE, _editTele);
	DDX_Control(pDX, IDC_BUTTON_COMMIT, _btnCommit);
	DDX_Control(pDX, IDC_BUTTON_SAVE, _btnSave);
}


BEGIN_MESSAGE_MAP(DlgPersonInfo, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_COMMIT, &DlgPersonInfo::OnBnClickedButtonCommit)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &DlgPersonInfo::OnBnClickedButtonSave)
END_MESSAGE_MAP()

//初始化要操作的控件
void DlgPersonInfo::InitCtrlData()
{
	_editCtrlArray[0]=&_editTele;
	_editCtrlArray[1] = &_editName;
	_editCtrlArray[2] = &_editHeight;
	_editCtrlArray[3] = &_editPro;
	_editCtrlArray[4] = &_editWeight;
	_editCtrlArray[5] = &_editAdress;
	_editCtrlArray[6] = &_editBirth;
	_editCtrlArray[7] = &_editStuation;
	_editCtrlArray[8] = &_editSprit;
	_editCtrlArray[9] = &_editAttention;	
}

void DlgPersonInfo::InitTextLimit()
{
	//设置字数
	_editTele.LimitText(11);
	_editName.LimitText(10);
	_editHeight.LimitText(3);
	_editPro.LimitText(20);
	_editWeight.LimitText(3);
	_editAdress.LimitText(20);
	_editBirth.LimitText(10);
	_editStuation.LimitText(100);
	_editSprit.LimitText(100);
	_editAttention.LimitText(30);
}
void DlgPersonInfo::SetReadMode(int readMode, _ConnectionPtr cPtr, CString key)
{
	_readMode = readMode;
	_pConnection = cPtr;
	_key = key;
}

void DlgPersonInfo::SetReadOnlyMode(bool only)
{
	int len = sizeof(_editCtrlArray)/sizeof(CRichEditCtrl *);
	_editCtrlArray[0]->SetReadOnly(_readMode!=_INFO_CREATE_);//主键不能动，其实不应该这样设定主键。
	for (int i = 1; i < len; i++)
	{
		_editCtrlArray[i]->SetReadOnly(only);
	}
	_btnSave.ShowWindow(!only);
	_btnCommit.ShowWindow(!only);
}

void DlgPersonInfo::SendUpdateMsg()
{
	HWND hwnd = ::AfxGetMainWnd()->m_hWnd;
	CMFCApplicationDlg *dlg = (CMFCApplicationDlg *)CMFCApplicationDlg::FromHandle(hwnd);
	if (dlg != NULL)
	{
		AfxMessageBox(L"保存成功!");
		dlg->OnDatabaseUpdate();
	}
}

void DlgPersonInfo::LoadHeadImage()
{
	if (_key.IsEmpty()) return;

	CFileFind pFind;
	CString defaultPath = DATABASE_DIRECTORY + L"\\" + _key;
	CString imgPath = defaultPath + L"\\" + _key + "head.bmp";
	if (!PathIsDirectory(defaultPath))
	{
		//没有则创建文件夹
		SECURITY_ATTRIBUTES attrib;
		attrib.bInheritHandle = FALSE;
		attrib.lpSecurityDescriptor = NULL;
		attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
		CreateDirectory(defaultPath, &attrib);
	}
	if (pFind.FindFile(imgPath))
	{
		try
		{
			HBITMAP hbmp = (HBITMAP)LoadImage(NULL, imgPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			_imgHead.SetBitmap(hbmp);
		}
		catch (_com_error &e)
		{
			ErrorMessage(L"LoadHeadImage", e.ErrorMessage());
		}
	}
}

void DlgPersonInfo::SaveImgToDatabase()
{
	try
	{
		if (!_headImgPath.IsEmpty())
		{			
			CString defaultPath = DATABASE_DIRECTORY+L"\\"+_key;
			CString imgPath = defaultPath + L"\\" + _key + "head.bmp";
			CFileFind pFind;
			if (!PathIsDirectory(defaultPath))
			{
				//没有则创建文件夹
				SECURITY_ATTRIBUTES attrib;
				attrib.bInheritHandle = FALSE;
				attrib.lpSecurityDescriptor = NULL;
				attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
				CreateDirectory(defaultPath, &attrib);
			}
			if (pFind.FindFile(imgPath))//文件存在的话先删除
			{
				CFile::Remove(imgPath);
			}
			CImage t_image;
			t_image.Load(_headImgPath);
			HRESULT hr = t_image.Save(imgPath);
			if (FAILED(hr))
			{
				AfxMessageBox(L"保存图片失败");
			}
		}
	}
	catch (_com_error &e)
	{
		ErrorMessage(L"SaveImgToDatabase", e.ErrorMessage());
	}
}

void DlgPersonInfo::SaveInfoToDatabase()
{
	CString sqlCmd;
	sqlCmd.Format(L"insert into %s values (",TABLE_NAME);
	int len = sizeof(_editCtrlArray) / sizeof(CRichEditCtrl *);
	for (int i = 0; i < len; i++)
	{
		CString word;
		_editCtrlArray[i]->GetWindowText(word);	
		if (word.IsEmpty())
		{
			word = L" ";
		}
		word = L"\'" + word + L"\'";
		sqlCmd += word;
		if (i != len - 1)
		{
			sqlCmd += L",";
		}
	}
	sqlCmd += L");";

	try
	{
		HRESULT hr=_pConnection->Execute((_bstr_t)sqlCmd, NULL, adCmdText);	
		if (SUCCEEDED(hr))
		{
			SaveImgToDatabase();
			SendUpdateMsg();
		}
	}
	catch (_com_error &e)
	{
		ErrorMessage(L"SaveInfoToDatabase", e.ErrorMessage());
	}
}

void DlgPersonInfo::UpdatePersonInfo()
{
	if (_pConnection == NULL || _key.IsEmpty()) return;
	CString sqlCmd;
	sqlCmd.Format(L"select * from %s where %s=\'%s\';",TABLE_NAME ,colNameArray[0], _key);
	try
	{
		_RecordsetPtr t_pRecordset;
		t_pRecordset.CreateInstance(__uuidof(Recordset));
		t_pRecordset = _pConnection->Execute((_bstr_t)sqlCmd, NULL, adCmdText);
		int len = sizeof(colNameArray) / sizeof(CString);
		for (int i = 0; i < len; i++)
		{
			_editCtrlArray[i]->SetWindowText(t_pRecordset->GetCollect(colNameArray[i].AllocSysString()).bstrVal);
		}
		t_pRecordset->Close();
	}
	catch (_com_error &e)
	{
		ErrorMessage(L"UpdatePersonInfo", e.ErrorMessage());
	}
}

void DlgPersonInfo::UpdateDatabase()
{
	CString sqlCmd;
	sqlCmd.Format(L"update %s set ", TABLE_NAME);
	int len = sizeof(_editCtrlArray) / sizeof(CRichEditCtrl *);
	for (int i = 1; i < len; i++)
	{
		CString word;
		_editCtrlArray[i]->GetWindowText(word);
		if (word.IsEmpty())
		{
			word = L" ";
		}
		word = L"\'" + word + L"\'";
		CString longWord;
		longWord.Format(L"%s=%s",colNameArray[i],word);
		sqlCmd += longWord;
		if (i != len - 1)
		{
			sqlCmd += ", ";
		}
		else
		{
			sqlCmd += " ";
		}
	}
	sqlCmd += L"where " + colNameArray[0] + L"=\'" + _key + L"\';";

	try
	{
		HRESULT hr = _pConnection->Execute((_bstr_t)sqlCmd, NULL, adCmdText);
		if (SUCCEEDED(hr))
		{
			SaveImgToDatabase();
			SendUpdateMsg();
		}
	}
	catch (_com_error &e)
	{
		ErrorMessage(L"UpdateDatabase", e.Description());
	}
}

// DlgPersonInfo 消息处理程序


void DlgPersonInfo::OnBnClickedButtonCommit()
{
	// TODO:  在此添加控件通知处理程序代码
	CString fillter = L"所有文件(*.bmp,*.jpg,*.gif,*tiff)|*.bmp;*.jpg;*.gif;*.tiff| BMP(*.bmp)|*.bmp| JPG(*.jpg)|*.jpg| GIF(*.gif)|*.gif| TIFF(*.tiff)|*.tiff||";
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, fillter, NULL);
	if (fileDlg.DoModal())
	{
		CString bmpPathName = fileDlg.GetPathName();//获取图片路径名
		CString bmpEntName = fileDlg.GetFileExt();//获取文件后缀名
		bmpEntName.MakeLower();

		if (bmpEntName.Compare(L"bmp") || bmpEntName.Compare(L"BMP") || bmpEntName.Compare(L"jpg") || bmpEntName.Compare(L"JPG"))
		{
			/*
			BITMAPINFO *pBmpInfo;//图像细节信息
			BYTE *pBmpData;//图像二进制数据
			BITMAPFILEHEADER bmpHeader; //文件头
			BITMAPINFOHEADER bmpInfo; //信息头
			CFile bmpFile;//记录打开的文件
			*/
			try
			{
				/*
				//以只读的方式打开文件，读取bmp图片各部分 bmp文件头，信息，细节
				if (!bmpFile.Open(bmpPathName, CFile::modeRead | CFile::typeBinary))
				{
					ErrorMessage(L"OnBnClickedButtonCommit", L"打开图像文件失败！");
					return;
				}
				if (bmpFile.Read(&bmpHeader, sizeof(BITMAPFILEHEADER)) != sizeof(BITMAPFILEHEADER))
				{
					ErrorMessage(L"OnBnClickedButtonCommit", L"读取图像文件头失败！BITMAPFILEHEADER大小不相等");
					return;
				}
				if (bmpFile.Read(&bmpInfo, sizeof(BITMAPINFOHEADER)) != sizeof(BITMAPINFOHEADER))
				{
					ErrorMessage(L"OnBnClickedButtonCommit", L"读取图像信息头失败！BITMAPINFOHEADER大小不相等");
					return;
				}
				pBmpInfo = (BITMAPINFO *)new char[sizeof(BITMAPINFOHEADER)];
				//为图像数据申请空间,之前的准备都是为了得到大小dataBytes，和转成二进制数据pBmpData
				memcpy(pBmpInfo, &bmpInfo, sizeof(BITMAPINFOHEADER));
				DWORD dataBytes = bmpHeader.bfSize - bmpHeader.bfOffBits;
				pBmpData = (BYTE *)new BYTE[dataBytes];
				bmpFile.Read(pBmpData, dataBytes);
				bmpFile.Close();
				//显示图像
				BITMAP bmp;
				GetObject(pBmpData, sizeof(BITMAP), (void *)&bmp);
				BITMAPINFOHEADER *m_pHeader = new BITMAPINFOHEADER();
				ZeroMemory(m_pHeader, sizeof(BITMAPINFOHEADER));
				*/
				
				HBITMAP hbmp = (HBITMAP)LoadImage(NULL, bmpPathName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);				
				BITMAP bmp;
				/*
				GetObject(hbmp, sizeof(BITMAP), (void *)&bmp);	
				BITMAPINFOHEADER *m_pHeader=new BITMAPINFOHEADER();
				ZeroMemory(m_pHeader,sizeof(BITMAPINFOHEADER));
				m_pHeader->biSize = sizeof(BITMAPINFOHEADER);
				m_pHeader->biWidth = bmp.bmWidth;
				m_pHeader->biHeight = bmp.bmHeight;
				m_pHeader->biPlanes = bmp.bmPlanes;
				m_pHeader->biBitCount = bmp.bmBitsPixel*(WORD)bmp.bmPlanes;
				m_pHeader->biSizeImage = bmp.bmWidthBytes*bmp.bmHeight;
				BYTE* m_pData = new BYTE[bmp.bmWidthBytes*bmp.bmHeight];
				HDC dc = ::GetDC(NULL);
				GetDIBits(dc, hbmp, 0, bmp.bmHeight, m_pData, (BITMAPINFO*)m_pHeader, DIB_RGB_COLORS);
				*/
				//显示图像
				/*
				CBitmap *pCbitmap=CBitmap::FromHandle(hbmp);
				pCbitmap->GetObject(sizeof(BITMAP), &bmp);
				int szCount = bmp.bmWidthBytes*bmp.bmHeight;
				BYTE *szBuffer = new BYTE[szCount];
				pCbitmap->GetBitmapBits(szCount, szBuffer);
				*/
				_imgHead.SetBitmap(hbmp);		
				_headImgPath = bmpPathName;
			}
			catch (_com_error &e)
			{
				ErrorMessage(L"OnBnClickedButtonCommit", e.ErrorMessage());
			}
		}
	}
}



void DlgPersonInfo::OnBnClickedButtonSave()
{
	// TODO:  在此添加控件通知处理程序代码
	switch (_readMode)
	{
	case _INFO_ALTER_:
		UpdateDatabase();
		break;
	case _INFO_CREATE_:
		SaveInfoToDatabase();
		break;
	default:
		break;
	}
}

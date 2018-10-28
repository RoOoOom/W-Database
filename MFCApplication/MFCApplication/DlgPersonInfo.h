#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// DlgPersonInfo 对话框
CONST int _INFO_CHECK_ = 0;
CONST int _INFO_ALTER_ = 1;
CONST int _INFO_CREATE_ = 2;
class DlgPersonInfo : public CDialogEx
{
	DECLARE_DYNAMIC(DlgPersonInfo)

public:
	DlgPersonInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgPersonInfo();

// 对话框数据
	enum { IDD = IDD_DLG_PERSON_INFO };

protected:
	virtual BOOL OnInitDialog( );
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
protected:
	void InitCtrlData();
	_ConnectionPtr _pConnection;
	CString _headImgPath;
	CRichEditCtrl *_editCtrlArray[10];
	void UpdatePersonInfo();
	void InitTextLimit();
	void LoadHeadImage();
	void SaveImgToDatabase();
	void SaveInfoToDatabase();
	void UpdateDatabase();
	void SendUpdateMsg();
public:
	int _readMode;
	//_ConnectionPtr _pConnection;
	CString _key;
	void SetReadMode(int readMode, _ConnectionPtr cPtr, CString key=L"");
	void SetReadOnlyMode(bool only);
public:
	afx_msg void OnBnClickedButtonCommit();
	afx_msg void OnStnClickedStaticHeadimg();
	CStatic _imgHead;
	CRichEditCtrl _editName;
	CRichEditCtrl _editHeight;
	CRichEditCtrl _editPro;
	CRichEditCtrl _editWeight;
	CRichEditCtrl _editBirth;
	CRichEditCtrl _editAdress;
	CRichEditCtrl _editStuation;
	CRichEditCtrl _editSprit;
	CRichEditCtrl _editAttention;
	CRichEditCtrl _editTele;
	CButton _btnCommit;
	CButton _btnSave;
	afx_msg void OnBnClickedButtonSave();
};



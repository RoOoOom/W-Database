
// MFCApplicationDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#import "msadox.dll"

// CMFCApplicationDlg 对话框
class CMFCApplicationDlg : public CDialogEx
{
// 构造
public:
	CMFCApplicationDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CMFCApplicationDlg();
// 对话框数据
	enum { IDD = IDD_MFCAPPLICATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CString _name;
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;
public:
	void InitDataListView();//初始化列表视图
	void ShowMyName();
	void OnDatabaseUpdate(); //数据库刷新回调
protected:
	void CheckDatabaseFile();//查看和创建Access数据库
	void HideListView(bool hide);//隐藏列表界面
	void HideLoginView(bool hide);//隐藏登录界面
	void Connect2DataBase();//登录成功后连接到数据库s	
protected:
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessag(MSG *pMsg);
	void OnOK();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl _dataList;
	CStatic _lblAdmin;
	CStatic _lblPassword;
	CButton _btnLogin;
	CEdit _editAdmin;
	CEdit _editPassword;
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnLvnItemchangedDataList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnBnClickedButtonAlter();
	CButton _btnAdd;
	CButton _btnDel;
	CButton _btnQuery;
	CButton _btnAlter;
	afx_msg void OnBnClickedButtonSelectpath();
	CButton _btnSelPath;
};


// MFCApplication.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "Resource.h"		// 主符号

#define DATABASE_PATH _T("E:\\WYData\\wydata.mdb")
#define DATABASE_LINK_STR _T("Provider=Microsoft.JET.OLEDB.4.0;Data source=%s")
#define TABLE_NAME _T("Tblwy")
#define ADMIN _T("WeiYi") //写死管理者账号
#define PASSWORD _T("wy7316") //写死管理者密码

const LPCSTR DEFAULT_DATABASE_PATH = "E:\\WYData\\wydata.mdb";
//const LPCSTR DATABASE_PATH = "E:\\WYData\\wydata.mdb";
const CString DATABASE_DIRECTORY = L"E:\\WYData";
const CString DATABASE_FILENAME = L"wydata.mdb";

const CString colNameArray[] = { L"电话号码", L"姓名", L"身高", L"职位", L"体重", L"地址", L"生日", L"身体状况", L"精神", L"训练目的" };//字段
const CString colTypeArray[] = { L"varchar(11)", L"varchar(30)", L"varchar(30)", L"varchar(30)", L"varchar(30)", L"varchar(30)", L"varchar(30)", L"varchar(100)", L"varchar(100)", L"varchar(60)" };//字段类型

inline void ErrorMessage(LPCWSTR posStr, const TCHAR *msg)
{
	CString errorMsg;
	errorMsg.Format(L"error where:%s,%s", posStr, msg);
	AfxMessageBox(errorMsg);
}

// CMFCApplicationApp: 
// 有关此类的实现，请参阅 MFCApplication.cpp
//

class CMFCApplicationApp : public CWinApp
{
public:
	CMFCApplicationApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CMFCApplicationApp theApp;
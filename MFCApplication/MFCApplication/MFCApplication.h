
// MFCApplication.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "Resource.h"		// ������

#define DATABASE_PATH _T("E:\\WYData\\wydata.mdb")
#define DATABASE_LINK_STR _T("Provider=Microsoft.JET.OLEDB.4.0;Data source=%s")
#define TABLE_NAME _T("Tblwy")
#define ADMIN _T("WeiYi") //д���������˺�
#define PASSWORD _T("wy7316") //д������������

const LPCSTR DEFAULT_DATABASE_PATH = "E:\\WYData\\wydata.mdb";
//const LPCSTR DATABASE_PATH = "E:\\WYData\\wydata.mdb";
const CString DATABASE_DIRECTORY = L"E:\\WYData";
const CString DATABASE_FILENAME = L"wydata.mdb";

const CString colNameArray[] = { L"�绰����", L"����", L"���", L"ְλ", L"����", L"��ַ", L"����", L"����״��", L"����", L"ѵ��Ŀ��" };//�ֶ�
const CString colTypeArray[] = { L"varchar(11)", L"varchar(30)", L"varchar(30)", L"varchar(30)", L"varchar(30)", L"varchar(30)", L"varchar(30)", L"varchar(100)", L"varchar(100)", L"varchar(60)" };//�ֶ�����

inline void ErrorMessage(LPCWSTR posStr, const TCHAR *msg)
{
	CString errorMsg;
	errorMsg.Format(L"error where:%s,%s", posStr, msg);
	AfxMessageBox(errorMsg);
}

// CMFCApplicationApp: 
// �йش����ʵ�֣������ MFCApplication.cpp
//

class CMFCApplicationApp : public CWinApp
{
public:
	CMFCApplicationApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCApplicationApp theApp;
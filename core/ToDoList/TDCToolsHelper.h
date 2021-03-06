// ToolsHelper.h: interface for the CToolsHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLSHELPER_H__6BAD432D_0189_46A9_95ED_EF869CFC6CE1__INCLUDED_)
#define AFX_TOOLSHELPER_H__6BAD432D_0189_46A9_95ED_EF869CFC6CE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDCToolsCmdlineParser.h"
#include "TDCcustomattribdata.h"

#include "..\shared\menuiconmgr.h"

//////////////////////////////////////////////////////////////////////

struct USERTOOL
{
	CString sToolName;
	CString sToolPath;
	CString sCmdline;
	BOOL bRunMinimized;
	CString sIconPath;
};

struct USERTOOLARGS
{
	CString sTasklist;
	CString sTaskIDs;
	CString sTaskTitle;
	CString sTaskExtID;
	CString sTaskComments;
	CString sTaskFileLink;
	CString sTaskAllocBy;
	CString sTaskAllocTo;

	CTDCCustomAttributeDataMap mapTaskCustData;
};

//////////////////////////////////////////////////////////////////////

class CSysImageList;
class CPreferencesDlg;
class CEnToolBar;

//////////////////////////////////////////////////////////////////////

typedef CArray<USERTOOL, USERTOOL&> CUserToolArray;

//////////////////////////////////////////////////////////////////////

class CTDCToolsHelper  
{
public:
	CTDCToolsHelper(BOOL bTDLEnabled, UINT nStart, int nSize = 16);
	virtual ~CTDCToolsHelper();
	
	void UpdateMenu(CCmdUI* pCmdUI, const CUserToolArray& tools, CMenuIconMgr& iconMgr);
	BOOL RunTool(const USERTOOL& tool, const USERTOOLARGS& args);
	BOOL TestTool(const USERTOOL& tool, const USERTOOLARGS& args);
	void AppendToolsToToolbar(const CUserToolArray& aTools, CEnToolBar& toolbar, UINT nCmdAfter);
	void RemoveToolsFromToolbar(CEnToolBar& toolbar, UINT nCmdAfter);
	
protected:
	UINT m_nStartID;
	int m_nSize;
	BOOL m_bTDLEnabled;
	
protected:
	BOOL PrepareCmdline(const USERTOOL& tool, const USERTOOLARGS& args, 
						BOOL bEscapeSpaces, CString& sCmdline);
   	LPCTSTR GetFileFilter();
	LPCTSTR GetDefaultFileExt();
	BOOL CheckToDoListVersionCompatibility(const CString& sToolPath) const;
	BOOL RunTestTool(const USERTOOL& tool, const USERTOOLARGS& args, BOOL bTest);

	static HICON GetToolIcon(CSysImageList& sil, const USERTOOL& ut);
	static CString GetToolPath(const USERTOOL& tool);
	static BOOL GetToolPaths(const USERTOOL& tool, CString& sToolPath, CString& sIconPath);

	static BOOL ReplaceToolArgument(CTDCToolsCmdlineParser& tcp, CLA_TYPE nType, 
									const CString& sValue, BOOL bEscapeSpaces);
	static BOOL ReplaceToolArgument(CTDCToolsCmdlineParser& tcp, const CString& sName, 
									const CString& sValue, BOOL bEscapeSpaces);

};

#endif // !defined(AFX_TOOLSHELPER_H__6BAD432D_0189_46A9_95ED_EF869CFC6CE1__INCLUDED_)

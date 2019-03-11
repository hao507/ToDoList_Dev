// KanbanTreeList.h: interface for the CKanbanTreeList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANBANCTRLEX_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
#define AFX_KANBANCTRLEX_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Kanbanstruct.h"
#include "Kanbanenum.h"
#include "KanbanlistctrlEx.h"

#include "..\shared\graphicsmisc.h"
#include "..\shared\fontcache.h"
#include "..\shared\mapex.h"
#include "..\shared\enheaderctrl.h"

#include "..\Interfaces\itasklist.h"
#include "..\Interfaces\iuiextension.h"

#include <AFXTEMPL.H>

/////////////////////////////////////////////////////////////////////////////

class CThemed;

/////////////////////////////////////////////////////////////////////////////

class CKanbanCtrlEx : public CWnd  
{
public:
	CKanbanCtrlEx();
	virtual ~CKanbanCtrlEx();

	BOOL Create(DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID);

	void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib);
	bool PrepareNewTask(ITaskList* pTask) const;

	DWORD GetSelectedTaskID() const;
	BOOL SelectTask(DWORD dwTaskID);
	BOOL SelectTask(IUI_APPCOMMAND nCmd, const IUISELECTTASK& select);

	DWORD GetNextTask(DWORD dwTaskID, IUI_APPCOMMAND nCmd) const;
	DWORD HitTestTask(const CPoint& ptScreen) const;
	void ScrollToSelectedTask();

	BOOL SaveToImage(CBitmap& bmImage);
	BOOL CanSaveToImage() const;

	bool ProcessMessage(MSG* pMsg);
	void FilterToolTipMessage(MSG* pMsg);

 	void Sort(IUI_ATTRIBUTE nBy, BOOL bAscending);

	void SetOption(DWORD dwOption, BOOL bSet = TRUE);
	BOOL HasOption(DWORD dwOption) const { return (m_dwOptions & dwOption); }

	void SetDisplayAttributes(const CKanbanAttributeArray& aAttrib);
	int GetDisplayAttributes(CKanbanAttributeArray& aAttrib) const;

	BOOL CancelOperation();
	void SetReadOnly(bool bReadOnly);
	BOOL GetLabelEditRect(LPRECT pEdit);
	void SetPriorityColors(const CDWordArray& aColors);

	int GetVisibleColumnCount() const { return GetVisibleListCtrlCount(); }
	int GetVisibleTaskCount() const { return m_aListCtrls.GetVisibleTaskCount(); }

	int GetTaskTrackedAttributeValues(DWORD dwTaskID, CStringArray& aValues) const;
	int GetAttributeValues(IUI_ATTRIBUTE nAttrib, CStringArray& aValues) const;
	void LoadDefaultAttributeListValues(const IPreferences* pPrefs);
	BOOL TrackAttribute(IUI_ATTRIBUTE nAttrib, const CString& sCustomAttribID, const CKanbanColumnArray& aColumnDefs);
	IUI_ATTRIBUTE GetTrackedAttribute() const { return m_nTrackAttribute; }
	IUI_ATTRIBUTE GetTrackedAttribute(CString& sCustomAttrib) const;

	const CKanbanCustomAttributeDefinitionArray& GetCustomAttributeDefinitions() const { return m_aCustomAttribDefs; }
	int GetAttributeValues(CKanbanAttributeValueMap& mapValues) const;

	BOOL WantSortUpdate(IUI_ATTRIBUTE nAttribute) const;
	BOOL WantEditUpdate(IUI_ATTRIBUTE nAttribute) const;

protected:
	BOOL m_bDragging;
	BOOL m_bReadOnly;
	BOOL m_bSortAscending;
	BOOL m_bStrikeThruDone;
	BOOL m_bSelectTasks;
	BOOL m_bSettingListFocus;

	DWORD m_dwOptions;
	UINT m_nNextColor;
	IUI_ATTRIBUTE m_nTrackAttribute, m_nSortBy;
	CString m_sTrackAttribID;
	CDWordArray m_aPriorityColors;
	CFontCache m_fonts;

	CKanbanListCtrlEx* m_pSelectedList;
	CKanbanListCtrlExArray m_aListCtrls;
	CEnHeaderCtrl m_header;

	CKanbanItemMap m_data;
	CKanbanAttributeValueMap m_mapAttributeValues;
	CKanbanAttributeValueMap m_mapGlobalAttributeValues;
	CKanbanAttributeArray m_aDisplayAttrib;
	CKanbanColumnArray m_aColumnDefs;
	CKanbanCustomAttributeDefinitionArray m_aCustomAttribDefs;

protected:
	// Generated message map functions
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnListItemSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListEditLabel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginDragListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnListCheckChange(WPARAM wp, LPARAM lp);
	afx_msg void OnListSetFocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnListGetTaskIcon(WPARAM wp, LPARAM lp);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* );
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnSetFont(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSelectTask(WPARAM wp, LPARAM lp);
	afx_msg void OnHeaderCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	void RedrawListCtrls(BOOL bErase = FALSE);
	void RemoveDeletedTasks(const ITASKLISTBASE* pTasks);
	void Resize();
	void Resize(const CRect& rect);
	void ClearOtherListSelections(const CKanbanListCtrlEx* pList);

	void RebuildListCtrls(BOOL bRebuildData, BOOL bTaskUpdate);
	void RebuildDynamicListCtrls(const CKanbanItemArrayMap& mapKIArray);
	void RebuildFixedListCtrls(const CKanbanItemArrayMap& mapKIArray);
	int RemoveOldDynamicListCtrls(const CKanbanItemArrayMap& mapKIArray);
	int AddMissingDynamicListCtrls(const CKanbanItemArrayMap& mapKIArray);
	BOOL CheckAddBacklogListCtrl();
	void RebuildListCtrlData(const CKanbanItemArrayMap& mapKIArray);
	int GetVisibleListCtrlCount() const;
	void RebuildHeaderColumns();

	KBC_ATTRIBLABELS GetListAttributeLabelVisibility(int nListWidth);
	float GetAverageListCharWidth();
	int CalcRequiredColumnWidthForImage() const;
	BOOL CanFitAttributeLabels(int nAvailWidth, float fAveCharWidth, KBC_ATTRIBLABELS nLabelVis) const;

	KANBANITEM* GetKanbanItem(DWORD dwTaskID) const;
	BOOL HasKanbanItem(DWORD dwTaskID) const;

	CKanbanListCtrlEx* LocateTask(DWORD dwTaskID, HTREEITEM& hti, BOOL bForward) const;
	CKanbanListCtrlEx* GetListCtrl(const CString& sAttribValue) const;
	CKanbanListCtrlEx* GetListCtrl(HWND hwnd) const;
	CKanbanListCtrlEx* HitTestListCtrl(const CPoint& ptScreen, BOOL* pbHeader = NULL) const;

	CKanbanListCtrlEx* AddNewListCtrl(const KANBANCOLUMN& colDef);
	CKanbanListCtrlEx* GetSelListCtrl();
	CKanbanListCtrlEx* GetNextListCtrl(const CKanbanListCtrlEx* pList, BOOL bNext, BOOL bExcludeEmpty);

	const CKanbanListCtrlEx* GetSelListCtrl() const;
	const CKanbanListCtrlEx* GetNextListCtrl(const CKanbanListCtrlEx* pList, BOOL bNext, BOOL bExcludeEmpty) const;

	BOOL SelectListCtrl(CKanbanListCtrlEx* pList, BOOL bNotifyParent = TRUE);
	BOOL IsSelectedListCtrl(HWND hWnd) const;
	void FixupSelectedList();
	void FixupListFocus();
	BOOL DeleteListCtrl(int nList);
	BOOL HasFocus() const;
	BOOL SelectClosestAdjacentItemToSelection(int nAdjacentList);
	BOOL SelectNextItem(int nIncrement);

	inline BOOL UsingFixedColumns() const { return m_aColumnDefs.GetSize(); }
	inline BOOL UsingDynamicColumns() const { return !UsingFixedColumns(); }

	BOOL IsDragging() const;
	BOOL EndDragItem(CKanbanListCtrlEx* pSrcList, DWORD dwTaskID, CKanbanListCtrlEx* pDestList, const CString& sDestAttribValue);
	BOOL HandleKeyDown(WPARAM wp, LPARAM lp);
	
	BOOL NotifyParentAttibuteChange(DWORD dwTaskID);
	void NotifyParentSelectionChange();
	BOOL GetListCtrlAttributeValue(const CKanbanListCtrlEx* pDestList, const CPoint& ptScreen, CString& sValue) const;
	BOOL UpdateTrackableTaskAttribute(KANBANITEM* pKI, IUI_ATTRIBUTE nAttrib, const CString& sNewValue);
	BOOL UpdateTrackableTaskAttribute(KANBANITEM* pKI, IUI_ATTRIBUTE nAttrib, int nNewValue);
	BOOL UpdateTrackableTaskAttribute(KANBANITEM* pKI, IUI_ATTRIBUTE nAttrib, const CStringArray& aNewValues);
	BOOL UpdateTrackableTaskAttribute(KANBANITEM* pKI, const CString& sAttribID, const CStringArray& aNewValues);
	void LoadDefaultAttributeListValues(const IPreferences* pPrefs, LPCTSTR szAttribID, LPCTSTR szSubKey);
	BOOL IsTrackedAttributeMultiValue() const;
	BOOL IsTracking(const CString& sAttribID) const;
	BOOL CanDrag(const CKanbanListCtrlEx* pSrcList, const CKanbanListCtrlEx* pDestList) const;
	BOOL UpdateNeedsItemHeightRefresh(const CSet<IUI_ATTRIBUTE>& attrib) const;

	BOOL UpdateData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings);
	BOOL RebuildData(const ITASKLISTBASE* pTasks, const CSet<IUI_ATTRIBUTE>& attrib);
	BOOL AddTaskToData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, DWORD dwParentID, const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings);
	BOOL UpdateGlobalAttributeValues(const ITASKLISTBASE* pTasks, const CSet<IUI_ATTRIBUTE>& attrib);
	BOOL UpdateGlobalAttributeValues(const ITASKLISTBASE* pTasks, IUI_ATTRIBUTE nAttribute);
	BOOL UpdateGlobalAttributeValues(LPCTSTR szAttribID, const CStringArray& aValues);

	BOOL WantShowColumn(LPCTSTR szAttribID, const CKanbanItemArrayMap& mapKIArray) const;
	BOOL WantShowColumn(const CKanbanListCtrlEx* pList) const;

	static int GetTaskAllocTo(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CStringArray& aValues);
	static int GetTaskCategories(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CStringArray& aValues);
	static int GetTaskTags(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CStringArray& aValues);
	static BOOL RebuildListContents(CKanbanListCtrlEx* pList, const CKanbanItemArrayMap& mapKIArray, BOOL bShowParents);
	static CString GetXMLTag(IUI_ATTRIBUTE nAttrib);
	static BOOL HasNonParentTasks(const CKanbanItemArray* pItems);
	static void UpdateItemDisplayAttributes(KANBANITEM* pKI, const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CSet<IUI_ATTRIBUTE>& attrib);
	static void BuildTaskIDMap(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CDWordSet& mapIDs, BOOL bAndSiblings);

};

#endif // !defined(AFX_KANBANCTRLEX_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
// GanttDateRangeSlider.cpp : implementation file
//

#include "stdafx.h"
#include "GanttDateRangeSlider.h"
#include "GanttStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGanttDateRangeSlider

CGanttDateRangeSlider::CGanttDateRangeSlider() 
	: 
	CRangeSliderCtrl(TBS_BOTTOM),
	m_nMonthDisplay(GTLC_DISPLAY_NONE)
{
}

CGanttDateRangeSlider::~CGanttDateRangeSlider()
{
}


BEGIN_MESSAGE_MAP(CGanttDateRangeSlider, CRangeSliderCtrl)
	//{{AFX_MSG_MAP(CGanttDateRangeSlider)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGanttDateRangeSlider message handlers

BOOL CGanttDateRangeSlider::Initialise(GTLC_MONTH_DISPLAY nDisplay)
{
	ASSERT(m_nMonthDisplay == GTLC_DISPLAY_NONE);

	return SetMonthDisplay(nDisplay);
}

BOOL CGanttDateRangeSlider::SetMonthDisplay(GTLC_MONTH_DISPLAY nDisplay)
{
	if (nDisplay == GTLC_DISPLAY_NONE)
	{
		ASSERT(0);
		return FALSE;
	}

	m_nMonthDisplay = nDisplay;
	return TRUE;
}

BOOL CGanttDateRangeSlider::SetMaxRange(const GANTTDATERANGE& dtRange)
{
	if (m_nMonthDisplay == GTLC_DISPLAY_NONE)
	{
		ASSERT(0);
		return FALSE;
	}
	
	if (!dtRange.IsValid())
	{
		ASSERT(0);
		return FALSE;
	}

	// Work out whether we need to reset the active range
	BOOL bSetMaxRange = !m_dtMaxRange.IsValid();

	if (!bSetMaxRange)
		bSetMaxRange = ((m_Left == m_Min) && (m_Right == m_Max));

	int nNumCols = GanttStatic::GetRequiredColumnCount(dtRange, m_nMonthDisplay) + 1;
	ASSERT(nNumCols);

	SetMinMax(0.0, nNumCols);
	SetStep(1.0);

	if (bSetMaxRange)
		SetRange(0.0, nNumCols);
	
	m_dtMaxRange = dtRange;

	return TRUE;
}

BOOL CGanttDateRangeSlider::GetMaxRange(GANTTDATERANGE& dtRange, BOOL bZeroBasedDecades) const
{
	int nNumCols = GanttStatic::GetRequiredColumnCount(m_dtMaxRange, m_nMonthDisplay) + 1;
	int nMonthsPerCol = GanttStatic::GetNumMonthsPerColumn(m_nMonthDisplay);

	COleDateTime dtStart(m_dtMaxRange.GetStart(m_nMonthDisplay, bZeroBasedDecades));

	COleDateTime dtEnd(dtStart);
	CDateHelper::IncrementMonth(dtEnd, (nNumCols - 1) * nMonthsPerCol);

	dtRange.SetStart(dtStart);
	dtRange.SetEnd(dtEnd, m_nMonthDisplay, bZeroBasedDecades);

	return dtRange.IsValid();
}

CString CGanttDateRangeSlider::FormatRange(BOOL bZeroBasedDecades, TCHAR cDelim) const
{
	GANTTDATERANGE dtRange;

	if (!GetSelectedRange(dtRange))
		VERIFY(GetMaxRange(dtRange));

	dtRange.SetStart(dtRange.GetStart(m_nMonthDisplay, bZeroBasedDecades));
	dtRange.SetEnd(dtRange.GetEnd(m_nMonthDisplay, bZeroBasedDecades));

	return dtRange.Format(GTLC_DISPLAY_MONTHS_MID, bZeroBasedDecades, FALSE, cDelim);
}

BOOL CGanttDateRangeSlider::HasSelectedRange() const
{
	if (m_nMonthDisplay == GTLC_DISPLAY_NONE)
	{
		ASSERT(0);
		return FALSE;
	}

	return (m_dtMaxRange.IsValid() && ((m_Left > m_Min) || (m_Right < m_Max)));
}

BOOL CGanttDateRangeSlider::GetSelectedRange(GANTTDATERANGE& dtRange, BOOL bZeroBasedDecades) const
{
	if (!HasSelectedRange())
		return FALSE;

	int nStartCol = (int)GetLeft();
	int nEndCol = (int)GetRight();

	ASSERT(nEndCol > nStartCol);

	// Subtract 1 because the end column index is really 
	// the start of the next column
	nEndCol--;
	
	int nMonthsPerCol = GanttStatic::GetNumMonthsPerColumn(m_nMonthDisplay);

	COleDateTime dtStart(m_dtMaxRange.GetStart());
	CDateHelper::IncrementMonth(dtStart, nStartCol * nMonthsPerCol);

	COleDateTime dtEnd(m_dtMaxRange.GetStart());
	CDateHelper::IncrementMonth(dtEnd, nEndCol * nMonthsPerCol);

	dtRange.SetStart(dtStart, m_nMonthDisplay, bZeroBasedDecades);
	dtRange.SetEnd(dtEnd, m_nMonthDisplay, bZeroBasedDecades);

	return TRUE;
}

BOOL CGanttDateRangeSlider::SetSelectedRange(const GANTTDATERANGE& dtRange)
{
	GANTTDATERANGE dtMax;

	if (!GetMaxRange(dtMax))
	{
		ASSERT(0);
		return FALSE;
	}

	if (!dtRange.IsValid() || !dtMax.Contains(dtRange))
	{
		ASSERT(0);
		return FALSE;
	}

	int nNumMonthsStart = CDateHelper::CalcMonthsFromTo(dtMax.GetStart(), dtRange.GetStart(), FALSE);
	int nNumMonthsEnd = CDateHelper::CalcMonthsFromTo(dtMax.GetStart(), dtRange.GetEnd(), TRUE); // inclusive
	ASSERT(nNumMonthsStart < nNumMonthsEnd);

	int nMonthsPerCol = GanttStatic::GetNumMonthsPerColumn(m_nMonthDisplay);
	SetRange((nNumMonthsStart / nMonthsPerCol), nNumMonthsEnd / nMonthsPerCol);

	return TRUE;
}

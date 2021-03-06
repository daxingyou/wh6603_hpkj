#include "stdafx.h"
#include "GamePlaza.h"
#include "TextButton.h"
#include "gdipng/MemDC.h"


IMPLEMENT_DYNAMIC(CTextButton, CButton)

CTextButton::CTextButton()
: m_pIamge(NULL)
, m_bHaveBitmaps(false)
// , m_pCurBtn(NULL)
, m_width(0)
, m_height(0)
, m_bIsHovering(false)
, m_bIsTracking(false)
, m_bIsDisabled(false)
, m_bPushed(false)
, m_TextClr(RGB(255, 255, 255))
, m_font(NULL)
{
	m_bDCStored = false;

	m_font1.CreateFont(19,0,0,0,800,0,0,0,0,0,0,0,0,TEXT("微软雅黑"));
}

CTextButton::~CTextButton()
{
	if(m_pIamge != NULL) 
	{
		delete m_pIamge;
		m_pIamge = NULL;
	}

// 	if (m_dcStd.m_hDC != NULL)
// 	{
// 		m_dcStd.DeleteDC();
// 	}
// 	if (m_dcBk.m_hDC != NULL)
// 	{
// 		m_dcBk.DeleteDC();
// 	}
// 	if (m_dcStdH.m_hDC != NULL)
// 	{
// 		m_dcStdH.DeleteDC();
// 	}
// 	if (m_dcGS.m_hDC != NULL)
// 	{
// 		m_dcGS.DeleteDC();
// 	}
// 	if (m_dcStdP.m_hDC != NULL)
// 	{
// 		m_dcStdP.DeleteDC();
// 	}

}
void CTextButton::EnableTextButton(bool bEnable)
{
	if (m_bIsDisabled != bEnable) 
	{
		m_bIsDisabled = bEnable;
		EnableWindow(!bEnable);
		Invalidate();
	}
}
void CTextButton::SetBkImage(LPCTSTR imgFile)
{
	if(m_pIamge != NULL) 
	{
		delete m_pIamge;
		m_pIamge = NULL;
	}

	m_pIamge = new Bitmap(imgFile);
	m_width = m_pIamge->GetWidth() / 4;
	m_height = m_pIamge->GetHeight();
}


void CTextButton::SetPushed(bool bPushed) 
{


	if (m_bPushed != bPushed) 
	{	
		m_bPushed = bPushed;
		Invalidate();
	}
}

BEGIN_MESSAGE_MAP(CTextButton, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_DRAWITEM()
//	ON_WM_CTLCOLOR_REFLECT()
//	ON_WM_MOUSEHOVER()
//	ON_WM_MOUSELEAVE()
	ON_MESSAGE( WM_MOUSEHOVER, OnMouseHover )  
	ON_MESSAGE( WM_MOUSELEAVE, OnMouseLeave ) 
	ON_WM_LBUTTONDOWN()

	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

BOOL CTextButton::OnEraseBkgnd(CDC* pDC)
{
	if(!m_bDCStored)
	{
		CRect  clientRect;
		GetClientRect(&clientRect);
		int cx=clientRect.Width ();
		int cy=clientRect.Height ();
		// Store our orignal DC.
		CBitmap	bitmap;
		memDC.CreateCompatibleDC (pDC);
		bitmap.CreateCompatibleBitmap (pDC,cx,cy);
		memDC.SelectObject (&bitmap);
		memDC.BitBlt (0,0,cx,cy,pDC,0,0,SRCCOPY);
		m_bDCStored=true;
	}

	return TRUE;
}
void CTextButton::PreSubclassWindow()
{
	CString str;
	GetWindowText(str);

	if(m_pIamge == NULL)
	{
		int nLength = str.GetLength();
		if(nLength == 2)
			m_pIamge = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\btn_txt_bk1.png"));
		else if(nLength == 3)
			m_pIamge = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\btn_txt_bk2.png"));
		else if(nLength == 5)
			m_pIamge = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\btn_txt_bk4.png"));
		else
			m_pIamge = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\btn_txt_bk3.png"));
		m_width = m_pIamge->GetWidth() / 4;
		m_height = m_pIamge->GetHeight();

	}

	ModifyStyle(0, BS_OWNERDRAW, SWP_FRAMECHANGED);
	CButton::PreSubclassWindow();
}
//鼠标消息
VOID CTextButton::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);
	TCHAR wav_path[MAX_PATH] = _T("");
	CBmpUtil::GetExePath(_T("skin\\game\\MouseDown.wav"), wav_path);
	::PlaySound(wav_path, NULL, SND_ASYNC);
	return;
}

void CTextButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{

	if(m_pIamge == NULL)
		return;
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	Gdiplus::Graphics graphics(pDC->m_hDC);

	graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), 0, 0, m_width, m_height, UnitPixel);
	if(m_bIsDisabled)
	{
		graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_width*3, 0, m_width, m_height, UnitPixel);
		return;
	}

	BOOL bIsPressed = (lpDIS->itemState & ODS_SELECTED);
	if (m_bPushed) 
	{
		graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_width*2, 0, m_width, m_height, UnitPixel);
	}
	else 
	{
		if(bIsPressed)
		{
			graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_width*2, 0, m_width, m_height, UnitPixel);
		}
		else if(m_bIsHovering)
		{
			graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_width, 0, m_width, m_height, UnitPixel);
		}
		else
		{
			graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), 0, 0, m_width, m_height, UnitPixel);
		}
	}

	PaintText(pDC);
	// paint the button
//	PaintBtn(pDC);
	return;
}

// void CTextButton::PaintBk(CDC *pDC)
// {
// 	CRect rect;
// 	GetClientRect(rect);
// 	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcBk, 0, 0, SRCCOPY);
// }
// 
// void CTextButton::PaintBtn(CDC *pDC)
// {
// 	CRect rect;
// 	GetClientRect(rect);
// 	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), m_pCurBtn, 0, 0, SRCCOPY);
// 
// 	PaintText(pDC);
// }

void CTextButton::PaintText(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);

	CString str;
	GetWindowText(str);
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF oldTextClr = pDC->SetTextColor(m_TextClr);

	if(m_bIsHovering)
		pDC->SetTextColor(RGB(255,255,255));
	if (m_font != NULL) 
	{
		CFont* pOldFont = pDC->SelectObject(m_font);
		pDC->DrawText(str, &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		pDC->SelectObject(pOldFont);
	}
	else
	{
		m_font = &m_font1;
		CFont* pOldFont = pDC->SelectObject(m_font);
		pDC->DrawText(str, &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		pDC->SelectObject(pOldFont);
	}
	pDC->SetTextColor(oldTextClr);
	pDC->SetBkMode(oldBkMode);
}

LRESULT CTextButton::OnMouseHover(WPARAM wParam,LPARAM lParam)
{
// 	TCHAR wav_path[MAX_PATH] = _T("");
// 	CBmpUtil::GetExePath(_T("skin\\game\\MouseMove.wav"), wav_path);
// 	::PlaySound(wav_path, NULL, SND_ASYNC);

	m_bIsHovering = true;
	Invalidate();
	return 1;
}

LRESULT CTextButton::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	m_bIsTracking = FALSE;
	m_bIsHovering = FALSE;
	Invalidate();
	return 1;
}
void CTextButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bIsTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bIsTracking = !!_TrackMouseEvent(&tme);
	}

	CButton::OnMouseMove(nFlags, point);
}

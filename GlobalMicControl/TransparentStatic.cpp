// TransparentStatic.cpp : implementation file
//

#include "pch.h"
#include "GlobalMicControl.h"
#include "TransparentStatic.h"


// CTransparentStatic

IMPLEMENT_DYNAMIC(CTransparentStatic, CStatic)

CTransparentStatic::CTransparentStatic()
{

}

CTransparentStatic::~CTransparentStatic()
{
}


BEGIN_MESSAGE_MAP(CTransparentStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CTransparentStatic message handlers




void CTransparentStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CStatic::OnPaint() for painting messages

	BOOL bfont = FALSE;
	BOOL bIcon = FALSE;
	BOOL bBmp = FALSE;

	// Where to draw text
	CRect client_rect;
	GetClientRect(client_rect);

	// Get the caption
	CString szText;
	GetWindowText(szText);

	HICON hIcon;
	hIcon = GetIcon();
	if (hIcon)
	{
		// mean icon type
		bIcon = TRUE;
	}

	HBITMAP hBmp;
	hBmp = GetBitmap();
	if (hBmp)
	{
		// mean bitmap type
		bBmp = TRUE;
	}

	// Get the font
	CFont* pFont, * pOldFont;
	pFont = GetFont();
	if (pFont)
	{
		bfont = TRUE;
	}

	if (bIcon)
	{
		dc.DrawIcon(0, 0, hIcon);
	}

	if (bBmp)
	{
		RECT rect;
		GetClientRect(&rect);
		BITMAP bmpInfo;
		CBitmap* pBitmap = CBitmap::FromHandle(hBmp);
		pBitmap->GetBitmap(&bmpInfo);
		CDC theCDC;
		theCDC.CreateCompatibleDC(&dc);
		theCDC.SelectObject(pBitmap);
		//dc.BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &theCDC, 0, 0, SRCCOPY);
		dc.StretchBlt(rect.top, rect.left, rect.bottom, rect.bottom, &theCDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
		theCDC.DeleteDC();
	}

	if (bfont)
	{
		pOldFont = dc.SelectObject(pFont);

		// Map "Static Styles" to "Text Styles"
#define MAP_STYLE(src, dest) if(dwStyle & (src)) dwText |= (dest)
#define NMAP_STYLE(src, dest) if(!(dwStyle & (src))) dwText |= (dest)

		DWORD dwStyle = GetStyle(), dwText = 0;

		MAP_STYLE(SS_RIGHT, DT_RIGHT);
		MAP_STYLE(SS_CENTER, DT_CENTER);
		MAP_STYLE(SS_CENTERIMAGE, DT_VCENTER | DT_SINGLELINE);
		MAP_STYLE(SS_NOPREFIX, DT_NOPREFIX);
		MAP_STYLE(SS_WORDELLIPSIS, DT_WORD_ELLIPSIS);
		MAP_STYLE(SS_ENDELLIPSIS, DT_END_ELLIPSIS);
		MAP_STYLE(SS_PATHELLIPSIS, DT_PATH_ELLIPSIS);

		NMAP_STYLE(SS_LEFTNOWORDWRAP |
			SS_CENTERIMAGE |
			SS_WORDELLIPSIS |
			SS_ENDELLIPSIS |
			SS_PATHELLIPSIS, DT_WORDBREAK);

		// Set transparent background
		dc.SetBkMode(TRANSPARENT);

		// Draw the text
		dc.DrawText(szText, client_rect, dwText);

		// Select old font
		dc.SelectObject(pOldFont);
	}
}

// Layered.h : interface of the CLayered class
//
/////////////////////////////////////////////////////////////////////////////

template <class T>
class CLayered
{
	enum Styles
	{
		WIN_TRANSPARENT = WS_EX_LAYERED,
		WIN_CLICK_THRU = WS_EX_LAYERED | WS_EX_TRANSPARENT
	};

	bool m_bSupported;

	// SetLayeredWindowAttributes() function pointer
	typedef BOOL (__stdcall *PFUNCSETLAYEREDWINDOWATTR)(HWND, COLORREF, BYTE, DWORD);
	PFUNCSETLAYEREDWINDOWATTR m_pfSetLayeredWindowAttributes;

	BOOL SetLayered(Styles Style)
	{
		if (!m_bSupported)
		{
			ATLTRACE(_T("Your OS does not support the transparency\n"));
			return FALSE;
		}

		T* pT = static_cast<T*>(this);

		return pT->ModifyStyleEx(0, Style);
	}

	BOOL RemoveLayered(Styles Style)
	{
		T* pT = static_cast<T*>(this);

		return pT->ModifyStyleEx(Style, 0);
	}

public:

	CLayered()
	{
		// To support a transparency try to get a SetLayeredWindowAttributes function address
		HMODULE hUser32 = GetModuleHandle(_T("User32.dll"));

		if (hUser32)
		{
			// Get the address of the SetLayeredWindowAttributes() function
			m_pfSetLayeredWindowAttributes = 
				(PFUNCSETLAYEREDWINDOWATTR)::GetProcAddress(hUser32, "SetLayeredWindowAttributes");

			// Set the supported indicator
			if (m_pfSetLayeredWindowAttributes)
				m_bSupported = true;
			else
				m_bSupported = false;
		}
	}

	BOOL SetLayered()
	{
		// Need an extended style WS_EX_LAYERED to make it layered
		return SetLayered(WIN_TRANSPARENT);
	}

	BOOL RemoveLayered()
	{
		return RemoveLayered(WIN_TRANSPARENT);
	}

	BOOL SetClickThru()
	{
		// Need an extended style WS_EX_LAYERED and WS_EX_TRANSPARENT to pass mouse clicks through the window
		if (SetLayered(WIN_CLICK_THRU))
		{
			// Without this call the window will not be visible
			return MakeTransparent(255);
		}
		else
			return FALSE;
	}

	BOOL RemoveClickThru()
	{
		return RemoveLayered(WIN_CLICK_THRU);
	}

	// Sets the transparency of the window
	BOOL MakeTransparent(int nOpacity)
	{
		if (m_bSupported)
		{
			ATLASSERT(m_pfSetLayeredWindowAttributes);
			T* pT = static_cast<T*>(this);

			return m_pfSetLayeredWindowAttributes(pT->m_hWnd, 0, (BYTE)nOpacity, LWA_ALPHA);
		}
			
		return FALSE;
	}
};
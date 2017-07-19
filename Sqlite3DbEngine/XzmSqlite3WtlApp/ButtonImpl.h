#pragma once

// CButtonImpl - CWindowImpl-derived class that implements a button.  We need a
// class like this to do subclassing or DDX.
class CButtonImpl : public CWindowImpl<CButtonImpl, CButton>
{
public:
	CButtonImpl(int type);
	BEGIN_MSG_MAP(CButtonImpl)
		MSG_WM_SETCURSOR(OnSetCursor)
	END_MSG_MAP()

	// 设置鼠标在按钮上光标的样式
	LRESULT OnSetCursor(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg);

protected:
	int m_type;
};
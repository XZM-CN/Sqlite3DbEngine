#pragma once


// A button that has an owner draw handler.
class CODButtonImpl : public CWindowImpl<CODButtonImpl, CButton>,
	public COwnerDraw<CODButtonImpl>
{
public:
	CODButtonImpl();

	BEGIN_MSG_MAP(CODButtonImpl)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CODButtonImpl>, 1)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	void DrawItem ( LPDRAWITEMSTRUCT lpdis );

protected:
	CBitmap m_bmp;
};

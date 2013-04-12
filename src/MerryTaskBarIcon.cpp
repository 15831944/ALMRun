#include "MerryTaskBarIcon.h"
#include "MerryApp.h"
#include "MerryInformationDialog.h"

enum
{
	MENU_ITEM_OPEN = 10001,
	MENU_ITEM_OPEN_CONFIG,
	MENU_ITEM_CONFIG,
	MENU_ITEM_ABOUT,
	MENU_ITEM_EXIT,
};

BEGIN_EVENT_TABLE(MerryTaskBarIcon, wxTaskBarIcon)
	EVT_MENU(MENU_ITEM_OPEN, MerryTaskBarIcon::OnOpenEvent)
	EVT_MENU(MENU_ITEM_OPEN_CONFIG, MerryTaskBarIcon::OnOpenConfigEvent)
	EVT_MENU(MENU_ITEM_CONFIG, MerryTaskBarIcon::OnConfigEvent)
	EVT_MENU(MENU_ITEM_ABOUT, MerryTaskBarIcon::OnAboutEvent)
	EVT_MENU(MENU_ITEM_EXIT, MerryTaskBarIcon::OnExitEvent)
	EVT_TASKBAR_LEFT_DCLICK(MerryTaskBarIcon::OnLeftButtonDClickEvent)
END_EVENT_TABLE()

wxMenu* MerryTaskBarIcon::CreatePopupMenu()
{
	wxMenu* menu = new wxMenu;

	menu->Append(MENU_ITEM_OPEN, wxT("��ʾ����(&O)"));
	menu->Append(MENU_ITEM_OPEN_CONFIG, wxT("��ʾ����(&S)"));
	menu->Append(MENU_ITEM_CONFIG, wxT("ˢ������(R)"));
	menu->Append(MENU_ITEM_ABOUT, wxT("����Merry(&A)"));
	menu->Append(MENU_ITEM_EXIT, wxT("�˳�(&X)"));

	return menu;
}

void MerryTaskBarIcon::OnOpenEvent(wxCommandEvent& e)
{
	::wxGetApp().GetFrame().Show();
}

void MerryTaskBarIcon::OnOpenConfigEvent(wxCommandEvent& e)
{
	::wxGetApp().GetFrame().OpenConfigDir();
}

void MerryTaskBarIcon::OnConfigEvent(wxCommandEvent& e)
{
	::wxGetApp().NewFrame();
}

void MerryTaskBarIcon::OnAboutEvent(wxCommandEvent& e)
{
	new MerryInformationDialog(wxT("���� Merry"),wxString::Format(wxT("version 1.0.0 --- http://name5566.com\r\n�޸���(SVN Rev 98) --- http://chenall.net\r\n����ʱ��:%s %s"),__DATE__,__TIME__));
}

void MerryTaskBarIcon::OnExitEvent(wxCommandEvent& e)
{
	::wxGetApp().GetFrame().Close();
}

void MerryTaskBarIcon::OnLeftButtonDClickEvent(wxTaskBarIconEvent& e)
{
	::wxGetApp().GetFrame().Show();
	e.Skip();
}

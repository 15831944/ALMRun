#include "MerryTaskBarIcon.h"
#include "MerryApp.h"
#include "MerryInformationDialog.h"

BEGIN_EVENT_TABLE(MerryTaskBarIcon, wxTaskBarIcon)
	EVT_MENU(MENU_ITEM_OPEN, MerryTaskBarIcon::onPopMenu)
	EVT_MENU(MENU_ITEM_OPEN_CONFIG, MerryTaskBarIcon::onPopMenu)
	EVT_MENU(MENU_ITEM_CONFIG, MerryTaskBarIcon::onPopMenu)
	EVT_MENU(MENU_ITEM_ABOUT, MerryTaskBarIcon::onPopMenu)
	EVT_MENU(MENU_ITEM_EXIT, MerryTaskBarIcon::onPopMenu)
	EVT_TASKBAR_LEFT_DCLICK(MerryTaskBarIcon::OnLeftButtonDClickEvent)
END_EVENT_TABLE()

wxMenu* MerryTaskBarIcon::CreatePopupMenu()
{
	wxMenu* menu = new wxMenu;

	menu->Append(MENU_ITEM_OPEN, wxT("��ʾ����(&O)"));
	menu->Append(MENU_ITEM_OPEN_CONFIG, wxT("��ʾ����(&S)"));
	menu->Append(MENU_ITEM_CONFIG, wxT("ˢ������(R)"));
	menu->Append(MENU_ITEM_ABOUT, wxT("����ALMRun(&A)"));
	menu->Append(MENU_ITEM_EXIT, wxT("�˳�(&X)"));

	return menu;
}

void MerryTaskBarIcon::onPopMenu(wxCommandEvent& e)
{
	switch(e.GetId())
	{
		case MENU_ITEM_ABOUT:
			new MerryInformationDialog(wxT("���� ALMRun"),wxString::Format(wxT("version 1.1.2 --- http://chenall.net\r\n�޸���(merry SVN R:98 http://name5566.com)\r\n����ʱ��:%s %s"),__DATE__,__TIME__));
			break;
		case MENU_ITEM_OPEN:
			::wxGetApp().GetFrame().Show();
			break;
		case MENU_ITEM_OPEN_CONFIG:
			::wxGetApp().GetFrame().OpenConfigDir();
			break;
		case MENU_ITEM_CONFIG:
			::wxGetApp().GetFrame().NewConfig();
			break;
		case MENU_ITEM_EXIT:
			::wxGetApp().GetFrame().Close();
			break;
	}
}

void MerryTaskBarIcon::OnLeftButtonDClickEvent(wxTaskBarIconEvent& e)
{
	::wxGetApp().GetFrame().Show();
	e.Skip();
}

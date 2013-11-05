#include "MerryTaskBarIcon.h"
#include "MerryApp.h"
#include "MerryInformationDialog.h"
#include "cmdmgr.h"

BEGIN_EVENT_TABLE(MerryTaskBarIcon, wxTaskBarIcon)
	EVT_MENU(MENU_ITEM_OPEN, MerryTaskBarIcon::onPopMenu)
	EVT_MENU(MENU_ITEM_OPEN_CONFIG, MerryTaskBarIcon::onPopMenu)
	EVT_MENU(MENU_ITEM_CONFIG, MerryTaskBarIcon::onPopMenu)
	EVT_MENU(MENU_ITEM_ABOUT, MerryTaskBarIcon::onPopMenu)
	EVT_MENU(MENU_ITEM_EXIT, MerryTaskBarIcon::onPopMenu)
	EVT_MENU(MENU_ITEM_GUI_CONFIG, MerryTaskBarIcon::onPopMenu)
	EVT_MENU(MENU_ITEM_CMDMGR,MerryTaskBarIcon::onPopMenu)
	EVT_TASKBAR_LEFT_DCLICK(MerryTaskBarIcon::OnLeftButtonDClickEvent)
END_EVENT_TABLE()

MerryTaskBarIcon::~MerryTaskBarIcon()
{
	__DEBUG_BEGIN("")
	__DEBUG_END("")
}

wxMenu* MerryTaskBarIcon::CreatePopupMenu()
{
	wxMenu* menu = new wxMenu;

	menu->Append(MENU_ITEM_OPEN, wxT("��ʾ����(&O)"));
	menu->Append(MENU_ITEM_GUI_CONFIG, wxT("��������(&C)"));
	menu->Append(MENU_ITEM_CMDMGR, wxT("�������(&S)"));
	menu->Append(MENU_ITEM_OPEN_CONFIG, wxT("����Ŀ¼(&S)"));
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
			new MerryInformationDialog(wxT("���� ALMRun"),wxString::Format("version " VERSION_STR " --- http://ALMRun.chenall.net\r\n�޸���(merry SVN R:98 http://name5566.com)\r\n����ʱ��:%s %s",__DATE__,__TIME__));
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
			//wxGetApp().ExitMainLoop();
			// insert your code here
			//ExitProcess(0);
		   // this->EndModal(true);
			//::PostMessage((HWND__ *)::wxGetApp().GetFrame().GetHandle(),WM_CLOSE,0,0);
		   //::PostMessage((HWND__ *)::wxGetApp().GetFrame().GetHandle(),WM_QUIT,0,0);
		   //exit(0);
			wxExit();
			break;
	#ifdef _ALMRUN_CONFIG_H_
		case MENU_ITEM_GUI_CONFIG:
			g_config->GuiConfig();
			break;
	#endif//#ifdef _ALMRUN_CONFIG_H_
	#ifdef _CMDMGR_H_
		case MENU_ITEM_CMDMGR:
			cmdMgr *dlg = new cmdMgr(0);
			dlg->ShowModal();
			dlg->Destroy();
			::wxGetApp().GetFrame().NewConfig();
			break;
	#endif
	}
}

void MerryTaskBarIcon::OnLeftButtonDClickEvent(wxTaskBarIconEvent& e)
{
	::wxGetApp().GetFrame().Show();
	e.Skip();
}

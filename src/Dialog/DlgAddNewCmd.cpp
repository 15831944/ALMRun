/////////////////////////////////////////////////////////////////////////////
// Name:        dlgaddnewcmd.cpp
// Purpose:     
// Author:      test
// Modified by: 
// Created:     09/06/2013 11:53:23
// RCS-ID:      
// Copyright:   test
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "DlgAddNewCmd.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "ALMRunConfig.h"
#include "ALMRunCommon.h"
#include "DlgAddNewCmd.h"
#include "DlgAddNewDir.h"
#include "MerryLua.h"

#ifndef _DISABLE_DND_
	#include <wx/dnd.h>
	class NewCmdDnd : public wxFileDropTarget
	{
	public:
		NewCmdDnd(DlgAddNewCmd *pOwner) { m_pOwner = pOwner; }
 
		virtual bool OnDropFiles(wxCoord x, wxCoord y,
								 const wxArrayString& filenames);
	private:
		//�Ի����࣬��ԱTextCtrlPath�����ļ�·��
		DlgAddNewCmd *m_pOwner;
 
	};
	bool NewCmdDnd::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filenames)
	{
		//ֻ��һ���ļ�ʱ������������
		if (filenames.Count() == 1)
		{
			wxString cmd = filenames[0];
			cmd.Replace('\\','/');
			m_pOwner->cmdLine->SetValue(cmd);
			m_pOwner->cmdName->SetValue(wxFileNameFromPath(cmd));
			cmd.Clear();
			return true;
		}
		return false;
	}
#endif

#ifdef _ALMRUN_CONFIG_H_

////@begin XPM images
////@end XPM images


/*!
 * DlgAddNewCmd type definition
 */

IMPLEMENT_DYNAMIC_CLASS( DlgAddNewCmd, wxDialog )


/*!
 * DlgAddNewCmd event table definition
 */

BEGIN_EVENT_TABLE( DlgAddNewCmd, wxDialog )

////@begin DlgAddNewCmd event table entries
////@end DlgAddNewCmd event table entries
	EVT_BUTTON(wxID_OK, OnOkButtonClick)
	EVT_BUTTON(ID_CMD_BROWSE,OnBrowseClick)
	EVT_BUTTON(ID_CMD_ADD_DIR,OnBrowseClick)
	EVT_SHOW(OnShow)
END_EVENT_TABLE()


/*!
 * DlgAddNewCmd constructors
 */

DlgAddNewCmd::DlgAddNewCmd()
{
    Init();
}

DlgAddNewCmd::DlgAddNewCmd(const int flags)
{
	this->flags = flags;
	this->Create(NULL);
}

DlgAddNewCmd::DlgAddNewCmd( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * DlgAddNewCmd creator
 */

bool DlgAddNewCmd::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin DlgAddNewCmd creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end DlgAddNewCmd creation
    return true;
}


/*!
 * DlgAddNewCmd destructor
 */

DlgAddNewCmd::~DlgAddNewCmd()
{
	__DEBUG_BEGIN("")
#ifndef _DISABLE_DND_
		this->SetDropTarget(NULL);
#endif
////@begin DlgAddNewCmd destruction
////@end DlgAddNewCmd destruction
	__DEBUG_END("")
}

void DlgAddNewCmd::OnBrowseClick(wxCommandEvent& e)
{
	wxString cmd;
	if (e.GetId() == ID_CMD_ADD_DIR)
	{
		cmd = wxDirSelector("ѡ��Ŀ¼",wxEmptyString,0,wxDefaultPosition,this);
		if (cmd.empty())
			return;
		this->Hide();
		//cmd.Replace("\\","/");
		DlgAddNewDir *dlg = new DlgAddNewDir(this);
		dlg->dirName->SetValue(cmd);
		dlg->SetMode(ADDDIR_FLAG_CMDS);
		//Ĭ�ϲ�������
		dlg->dirExclude->SetValue(g_config->GetDefaultDirExclude());
		dlg->dirInclude->SetValue(g_config->GetDefaultDirInclude());
		dlg->dirSub->SetValue(g_config->GetDefaultDirSub());

		if (dlg->ShowModal() == wxID_OK)
		{
			wxArrayString files;
			wxString Exclude = dlg->dirExclude->GetValue();
			ListFiles(dlg->dirName->GetValue(),&files,dlg->dirInclude->GetValue(),dlg->dirExclude->GetValue(),dlg->dirSub->GetValue());
			if (!files.empty())
			{
				for(int i = files.Count() - 1 ;i>=0;--i)
				{
					//files[i].Replace("\\","/");
					g_config->AddCmd(files[i],wxFileNameFromPath(files[i]));
				}
			}
		}
		dlg->Destroy();
		this->EndModal(wxID_OK);
		return;
	}
	else
		cmd = wxFileSelector("��ѡ��",wxEmptyString,wxEmptyString,wxEmptyString,"*.exe;*.bat;*.cmd",0,this);
	if (!cmd.empty())
	{
		//cmd.Replace("\\","/");
		cmdLine->SetValue(cmd);
	}
}

void DlgAddNewCmd::OnShow(wxShowEvent& e)
{
	if (!e.GetShow())
		return;
	this->SetFocus();
	wxString cmd = cmdLine->GetValue();
	if (cmd.empty())
		return;
	while(true)
	{
		if (cmd[0] == '@')//ǰ��'@'���ش�������
			this->HideRun->SetValue(true);
		else if (cmd[0] == '>')//ǰ��'>'�������ԱȨ��(NT6������Ч)
			this->RunAs->SetValue(true);
		else if (cmd[0] == '+')//ǰ��'+'�������������
			this->RequiredArg->SetValue(true);
		else
			break;
		cmd.erase(0,1);
	}
//	cmd.Replace('\\','/');
	cmdLine->SetValue(cmd);
}

static int AddALTRunCMD(lua_State *L)
{
	if (!lua_istable(L, -1))
		return 0;
	int it = lua_gettop(L);

	lua_rawgeti(L,it,3);//�����
	wxString commandName(wxString(lua_tostring(L, -1), wxConvLocal));

	lua_rawgeti(L,it,4);//��ע
	wxString commandDesc(wxString(lua_tostring(L, -1), wxConvLocal));

	lua_rawgeti(L,it,5);//����
	wxString commandLine(wxString(lua_tostring(L, -1), wxConvLocal));

	lua_rawgeti(L,it,2);//��������
	wxString commandParam(wxString(lua_tostring(L, -1), wxConvLocal));

	lua_settop(L,it);//�ָ�

	if (!commandParam.Trim(false).empty())//�����Ҫ����������ǿ�Ʋ�����־
		commandLine.insert(0,'+');
	if (g_config->AddCmd(commandLine,commandName,wxEmptyString,commandDesc) > 0)
		return 1;
	return false;
}

static bool ALTRunCheck(wxString& cmd)
{
	if (!g_lua)
		return false;
	if (wxFileNameFromPath(cmd).Upper().StartsWith("SHORTCUTLIST.") == false || 
		wxMessageBox("���ļ�������ALTRun�������ļ����Ƿ�������?","ALTRun�������ʾ",wxYES_NO|wxICON_INFORMATION) != wxYES)
		return false;
	lua_State *L = g_lua->GetLua();
	lua_getglobal(L, "read_altrun_config");
	if (!lua_isfunction(L, 1))
	{
		lua_pop(L, 1);
		wxMessageBox("�ù�������Ҫ��LUA���������ڣ���ȷ��LuaEx\\base.lua�ļ�����","����");
		return true;
	}
	lua_pushstring(L,cmd.c_str());
	if (lua_pcall(L, 1, 1, 0) || !lua_istable(L,-1))
	{
		wxMessageBox("LUA�ű�ִ�����󣬻򷵻�ֵ����");
		lua_pop(L, 1);
		return true;
	}
	int it=lua_gettop(L);
	int num = 0;
	lua_pushnil(L);
	while(lua_next(L, it))
	{
		num += AddALTRunCMD(L);
		lua_pop(L, 1);
	}
	lua_pop(L,1);
	return true;
}
void DlgAddNewCmd::OnOkButtonClick(wxCommandEvent& e)
{
	if (!g_config)
		return;
	wxString cmd = cmdLine->GetValue().Trim().Trim(false);
	if (cmd.empty())
	{
		wxMessageBox("�����в���Ϊ��,����������!","��ʾ");
		return;
	}

	if (ALTRunCheck(cmd))
	{
		if (this->IsModal())
			this->EndModal(wxID_OK);
		else
			this->Destroy();
		return;
	}

	cmdLine->SetValue(cmd);
	if (RunAs->GetValue())
		cmd.insert(0,'>');
	if (HideRun->GetValue())
		cmd.insert(0,'@');
	if (RequiredArg->GetValue())
		cmd.insert(0,'+');
	if ((flags == MENU_CMD_EDIT && g_config->ModifyCmd(cmdID,cmd,cmdName->GetValue(),cmdKey->GetValue(),cmdDesc->GetValue()))
		|| (cmdID = g_config->AddCmd(cmd,cmdName->GetValue(),cmdKey->GetValue(),cmdDesc->GetValue()))>0 )
	{
		if (this->IsModal())
			this->EndModal(wxID_OK);
		else
			this->Destroy();
	}
}

/*!
 * Member initialisation
 */

void DlgAddNewCmd::Init()
{
////@begin DlgAddNewCmd member initialisation
	flags = 0;
	cmdID = -1;
////@end DlgAddNewCmd member initialisation
}


/*!
 * Control creation for DlgAddNewCmd
 */

void DlgAddNewCmd::CreateControls()
{    
////@begin DlgAddNewCmd content construction
    DlgAddNewCmd* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, "�������");
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
//    itemStaticBoxSizer3Static->SetFont(wxFont(9, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Tahoma")));
    itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	 wxBoxSizer* itemBoxSizer_hotkey = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer3->Add(itemBoxSizer4, 0, wxGROW|wxLEFT, 5);
	itemStaticBoxSizer3->Add(itemBoxSizer_hotkey, 0, wxGROW|wxLEFT, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC, "����:", wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    cmdName = new wxTextCtrl( itemStaticBoxSizer3->GetStaticBox(), ID_CMD_NAME, wxEmptyString, wxDefaultPosition, wxSize(300, -1), 0 );
    cmdName->SetMaxLength(30);
   cmdName->SetHelpText("�������,�б���ʾ������.");
    if (DlgAddNewCmd::ShowToolTips())
        cmdName->SetToolTip("�������,�б���ʾ������.");
    itemBoxSizer4->Add(cmdName, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC, "�ȼ�:", wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer_hotkey->Add(itemStaticText7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    //cmdKey = new wxTextCtrl( itemStaticBoxSizer3->GetStaticBox(), ID_CMD_KEY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );

	//    itemBoxSizer4->Add(cmdKey, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);
	cmdKey = new HotkeyCtrl(itemStaticBoxSizer3->GetStaticBox(),ID_CMD_KEY,wxEmptyString,wxDefaultPosition,wxSize(300, -1));
    cmdKey->SetHelpText("��ѡ����,�����Ӧȫ���ȼ�");
    if (DlgAddNewCmd::ShowToolTips())
        cmdKey->SetToolTip("��ѡ����,�����Ӧȫ���ȼ�");
	itemBoxSizer_hotkey->Add(cmdKey, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer3->Add(itemBoxSizer9, 0, wxGROW|wxLEFT, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC, "��ע:", wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemStaticText10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    cmdDesc = new wxTextCtrl( itemStaticBoxSizer3->GetStaticBox(), ID_CMD_DESC, wxEmptyString, wxDefaultPosition, wxSize(300, -1), 0 );
    cmdDesc->SetHelpText("��ѡ����,���ע����ʾ��Ϣ");
    if (DlgAddNewCmd::ShowToolTips())
        cmdDesc->SetToolTip("��ѡ����,���ע����ʾ��Ϣ");
    itemBoxSizer9->Add(cmdDesc, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText12 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemStaticText12, 0,wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer3->Add(itemBoxSizer13, 0, wxGROW|wxLEFT, 5);

    wxStaticText* itemStaticText14 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC, "����:", wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(itemStaticText14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    cmdLine = new wxTextCtrl( itemStaticBoxSizer3->GetStaticBox(), ID_CMD_CMDLINE, wxEmptyString, wxDefaultPosition, wxSize(220, -1), 0 );
    cmdLine->SetHelpText("��Ҫ����,Ҫִ�е�������");
    if (DlgAddNewCmd::ShowToolTips())
        cmdLine->SetToolTip("��Ҫ����,Ҫִ�е�������");
    itemBoxSizer13->Add(cmdLine, 0, wxGROW|wxALL, 5);

    wxButton* itemButton16 = new wxButton( itemStaticBoxSizer3->GetStaticBox(), ID_CMD_BROWSE, "���(&B)", wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer13->Add(itemButton16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	//�߼�ѡ��
	wxBoxSizer* itemBoxSizer_checkbox = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer3->Add(itemBoxSizer_checkbox, 0, wxGROW|wxALL, 5);
	wxStaticText* itemBoxSizer_checkbox_text = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC, "�߼�ѡ��:", wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer_checkbox->Add(itemBoxSizer_checkbox_text, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	HideRun = new wxCheckBox( itemStaticBoxSizer3->GetStaticBox(),wxID_ANY, _T("����ִ��"), wxDefaultPosition, wxDefaultSize, 0 );
    HideRun->SetValue(false);
	itemBoxSizer_checkbox->Add(HideRun,0,wxALIGN_CENTER_VERTICAL|wxALL);
	RunAs = new wxCheckBox( itemStaticBoxSizer3->GetStaticBox(),wxID_ANY, _T("����ԱȨ��"), wxDefaultPosition, wxDefaultSize, 0 );
	RunAs->SetValue(false);
	itemBoxSizer_checkbox->Add(RunAs,0,wxALIGN_CENTER_VERTICAL|wxALL);
	RequiredArg = new wxCheckBox( itemStaticBoxSizer3->GetStaticBox(),wxID_ANY, _T("ǿ���������"), wxDefaultPosition, wxDefaultSize, 0 );
	RunAs->SetValue(false);
	itemBoxSizer_checkbox->Add(RequiredArg,0,wxALIGN_CENTER_VERTICAL|wxALL);

    wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer17, 0, wxGROW|wxALL, 5);
	if (this->flags == MENU_CMD_EDIT)
	{
		this->SetTitle("�༭�������");
	}
	else
	{
		wxButton* itemButton20 = new wxButton( itemDialog1, ID_CMD_ADD_DIR,"�������" , wxDefaultPosition, wxDefaultSize,wxBU_EXACTFIT);
		itemButton20->SetHelpText("�Զ�ɨ��Ŀ¼�������");
		if (DlgAddNewCmd::ShowToolTips())
			itemButton20->SetToolTip("�Զ�ɨ��Ŀ¼�������");
		itemBoxSizer17->Add(itemButton20, 0, wxALIGN_TOP|wxALL, 5);
	}

    wxStdDialogButtonSizer* itemStdDialogButtonSizer = new wxStdDialogButtonSizer;

    itemBoxSizer17->Add(itemStdDialogButtonSizer, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxButton* itemButton18 = new wxButton( itemDialog1, wxID_OK, "ȷ��(&O)", wxDefaultPosition, wxDefaultSize, 0 );
    itemButton18->SetDefault();
    itemStdDialogButtonSizer->AddButton(itemButton18);

    wxButton* itemButton19 = new wxButton( itemDialog1, wxID_CANCEL,"ȡ��(&C)", wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer->AddButton(itemButton19);

    itemStdDialogButtonSizer->Realize();
	cmdName->SetFocus();
#ifndef _DISABLE_DND_
	this->SetDropTarget(new NewCmdDnd(this));
#endif
////@end DlgAddNewCmd content construction
}


/*!
 * Should we show tooltips?
 */

bool DlgAddNewCmd::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap DlgAddNewCmd::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin DlgAddNewCmd bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end DlgAddNewCmd bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon DlgAddNewCmd::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin DlgAddNewCmd icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end DlgAddNewCmd icon retrieval
}
#endif
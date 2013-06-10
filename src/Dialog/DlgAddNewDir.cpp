/////////////////////////////////////////////////////////////////////////////
// Name:        DlgAddNewDir.cpp
// Purpose:     
// Author:      test
// Modified by: 
// Created:     09/06/2013 21:11:35
// RCS-ID:      
// Copyright:   test
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "DlgAddNewDir.h"
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

#include "DlgAddNewDir.h"
#include "ALMRunConfig.h"

////@begin XPM images

#include "find.xpm"
////@end XPM images


/*!
 * DlgAddNewDir type definition
 */

IMPLEMENT_DYNAMIC_CLASS( DlgAddNewDir, wxDialog )


/*!
 * DlgAddNewDir event table definition
 */

BEGIN_EVENT_TABLE( DlgAddNewDir, wxDialog )

////@begin DlgAddNewDir event table entries
////@end DlgAddNewDir event table entries
EVT_BUTTON(ID_BUTTON,onButtonClick)
EVT_BUTTON(wxID_OK,onButtonClick)
END_EVENT_TABLE()


/*!
 * DlgAddNewDir constructors
 */

DlgAddNewDir::DlgAddNewDir()
{
    Init();
}

DlgAddNewDir::DlgAddNewDir( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * DlgAddNewDir creator
 */

bool DlgAddNewDir::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin DlgAddNewDir creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    SetIcon(GetIconResource(wxT("find.xpm")));
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end DlgAddNewDir creation
    return true;
}


/*!
 * DlgAddNewDir destructor
 */

DlgAddNewDir::~DlgAddNewDir()
{
////@begin DlgAddNewDir destruction
////@end DlgAddNewDir destruction
}


/*!
 * Member initialisation
 */

void DlgAddNewDir::Init()
{
////@begin DlgAddNewDir member initialisation
////@end DlgAddNewDir member initialisation
}


/*!
 * Control creation for DlgAddNewDir
 */

void DlgAddNewDir::CreateControls()
{    
////@begin DlgAddNewDir content construction
    DlgAddNewDir* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, wxGetTranslation(wxString() + (wxChar) 0x76EE + (wxChar) 0x5F55 + wxT(":")), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    dirName = new wxTextCtrl( itemDialog1, ID_DLG_DIR_DIR, wxEmptyString, wxDefaultPosition, wxSize(220, -1), wxTE_READONLY );
    dirName->Enable(false);
    itemBoxSizer3->Add(dirName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton6 = new wxButton( itemDialog1, ID_BUTTON, wxGetTranslation(wxString() + (wxChar) 0x6D4F + (wxChar) 0x89C8 + wxT("(&B)")), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer3->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer7, 0, wxGROW|wxLEFT|wxRIGHT, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, wxGetTranslation(wxString() + (wxChar) 0x8FC7 + (wxChar) 0x6EE4 + wxT("(") + (wxChar) 0x5305 + (wxChar) 0x542B + wxT("):")), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer7->Add(itemStaticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    dirInclude = new wxTextCtrl( itemDialog1, ID_DIR_INCLUDE, wxEmptyString, wxDefaultPosition, wxSize(250, -1), 0 );
    itemBoxSizer7->Add(dirInclude, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer10, 0, wxGROW|wxLEFT|wxRIGHT, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemDialog1, wxID_STATIC, wxGetTranslation(wxString() + (wxChar) 0x8FC7 + (wxChar) 0x6EE4 + wxT("(") + (wxChar) 0x6392 + (wxChar) 0x9664 + wxT("):")), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemStaticText11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    dirExclude = new wxTextCtrl( itemDialog1, ID_DIR_EXCLUDE, wxEmptyString, wxDefaultPosition, wxSize(250, -1), 0 );
    itemBoxSizer10->Add(dirExclude, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer13, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxStaticText* itemStaticText14 = new wxStaticText( itemDialog1, wxID_STATIC, wxGetTranslation(wxString() + (wxChar) 0x5B50 + (wxChar) 0x76EE + (wxChar) 0x5F55 + (wxChar) 0x5C42 + (wxChar) 0x6570 + wxT(":")), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(itemStaticText14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    dirSub = new wxSpinCtrl( itemDialog1, ID_DIR_SUB, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -1, 100, 0 );
    itemBoxSizer13->Add(dirSub, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer16 = new wxStdDialogButtonSizer;

    itemBoxSizer13->Add(itemStdDialogButtonSizer16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxButton* itemButton17 = new wxButton( itemDialog1, wxID_OK, wxGetTranslation(wxString() + (wxChar) 0x786E + (wxChar) 0x5B9A + wxT("(&O)")), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemStdDialogButtonSizer16->AddButton(itemButton17);

    wxButton* itemButton18 = new wxButton( itemDialog1, wxID_CANCEL, wxGetTranslation(wxString() + (wxChar) 0x53D6 + (wxChar) 0x6D88 + wxT("(&C)")), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemStdDialogButtonSizer16->AddButton(itemButton18);

    itemStdDialogButtonSizer16->Realize();

////@end DlgAddNewDir content construction
}

void DlgAddNewDir::onButtonClick(wxCommandEvent& e)
{
	if (e.GetId() == ID_BUTTON)
	{
		wxString name = wxDirSelector("��ѡ��һ��Ŀ¼");
		if (!name.empty())
			dirName->SetValue(name);
		return;
	}
	if (!g_config)
		return;
	wxString dName = dirName->GetValue();
	if (dName.empty())
		return;
	if (g_config->AddDir(dName,dirInclude->GetValue(),dirExclude->GetValue(),dirSub->GetValue()) == -1)
	{
		wxMessageBox("���Ŀ¼ʧ��,�����ǲ�������","��ʾ");
		return;
	}
	this->EndDialog(wxID_OK);
}

/*!
 * Should we show tooltips?
 */

bool DlgAddNewDir::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap DlgAddNewDir::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin DlgAddNewDir bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end DlgAddNewDir bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon DlgAddNewDir::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin DlgAddNewDir icon retrieval
    wxUnusedVar(name);
    if (name == _T("find.xpm"))
    {
        wxIcon icon(find_xpm);
        return icon;
    }
    return wxNullIcon;
////@end DlgAddNewDir icon retrieval
}

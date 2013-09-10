#include "MerryCommandManager.h"
#include "MerryHotkey.h"
#include "ALMRunConfig.h"
#include "MerryApp.h"
#include "DlgConfig.h"
#include "wx/dir.h"

ALMRunConfig* g_config = NULL;
const char *ALMRunConfig::config_str[] = {"StayOnTop","NumberKey","ShowTrayIcon","ShowTopTen","ExecuteIfOnlyOne","IndexFrom0to9","OrderByPre","ShowTip","DisableWow64FsRedirection"};
const char *ALMRunConfig::config_tip[] = {
	"���ֳ��򴰿��ö�,Ĭ�Ͻ���.",
	"ѡ��ʱ��0-9��ִ�ж�Ӧ��ŵĿ����",
	"ѡ��ʱ��ϵͳ������ʾͼ��",
	"ѡ��ʱ����ʾǰ10������",
	"ѡ��ʱ�б�ֻʣһ��ʱ���谴������ִ��",
	"���δѡ�У����˳��Ϊ 1, 2, ..., 9, 0",
	"���ѡ��, �����б���ǰ�ƥ�����ǰ��",
	"���ѡ��,����ƶ��б����Ŀʱ����ʾ��ע��Ϣ��������",
	"���г���֮ǰ����ϵͳ��WOW64�ض���,�����64λϵͳ�ϲ���64λ�����޷����е�����",
};
ALMRunConfig::ALMRunConfig()
{
	if (wxGetEnv(wxT("ALMRUN_HOME"),&Home))
	{	
		cfg_file = Home + wxT("config/ALMRun.ini");
		if (!wxFileExists(cfg_file))
			MoveFile(Home + wxGetApp().GetAppName().Append(".ini"),cfg_file);
	}
	if (wxFileExists(cfg_file) == false)
	{
		CompareMode = 0;
		config[NumberKey] = false;
		config[ShowTrayIcon] = true;
		config[ShowTopTen] = true;
		config[ExecuteIfOnlyOne] = false;
		config[IndexFrom0to9] = false;
		config[OrderByPre] = false;
		config[StayOnTop] = false;
		conf = NULL;
		cfg_time = 0;
		return;
	}
	//lastId = 0;
	conf = new wxFileConfig(wxT("ALMRun"),wxEmptyString,cfg_file,wxEmptyString,wxCONFIG_USE_LOCAL_FILE);
	conf->SetPath("/Config");
	conf->SetRecordDefaults(false);
	config[OrderByPre] = conf->ReadBool(config_str[OrderByPre],false);
	config[NumberKey] = conf->ReadBool(config_str[NumberKey],false);
	config[ShowTopTen] = conf->ReadBool(config_str[ShowTopTen],true);
	config[ExecuteIfOnlyOne] = conf->ReadBool(config_str[ExecuteIfOnlyOne],false);
	config[IndexFrom0to9] = conf->ReadBool(config_str[IndexFrom0to9],true);
	config[ShowTip] = conf->ReadBool(config_str[ShowTip],true);

	config[DisableWow64FsRedirection] = conf->ReadBool(config_str[DisableWow64FsRedirection],true);
	CompareMode = conf->ReadLong("CompareMode",0);
	HotKey = conf->Read("HotKey","A-R");
	HotKeyReLoad = conf->Read("HotKeyReLoad");
	this->set("Explorer",conf->Read("Explorer"));
	this->set("Root",conf->Read("Root"));
	this->set("ShowTrayIcon",conf->ReadBool(config_str[ShowTrayIcon],true));
	this->set("StayOnTop",conf->ReadBool("StayOnTop",false));
	if (!g_hotkey->RegisterHotkey(g_commands->AddCommand(wxEmptyString,wxEmptyString,"toggleMerry",-1,HotKey,0)))
	{
		this->set("ShowTrayIcon",true);
		wxMessageBox(wxString::Format("�ȼ� %s ע��ʧ��!",HotKey),"������ʾ",0x00000100);
	}
	if (!HotKeyReLoad.empty())
		g_hotkey->RegisterHotkey(g_commands->AddCommand(wxEmptyString,wxEmptyString,"ReConfig",-1,HotKeyReLoad,0));
	this->OldToNew();
	this->ConfigCommand();
	conf->SetPath("/Config");
	cfg_time = wxFileModificationTime(cfg_file);
}

bool ALMRunConfig::set(const wxString& name,const wxString &value)
{
	if (name.Cmp("Explorer") == 0)
		Explorer = value;
	else if (name.Cmp("Root") == 0)
	{
		Root = value;
		if (Root.empty())
			return false;
		wxSetWorkingDirectory(Root);
	}
	else if(name.Cmp("HotKey") == 0)
		HotKey = value;
	else if(name.Cmp("HotKeyReLoad") == 0)
		HotKeyReLoad = value;
	else
		return false;
	return ::wxSetEnv(wxT("ALMRUN_")+name.Upper(),value);
}

bool ALMRunConfig::set(const wxString& name,const int value)
{
	if (name.Cmp("CompareMode") == 0)
		CompareMode = value;
	else if (name.Cmp("NumberKey") == 0)
		config[NumberKey] = (value != 0);
	else if (name.Cmp("ShowTrayIcon") == 0)
	{
		config[ShowTrayIcon] = value != 0;
		::wxGetApp().GetFrame().ShowTrayIcon(config[ShowTrayIcon]);
	}
	else if (name.Cmp("ShowTopTen") == 0)
		config[ShowTopTen] = value != 0;
	else if (name.Cmp("ExecuteIfOnlyOne") == 0)
		config[ExecuteIfOnlyOne] = value != 0;
	else if (name.Cmp("StayOnTop") == 0)
	{
		config[StayOnTop] = value != 0;
		MerryFrame& frm = wxGetApp().GetFrame();
		long style = frm.GetWindowStyle();
		if (config[StayOnTop])
			frm.SetWindowStyle(style|wxSTAY_ON_TOP);
		else if (style & wxSTAY_ON_TOP)
			frm.SetWindowStyle(style^wxSTAY_ON_TOP);
	}
	else
		return false;
	return true;
}

void ALMRunConfig::WriteConfig(const wxString& name,const wxString& value)
{
	if (conf)
	{
		conf->Write(name,value);
		conf->Flush();
	}
}

void ALMRunConfig::GuiConfig()
{
	if (!conf)
		return;
	DlgConfig* dlg = new DlgConfig(0);
	dlg->config_hotkey->SetValue(HotKey);
	for(int i=sizeof(config)-1;i>=0;--i)
	{
		if (config[i])
			dlg->config->Check(i);
	}
	if (dlg->ShowModal() == wxID_OK)
	{
		conf->SetPath("/Config");
		if (!dlg->config_hotkey->GetValue().IsSameAs(HotKey))
			conf->Write("HotKey",dlg->config_hotkey->GetValue());
		for(int i=sizeof(config)-1;i>=0;--i)
		{
			if (config[i] != dlg->config->IsChecked(i))
				conf->Write(config_str[i],!config[i]);
		}
		conf->Flush();
		::wxGetApp().GetFrame().NewConfig();
	}
	dlg->Destroy();
}

bool ALMRunConfig::SaveCfg()
{
	if (conf)
	{
		conf->Flush();
		cfg_time = wxFileModificationTime(cfg_file);
		return true;
	}
	return false;
}

int ALMRunConfig::AddCmd(const wxString& cmd,const wxString& name,const wxString& key,const wxString& desc,const int id)
{
	wxString cmdName = name;
	if (cmdName.empty() && key.empty())
	{
		cmdName = wxFileName(cmd).GetFullName();
		//if (!cmdName.empty())
		//	cmdName.Append(".ALMRun");
	}
	int Id = id;
	if (id == -1)
	{
		for(Id=0;;++Id)
		{
			if (!conf->HasGroup(wxString::Format("/cmds/%d",Id)))
				break;
			if (Id >= 4000)
				return -1;
		}
	}
	int cmdId = g_commands->AddCommand(cmdName,desc,cmd,0,key,0,(Id << 4) | CMDS_FLAG_ALMRUN_CMDS);
	if (cmdId < 0)
	{
		wxMessageBox(wxString::Format("�������ʧ��->����[%d]:%s",Id,name),"��������");
		return -1;
	}
	if (!key.empty() && !g_hotkey->RegisterHotkey(cmdId))
	{
		wxMessageBox(wxString::Format("�������ʧ��->ע���ȼ�ʧ��:%s\n,����[%d]:%s",key,Id,name),"����!");
		g_commands->DelCommand(cmdId);
		return -1;
	}
	if (id == -1)
	{
		if (!this->ModifyCmd(Id,cmd,name,key,desc))
			return -1;
	}

	return Id;
}

bool ALMRunConfig::ModifyCmd(const int id,const wxString& cmd,const wxString& name,const wxString& key,const wxString& desc)
{
	if (!conf || id < 0)
		return false;
	if (cmd.empty() && !conf->DeleteGroup(wxString::Format("/cmds/%d",id)))
		return false;

	wxString oldPath = conf->GetPath();
	conf->SetPath(wxString::Format("/cmds/%d",id));
	conf->Write("cmd",cmd);
	if (!name.empty())
		conf->Write("name",name);
	else
		conf->DeleteEntry("name");

	if (!key.empty())
		conf->Write("key",key);
	else
		conf->DeleteEntry("key");

	if (!desc.empty())
		conf->Write("desc",desc);
	else
		conf->DeleteEntry("desc");

	conf->SetPath(oldPath);
	return this->SaveCfg();
}

bool ALMRunConfig::DeleteCmd(const int id)
{
	if (conf && conf->DeleteGroup(wxString::Format("/cmds/%d",id)))
		return this->SaveCfg();
	return false;
}

bool ALMRunConfig::DeleteDir(const int id)
{
	if (conf && conf->DeleteGroup(wxString::Format("/dirs/%d",id)))
	{
		conf->Flush();
		return true;
	}
	return false;
}

int ALMRunConfig::AddDir(const wxString& path,const wxString& inc,const wxString& exc,const int sub,const UINT id)
{
	if (!conf)
		return -1;
	int i = 0;
	wxString entry;
	if (id == -1)
	{
		do
		{
			entry.Printf("/dirs/%d",i++);
			if (i == 1000)
				return -1;
		
		} while (conf->HasGroup(entry));
	}
	else
	{
		i = id;
		entry.Printf("/dirs/%d",i);
	}
	wxString oldPath = conf->GetPath();
	conf->SetPath(entry);
	conf->Write("path",path);
	conf->Write("sub",sub);
	if (!inc.empty())
		conf->Write("include",inc);
	if (!exc.empty())
		conf->Write("exclude",exc);
	conf->Flush();
	return i;
}

bool ALMRunConfig::Changed()
{
	return (cfg_time  && cfg_time != wxFileModificationTime(cfg_file));
}

void ALMRunConfig::ListFiles(const wxString& dirname,wxArrayString *files,const wxString& filespec,const wxString& exclude, const int sub)
{
	wxArrayString specs = wxSplit(filespec,'|');
	wxArrayString exc = wxSplit(exclude,'|');
	if (specs.GetCount() == 0)
		specs.Add(wxEmptyString);
	this->ListFiles(dirname,files,specs,sub);
	for(int i = files->Count() - 1;i>=0;--i)
	{
		int j = -1;
		for(j = exc.size()-1;j >= 0;--j)
		{//���˷�������������
			if (files->Item(i).Matches(exc[j]))
				break;
		}
		if (j >= 0)
			files->RemoveAt(i);
	}
	specs.Clear();
	exc.Clear();
}

void ALMRunConfig::ListFiles(const wxString& dirname,wxArrayString *files,const wxArrayString& filespec,const int sub)
{
	static int cur_sub_dir = 0;
	int flags = wxDIR_HIDDEN|wxDIR_FILES;
	wxDir dir(dirname);
	if (!dir.IsOpened())
		return;
	if (sub == -1)
		flags |= wxDIR_DIRS;
	for(int i=filespec.GetCount() - 1; i >= 0;--i)
		dir.GetAllFiles(dirname,files,filespec[i],flags);

	if (sub == -1 || cur_sub_dir == sub)
		return;
	++cur_sub_dir;//��Ŀ¼����+1
	wxString file;
	bool test = dir.GetFirst(&file,wxEmptyString,wxDIR_DIRS|wxDIR_HIDDEN);
	wxString path = dir.GetNameWithSep();
	while(test)
	{
		ListFiles(path+file,files,filespec,sub);
		test = dir.GetNext(&file);
	}
	--cur_sub_dir;//��Ŀ¼����-1
	return;
}

//�°�������ļ�?
void ALMRunConfig::ConfigCommand()
{
	wxString name;
	wxString key;
	wxString cmd;
	wxString desc;
	wxString cmds;
	long cmdId;
	long index = 0;
	//����
	conf->SetPath("/cmds");
	for(bool test = conf->GetFirstGroup(cmds,index); test ; conf->SetPath("../"),test = conf->GetNextGroup(cmds,index))
    {
		if (!cmds.IsNumber())
			continue;
		conf->SetPath(cmds);
		cmd = conf->Read("cmd");
		if (cmd.empty() && key.empty())
			continue;
		name = conf->Read("name");
		key = conf->Read("key");
		desc = conf->Read("desc");
		cmds.ToLong(&cmdId,10);
		//if (cmdId < 1000 && cmdId > lastId)
		//	lastId = cmdId + 1;
		this->AddCmd(cmd,name,key,desc,cmdId);
    }
	//�Զ�ɨ��Ŀ¼����
	conf->SetPath("/dirs");
	//��ȡĬ�ϲ���
	def_dir_cfg.sub = conf->ReadLong("sub",0);
	def_dir_cfg.include = conf->Read("include");
	def_dir_cfg.exclude = conf->Read("exclude");

	wxArrayString paths;
	for(bool test = conf->GetFirstGroup(cmds,index); test ; conf->SetPath("../"),test = conf->GetNextGroup(cmds,index))
	{
		wxArrayString files;
		conf->SetPath(cmds);
		paths = wxSplit(conf->Read("path"),'|');
		if (paths.empty())
			continue;
		for(int i=paths.size()-1;i>=0;--i)
			this->ListFiles(paths[i],&files, conf->Read("include",def_dir_cfg.include) , conf->Read("exclude",def_dir_cfg.exclude) , conf->ReadLong("sub",def_dir_cfg.sub));
		g_commands->AddFiles(files);
	}
	conf->SetPath("/Config");
}

//�ɰ��Զ�ת��Ϊ�°汾
void ALMRunConfig::OldToNew()
{
	size_t sizes;
	wxString name;
	wxString key;
	wxString cmd;
	wxString desc;
	wxString cmds;
	conf->SetExpandEnvVars(false);
	if (conf->HasGroup("/Command"))
	{
		sizes = conf->ReadLong("/Command/cmds.size",0);
		while(sizes)
		{
			conf->SetPath(wxString::Format("/cmds/%d",1000+sizes));
			cmds = wxString::Format("/Command/cmds.%d.",sizes);
			--sizes;
			cmd = conf->Read(cmds + "cmd");
			if (cmd.empty() && key.empty())
				continue;
			conf->Write("cmd",cmd);
			conf->Write("name",conf->Read(cmds + "name"));
			conf->Write("key",conf->Read(cmds + "key"));
			conf->Write("desc",conf->Read(cmds + "desc"));
		}
		conf->DeleteGroup("/Command");
		conf->Flush();
	}
	if (conf->HasGroup("/directories"))
	{
		conf->SetPath("/directories");
		sizes = conf->ReadLong("dirs.size",0);
		conf->Write("/dirs/sub",conf->ReadLong("dirs.def.sub",0));
		conf->Write("/dirs/include",conf->Read("dirs.def.include"));
		conf->Write("/dirs/exclude",conf->Read("dirs.def.exclude"));
		while(sizes)
		{
			conf->SetPath(wxString::Format("/dirs/%d",1000+sizes));
			cmds = wxString::Format("/directories/dirs.%d.",sizes);
			--sizes;
			conf->Write("path",conf->Read(cmds + "path"));
			conf->Write("include",conf->Read(cmds + "include"));
			conf->Write("exclude", conf->Read(cmds + "exclude"));
			conf->Write("sub",conf->ReadLong(cmds + "sub",0));
		}
		conf->DeleteGroup("/directories");
		conf->Flush();
	}
	conf->SetExpandEnvVars(true);
	conf->SetPath("/Config");
}

void ALMRunConfig::get(const wxString& name)
{

}

ALMRunConfig::~ALMRunConfig()
{
	__DEBUG_BEGIN("")
	if (conf)
		delete conf;
	conf = NULL;
	__DEBUG_END("")
}

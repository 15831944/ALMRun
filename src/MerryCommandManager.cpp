#include "MerryCommandManager.h"
#include "MerryError.h"
#include "MerryLua.h"
#include "MerryInformationDialog.h"
#include "ALMRunConfig.h"
#include  <algorithm>

MerryCommandManager* g_commands = NULL;
static wxString cmdPrefix;
MerryCommandManager::~MerryCommandManager()
{
	__DEBUG_BEGIN("")
	for (size_t i=0; i<m_commands.size(); ++i)
		delete m_commands[i];
	__DEBUG_END("")
}

const void MerryCommandManager::AddFiles(const wxArrayString& files)
{
	for(int i=files.GetCount()-1;i >= 0;--i)
	{
		m_commands.push_back(new MerryCommand(m_commands.size() | (CMDS_FLAG_DIRS<<16),wxFileNameFromPath(files[i]),wxEmptyString,wxString::Format("\"%s\"",files[i])));
	}
}

const void MerryCommandManager::AddFiles(const wxArrayString& files,const wxArrayString& excludes)
{
	if (excludes.empty())
		return AddFiles(files);
	for(int i=files.GetCount()-1;i >= 0;--i)
	{
		int j;
		for(j = excludes.size()-1;j >= 0;--j)
		{//���˷�������������
			if (files[i].Matches(excludes[j]))
				break;
		}
		if (j == -1)
			m_commands.push_back(new MerryCommand(m_commands.size() |(CMDS_FLAG_DIRS<<16),wxFileNameFromPath(files[i]),wxEmptyString,wxString::Format("\"%s\"",files[i])));
	}
}

bool MerryCommandManager::DelCommand(int commandID)
{
	if (GetCommand(commandID))
	{
		m_commands[commandID] = new MerryCommand(-1,wxEmptyString);
		return true;
	}
	return false;
}

const int MerryCommandManager::AddCommand(const wxString& commandName,const wxString& commandDesc,const wxString& commandLine, int funcRef, const wxString& triggerKey,int flags)
{
	if (m_commands.size() >= 1000)
	{
		MerrySetLastError(wxT("\n����1000���������ƣ�Ŀǰ�����������������Գ���1000������������������ϵ�һ򵽵���վ���� http://chenall.net"));
		return -2;
	}

	if (commandName.empty() && triggerKey.empty())
	{
		MerrySetLastError(wxT("\n�ȼ�(KEY)������(NAME),������Ҫ����һ��"));
		return -1;
	}
	for (size_t i=0; i<m_commands.size(); ++i)
	{
		const MerryCommand* command = m_commands[i];
		assert(command);
		if (!commandName.empty() && commandName.IsSameAs(command->GetCommandName(), false))
		{
			MerrySetLastError(wxString::Format(wxT("\n����[%s]�Ѿ�����\n\n%s"),commandName,command->GetDetails()));
			return -1;
		}
		if (!triggerKey.empty() && triggerKey.IsSameAs(command->GetTriggerKey(), false))
		{
			MerrySetLastError(wxString::Format(wxT("\n����[%s]�ȼ��ظ�\n\n%s\n"), commandName,command->GetDetails()));
			return -1;
		}
	}
	MerryCommand* command = new MerryCommand(m_commands.size() | (flags<<16), commandName,commandDesc,commandLine, funcRef, triggerKey);
	m_commands.push_back(command);
	return command->GetCommandID();
}

const MerryCommand* MerryCommandManager::GetCommand(int commandID) const
{
	if (commandID >= (int)m_commands.size() || commandID < 0)
		return NULL;
	assert(m_commands[commandID]);
	return m_commands[commandID];
}

/*
	��������
	1.��������ֵm_order,ֵԽ������Խǰ��.
	2.ǰ׺ƥ������
	3.����������
*/
static bool mysort(MerryCommand *s1,MerryCommand  *s2)
{
	int cmp  = s1->GetOrder() - s2->GetOrder();
	if (cmp == 0)//����ֵһ��ʱ
	{
		if (s1->m_compare == 0)//ǰ׺ƥ������
		{
			if (s2->m_compare != 0)
				return true;
		}
		else if (s2->m_compare == 0)
			return false;
		return s2->GetCommandName().Upper() > s1->GetCommandName().Upper();
	}
	return cmp > 0;
}
/*
	��������
	1.ǰ׺ƥ������
	2.��������ֵm_order,ֵԽ������Խǰ��.
	3.����������
*/
static bool SortPreOrder(MerryCommand *s1,MerryCommand  *s2)
{
	if (s1->m_compare == 0)//ǰ׺ƥ��
	{
		if (s2->m_compare != 0)//ǰ׺��ƥ��
			return true;
		int cmp = s1->GetOrder() - s2->GetOrder();
		if (cmp != 0)
			return cmp >0 ;
	}
	else if (s2->m_compare == 0)
		return false;
	return s2->GetCommandName().Upper() > s1->GetCommandName().Upper();
}

//��ʼ����
static bool command_sort(MerryCommand *s1,MerryCommand  *s2)
{
	int cmp = s1->GetOrder() - s2->GetOrder();
	if (cmp == 0)
		return s2->GetCommandName().Upper() > s1->GetCommandName().Upper();
	return cmp > 0;
}

MerryCommandArray MerryCommandManager::Collect(const wxString& commandPrefix) const
{
	MerryCommandArray commands;
	MerryCommandArray l_commands;
	bool test_cmp = false;
	int cmp_find = -1;
	for (size_t i=0; i<m_commands.size(); ++i)
	{
		if (m_commands[i]->GetCommandName().empty())
			continue;
		l_commands.push_back(m_commands[i]);
	}
	sort(l_commands.begin(),l_commands.end(),command_sort);
	if (commandPrefix.empty())
		return l_commands;
	cmdPrefix = commandPrefix.Upper();
	if (commandPrefix.empty())
		return l_commands;
	for (size_t i=0; i<l_commands.size(); ++i)
	{
		MerryCommand* command = l_commands[i];
		assert(command);
		const wxString& commandName = command->GetCommandName(0);
		if (commandName.size() < commandPrefix.size())
			continue;
	#ifdef _ALMRUN_CONFIG_H_
		switch(g_config->CompareMode)
		{
			case 1:
				test_cmp = (commandName.Upper().compare(0, commandPrefix.size(), cmdPrefix) == 0);
				break;
			case 2:
				test_cmp = g_lua->onCompare(commandName.c_str(),commandPrefix.c_str());
				break;
			default:
	#endif//ifdef _ALMRUN_CONFIG_H_
				cmp_find = commandName.Upper().find(cmdPrefix);
				if (cmp_find == wxNOT_FOUND)
					test_cmp = false;
				else
				{
					test_cmp = true;
					int cmdLen = command->GetCommandName().Len();
					if (cmp_find > cmdLen)
						cmp_find -= cmdLen + 1;
				}
	#ifdef _ALMRUN_CONFIG_H_
				break;
		}
	#endif//ifdef _ALMRUN_CONFIG_H_
		if (test_cmp)
		{
			command->m_compare = cmp_find;
			commands.push_back(command);
			if (g_config->config[ShowTopTen] && commands.size() >= 10 && !g_config->config[OrderByPre])
				break;
		}
	}
#ifdef _ALMRUN_CONFIG_H_
	sort(commands.begin(),commands.end(),g_config->config[OrderByPre]?SortPreOrder:mysort);
	if (g_config->config[ShowTopTen] && commands.size() >= 10)
		commands.resize(10);
#endif//ifdef _ALMRUN_CONFIG_H_
	return commands;
}
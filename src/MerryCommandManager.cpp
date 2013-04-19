#include "MerryCommandManager.h"
#include "MerryError.h"
#include "MerryLua.h"
#include "MerryInformationDialog.h"
#include  <algorithm>

MerryCommandManager* g_commands = NULL;
int Ex_CompareMode;
MerryCommandManager::~MerryCommandManager()
{
	for (size_t i=0; i<m_commands.size(); ++i)
		delete m_commands[i];
}

const int MerryCommandManager::AddCommand(const wxString& commandName,const wxString& commandDesc,const wxString& commandLine, int funcRef, const wxString& triggerKey,int order)
{
	int order_id = 0;
	if (m_commands.size() >= 1000)
	{
		::MerrySetLastError(wxT("1000 commands limit,If you need more please contact me!"));
		return -2;
	}

	if (commandName.empty() && triggerKey.empty())
	{
		//::MerrySetLastError(wxT("Command name and key not found"));
		return -1;
	}
	for (size_t i=0; i<m_commands.size(); ++i)
	{
		const MerryCommand* command = m_commands[i];
		assert(command);
		if (!commandName.empty() && commandName.IsSameAs(command->GetCommandName(), false))
		{
			new MerryInformationDialog(wxT("Command name exists"),wxString::Format(wxT("�������������ظ�:\r\n%s"),commandName));
			//::MerrySetLastError(wxString::Format(wxT("Command name \"%s\" already exists[%d]"), commandName,command->GetCommandID()));
			return -1;
		}
		if (!triggerKey.empty() && triggerKey.IsSameAs(command->GetTriggerKey(), false))
		{
			new MerryInformationDialog(wxT("Command key exists"),wxString::Format(wxT("���������ȼ��ظ�:\r\n%s"),triggerKey));
			//::MerrySetLastError(wxString::Format(wxT("Command key \"%s\" already exists[%d]"), triggerKey,command->GetCommandID()));
			return -1;
		}
		if (command->m_order > order_id)
			order_id = i;
	}
	MerryCommand* command = new MerryCommand(m_commands.size(), commandName,commandDesc,commandLine, funcRef, triggerKey,order);
#if 1
	m_commands.push_back(command);
#else
	if (order == 0)
		m_commands.push_back(command);
	else
		m_commands.insert(m_commands.begin()+order_id,command);
#endif
	return command->GetCommandID();
}

const MerryCommand* MerryCommandManager::GetCommand(int commandID) const
{
	if (commandID >= (int)m_commands.size() || commandID < 0)
		return NULL;
	assert(m_commands[commandID]);
	return m_commands[commandID];
	/*
	size_t mx = m_commands.size();
	for (size_t i=0; i<mx; ++i)
	{
		MerryCommand* command = m_commands[i];
		assert(command);
		if (command->GetCommandID() == commandID)
			return command;
	}
	return NULL;
	*/
}


static bool mysort(MerryCommand *s1,MerryCommand  *s2)
{
	return s1->m_order > s2->m_order;
}

const int MerryCommandManager::SetCmdOrder(int commandID)
{
	assert(m_commands[commandID]);
	MerryCommand* cmd= m_commands[commandID];
	wxString commandName = cmd->GetCommandName();
	if (commandName.empty() || !g_lua)
		return ++cmd->m_order;
	lua_State* L = g_lua->GetLua();
	assert(L);
	lua_getglobal(L, "SetCmdOrder");
	if (!lua_isnil(L, 1))
	{
		lua_pushstring(L, commandName.c_str());
		lua_pushinteger(L, cmd->m_order);
		if (lua_pcall(L, 2, 1, 0) == 0)
			cmd->m_order = lua_tointeger(L,-1);

	}
	lua_pop(L, 1);
	commandName.Clear();
	return cmd->m_order;
}

MerryCommandArray MerryCommandManager::Collect(const wxString& commandPrefix) const
{
	MerryCommandArray commands;
	bool test_cmp = false;
	for (size_t i=0; i<m_commands.size(); ++i)
	{
		MerryCommand* command = m_commands[i];
		assert(command);
		const wxString& commandName = command->GetCommandName(0);
		if (commandName.empty())
			continue;
		if (commandName.size() < commandPrefix.size())
			continue;
		switch(Ex_CompareMode)
		{
			case 1:
				test_cmp = (commandName.Upper().compare(0, commandPrefix.size(), commandPrefix.Upper()) == 0);
				break;
			case 2:
				test_cmp = g_lua->onCompare(commandName.c_str(),commandPrefix.c_str());
				break;
			default:
				test_cmp = (commandName.Upper().find(commandPrefix.Upper()) != wxNOT_FOUND);
				break;
		}
		if (test_cmp)
			commands.push_back(command);
	}
	sort(commands.begin(),commands.end(),mysort);
	return commands;
}

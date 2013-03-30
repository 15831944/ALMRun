local eventTable = {}
function addEventHandler(eventName, eventHandler)
	if not eventTable[eventName] then
		eventTable[eventName] = {}
	end

	local events = eventTable[eventName] 
	table.insert(events, eventHandler)

	if not _G[eventName] then
		_G[eventName] = function(...)
			for _, _eventHandler in ipairs(events) do
				_eventHandler(...)
			end
		end
	end
end

function enterString(str)
	local t = {}
	str:gsub('.', function(c)
		table.insert(t, c)
	end)
	enterKey(table.unpack(t))
end

function trim(s)
  return (s:gsub("^%s*(.-)%s*$", "%1"))
end

function get_name(file)
	return file:match("[^\\]+$")
end

-- �Զ�ɨ��ָ��Ŀ¼��ָ����չ�������б��� --WINDOWS
function scan_dir(path,ext)
	if ext == nil then
		ext = "*.exe"
	end
	local f = io.popen('dir '..path..'\\'..ext..' /b /s')
	local path_len = string.len(path)+1
	for line in f:lines() do
		m_name = get_name(string.sub(line,path_len))
		addCommand{ name = m_name,cmd = line}
	end
	f:close()
end

-- Events
local histroy = io.open(MERRY_ROOT_PATH .. 'histroy.lua', 'a')
addEventHandler('onUndefinedCommand', function(commandName, commandArg)
	local commandNameArray = { commandName }
	if MAC then
		table.insert(commandNameArray, "/Applications/" .. commandName .. ".app")
		table.insert(commandNameArray, "/Applications/Utilities/" .. commandName .. ".app")
	end

	for _, commandNameFull in ipairs(commandNameArray) do
		if shellExecute(commandNameFull, commandArg) then
			addCommand{ name = commandName, func = function() shellExecute(commandNameFull) end }
			local strCommandName = string.gsub(commandName, '\\', '\\\\')
			local strCommandNameFull = string.gsub(commandNameFull, '\\', '\\\\')
			histroy:write(string.format("addCommand{ name = '%s', func = function() shellExecute('%s') end }\n",
				strCommandName, strCommandNameFull))
			histroy:flush()
			break
		end
	end
end)

addEventHandler('onClose', function()
	histroy:close()
	showWindow(curHideWindow, 'normal')
end)

-- ����ƥ�亯��,�����Լ���չ
function HookCompare(commandName,Prefix)
	return commandName:lower():find(Prefix:lower())
end

-- Ĭ�ϵ�������ú���
CmdCallFunc = function(cmdLine,cmdArg,Flags)
-- ��������'@'��ͷ,���ش���ִ��
	local show = 'normal'
	local DestDir = ''
	if cmdLine:sub(1,1) == '@' then
		cmdLine = cmdLine:sub(2)
		show = 'hide'
	end

	cmdLine = cmdLine:gsub("%%(%S+)%%",os.getenv) --ϵͳ����������չ

	pos = cmdLine:find(":::") -- ����ʹ��:::�ָ�����
	if pos == nil then
		T_arg = cmdLine:match("%s+[%-|/]%a") --����������г���" -"��" /",Ҳ��Ϊ�ǲ�������,Ҫ�ָ�����.
		if T_arg == nil then--���ļ����ָ�����������Ҫ������չ��,��������д���
			cmd = cmdLine:gsub("(.*%.[^%s]+).*", "%1")
			arg = cmdLine:gsub("(.*%.[^%s]+)(.*)$", "%2")
		else
			pos = cmdLine:find(T_arg,1,true)
			cmd = cmdLine:sub(1,pos-1)
			arg = cmdLine:sub(pos+1)
		end
	else
		cmd = cmdLine:sub(1,pos-1)
		arg = cmdLine:sub(pos+3)
	end
	if not (arg == "") then
		cmdArg = arg..' '..cmdArg
	end
-- ��ȡ��ִ�г�������Ŀ¼
	if cmd:match("^%a:") then
		local FileName = cmd:match("[^\\]+$")
		DestDir = cmd:sub(1,-FileName:len()-1)
	end
	if (Flags%2) == 1 then
		shellExecute("explorer.exe","/n,/select,"..cmd,'','')
	else
		shellExecute(cmd,cmdArg,DestDir,show)
	end
end

function read_csv (file,delims)
  local fp = assert(io.open (file))
  local csv = {}
  for line in fp:lines() do
    local row = {}
    for value in line:gmatch("[^"..delims.."]+") do
      row[#row+1] = trim(value)
    end
    if not (row[1] == nil or row[1] == "") then
		csv[#csv+1] = row
    end
  end
  return csv
end

-- ͵��,ֱ�ӵ���ALTRUN������,^_^
function altrun_config(file)
	altrun = read_csv(file,"|")
	for i=1,#altrun do
		addCommand{ name = altrun[i][3], desc = altrun[i][4],cmd = altrun[i][5]}
	end
end

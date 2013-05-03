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
-- �Զ�ɨ��ָ��Ŀ¼��ָ����չ�������б���
function scan_dir(path,ext,sub)
    if ext == nil then
	ext = "*.exe"
    end
    if sub == nil then
	sub = -1
    end
    path = path:gsub("%%(%S+)%%",os.getenv)
    local Files = ListDir(path,ext,sub)
    if Files == nil then 
	return
    end
    for key,value in pairs(Files) do  
	m_name = get_name(value)
	addCommand{ name = m_name,cmd = value}
    end
end
-- Events
addEventHandler('onUndefinedCommand', function(commandName, commandArg)
	local commandNameArray = { commandName }
	if MAC then
		table.insert(commandNameArray, "/Applications/" .. commandName .. ".app")
		table.insert(commandNameArray, "/Applications/Utilities/" .. commandName .. ".app")
	end
	local f = io.open(ALMRUN_CONFIG_PATH .. 'histroy.lua', 'a')
	for _, commandNameFull in ipairs(commandNameArray) do
		if shellExecute(commandNameFull, commandArg) then
			addCommand{ name = commandName, func = function() shellExecute(commandNameFull) end }
			f:write(string.format("addCommand{ name = [[%s]], desc='δ��������,��histroy.lua�ļ���',func = function() shellExecute([[%s]]) end }\n",
				commandName, commandNameFull))
			break
		end
	end
	f:close()
end)
addEventHandler('onClose', function()
-- �����������ȼ����ÿ�ʼ
	local f = io.open(MERRY_ORDER_FILE,'w+')
	for i,v in pairs(CmdOrder) do
		f:write(v..'|'..i.."\n")
	end
	f:close()
-- �����������ȼ��������
	showWindow(curHideWindow, 'normal')
end)
function SplitCmd(cmdLine)
   cmdLine = trim(cmdLine)
   local pos = cmdLine:find(":::") -- ����ʹ��:::�ָ�����
   local cmd,arg
    if pos then
	cmd = cmdLine:sub(1,pos-1)
	arg = cmdLine:sub(pos+3)
	return cmd,arg
    end
    --�����ŵ��ļ���
    cmd = cmdLine:match("^\"(.-)\"")
    if cmd then
	arg = cmdLine:match("^\".-\"%s*(.*)")
	return cmd,arg
    end
    --����������г���" -"��" /",Ҳ��Ϊ�ǲ�������,Ҫ�ָ�����.
    cmd = cmdLine:match("^(.-)%s+[%-|/]%a")
    if cmd then
	arg = cmdLine:match("%s+[%-|/]%a.*")
	return cmd,arg
    end
    cmd = cmdLine:match(".-%.[^%s]+")
    if cmd then
	arg = cmdLine:sub(cmd:len()+1)
	return cmd,arg
    end
    return cmdLine
end
-- Ĭ�ϵ�������ú���
CmdCallFunc = function(cmdLine,cmdArg)
    cmdLine = cmdLine:gsub("%%(%S+)%%",os.getenv) --ϵͳ����������չ
    local cmd,arg = SplitCmd(cmdLine)
    if arg then
	cmdArg = arg..' '..cmdArg
    end
-- ��ȡ��ִ�г�������Ŀ¼
    local DestDir = cmd:match("^(.-)[^\\]+$")
    shellExecute(cmd,cmdArg,DestDir)
end
--��������ÿ���һ������֮�����,��ȡ��Ӧ��������ȼ�,������ֵԽ�����ȼ�Խ��.
function GetCmdOrder(cmdName)
	local order = CmdOrder[cmdName]
	if order then
		return order
	else
		return 0
	end
end
--��������ÿִ��һ������֮�����,������ֵԽ�����ȼ�Խ��.
function SetCmdOrder(cmdName,order)
	order = order + 1
	CmdOrder[cmdName] = order
	return order
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
	fp:close()
  return csv
end
-- ͵��,ֱ�ӵ���ALTRUN������,^_^
function altrun_config(file)
    local FileName = file:match("[^\\]+$")
    local DestDir = file:sub(1,-FileName:len()-1)
    local altrun = read_csv(file,"|")
    for i=1,#altrun do
	if altrun[i][5]:sub(1,1) == "." then
	    altrun[i][5] = DestDir .. altrun[i][5]
	end
	if altrun[i][5]:sub(1,2) == "@." then
	    altrun[i][5] = "@" .. DestDir .. altrun[i][5]:sub(2)
	end
	addCommand{ name = altrun[i][3],desc=altrun[i][4],cmd = altrun[i][5]}
    end
end

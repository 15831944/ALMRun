ALMRUN_CONFIG_PATH = ALMRUN_HOME .. "config\\"
ALMRUN_ORDER_FILE = ALMRUN_CONFIG_PATH .. 'order.txt'

--LUA���󲶻���
function error_hook(msg)
    message(msg:gsub("^(.+):(%d+):(.+)$","�ļ���%1\n������ [%2] �Ժ�����ݽ�������\n%3"),"Configure failed")
end

--ʹ��luajit��xpcall�����ÿ��Է���������������³������
function DoFile(file)
    xpcall(dofile,error_hook,file)
end

DoFile(ALMRUN_HOME .. 'LuaEx/base.lua')--������

--��ȡ�û���չ��
local Files = ListDir(ALMRUN_CONFIG_PATH,"*.lua",-1)
if Files == nil then 
    return
end
for key,value in pairs(Files) do  
    DoFile(value)
end

CmdOrder = {}--���������
--[[
	order.txt ���ݸ�ʽ  �����|������
	����:
	1|radmin
	100|airview
	ע:��Ҳ����ʹ���Լ�ϲ���ĸ�ʽ,��Ҫ�޸Ķ�ȡ�ͱ���Ĵ���
		��ȡ�Ĵ���������,����Ĵ�����base.lua��onClose��������.
]]

--��ȡ������������ݺ����������ʱҪ�õ�
local f = io.open(ALMRUN_ORDER_FILE,'r')
if f then
	for line in f:lines() do
		local row = {}
		for value in line:gmatch("[^|]+") do
			row[#row+1] = trim(value)
		end
		if not (row[1] == nil or row[1] == "") then
			CmdOrder[row[2]]=row[1]
		end
	end
	f:close()
end


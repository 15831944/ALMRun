RegisterPlugin('command',{name='LUA',func = function(commandName,...)
    local t_command = {name="ִ��LUA����:"..commandName,desc="ALMRun ���ʾ��",order=1000}
    if commandName:len() > 1 then
	t_command.cmd = function(arg)
	    MessageBox(loadstring("return "..commandName)(arg))
	end
    end
    return t_command
end})

--orderֵΪ����,ʹ�������������ȼ��Ƚϵ�,Ҳ���Ǳ�������������б�β��
RegisterPlugin('command',{func=function(commandName,...)
    return {name="Run "..commandName,desc="ALMRun ���в��",cmd = commandName,order=-1}
end})

--���������ʾ�������ض�����ͨ��orderֵ�������������ֵ�ߵ���ǰ��
RegisterPlugin('command',{name="CALC",func=function(commandName,...)
    local t_command = {}
    table.insert(t_command,{name=string.format("����: %s",commandName),desc="ALMRun ���������",order=1099});
    local _,result = pcall(loadstring("return "..commandName))
    result = tonumber(result)
    if result ~= nil then
	table.insert(t_command,{name=string.format("    = %d(0x%X)",result,result),desc="ALMRun ���������",order=1090})
    end
    return t_command
end})

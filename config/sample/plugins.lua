RegisterPlugin('command', function(commandName)
    return {name="ִ��LUA����:"..commandName,desc="ALMRun ���ʾ��",cmd = function(arg)
	    MessageBox(loadstring("return "..commandName)(arg))
	end}
end)

RegisterPlugin('command', function(commandName)
    return {name="run "..commandName,desc="ALMRun ���в��",cmd=function(arg)
	shellExecute(commandName,arg)
    end
    }
end)
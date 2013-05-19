--ALMRun������ʾ�ȼ�(����ALMRun.ini������)
--addCommand{ key = 'A-R', func = toggleMerry }

--���/�ָ���ǰ����
addCommand{ key = 'A-M', func = function()
    local window = GetForegroundWindow()
    if IsWindowMax(window) then
	ShowWindow(window, 'restore')
    else
	ShowWindow(window, 'max')
    end
end }
--�رյ�ǰ����
--addCommand{ key = 'A-Q', func = function()
--	closeWindow(getForegroundWindow())
--end }

local curHideWindow
addCommand{ key = 'A-H', func = function()
    if not curHideWindow then
        curHideWindow = GetForegroundWindow()
        ShowWindow(curHideWindow, 'hide')
    else
        ShowWindow(curHideWindow, 'normal')
        curHideWindow = nil
    end
end }

if WINDOWS then
	for i = string.byte('c'), string.byte('z') do
		local disk = string.char(i).. ':'
		local isOk, errMsg = io.open(disk)
		if isOk or errMsg:find('Permission denied') then
			addCommand{name=disk,key='',cmd=disk,desc="�򿪴���"..disk}
		else
			break
		end
	end

	addCommand{ name = 'cmd', key = 'A-T', func = function(arg)
		local window = getForegroundWindow()
		local dir = getWindowText(window)
		shellExecute('cmd', '', dir)
	end }

	addCommand{ key = 'A-Up', func = function() shellExecute('nircmdc', 'changesysvolume 65535', '', 'hide') end }
	addCommand{ key = 'A-Down', func = function() shellExecute('nircmdc', 'changesysvolume -65535', '', 'hide') end }
	addCommand{ key = 'A-Left', func = function() shellExecute('nircmdc', 'changesysvolume -4000', '', 'hide') end }
	addCommand{ key = 'A-Right', func = function() shellExecute('nircmdc', 'changesysvolume 4000', '', 'hide') end }
elseif MAC then
	addCommand{ name = 'terminal', key = 'A-T', func = function() shellExecute('/Applications/Utilities/terminal.app') end }
end

addCommand{ name = 'google',desc = "��Google����", func = function(str) shellExecute('http://www.google.com.hk/search?q=' .. str) end }
addCommand{ name = 'googlecode',func = function(str) shellExecute('http://code.google.com/query/#q=' .. str) end }
addCommand{ name = 'baidu', func = function(str) shellExecute('http://www.baidu.com/s?wd=' .. str) end }
addCommand{ name = 'youku', func = function(str) shellExecute('http://www.soku.com/search_video/q_' .. str) end }
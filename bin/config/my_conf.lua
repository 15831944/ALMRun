-- �߼�ģʽ,ʹ��LUA�����Զ�������
addCommand{ name = "test_func", desc = "���������",func = function(arg,id,flags)
	message('����',"����test����,����:"..arg..',����ID:'..id,"aa")
end}

-- ��ͨģʽ,ֱ�ӵ���cmd����ָ���ĳ���
addCommand{ name = "test_cmd", desc = "�����������",cmd = "cmd.exe /c pause >nul|echo ����:"}
--[[
 ɨ��ָ��Ŀ¼�Զ��������
 scan_dir(PATH,EXT,SUB)
 PATH  Ҫɨ���Ŀ¼
 EXT   ��������*.exe(����.exe���ļ�),k*.exe(������k��ͷ���ļ�)
 SUB   ɨ�輶��,Ĭ��(-1)
       -1 ����.
       0  ������Ŀ¼
       N  N����Ŀ¼
]]
scan_dir("E:\\��ɫ���","*.exe")

-- ����altrun������
altrun_config("ShortCutList.txt")

-- ����ƥ��ģʽ
-- 1 ƥ�俪ͷ
-- 2 ʹ��lua����HookCompre�Զ���ƥ��
-- 0 Ĭ��,����λ��ƥ��
config{CompareMode=0}
-- ����ƥ�亯��,�����Լ���չ,��CompareModeΪ2ʱ����
--function HookCompare(commandName,Prefix)
--	return commandName:lower():find(Prefix:lower())
--end

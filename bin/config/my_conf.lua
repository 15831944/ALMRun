-- �߼�ģʽ,ʹ��LUA�����Զ�������
addCommand{ name = "test_func", desc = "���������",func = function(arg,id,flags)
	message('����',"����test����,����:"..arg..',����ID:'..id,"aa")
end}

-- ��ͨģʽ,ֱ�ӵ���cmd����ָ���ĳ���
addCommand{ name = "test_cmd", desc = "�����������",cmd = "cmd.exe /c pause >nul|echo ����:"}

-- ɨ��ָ��Ŀ¼�Զ����
scan_dir("E:\\��ɫ���","*.exe")

-- ����altrun������
altrun_config("ShortCutList.txt")


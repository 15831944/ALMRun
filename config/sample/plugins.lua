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
RegisterPlugin('command',{name="CAL",func=function(commandName,...)
    local t_command = {}
    table.insert(t_command,{name=string.format("����: %s",commandName),desc="���������",order=1099});
    local _,result = pcall(loadstring("return "..commandName))
    local nresult = tonumber(result)
    --result = tonumber(result)
    if result ~= nil then
        if math.ceil(result) == result then
            table.insert(t_command,{name=string.format("    = %d(0x%X)",result,result),desc="������",order=1090})
        else
            local strresult = string.format("%f",result)
            local rightstr = string.sub(strresult,string.len(strresult),string.len(strresult)+1)
            while rightstr =="0" do
                strresult = string.sub(strresult,0,string.len(strresult)-1)
                rightstr = string.sub(strresult,string.len(strresult),string.len(strresult)+1)
            end
            table.insert(t_command,{name=string.format("    = %s",strresult),desc="������",order=1090})
        end
    end
    return t_command
end})
--everything�����ѯ�ļ�����
RegisterPlugin('command',{name="EV",func=function(commandName,...)
    local t_command = {}
    if commandName:len() > 0 then
        local files = Everything(commandName);
        for k,v in pairs(files) do
            local pre = "*"
            if v["TYPE"]=="FILE" then
                pre = ""
            end
            table.insert(t_command,{name=pre..v["NAME"],desc=v["PATH"],order=1090,cmd = 'Explorer /select, "'..v["FULL_PATH"]..'"'})
        end
    else
        table.insert(t_command,{name="��Everything�����ļ�",desc="��Everything�����ļ�",order=1099});
    end
    return t_command
end})
--��ѯDocset��������ܲο���Wox���
RegisterPlugin('command',{name="DOC",func=function(commandName,...)
    local t_command = {}
    if commandName:len() > 0 then
        local sql = require "ljsqlite3"
        local path = ALMRUN_HOME.."Docset"
        if DirExists(path) then
            local listdir = ListDir(path,"docSet.dsidx",3)
            --table.insert(t_command,{name="��ѯ�ĵ�",desc="��ѯ�ĵ�",order=1099});
            for k,v in pairs(listdir) do
                local htmldir = string.gsub(v,"\\","/")
                local docdir = string.gsub(htmldir,"docSet.dsidx","")
                local lan = string.sub(v,string.len(path)+2,string.find(v,"\\Contents\\Resources")-1)
                lan = string.gsub(lan,".docset","")
                local dbfile = v
                local conn = sql.open(dbfile)
                local checktb = conn:exec("select * from sqlite_master where name = 'searchIndex'")
                local sql = "select * from searchIndex where name like '%"..commandName.."%' order by lower(name) asc, path asc limit 30"
                if checktb ==nil then
                    sql = "select ztokenname as name, zpath as path from ztoken join ztokenmetainformation on ztoken.zmetainformation = ztokenmetainformation.z_pk join zfilepath on ztokenmetainformation.zfile = zfilepath.z_pk where (ztokenname like '%"..commandName.."%') order by lower(ztokenname) asc, zpath asc limit 30"
                end
                local arr = conn:exec(sql)
                if arr~=nil then
                    for i=1,#arr["name"] do
                        table.insert(t_command,{name=arr["name"][i],desc=lan,order=1090,cmd = function(arg) shellExecute('"'..docdir..'Documents/'..arr["path"][i]..'"', '', '')
                        end
                        })
                    end
                end
            end
        end
    else
        table.insert(t_command,{name="DOC",desc="��ѯ�ĵ�",order=1099});
    end
    return t_command
end})
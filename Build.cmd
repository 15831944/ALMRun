@echo off
title ALMRun �Զ�����ű� by chenall http://almrun.chenall.net
setlocal
REM 
REM ������wxWidgets��û�а�װ������λ����ɾ����һ�е�REMע�ͣ����޸�Ϊ��ȷ��λ�ã������޷�����
REM set WXWIN=D:\dev\wxWidgets-3.0.0
REM
REM ����ʹ�þ�̬�����,��������������ļ���������DLL�ļ�.(Ŀǰֻ��LUA��)
REM ��ͬ����ִ��Build.cmdʱ���ϲ���static
REM set STATIC=1
REM
REM PARAMS ��Ҫ���ӵĲ���
set PARAMS=

if /i "%1"=="STATIC" set STATIC=1

pushd %~dp0build
if not DEFINED VS100COMNTOOLS goto :END
if defined STATIC set PARAMS=%PARAMS% -DSTATIC=1
cmake .. -DRTL=MT %PARAMS% || goto :END
if not exist ALMRun.sln goto :END
"%VS100COMNTOOLS%\..\IDE\devenv.com" ALMRun.sln /build Release
if errorlevel 1 goto :END
copy /y Release\ALMRun.exe ..\bin\ > nul
popd
if not exist bin\doc md bin\doc
copy /y doc\*.* bin\doc\ >nul
copy /y README.md bin\doc\ > nul
copy /y README.md bin\ > nul
echo.
echo.
echo ����ɹ���ɣ����ɵ��ļ���BINĿ¼�¡�
echo.
goto :END
:NEED_VS100
echo.
echo ϵͳδ��װVS2010�������������롣
echo.
:END
pause

REM	@Date	:	2026/4/1 12:43:19
REM @Author	: 	WULEI,gcwulei@gdut.edu.cn 
REM @Version:	V1.0
REM @Function:	在三个文件夹中同步创建同名文件夹
REM make\%prj_name%

echo 警告：确认要执行删除项目！
pause
set /p name=输入项目文件夹名称: 
echo 你要删除的项目文件夹名为：%name%
echo 以下三个文件夹及其内部文件将被删除

echo  make\%name%
echo  src\%name%
echo  inc\%name%

set /p act=请输入YES确认执行（将无法恢复，如未备份请慎重操作）: 

if NOT "%act%"=="YES" ( GOTO EXIT )
	
del  make\%name%\*.*  /S/Q
rd  make\%name%  /S/Q

del  src\%name%\*.*  /S/Q
rd  src\%name%  /S/Q

del  include\%name%\*.*  /S/Q
rd  include\%name%  /S/Q

echo 执行删除完毕，退出	
exit /b 0

:EXIT

echo 未执行删除，退出
exit /b 0

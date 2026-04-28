REM	@Date	:	2026/4/1 12:43:19
REM @Author	: 	WULEI,gcwulei@gdut.edu.cn 
REM @Version:	V1.0
REM @Function:	在三个文件夹中同步创建同名文件夹
REM make\%prj_name%

REM Check1 
if "%1"=="" (
	echo ERROR	:	Project Name is REQUIRED!
	echo USAGE  :	makePrj.bat  $prjname
	echo EXAMPLE:	makePrj MyNewProj
	EXIT /B
)

if EXIST "make\%1" (
	echo ERROR	:	Project %1 EXISTED!
	cd make
	echo ---------project list-------------
	DIR *.* /B
	echo ---------project list-------------
	cd ..
	EXIT /B
)

echo 新建项目名为：%1
set /p namex=请输入全大写项目别名(eg.hw~HW): 

set proj_folder_name=%1
set proj_alias_UpCase=%namex%

MKDIR make\%proj_folder_name%
MKDIR src\%proj_folder_name%
MKDIR include\%proj_folder_name%

ECHO make\%proj_folder_name%  created
ECHO src\%proj_folder_name%   created
ECHO include\%proj_folder_name%   created

REM  以下基于模版创建三个基础文件
REM 项目名大写替换dllDeclaration.h中的 %TARGET%
set targetFile="include\%1\dllDeclaration.h"
REM echo. > %targetFile%
setlocal enabledelayedexpansion
	set sourceFile="file_templates\dllDeclaration_template.h"
	set "search=#TARGET#"
	set "replace=!proj_alias_UpCase!"
	
	for /f "tokens=*" %%a in ('type "%sourceFile%"') do (
	    set "line=%%a"
	    set "line=!line:%search%=%replace%!"
	    echo !line! >>%targetFile%
	) 
endlocal

REM 项目名Alias替换pro_template.pro中的 %TARGET%
set targetFile2="make\%1\%1.pro"
REM echo. > %targetFile2%
setlocal enabledelayedexpansion
	set sourceFile2="file_templates\pro_template.pro"
	set "search1=#TARGETX#"
	set "replace1=!proj_alias_UpCase!"
	set "search2=#TARGET#"
	set "replace2=!proj_folder_name!"
	for /f "tokens=*" %%a in ('type "%sourceFile2%"') do (
	    set "line=%%a"
	    set "line=!line:%search1%=%replace1%!"
	    set "line=!line:%search2%=%replace2%!"
	    echo !line! >>%targetFile2%
	) 
endlocal

REM 资源编译及复制批处理文件
set targetFile3="make\%1\res_compile.bat"
REM echo. > %targetFile3%
setlocal enabledelayedexpansion
	set sourceFile3="file_templates\res_compile_template.bat"
	set "search2=#TARGET#"
	set "replace2=!proj_folder_name!"
	for /f "tokens=*" %%a in ('type "%sourceFile3%"') do (
	    set "line=%%a"
	    set "line=!line:%search2%=%replace2%!"
	    echo !line! >>%targetFile3%
	) 
endlocal
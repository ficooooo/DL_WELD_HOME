REM	@Date	:	2026/4/7 10:53:56
REM @Author	: 	WULEI,gcwulei@gdut.edu.cn 
REM @Version:	V1.0
REM @Function:	按package名称将三个文件夹下的%package%文件夹

REM Check1 
if "%1"=="" (
	echo ERROR	:	Project Name is REQUIRED!
	echo USAGE  :	distPrj.bat  $prjname
	echo EXAMPLE:	distPrj MyNewProj
	EXIT /B
)

if NOT EXIST "make\%1" (
	echo ERROR	:	Project %1 NOT FOUND!
	cd make
	echo ---------project list-------------
	DIR *.* /B
	echo ---------project list-------------
	cd ..
	EXIT /B
)

REM 修改指向7-Zip 7z.exe所在文件夹
SET PATH=%PATH%;"C:\Program Files\7-Zip"

REM 各成员需修改此变量名称为自己姓名全拼
set proj_packager_name=lihaikuo
REM pfn = proj_folder_name
set pfn=%1

REM mkdir history_release_strage
REM mkdir my_distributed
REM mkdir my_recieved

REM mkdir temp

cd temp 
mkdir %pfn%

mkdir %pfn%\make\%pfn%
mkdir %pfn%\src\%pfn%
mkdir %pfn%\include\%pfn%

copy ..\make\%pfn%\%pfn%.pro %pfn%\make\%pfn%\%pfn%.pro /Y
copy ..\make\%pfn%\*.bat %pfn%\make\%pfn% /Y
xcopy ..\src\%pfn%\*.* %pfn%\src\%pfn% /E/Y
xcopy ..\include\%pfn%\*.* %pfn%\include\%pfn% /E/Y

set curtime=%time%
set curdate=%date%
set cur_hou=%curtime:~0,2%
set cur_min=%curtime:~3,2%
set cur_yea=%curdate:~0,4%
set cur_mon=%curdate:~5,2%
set cur_day=%curdate:~8,2%
set archievename=%pfn%_%cur_yea%%cur_mon%%cur_day%%cur_hou%%cur_min%_%proj_packager_name%.zip

7z a -y ..\history_release_strage\my_distributed\%archievename% .\%pfn%
rd %pfn% /S/Q
cd ..

ECHO %pfn% distribution file: ..\history_release_strage\my_distributed\%archievename% ready, send to others
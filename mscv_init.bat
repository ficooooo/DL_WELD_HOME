REM	@Date	:	2026/4/1 12:36:46
REM @Author	: 	WULEI,gcwulei@gdut.edu.cn 
REM @Version:	V1.0
REM @Function:	MSVC+QT 64 Compile Enviroments initializing

REM HELP:
REM USAGE:	mscv_init.bat  
REM INFO1:	This File shoule be placed in "[Qt installation Directory]\QtN.X.Y\N.X.Y\msvc2015_64>"
REM INFO2:	line 3: cd to "[MSVC安装目录]\Application\VS\VC\bin\amd64" where [vcvars64.bat] can be found.

echo mscv_init.bat
echo ===============MSVC+QT 64 Compile Enviroments initializing
echo ===============GDUT.PRC, WULEI, gcwulei@gdut.edu.cn===============

REM /B keep dos window open

cd "D:\Application\VS\VC\bin\amd64"

echo Call Application\VS\VC\bin\amd64\vcvars64.bat

call vcvars64.bat

REM : 修改以下一行进入到目标系统开发文件夹，并配置环境变量
set "DL_BASE_PATH=D:\DL_WELD_HOME"
cd /d "%DL_BASE_PATH%"

echo CALL %DL_BASE_PATH%\setPath4Compile.bat

call %DL_BASE_PATH%\setPath4Compile.bat

echo ===============MSVC+QT Compile Enviroments initializing Complete

REM 如果两个目录在同一文件夹下，无需盘符切换
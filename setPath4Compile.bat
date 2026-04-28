REM	@Date	:	2026/4/1 12:43:19
REM @Author	: 	WULEI,gcwulei@gdut.edu.cn 
REM @Version:	V1.0
REM @Function:	本文件为设置编译环境，QT+MSVC(64)位已安装，后者可选，可用mingw32替换

REM ==================================================================
REM %~dp0  当前文件夹 end with \
REM ==================================================================

ECHO START

ECHO DL_BASE_PATH	:	%DL_BASE_PATH%

SET DL_THIRDPARTY_IMP_BASE=%DL_BASE_PATH%\_3rdPartyImp
ECHO DL_THIRDPARTY_IMP_BASE	:	%DL_THIRDPARTY_IMP_BASE%

set DL_MAKE_PATH=%DL_BASE_PATH%\make
ECHO DL_MAKE_PATH	:	%DL_MAKE_PATH%

set DL_INCLUDE_PATH=%DL_BASE_PATH%\include
ECHO DL_INCLUDE_PATH :	%DL_INCLUDE_PATH%

REM 此处可以修改生成文件的目标文件夹位置
SET  DL_TARGET_BASE=%DL_BASE_PATH%\output
ECHO DL_TARGET_BASE	:	%DL_TARGET_BASE%

SET DL_DESTDIR=%DL_TARGET_BASE%\bin_amd64
ECHO DL_DESTDIR	:	%DL_DESTDIR%

REM Keep this framework isolated in the current shell:
REM prefer its own output DLLs and bundled OCCT DLLs before any global PATH entry.
SET PATH=%DL_DESTDIR%;%DL_THIRDPARTY_IMP_BASE%\OCCT-7_9_3\win64\vc14\bin;%PATH%
ECHO PATH prepended with current DL output and OCCT bin

cd %DL_MAKE_PATH%

ECHO END
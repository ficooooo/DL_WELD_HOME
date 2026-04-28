REM ########################################################################### 
REM #Copyright (C) 2026-2030 吴磊 And GDUT.CIMS. All rights reserved. 
REM #Contact: gcwulei@gdut.edu.cn 
REM ########################################################################### 
REM 资源编辑并复制到输出文件夹
REM: 变量定义等号左右不要有空格
set targetDir=%DL_DESTDIR%\res

IF NOT EXIST "%targetDir%" (
    mkdir %targetDir%
    echo %targetDir% created.
)

set currentpackage=occtutorial

set resfileName=Tutorial-icon
lrelease ..\..\src\%currentpackage%\%resfileName%.ts -qm %targetDir%\%resfileName%.qm

set resfileName=Tutorial-string
lrelease ..\..\src\%currentpackage%\%resfileName%.ts -qm %targetDir%\%resfileName%.qm

xcopy ..\..\src\%currentpackage%\res\*.* %targetDir%\*.* /Y
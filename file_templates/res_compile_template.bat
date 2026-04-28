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
set currentpackage=#TARGET#
REM :仅需修改文件名
set resfileName=YOURFILENAME
lrelease ..\..\src\%currentpackage%\%resfileName%.ts -qm %targetDir%\%resfileName%.qm
REM :复制以上两行处理多个文件 

REM 如果没有资源文件或者资源文件夹不是res，修改一致
xcopy ..\..\src\%currentpackage%\res\*.* %targetDir%\*.* /Y
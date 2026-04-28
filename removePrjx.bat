REM	@Date	:	2026/4/1 12:43:19
REM @Author	: 	WULEI,gcwulei@gdut.edu.cn 
REM @Version:	V1.0
REM @Function:	在三个文件夹中同步创建同名文件夹


del  make\%1\*.*  /S/Q
rd  make\%1  /S/Q

del  src\%1\*.*  /S/Q
rd  src\%1  /S/Q

del  include\%1\*.*  /S/Q
rd  include\%1  /S/Q

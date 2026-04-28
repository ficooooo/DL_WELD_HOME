########################################################################### 
#Copyright (C) 2026-2030 吴磊 And GDUT.CIMS. All rights reserved. 
#Contact: gcwulei@gdut.edu.cn 
########################################################################### 
#除了exe使用app 其它都是Lib 
TEMPLATE 	 = app
###每个工程此两项务必不能重复冲突########################################## 
#READONLY# 
TARGET 		 = DL_RobotConfig	 
#添加此项本工程编译时将作为库文件导出 
#其它包含本库头文件的位置编译时将作为导入对象  used in dllDeclaration.h 
DEFINES		+= EXPORT_DL_ROBOTCONFIG 
###每个工程此两项务必不能重复冲突########################################## 
###所有核心DLL工程pro此部分相同############################################ 
#READONLY 
DESTDIR	 	 = $$(DL_DESTDIR) 
OBJECTS_DIR	 = ./obj 
MOC_DIR 	 = ./moc  
CONFIG		+= release warn_on  
INCLUDEPATH += $$(DL_INCLUDE_PATH)  
###所有核心DLL工程pro此部分相同############################################ 
########################################################################### 
#在此处修改QT package 
QT 			-= core 
QT 			-= gui
QT 			+= widgets
########################################################################### 
LIBS += -L$$(DL_DESTDIR) -ldlOcctMdiFrame -ldlModelContext
########################################################################### 
#在这里处理第三库的包含文件，库文件 
INCLUDEPATH += $$(DL_THIRDPARTY_IMP_BASE)/OCCT-7_9_3/inc
LIBS += -L$$(DL_THIRDPARTY_IMP_BASE)/OCCT-7_9_3/win64/vc14/lib \
                 -lTKernel -lTKService -lTKV3d  -lTKOpenGl \
                 -lTKMath   -lTKBRep     \
                 -lTKGeomBase -lTKGeomAlgo -lTKG3d -lTKG2d \
         -lTKHLR -lTKTopAlgo -lTKMesh -lTKPrim \
         -lTKBool -lTKBO -lTKFillet -lTKOffset
########################################################################### 
#########################以下添加头文件和源文件############################ 
# Input 此文件不动 
HEADERS	+= ../../include/DL_RobotConfig/dllDeclaration.h 
# 以下逐对替换添加 
#HEADERS	+= ../../include/DL_RobotConfig/DL_XX.h 
#SOURCES	+= ../../src/DL_RobotConfig/DL_XX.cpp 
SOURCES	+= ../../src/DL_RobotConfig/Main.cxx
#SOURCES	+= ../../src/occtutorial/MakeBottle.cxx

HEADERS	+= ../../include/DL_RobotConfig/ApplicationTut.h
SOURCES	+= ../../src/DL_RobotConfig/ApplicationTut.cxx

HEADERS	+= ../../include/DL_RobotConfig/DocumentTut.h
SOURCES	+= ../../src/DL_RobotConfig/DocumentTut.cxx
#########################以上添加头文件和源文件############################ 

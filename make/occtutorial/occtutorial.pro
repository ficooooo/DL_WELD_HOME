########################################################################### 
#Copyright (C) 2026-2030 吴磊 And GDUT.CIMS. All rights reserved. 
#Contact: gcwulei@gdut.edu.cn 
########################################################################### 
#除了exe使用app 其它都是Lib 
TEMPLATE 	 = app 
CONFIG		+= console  
###每个工程此两项务必不能重复冲突########################################## 
#READONLY# 
TARGET 		 = occtutorial	 
#添加此项本工程编译时将作为库文件导出 
#其它包含本库头文件的位置编译时将作为导入对象  used in dllDeclaration.h 
DEFINES		+= EXPORT_OCCTUTORIAL 
###每个工程此两项务必不能重复冲突########################################## 
###所有核心DLL工程pro此部分相同############################################ 
#READONLY 
DESTDIR	 	 = $$(DL_DESTDIR) 
OBJECTS_DIR	 = ./obj 
MOC_DIR 	 = ./moc  
CONFIG		+= release warn_on no_batch  
INCLUDEPATH += $$(DL_INCLUDE_PATH)  
###所有核心DLL工程pro此部分相同############################################ 
########################################################################### 
#在此处修改QT package 
QT 			+= core 
QT 			+= gui
QT 			+= widgets 
########################################################################### 
LIBS += -L$$(DL_DESTDIR) -ldlOcctMdiFrame -ldlModelContext
########################################################################### 
#在这里处理第三库的包含文件，库文件 
#OCCT-7_9_3
INCLUDEPATH += $$(DL_THIRDPARTY_IMP_BASE)/OCCT-7_9_3/inc
LIBS += -L$$(DL_THIRDPARTY_IMP_BASE)/OCCT-7_9_3/win64/vc14/lib \
		 -lTKernel -lTKService -lTKV3d  -lTKOpenGl \
		 -lTKMath   -lTKBRep     \
		 -lTKGeomBase -lTKGeomAlgo -lTKG3d -lTKG2d \
         -lTKHLR -lTKTopAlgo -lTKMesh -lTKPrim \
         -lTKBool -lTKBO -lTKFillet -lTKOffset
#       -lTKCDF     -lTKLCAF 		 
#       -lTKXSBase -lTKShHealing	-lTKDEVRML -lTKDESTL -lTKDEIGES -lTKDESTEP
########################################################################### 
#########################以下添加头文件和源文件############################ 

# 以下逐对替换添加 
SOURCES	+= ../../src/occtutorial/Main.cxx
#SOURCES	+= ../../src/occtutorial/MakeBottle.cxx

HEADERS	+= ../../include/occtutorial/ApplicationTut.h 
SOURCES	+= ../../src/occtutorial/ApplicationTut.cxx

HEADERS	+= ../../include/occtutorial/DocumentTut.h 
SOURCES	+= ../../src/occtutorial/DocumentTut.cxx 

HEADERS	+= ../../include/occtutorial/DLMUI_Bottle.h 
SOURCES	+= ../../src/occtutorial/DLMUI_Bottle.cxx 

#########################以上添加头文件和源文件############################ 


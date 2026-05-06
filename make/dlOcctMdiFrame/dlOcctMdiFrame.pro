########################################################################### 
#Copyright (C) 2026-2030 吴磊 And GDUT.CIMS. All rights reserved. 
#Contact: gcwulei@gdut.edu.cn 
########################################################################### 
#除了exe使用app 其它都是Lib 
TEMPLATE 	 = lib 
###每个工程此两项务必不能重复冲突########################################## 
#READONLY# 
TARGET 		 = dlOcctMdiFrame	 
#添加此项本工程编译时将作为库文件导出 
#其它包含本库头文件的位置编译时将作为导入对象  used in dllDeclaration.h 
DEFINES		+= EXPORT_DL_OCCTMDI_FRAME 
###每个工程此两项务必不能重复冲突########################################## 
###所有核心DLL工程pro此部分相同############################################ 
#READONLY

DESTDIR	 	 = $$(DL_DESTDIR)
OBJECTS_DIR	 = ./obj
MOC_DIR 	 = ./moc 
CONFIG      += release warn_on no_batch
INCLUDEPATH += $$(DL_INCLUDE_PATH)

###所有核心DLL工程pro此部分相同############################################ 
########################################################################### 
#在此处修改QT package 
QT 			+= core 
QT 			+= gui
QT 			+= widgets 
########################################################################### 
########################################################################### 
#在这里处理第三库的包含文件，库文件 
#OCCT-7_9_3
INCLUDEPATH += $$(DL_THIRDPARTY_IMP_BASE)/OCCT-7_9_3/inc
# 仅OCCT 核心库	
LIBS += -L$$(DL_THIRDPARTY_IMP_BASE)/OCCT-7_9_3/win64/vc14/lib \
		 -lTKernel -lTKService -lTKV3d  -lTKOpenGl
         
#         -lTKMath   -lTKBRep -lTKDEIGES -lTKDESTL -lTKDEVRML -lTKDESTEP 
#        -lTKGeomBase -lTKGeomAlgo -lTKG3d -lTKG2d 
#        -lTKXSBase -lTKShHealing -lTKHLR -lTKTopAlgo -lTKMesh -lTKPrim 
#       	-lTKCDF -lTKBool -lTKBO -lTKFillet -lTKOffset -lTKLCAF 
########################################################################### 
#########################以下添加头文件和源文件############################ 
# Input 此文件不动 
HEADERS	+= ../../include/dlOcctMdiFrame/dllDeclaration.h 
# 以下逐对替换添加 
HEADERS	+= ../../include/dlOcctMdiFrame/Transparency.h 
SOURCES	+= ../../src/dlOcctMdiFrame/Transparency.cxx
HEADERS	+= ../../include/dlOcctMdiFrame/Material.h
SOURCES	+= ../../src/dlOcctMdiFrame/Material.cxx

HEADERS	+= ../../include/dlOcctMdiFrame/OcctWindow.h
SOURCES	+= ../../src/dlOcctMdiFrame/OcctWindow.cxx

HEADERS	+= ../../include/dlOcctMdiFrame/View.h
SOURCES	+= ../../src/dlOcctMdiFrame/View.cxx

HEADERS	+= ../../include/dlOcctMdiFrame/DocumentCommon.h
SOURCES	+= ../../src/dlOcctMdiFrame/DocumentCommon.cxx

HEADERS	+= ../../include/dlOcctMdiFrame/ApplicationCommon.h
#和DocumentCommon.cxx 实现多文档/窗口的管理：创建，关闭，切换
SOURCES	+= ../../src/dlOcctMdiFrame/ApplicationCommon.cxx

HEADERS	+= ../../include/dlOcctMdiFrame/MDIWindow.h
SOURCES	+= ../../src/dlOcctMdiFrame/MDIWindow.cxx

#2026/4/29 10:03:56 地板网格类,展示如何用基本元素构建可显示对象
HEADERS	+= ../../include/dlOcctMdiFrame/DL_AIS_Scene_Floor.h
SOURCES	+= ../../src/dlOcctMdiFrame/DL_AIS_Scene_Floor.cxx

#2026/5/1 11:00:53  按工具栏（菜单）组织功能模块,popup菜单管理
HEADERS	+= ../../include/dlOcctMdiFrame/DLModuleUIEntry.h
SOURCES	+= ../../src/dlOcctMdiFrame/DLModuleUIEntry.cxx
#与窗口创建关闭切换，程序退出相关的基础菜单工具栏
HEADERS	+= ../../include/dlOcctMdiFrame/DLMUI_FileWnd.h
SOURCES	+= ../../src/dlOcctMdiFrame/DLMUI_FileWnd.cxx
#视角，鼠标控制
HEADERS	+= ../../include/dlOcctMdiFrame/DLMUI_ViewOps.h
SOURCES	+= ../../src/dlOcctMdiFrame/DLMUI_ViewOps.cxx
#显示(线框，颜色，材质.... +对象删除)控制工具栏和右键弹出菜单
HEADERS	+= ../../include/dlOcctMdiFrame/DLMUI_ModelSkin.h
SOURCES	+= ../../src/dlOcctMdiFrame/DLMUI_ModelSkin.cxx
#光线追踪按钮
HEADERS	+= ../../include/dlOcctMdiFrame/DLMUI_Raytrace.h
SOURCES	+= ../../src/dlOcctMdiFrame/DLMUI_Raytrace.cxx
#背景设置
HEADERS	+= ../../include/dlOcctMdiFrame/DLMUI_BackgroundSet.h
SOURCES	+= ../../src/dlOcctMdiFrame/DLMUI_BackgroundSet.cxx

#########################以上添加头文件和源文件############################ 




/************************************************************************************ 
*	串联多自由度机器人离线编程仿真系统	DL_RobotSimulator V1.0						* 
*																					* 
*	Copyright (C) 2026-2030 吴磊 And GDUT.CIMS. All rights reserved. 				* 
*	Contact: victor.woo@163.net, gcwulei@gdut.edu.cn								* 
*																					* 
*	广东工业大学 																	* 
*	广州 中国                                                                       * 
*	ME, GDUT, GUANGZHOU P.R.C														* 
*************************************************************************************/ 
#ifndef DCL_DL_MODELLER____H__EBC66E19_A784_4041_BE7A_3533AC3DA236 
#define DCL_DL_MODELLER____H__EBC66E19_A784_4041_BE7A_3533AC3DA236 

#include "dllDeclaration.h"

#include <TopoDS_Shape.hxx>

DCL_DL_MODEL_CONTEXT__FUNCTION TopoDS_Shape
		MakeBottle(const Standard_Real myWidth
				 , const Standard_Real myHeight
				 , const Standard_Real myThickness);

#endif //DCL_DL_MODELLER____H__EBC66E19_A784_4041_BE7A_3533AC3DA236 

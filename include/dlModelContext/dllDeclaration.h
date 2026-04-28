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
#ifndef DCL_DL_MODEL_CONTEXT____H__EBC66E19_A784_4041_BE7A_3533AC3DA236 
#define DCL_DL_MODEL_CONTEXT____H__EBC66E19_A784_4041_BE7A_3533AC3DA236 
/*  
下列 ifdef 块是创建使从 DLL 导出更简单的宏的标准方法。 
此 DLL 中的所有需导出类或API均定义 EXPORT_DL_MODEL_CONTEXT 符号编译的； 
在使用此 DLL 的任何其他项目上不应定义此符号。 
源文件中包含此文件的任何其他项目都会将DCL_DL_MODEL_CONTEXT__* 类/函数视为是从 DLL 导入的； 
而此 DLL 则将用此宏定义的符号视为是被导出的。 
*/ 
#ifdef EXPORT_DL_MODEL_CONTEXT 
#define DCL_DL_MODEL_CONTEXT__CLASS			__declspec(dllexport) 
#define DCL_DL_MODEL_CONTEXT__FUNCTION		__declspec(dllexport) 
#else 
#define DCL_DL_MODEL_CONTEXT__CLASS			__declspec(dllimport) 
#define DCL_DL_MODEL_CONTEXT__FUNCTION		__declspec(dllimport) 
#endif //EXPORT_DL_MODEL_CONTEXT 
#endif //DCL_DL_MODEL_CONTEXT____H__EBC66E19_A784_4041_BE7A_3533AC3DA236 

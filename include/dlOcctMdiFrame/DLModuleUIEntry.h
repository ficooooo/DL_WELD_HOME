#ifndef DLMODULEUIENTRY_H
#define DLMODULEUIENTRY_H

#include "dllDeclaration.h"
#include "ApplicationCommon.h"

#include <QMainWindow>

#include <QAction>
#include <QToolBar>
#include <QMenu>
#include <QList>

class MDIWindow;

class DCL_DL_OCCTMDI_FRAME__CLASS DLModuleUIEntry: public QObject
{
	Q_OBJECT
public:
	/*
		2026/5/3 15:48:53
		本库基于OCCTutorial架构，创建功能管理单元 (工具栏菜单入口 ModuleUIEntry)
		所有Application的工具栏菜单入口按需创建，本类统一管理，但是各个入口自己按照当前状态或者窗口动作自行决定：
			1. 程序主窗口打开，自己是否可见
			2. Document-View子窗口创建后，自己是否和当前窗口(功能分组ID)相关，是否需要可见（启用功能）
				*注意解决多个功能中复用
			3. 自己如果可见的话，有无成员需要按照当前View做Checked状态同步
			4. 自己如果可见的话，有无成员需要按照当前View做Enabled状态同步
			5. 全部View关闭时，隐藏处理及其他额外的处理 
			
		* 每个窗口（Document-View）有自己的功能分组ID，知道需要用到哪个管理单元
		* 窗口提供 checkModuleUIEntry(id)决定是否用到某个功能单元
		* 功能单元检查时传入当前窗口的功能分组ID
			*有些单元是窗口无关的，比如视图操控ViewOp
			*有些单元窗口相关，只有功能分组ID一致才显示 (实现不同上下文窗口胡功能单元切换)
	*/
	enum DOCUMENT_VIEW_ACTION	{	NO_VIEW
								  , NEW_DOCUMENT_VIEW
								  , COPY_DOCUMENT_VIEW
								  , CLOSE_VIEW
								  , SWITCH_VIEW
								  , VIEW_SELECTION_CHANGED 	};	
public:
	static QMap<int,DLModuleUIEntry*>  DLModuleUIEntryMap;
	static void 					clearGlobleList();
	static void 					transMap2List();
	static void 					switchAll(bool show);
	static void						checkWindowActionAll(enum DOCUMENT_VIEW_ACTION eva,MDIWindow* w);//集中入口，传入对应的action id
	static DLModuleUIEntry*			getEntry(int id)		{	return DLModuleUIEntryMap.value(id);	}	
private:
	static QList<DLModuleUIEntry*>  DLModuleUIEntryList;	
public:                      
  	DLModuleUIEntry(ApplicationCommonWindow* mainWnd,int id);
  	~DLModuleUIEntry();
  	
protected:
	ApplicationCommonWindow*		myAppMainWindow;
	QMdiArea*						myMdiArea; 
	//在主程序框架中对应的模块功能ID
	//在主框架中统一管理
	int								myModuleID;		
	/*以Menu和ToolBar为模块功能组织Entry*/
	//按需选用或者都用
	QToolBar*                       myToolBar;
	QMenu*							myMenu;
	//所有的Action按enum 编号数组管理
	QList<QAction*>                 myActions;
public:
	virtual void                   	create()=0;
	//检查逻辑基于窗口操作及选择操作，调用
	void							checkWindowAction(enum DOCUMENT_VIEW_ACTION,MDIWindow* w);
	void 							_switch(bool show);
	QAction*						getAction(int id)		{	return myActions.at(id);	}
	//以下各函数在各继承功能单元中按需定义	
	virtual void 					checkNoView(MDIWindow*){};
	virtual void 					checkNewView(MDIWindow*){};	
	virtual void 					checkViewActivated(MDIWindow*){};
	virtual void 					checkSelectionChanged(MDIWindow*){};
	virtual void 					popup()		{};
public slots:
	//
};
#endif

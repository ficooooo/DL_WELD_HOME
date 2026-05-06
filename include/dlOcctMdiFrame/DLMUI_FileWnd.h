#ifndef DLMUI_FILEWND_H
#define DLMUI_FILEWND_H

#include "DLModuleUIEntry.h"

class DCL_DL_OCCTMDI_FRAME__CLASS DLMUI_FileWnd: public DLModuleUIEntry
{
	Q_OBJECT
	
	//最后两个是菜单顶级Menu对应的QAction , FilePreferencesId 未使用，导致出错
	enum { FileNewId, FilePrefUseVBOId, FileCloseId, /*FilePreferencesId,*/ FileQuitId, ViewToolId, ViewStatusId, HelpAboutId, HelpAboutIdX,WindowPopIdX };	
public:                      
  	DLMUI_FileWnd(ApplicationCommonWindow* mainWnd,int id);
  	~DLMUI_FileWnd();
	
public:
	virtual void                   	create();
	
	virtual void 					checkNoView(MDIWindow*);
	virtual void 					checkNewView(MDIWindow*);	
private:
	void 							createWindowPopup();
public slots:
	//第一组：与文档，窗口创建关闭相关
  	DocumentCommon*                 onNewDoc();
  	void                            onCloseMDIWindow();
	void                            onCreatMDIWindow();
//	virtual void                    onCloseDocument( DocumentCommon* theDoc );	
  	void 							closeAllWindows();
//-------------文档窗口管理相关-----------------------------------	
public slots:
	//第二组：其他简单功能
  	void                            onUseVBO();
  	virtual void                    onAbout();
  	void                            onViewToolBar();
	void                            onViewStatusBar();
	//动态窗口管理菜单创建及窗口选择
	void                            windowsMenuAboutToShow();
  	//窗口菜单项下方窗口List选中时操作
  	void                            windowsMenuActivated( bool checked );		
};
#endif
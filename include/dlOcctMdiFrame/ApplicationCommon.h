#ifndef APPLICATIONCOMMON_H
#define APPLICATIONCOMMON_H

#include "dllDeclaration.h"

#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QMenu>
#include <QMdiArea>
#include <QList>

class DocumentCommon;
class MDIWindow;
class View;

class DCL_DL_OCCTMDI_FRAME__CLASS ApplicationCommonWindow: public QMainWindow
{
    Q_OBJECT
public:
	enum DLModuleUIEntryIds	{DLMUI_FileWndId
							,DLMUI_ViewOpsId
							,DLMUI_ModelSkinId
							,DLMUI_RaytraceId
							,DLMUI_BackgroundSetId
							,DLMUI_Count		//派生类继续扩展此enum,第一个数字等于此数
							};
public:
	static QMdiArea*              	getMdiArea();
	static ApplicationCommonWindow* getApplication();
	static QString                  getResourceDir();
public:                      
  	ApplicationCommonWindow();
  	~ApplicationCommonWindow();
//-------------文档窗口管理相关-----------------------------------	
protected:
	QList<DocumentCommon*>          myDocuments;
  	int                             myNbDocuments;		
public:
  	virtual DocumentCommon*         createNewDocument()=0;
  	void							appendDocument(DocumentCommon* aDoc){	myDocuments.append (aDoc);		}
  	int                            	getNbDocument()		const			{  	return myDocuments.size();		}
  	int								getNbWndView();
  	//当前活动窗口对应的文档和View
	virtual DocumentCommon*         getActiveDocument();
	virtual MDIWindow*         		getActiveWnd();
	virtual View*         			getActiveWndView();

public:	
  	virtual void                    resizeEvent( QResizeEvent* );
  	virtual void               		closeEvent(QCloseEvent* e);        
public slots:
	//关闭Document的最后一个Window时，关闭Document
	virtual void                    onCloseDocument( DocumentCommon* theDoc );	 	
	//窗口中对象拾取状态改变时的动作
  	virtual void                    onSelectionChanged();	
	//各种操作切换窗口时操作
	virtual void                    onWindowActivated ( QMdiSubWindow * w );
public:  	
  	//2026/4/24 9:45:20   所有virtual 函数都需要关注用法
	virtual	void					addContextItemInPopup(QMenu*)=0;
	virtual	void                    popup( const int x, const int y );
};

/*2026/4/21 10:06:44 载入资源（文本，图片）映射文件
Input:
	A： 	main函数中创建的QApplication实例
	LABEL： 变量后缀，多个文件载入不能重复，1,2,3...  A,B,C均可
	FN： 	qm资源文件名，不包含后缀
	RESD：	qm资源文件所在文件夹，相对于main函数所在exe文件的位置
usage：
	QApplication a( argc, argv );
	QString resDir = ApplicationTut::getResourceDir();
	
	GENERAL_TRANSLATOR_INSTALL(a,A,"Common-string",resDir)
	GENERAL_TRANSLATOR_INSTALL(a,B,"Common-icon",resDir)
	GENERAL_TRANSLATOR_INSTALL(a,C,"Tutorial-string",resDir)
	GENERAL_TRANSLATOR_INSTALL(a,D,"Tutorial-icon",resDir)			
*/
#define GENERAL_TRANSLATOR_INSTALL(APPX,LABEL,FN,RESD)\
	QTranslator strTrans##LABEL( 0 );\
	if( strTrans##LABEL.load( FN, RESD ) )\
		APPX.installTranslator( &(strTrans##LABEL) );
#endif
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
	static const int COMMON_TOOLBAR_COUNT=4;
	enum { BasicToolBar, AppearenceBar, ViewOperBar, RTrOptBar };  //基础工具栏，外观显示，视图操作，光线选项  ，派生类在此基础上扩展toolbar
public:
	//最后两个是菜单顶级Menu对应的QAction , FilePreferencesId 未使用，导致出错
	enum { FileNewId, FilePrefUseVBOId, FileCloseId, /*FilePreferencesId,*/ FileQuitId, ViewToolId, ViewStatusId, HelpAboutId, HelpAboutIdX,WindowPopIdX };
  	enum { ToolWireframeId, ToolShadingId, ToolColorId, ToolMaterialId, ToolTransparencyId, ToolDeleteId };
	enum RaytraceAction { ToolRaytracingId, ToolShadowsId, ToolReflectionsId, ToolAntialiasingId };
	enum ViewAction { ViewFitAllId, 
					  ViewFitAreaId, ViewZoomId, ViewPanId, /*ViewGlobalPanId,*/ ViewRotationId,  /*对应5个配套指针的动作过程*/
                      ViewFrontId, ViewBackId, ViewTopId, ViewBottomId, ViewLeftId, ViewRightId,
                      ViewAxoId, ViewResetId, ViewHlrOffId, ViewHlrOnId };
    enum BackgroundSet {BackgroundSet_ColorId,BackgroundSet_EnvironmentMapId };
    
public:                      
  	ApplicationCommonWindow();
  	~ApplicationCommonWindow();

	static QMdiArea*              	getMdiArea();
	static ApplicationCommonWindow* getApplication();
	static QString                  getResourceDir();
	
//-------------文档窗口管理相关-----------------------------------	
protected:
	QList<DocumentCommon*>          myDocuments;
  	int                             myNbDocuments;
	bool                            myIsNotDocumentFound;   
	
protected:
  	virtual DocumentCommon*         createNewDocument();
  	int&                            getNbDocument()				{  	return myNbDocuments;		}
  	//当前活动窗口对应的文档和View
	virtual DocumentCommon*         getActiveDocument();
	virtual MDIWindow*         		getActiveWnd();
	virtual View*         			getActiveWndView();
  	virtual void                    resizeEvent( QResizeEvent* );
public slots:
	//第一组：与文档，窗口创建关闭相关
  	DocumentCommon*                 onNewDoc();
  	void                            onCloseMDIWindow();
	void                            onCreatMDIWindow();
	virtual void                    onCloseDocument( DocumentCommon* theDoc );	
  	void 							closeAllWindows();
//-------------文档窗口管理相关-----------------------------------	
public slots:
	//第二组：其他简单功能
  	void                            onUseVBO();
  	virtual void                    onAbout();
  	void                            onViewToolBar();
	void                            onViewStatusBar();

public slots:  	
	//窗口中对象拾取状态改变时的动作
  	virtual void                    onSelectionChanged();	
	//各种操作切换窗口时操作
	void                            onWindowActivated ( QMdiSubWindow * w );
protected:  
	//Toolbar+Action数组管理   TBMUI   ToolBar+Menu UI
	//基础菜单 文件窗口管理
	QToolBar*                       myTBMUI_FileMisc;//	myStdToolBar;
	QList<QAction*>                 myTBMUI_FileMisc_Actions;  //myToolBar_Actions_File_Wnd
	//显示(线框，颜色，材质.... +对象删除)控制工具栏和右键弹出菜单
  	QToolBar*                       myTBMUI_ModelSkin;
	QList<QAction*>                 myTBMUI_ModelSkin_Actions;
  	QList<QAction*>                 myTBMUI_ModelSkin_Materials_Actions;
  	//光影跟踪工具栏
  	QToolBar*                       myTBMUI_Raytrace;
  	QList<QAction*>                	myTBMUI_Raytrace_Actions;
  	//光影跟踪工具栏
  	QToolBar*                       myTBMUI_ViewOps;
  	QList<QAction*>                	myTBMUI_ViewOps_Actions;
  	
  	//背景色，背景图片
  	QList<QAction*>                	myTBMUI_BackgroundSet_Actions;  	
  	QMenu* 							myTBMUI_BackgroundSet_PopupMenu; //未使用
public slots:  
	//动态窗口管理菜单创建及窗口选择
	void                            windowsMenuAboutToShow();
  	//窗口菜单项下方窗口List选中时操作
  	void                            windowsMenuActivated( bool checked );	
	//和两个Toolbar相关的执行函数
	//光线控制 从View转移到顶级窗口，但此部分是窗口状态关联
	void 							onTBMUI_Raytrace();
	//显示控制：颜色材质...
  	void                            onTBMUI_ModelSkin();	
  	void                            onTBMUI_SetMaterial( int theMaterial );
  	
  	void                          	onTBMUI_BackgroundSet_Color();
  	void                          	onTBMUI_BackgroundSet_EnvironmentMap();
public slots:
    void                          	onTBMUI_ViewOps_fitAll();
    void                          	onTBMUI_ViewOps_fitArea();
    void                          	onTBMUI_ViewOps_zoom();
    void                          	onTBMUI_ViewOps_pan();
    //void                          onTBMUI_ViewOps_globalPan();
    void                          	onTBMUI_ViewOps_front();
    void                          	onTBMUI_ViewOps_back();
    void                          	onTBMUI_ViewOps_top();
    void                          	onTBMUI_ViewOps_bottom();
    void                          	onTBMUI_ViewOps_left();
    void                          	onTBMUI_ViewOps_right();
    void                          	onTBMUI_ViewOps_axo();
    void                          	onTBMUI_ViewOps_rotation();
    void                          	onTBMUI_ViewOps_reset();
    void                          	onTBMUI_ViewOps_hlrOn();
    void                          	onTBMUI_ViewOps_hlrOff();
protected:
	//创建
	void                            createTBMUI_FileMisc();
	void                            createTBMUI_ModelSkin();  	
	void                            createTBMUI_WindowPopup();	
	void                            createTBMUI_Raytrace();		
	void                            createTBMUI_Raytrace_init();
	void                            createTBMUI_ViewOps();		
	void                            createTBMUI_ViewOps_init();
	void 							createTBMUI_BackgroundSet();
	void 							createTBMUI_BackgroundSet_init();
	//-----------对象选择状态（有选中，无选中）+ 窗口切换时的菜单 工具栏管理------------------------
	//第一个窗口和关闭全部窗口后的菜单工具栏管理（显示、隐藏 功能菜单工具栏）
	void                    		checkTBMUI(MDIWindow* w);	
	//继承类自己需要管理的菜单按钮，在子类实现
	virtual void					checkTBMUIX(bool show)=0;
	//针对各分组的检查 显示隐藏控制
	void 							checkTBMUI_FileMisc(MDIWindow* w);
	void 							checkTBMUI_Raytrace(MDIWindow* w);
	void							checkTBMUI_ModelSkin(MDIWindow* w);
	void							checkTBMUI_ViewOps(MDIWindow* w);
	void							checkTBMUI_BackgroundSet(MDIWindow* w);
	//当selection变化时调用
	void							checkTBMUI_ModelSkin_SelectionChanged(MDIWindow* w);
	//View中右键菜单如果选中Object，弹出菜单
  	void							TBMUI_Popup_ModelSkin();
  	void							TBMUI_Popup_BackgroundSet();
protected:  	
  	//2026/4/24 9:45:20   所有virtual 函数都需要关注用法
	virtual	void					addContextItemInPopup(QMenu*)=0;
	virtual	void                    Popup( const int x, const int y );
friend class View;
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



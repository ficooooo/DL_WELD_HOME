#include "dlOcctMdiFrame/ApplicationCommon.h"

#include "dlOcctMdiFrame/DocumentCommon.h"
#include "dlOcctMdiFrame/View.h"
#include "dlOcctMdiFrame/MDIWindow.h"

#include <QApplication>
#include <QFrame>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QTranslator>

#include <stdlib.h>

#include <QMenuBar>

static ApplicationCommonWindow* stApp = 0;
static QMdiArea* stWs = 0;

QMdiArea * ApplicationCommonWindow::getMdiArea()
{
  	return stWs;
}

ApplicationCommonWindow* ApplicationCommonWindow::getApplication()
{
  	return stApp;
}

ApplicationCommonWindow::ApplicationCommonWindow()
						: QMainWindow( 0 ),
						myNbDocuments( 0 ),
						myIsNotDocumentFound(true),
						myTBMUI_FileMisc( 0 ),
						myTBMUI_ModelSkin( 0 ),
						myTBMUI_Raytrace( 0 ),
						myTBMUI_ViewOps( 0 ),
						myTBMUI_BackgroundSet_PopupMenu( 0 )
{
	stApp = this;
	
	stWs = new QMdiArea( this );
	connect (stWs, SIGNAL (subWindowActivated (QMdiSubWindow *)),this, SLOT (onWindowActivated (QMdiSubWindow*)));
	setCentralWidget( stWs );
	
	//主窗口基础菜单工具栏
	createTBMUI_FileMisc();
	createTBMUI_WindowPopup();
	createTBMUI_ModelSkin();
	createTBMUI_Raytrace();
	createTBMUI_ViewOps();
	createTBMUI_BackgroundSet();
	
	statusBar()->showMessage( QObject::tr("INF_READY"), 5000 );
	resize( 1000, 700 );
}

ApplicationCommonWindow::~ApplicationCommonWindow()
{
}

void ApplicationCommonWindow::onSelectionChanged()
{
	checkTBMUI_ModelSkin_SelectionChanged(getActiveWnd());
	
	//2026/4/2 14:34:57 TODO 选择后处理
}

QString ApplicationCommonWindow::getResourceDir()
{
	return "./res";
}

void ApplicationCommonWindow::resizeEvent( QResizeEvent* e )
{
    QMainWindow::resizeEvent( e );
    statusBar()->setSizeGripEnabled( !isMaximized() );
}

//文档创建 窗口创建 关闭 实现函数
DocumentCommon* ApplicationCommonWindow::createNewDocument()
{
  	return new DocumentCommon( ++myNbDocuments, this );
}

//File菜单下的New 新Document 新窗口
DocumentCommon* ApplicationCommonWindow::onNewDoc()
{
	DocumentCommon* aDoc = createNewDocument();
	
	MDIWindow* w = aDoc->onCreatMDIWindow();
	checkTBMUI(w);
	
	//Document检查到最后一个窗口被关闭，发出消息App关闭自己
	connect (aDoc, SIGNAL (sendCloseDocument (DocumentCommon*) ),this, SLOT (onCloseDocument (DocumentCommon*)));
	myDocuments.append (aDoc);
	
	myTBMUI_FileMisc_Actions.at(FileCloseId)->setEnabled(myDocuments.count() > 0);	
	return aDoc;
}

//Windows菜单下的New Window 基于当前Document创建
void ApplicationCommonWindow::onCreatMDIWindow()
{
  	MDIWindow* window = qobject_cast< MDIWindow* >( stWs->activeSubWindow() );
  	window->getDocument()->onCreatMDIWindow();
}

/*
	The QMdiSubWindow class provides a subwindow class for QMdiArea.
	QMdiSubWindow represents a top-level window in a QMdiArea, and consists of a title bar with window decorations, 
	an internal widget, and (depending on the current style) a window frame and a size grip. 
	
	QMdiSubWindow has its own layout, which consists of the title bar and a center area for the internal widget.
*/
MDIWindow*  ApplicationCommonWindow::getActiveWnd()
{
	QMdiArea* ws = ApplicationCommonWindow::getMdiArea();
  	return qobject_cast<MDIWindow*>( ws->activeSubWindow());
}	

DocumentCommon* ApplicationCommonWindow::getActiveDocument()
{
	MDIWindow* acWnd = getActiveWnd();
	return acWnd==NULL?NULL:acWnd->getDocument();
}	

View* ApplicationCommonWindow::getActiveWndView()
{
	MDIWindow* acWnd = getActiveWnd();
  	return acWnd==NULL?NULL:acWnd->getWndView();
}	

void ApplicationCommonWindow::onCloseMDIWindow()
{
    getMdiArea()->activeSubWindow()->close();
}

void ApplicationCommonWindow::onCloseDocument(DocumentCommon* theDoc)
{
	theDoc->removeViews();
	myDocuments.removeAll( theDoc );
	delete theDoc;
	checkTBMUI(NULL);
	myTBMUI_FileMisc_Actions.at(FileCloseId)->setEnabled(myDocuments.count() > 0);
}

void ApplicationCommonWindow::closeAllWindows()
{
	qApp->closeAllWindows();
}

//TODO 2026/4/11 14:38:32
void ApplicationCommonWindow::onWindowActivated ( QMdiSubWindow * w )
{
	if (w == NULL)	//关闭窗口时也会触发本函数
		return;

	checkTBMUI_ModelSkin_SelectionChanged(getActiveWnd());
	checkTBMUI_Raytrace(getActiveWnd());
	checkTBMUI_ViewOps(getActiveWnd());
	
	MDIWindow* window = qobject_cast< MDIWindow* >(w);
	window->onWindowActivated();
}

//onCloseDocument  onNewDoc 两次调用本方法
//实现第一个窗口时插入Windows,关闭全部时隐藏
//同时控制一个工具栏的显示隐藏
//新建时先检查再append myDocuments.append (aDoc);
/*
QAction *QMenu::menuAction() const
	Returns the action associated with this menu.
QMenu *QAction::menu() const
	Returns the menu contained by this action. 
	Actions that contain menus can be used to create menu items with submenus, or inserted into toolbars to create buttons with popup menus.	
*/
void ApplicationCommonWindow::checkTBMUI(MDIWindow* w)
{
	//没有清空则无需处理菜单和工具栏，之前第一次已经时已经加载
	//每次新建是先调用本函数再append,此时isEmpty=true
	if (!myDocuments.isEmpty())		{   return;  }
	
	//是第一次操作（或者全部关闭后再创建），当前无文档存在，则需要加载显示  如果
	if ( myIsNotDocumentFound ) {  
		myIsNotDocumentFound = false;
		checkTBMUI_ModelSkin(w);
		checkTBMUI_Raytrace(w);
		checkTBMUI_FileMisc(w);
		checkTBMUI_ViewOps(w);
		checkTBMUIX(true);
	}else // 全部关闭，则需隐藏
	{
		myIsNotDocumentFound = true;
		checkTBMUI_ModelSkin(NULL);
		checkTBMUI_Raytrace(NULL);
		checkTBMUI_FileMisc(NULL);
		checkTBMUI_ViewOps(NULL);
		
		checkTBMUIX(false);
	}
}
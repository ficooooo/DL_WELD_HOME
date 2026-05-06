#include "dlOcctMdiFrame/ApplicationCommon.h"

#include "dlOcctMdiFrame/DocumentCommon.h"
#include "dlOcctMdiFrame/View.h"
#include "dlOcctMdiFrame/MDIWindow.h"
#include "dlOcctMdiFrame/DLModuleUIEntry.h"

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
						myNbDocuments( 0 )
{
	stApp = this;
	
	stWs = new QMdiArea( this );
	connect (stWs, SIGNAL (subWindowActivated (QMdiSubWindow *)),this, SLOT (onWindowActivated (QMdiSubWindow*)));
	setCentralWidget( stWs );
}

ApplicationCommonWindow::~ApplicationCommonWindow()
{
	printf("ApplicationCommonWindow::~ApplicationCommonWindow()\n");
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
//--------------------------------------------------------------------------------
void ApplicationCommonWindow::closeEvent(QCloseEvent* e)
{
	DLModuleUIEntry::clearGlobleList();
	QMainWindow::closeEvent( e );
	printf("ApplicationCommonWindow::closeEvent()\n");
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

int	ApplicationCommonWindow::getNbWndView()
{
	int _count = 0;
	for (int i = 0; i < myDocuments.size(); ++i)
		_count += myDocuments.at(i)->myMDIWindows.size();
	
	return _count;
}	

//文档创建 窗口创建 关闭 实现函数
void ApplicationCommonWindow::onCloseDocument(DocumentCommon* theDoc)
{
	//--myNbDocuments;  用于编号，只加不减
	myDocuments.removeOne( theDoc );
	delete theDoc;
}

//TODO 2026/4/11 14:38:32
void ApplicationCommonWindow::onWindowActivated ( QMdiSubWindow * w )
{
	if (w == NULL)	//关闭窗口时也会触发本函数
		return;

	MDIWindow* window = qobject_cast< MDIWindow* >(w);
	window->onWindowActivated();
	
	printf("--------------------ApplicationCommonWindow::onWindowActivated--------------------\n");	
	DLModuleUIEntry::checkWindowActionAll(DLModuleUIEntry::SWITCH_VIEW,window);
	printf("--------------------ApplicationCommonWindow::onWindowActivated--------------------\n");	
}

void ApplicationCommonWindow::onSelectionChanged()
{
	printf("--------------------ApplicationCommonWindow::onSelectionChanged--------------------\n");	
	DLModuleUIEntry::checkWindowActionAll(DLModuleUIEntry::VIEW_SELECTION_CHANGED,getActiveWnd());
	printf("--------------------ApplicationCommonWindow::onWindowActivated--------------------\n");		
}
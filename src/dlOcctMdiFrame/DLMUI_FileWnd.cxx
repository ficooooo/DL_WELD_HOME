#include "dlOcctMdiFrame/DLMUI_FileWnd.h"
#include "dlOcctMdiFrame/ApplicationCommon.h"
#include "dlOcctMdiFrame/DocumentCommon.h"
#include "dlOcctMdiFrame/MDIWindow.h"
#include "dlOcctMdiFrame/View.h"

#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QMdiSubWindow>
#include <OpenGl_GraphicDriver.hxx>

DLMUI_FileWnd::DLMUI_FileWnd(ApplicationCommonWindow* mainWnd,int id):DLModuleUIEntry(mainWnd,id)
{
	create();
}

DLMUI_FileWnd::~DLMUI_FileWnd()
{
}

void DLMUI_FileWnd::checkNewView(MDIWindow* /*w*/)
{
	myAppMainWindow->menuBar()->insertMenu( myActions.at( HelpAboutIdX )
							 , myActions.at( WindowPopIdX )->menu() );
	myActions.at(FileCloseId)->setEnabled(true);							 
}

void DLMUI_FileWnd::checkNoView(MDIWindow* /*w*/)
{
	myAppMainWindow->menuBar()->removeAction( myActions.at( WindowPopIdX ) );
	myActions.at(FileCloseId)->setEnabled(false);
}


#define ICOND_STD_ACTION(OBJ,ICON,NAME,TIP,SHORTCUT,MAPID,_SLOT)\
	OBJ = new QAction( ICON, QObject::tr(#NAME), this );\
	OBJ->setToolTip( QObject::tr(#TIP) );\
	OBJ->setStatusTip( QObject::tr(#TIP) );\
	OBJ->setShortcut( QObject::tr(SHORTCUT) );\
	connect( OBJ, SIGNAL( triggered() ) , this, SLOT( _SLOT() ) );\
	myActions.insert( MAPID, OBJ );

#define STD_ACTION(OBJ,NAME,TIP,MAPID,_SLOT)\
	OBJ = new QAction( QObject::tr(#NAME), this );\
	OBJ->setToolTip( QObject::tr(#TIP) );\
	OBJ->setStatusTip( QObject::tr(#TIP) );\
  	OBJ->setCheckable( true );\
  	OBJ->setChecked( true );\
  	connect( OBJ, SIGNAL( triggered() ) , this, SLOT( _SLOT() ) );\
  	myActions.insert( MAPID, OBJ );

//程序打开时的初始（最小）菜单工具栏
//enum { FileNewId, FilePrefUseVBOId, FileCloseId, FilePreferencesId, FileQuitId, ViewToolId, ViewStatusId, HelpAboutId, HelpAboutIdX,WindowPopIdX };
void DLMUI_FileWnd::create()
{
	QPixmap newIcon, helpIcon, closeIcon,quitIcon;
	
	QString dir = myAppMainWindow->getResourceDir() + QString( "/" );
	
	newIcon = QPixmap( dir + QObject::tr("ICON_NEW") );
	helpIcon = QPixmap( dir + QObject::tr("ICON_HELP") );
	closeIcon = QPixmap( dir + QObject::tr("ICON_CLOSE") );
	quitIcon = QPixmap( dir + QObject::tr("ICON_QUITAPP") );
	
	QAction * fileNewAction, * fileCloseAction, * filePrefUseVBOAction,
			* fileQuitAction, * viewToolAction, * viewStatusAction, * helpAboutAction;
	
	ICOND_STD_ACTION(fileNewAction,newIcon,MNU_NEW,TBR_NEW,"CTRL+N",FileNewId,onNewDoc)	
	ICOND_STD_ACTION(fileCloseAction,closeIcon,MNU_CLOSE,TBR_CLOSE,"CTRL+W",FileCloseId,onCloseMDIWindow)
	STD_ACTION(filePrefUseVBOAction,MNU_USE_VBO,TBR_USE_VBO,FilePrefUseVBOId,onUseVBO)
	ICOND_STD_ACTION(fileQuitAction,quitIcon,MNU_QUIT,TBR_QUIT,"CTRL+Q",FileQuitId,closeAllWindows)
	
	STD_ACTION(viewToolAction,MNU_TOOL_BAR,TBR_TOOL_BAR,ViewToolId,onViewToolBar)
	STD_ACTION(viewStatusAction,MNU_STATUS_BAR,TBR_STATUS_BAR,ViewStatusId,onViewStatusBar)
	
  	ICOND_STD_ACTION(helpAboutAction,helpIcon,MNU_ABOUT,TBR_ABOUT,"F1",HelpAboutId,onAbout)
	
	//构造菜单3项	
	QMenu* mainMenu_File = myAppMainWindow->menuBar()->addMenu( QObject::tr("MNU_FILE") );
	mainMenu_File->addAction( fileNewAction );
	mainMenu_File->addAction( fileCloseAction );
	mainMenu_File->addSeparator();
	
	// create preferences menu 二级弹出filePrefUseVBOAction
  	QMenu* aPrefMenu = new QMenu( QObject::tr("MNU_PREFERENCES") );
  	aPrefMenu->addAction( filePrefUseVBOAction );
	mainMenu_File->addMenu( aPrefMenu );
	mainMenu_File->addSeparator();
	mainMenu_File->addAction( fileQuitAction );
	
	QMenu * mainMenu_View = myAppMainWindow->menuBar()->addMenu( QObject::tr("MNU_VIEW") );
	mainMenu_View->addAction( viewToolAction );
	mainMenu_View->addAction( viewStatusAction );
	
	QMenu *mainMenu_Help = myAppMainWindow->menuBar()->addMenu( QObject::tr("MNU_HELP") );
	mainMenu_Help->addAction( helpAboutAction );
	myActions.insert( HelpAboutIdX, mainMenu_Help->menuAction() );   
		
	// populate a tool bar with some actions
	myToolBar = myAppMainWindow->addToolBar( tr( "File Operations" ) );
	myToolBar->addAction( fileNewAction );
	myToolBar->addAction( helpAboutAction );
	myActions.at(FileCloseId)->setEnabled(false);
	myActions.at(FilePrefUseVBOId)->setEnabled( true );
	
	createWindowPopup();
	//myToolBar->hide();
}

//新建文档后，基础菜单插入显示Window菜单，并且每次弹出时动态生成
//全部窗口关闭后，Window菜单隐藏
void DLMUI_FileWnd::createWindowPopup()
{
	QMenu* myWindowPopup = new QMenu( QObject::tr( "MNU_WINDOW" ), myAppMainWindow );
  	connect( myWindowPopup, SIGNAL( aboutToShow() ),
         this, SLOT( windowsMenuAboutToShow() ) );

    myActions.insert( WindowPopIdX, myWindowPopup->menuAction() );
}
//---------------------------------------------------------------------------------------------------
//File菜单下的New 新Document 新窗口
DocumentCommon* DLMUI_FileWnd::onNewDoc()
{
	DocumentCommon* aDoc = myAppMainWindow->createNewDocument();
	
	MDIWindow* w = aDoc->onCreatMDIWindow();
	aDoc->initScene();
	
	//Document检查到最后一个窗口被关闭，发出消息App关闭自己
	connect (aDoc, SIGNAL (sendCloseDocument (DocumentCommon*) ),myAppMainWindow, SLOT (onCloseDocument (DocumentCommon*)));
	myAppMainWindow->appendDocument(aDoc);
	
	if(myAppMainWindow->getNbWndView()==1)
	{	
		DLMUI_FileWnd::checkWindowActionAll(DLMUI_FileWnd::NEW_DOCUMENT_VIEW,w);
	}	
	
	w->getWndView()->fitAll();
	return aDoc;
}

//Windows菜单下的New Window 基于当前Document创建
void DLMUI_FileWnd::onCreatMDIWindow()
{
  	MDIWindow* window = qobject_cast< MDIWindow* >( myMdiArea->activeSubWindow() );
  	MDIWindow* w = window->getDocument()->onCreatMDIWindow();
  	
  	w->getWndView()->fitAll();
}

void DLMUI_FileWnd::onCloseMDIWindow()
{
    myMdiArea->activeSubWindow()->close();
}

void DLMUI_FileWnd::closeAllWindows()
{
	qApp->closeAllWindows();
}

//---------------------------------------------------------------------------------------------------
void DLMUI_FileWnd::onViewToolBar()
{
	bool show = myActions.at( ViewToolId )->isChecked();
	if ( show == myToolBar->isVisible() )
		return;
		
	DLModuleUIEntry::switchAll(show);
}

void DLMUI_FileWnd::onViewStatusBar()
{
	bool show = myActions.at( ViewStatusId )->isChecked();
	if ( show == myAppMainWindow->statusBar()->isVisible() )
		return;
	if ( show )
		myAppMainWindow->statusBar()->show();
	else
		myAppMainWindow->statusBar()->hide();
}

void DLMUI_FileWnd::onAbout()
{
  	QMessageBox::information( myAppMainWindow
  							, QObject::tr( "TIT_ABOUT" )
  							, QObject::tr( "INF_ABOUT" )
  							, QObject::tr("BTN_OK" )
                            , QString::null
                            , QString::null, 0, 0 );
}

void DLMUI_FileWnd::onUseVBO()
{
	MDIWindow* aWindow = qobject_cast<MDIWindow*> (myMdiArea->activeSubWindow()->widget());
	if (NULL == aWindow)
		return;
	
	Handle(AIS_InteractiveContext) aContextAIS = aWindow->getDocument()->getContext();
	if (aContextAIS.IsNull())
		return;
	
	Handle(OpenGl_GraphicDriver) aDriver =
			Handle(OpenGl_GraphicDriver)::DownCast (aContextAIS->CurrentViewer()->Driver());
	if (!aDriver.IsNull())
	{
		aDriver->ChangeOptions().vboDisable = Standard_True;
	}
}

//-----------------------------------------------------------------
void DLMUI_FileWnd::windowsMenuAboutToShow()
{
	QMenu* myWindowPopup = myActions.at( WindowPopIdX)->menu();
	myWindowPopup->clear();
	QAction* a;
	
	QString dir = myAppMainWindow->getResourceDir() + QString( "/" );
	
	a = new QAction( QPixmap( dir + QObject::tr( "ICON_WINDOW_NEW3D" ) ), QObject::tr( "MNU_WINDOW_NEW3D" ), this );
	a->setToolTip( QObject::tr( "TBR_WINDOW_NEW3D" ) );
	a->setStatusTip( QObject::tr( "TBR_WINDOW_NEW3D" ) );
	connect( a, SIGNAL( triggered() ), this, SLOT( onCreatMDIWindow() ) );
	myWindowPopup->addAction( a );
	
	a = new QAction( QPixmap( dir + QObject::tr( "ICON_WINDOW_CASCADE" ) ), QObject::tr( "MNU_WINDOW_CASCADE" ), this );
	a->setToolTip( QObject::tr( "TBR_WINDOW_CASCADE" ) );
	a->setStatusTip( QObject::tr( "TBR_WINDOW_CASCADE" ) );
	//connect( a, SIGNAL( triggered() ), getMdiArea(), SLOT( cascade() ) );
	connect( a, SIGNAL( triggered() ), myMdiArea, SLOT( cascadeSubWindows() ) );
	myWindowPopup->addAction( a );
	
	a = new QAction( QPixmap( dir + QObject::tr( "ICON_WINDOW_TILE" ) ), QObject::tr( "MNU_WINDOW_TILE" ), this );
	a->setToolTip( QObject::tr( "TBR_WINDOW_TILE" ) );
	a->setStatusTip( QObject::tr( "TBR_WINDOW_TILE" ) );
	//connect( a, SIGNAL( triggered() ), getMdiArea(), SLOT( tile() ) );
	connect(a, &QAction::triggered, myMdiArea, &QMdiArea::tileSubWindows);	
	myWindowPopup->addAction( a );
	
	myWindowPopup->addSeparator();
	QList<QMdiSubWindow *> windows = myMdiArea->subWindowList();
	for (int i = 0; i < windows.count(); ++i)
	{
		QAction* aAction = new QAction( windows.at(i)->windowTitle(), this );
		aAction->setCheckable( true );
		aAction->setData( i );
		myWindowPopup->addAction( aAction );
		aAction->setChecked( myMdiArea->activeSubWindow() == windows.at(i) );
		connect( aAction, SIGNAL( toggled( bool ) ), this, SLOT( windowsMenuActivated( bool ) ) );
	}
}

//多个窗口，切换到选中窗口
void DLMUI_FileWnd::windowsMenuActivated( bool checked )
{
	if(!checked)
		return;
		
	QAction* aSender = qobject_cast<QAction*>( sender() );
	if ( !aSender )
		return;
		
	QWidget * w = myMdiArea->subWindowList().at( aSender->data().toInt() );
	if ( w )
		w->setFocus();
}
#include "occtutorial/DLMUI_Bottle.h"
#include "dlOcctMdiFrame/View.h"
#include "dlOcctMdiFrame/MDIWindow.h"
#include "occtutorial\DocumentTut.h"
#include <QApplication>
#include <QStatusBar>

DLMUI_Bottle::DLMUI_Bottle(ApplicationCommonWindow* mainWnd,int id):DLModuleUIEntry(mainWnd,id)
{
	create();
}

DLMUI_Bottle::~DLMUI_Bottle()
{
	printf("DLMUI_Bottle::~DLMUI_Bottle()");
}

void DLMUI_Bottle::checkNoView(MDIWindow* /*w*/)
{
	printf("\tDLMUI_Bottle::~checkNoView()\n");
	myToolBar->hide();
}

void DLMUI_Bottle::checkNewView(MDIWindow* /*w*/)
{
	printf("\tDLMUI_Bottle::~checkNewView()\n");
	myToolBar->show();
}

void DLMUI_Bottle::checkViewActivated(MDIWindow* w)
{
	DocumentTut* doc = (DocumentTut*)w->getDocument();
	Handle(AIS_InteractiveObject) bot = doc->getAISIO(DocumentTut::DL_BOTTLE);
	
	myActions.at (MakeBottleId)->setEnabled (bot.IsNull());
	myActions.at (RotateBottleId)->setEnabled (!bot.IsNull());
}

void DLMUI_Bottle::create()
{
	QPixmap MakeBottleIcon;
	QString dir = myAppMainWindow->getResourceDir() + QString( "/" );
	
	myToolBar = myAppMainWindow->addToolBar( tr( "Make Bottle" ) );
	
	MakeBottleIcon = QPixmap( dir+QObject::tr( "ICON_MAKE_BOTTLE" ) );
	
	QAction * MakeBottleAction = new QAction( MakeBottleIcon, QObject::tr("TBR_MAKEBOT"), this );
	MakeBottleAction->setToolTip( QObject::tr( "TBR_MAKEBOT" ) );
	MakeBottleAction->setStatusTip( QObject::tr("TBR_MAKEBOT") );
	MakeBottleAction->setShortcut( QObject::tr( "CTRL+M" ) );
	connect( MakeBottleAction, SIGNAL( triggered() ) , this, SLOT( makeBottle() ) );
	
	myActions.insert(MakeBottleId, MakeBottleAction);
	myToolBar->addAction( MakeBottleAction );
		
	MakeBottleAction = new QAction( MakeBottleIcon, QObject::tr("TBR_ROTBOT"), this );
	MakeBottleAction->setToolTip( QObject::tr( "TBR_ROTBOT" ) );
	MakeBottleAction->setStatusTip( QObject::tr("TBR_ROTBOT") );
	connect( MakeBottleAction, SIGNAL( triggered() ) , this, SLOT( rotBottle() ) );
		//insertToolBar( getToolBar_ShapeOps(), myToolBar );

	myActions.insert(RotateBottleId, MakeBottleAction);
	myToolBar->addAction( MakeBottleAction );
	
	myToolBar->hide();
}

void DLMUI_Bottle::makeBottle()
{
  	DocumentTut* doc = (DocumentTut*)myAppMainWindow->getActiveDocument();
	myAppMainWindow->statusBar()->showMessage( QObject::tr("INF_MAKE_BOTTLE"), 5000 );
	doc->makeBottle();

	myActions.at (MakeBottleId)->setEnabled (false);
	myActions.at (RotateBottleId)->setEnabled (true);
	myAppMainWindow->statusBar()->showMessage(QObject::tr("INF_DONE"));
}


void DLMUI_Bottle::rotBottle()
{
/*	if(BOTTLE.IsNull())
		return;
		
	QApplication::setOverrideCursor( Qt::WaitCursor );
  	gp_Ax1 axis(gp_Pnt(0,0,0), gp_Dir(0,0,1)); // Z轴旋转
	gp_Trsf trsf;
 
	for(int angle =0;angle<=360;angle+=1)
	{
    	trsf.SetRotation(axis, angle * 3.14159/180);  //角度转弧度
    	TopLoc_Location loc(trsf);
    	getActiveDocument()->getContext()->SetLocation(BOTTLE, loc);
    	getActiveDocument()->getContext()->UpdateCurrentViewer();
    	//time.sleep(0.05);
    	QThread::msleep(20);
    }
    QApplication::restoreOverrideCursor();*/
    
    DocumentTut* doc = (DocumentTut*)myAppMainWindow->getActiveDocument();
    doc->clearScene();
    myActions.at (MakeBottleId)->setEnabled (true);
	myActions.at (RotateBottleId)->setEnabled (false);
}
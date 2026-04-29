#include "occtutorial\ApplicationTut.h"
#include "occtutorial\DocumentTut.h"

#include <OSD_Environment.hxx>

#include <QFileDialog>
#include <QStatusBar>
#include <QMdiSubWindow>

ApplicationTut::ApplicationTut()
    : ApplicationCommonWindow( )
{
  createTBMUI_FileMisc();
  createTBMUI_WindowPopup();
  createTBMUI_ModelSkin();
  createTBMUI_Raytrace();
  createTBMUI_ViewOps();
  createTBMUI_BackgroundSet();

  createTBMUI_MakeBottle();

  statusBar()->showMessage( QObject::tr("INF_READY"), 5000 );
  resize( 1000, 700 );
}

ApplicationTut::~ApplicationTut()
{
}

void ApplicationTut::createTBMUI_MakeBottle(){
	QPixmap MakeBottleIcon;
	QString dir = getResourceDir() + QString( "/" );
	MakeBottleIcon = QPixmap( dir+QObject::tr( "ICON_MAKE_BOTTLE" ) );
	
	QAction * MakeBottleAction = new QAction( MakeBottleIcon, QObject::tr("TBR_MAKEBOT"), this );
	MakeBottleAction->setToolTip( QObject::tr( "TBR_MAKEBOT" ) );
	MakeBottleAction->setStatusTip( QObject::tr("TBR_MAKEBOT") );
	MakeBottleAction->setShortcut( QObject::tr( "CTRL+M" ) );
	connect( MakeBottleAction, SIGNAL( triggered() ) , this, SLOT( onTBMUI_MakeBottle() ) );
	
	myTBMUI_MakeBottle = addToolBar( tr( "Make Bottle" ) );
	//insertToolBar( getToolBar_ShapeOps(), myTBMUI_MakeBottle );
	myTBMUI_MakeBottle->addAction( MakeBottleAction );
	myTBMUI_MakeBottle->hide();
}

void ApplicationTut::checkTBMUIX(bool show)
{
	if(show)
	{
		myTBMUI_MakeBottle->show();
	}else
	{
		myTBMUI_MakeBottle->hide();
	}	
}

void ApplicationTut::onTBMUI_MakeBottle()
{
  	DocumentTut* doc = (DocumentTut*)getActiveDocument();
	statusBar()->showMessage( QObject::tr("INF_MAKE_BOTTLE"), 5000 );
	doc->makeBottle();
	
	//onSelectionChanged();
	
	statusBar()->showMessage(QObject::tr("INF_DONE"));
}

void ApplicationTut::addContextItemInPopup(QMenu * menu)
{
	menu->addSeparator();
	menu->addAction( myTBMUI_ViewOps_Actions.at( ViewFitAllId ) );
	//menu->addSeparator();
}	

void ApplicationTut::Popup( const int x, const int y )
{
	ApplicationCommonWindow::Popup(x,y);
}

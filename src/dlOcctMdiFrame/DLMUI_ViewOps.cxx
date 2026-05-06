#include "dlOcctMdiFrame/DLMUI_ViewOps.h"
#include "dlOcctMdiFrame/View.h"
#include "dlOcctMdiFrame/MDIWindow.h"
#include <QApplication>

DLMUI_ViewOps::DLMUI_ViewOps(ApplicationCommonWindow* mainWnd,int id):DLModuleUIEntry(mainWnd,id)
{
	create();
}

DLMUI_ViewOps::~DLMUI_ViewOps()
{
}

void DLMUI_ViewOps::checkNoView(MDIWindow* /*w*/)
{
	myToolBar->hide();
}

void DLMUI_ViewOps::checkNewView(MDIWindow* /*w*/)
{
	myToolBar->show();
}

void DLMUI_ViewOps::checkViewActivated(MDIWindow* w)
{
	myActions.at (ViewHlrOffId)->setChecked (w->getWndView()->isShadingMode());
	myActions.at (ViewHlrOnId)->setChecked (!w->getWndView()->isShadingMode());
}

void DLMUI_ViewOps::create()
{
	init();
	myToolBar = myAppMainWindow->addToolBar( tr( "View Operations" ) );
	myToolBar->addActions( myActions );
	
	myToolBar->hide();
}

#define VIEW_ACTION(ICON_ID,MNU_ID,TIP,MAPID,_SLOT)\
	a = new QAction( QPixmap( dir+QObject::tr(#ICON_ID) ), QObject::tr(#MNU_ID), this );\
	a->setToolTip( QObject::tr(#TIP) );\
	a->setStatusTip( QObject::tr(#TIP) );\
	connect( a, SIGNAL( triggered() ) , this, SLOT( _SLOT() ) );\
	myActions.insert(MAPID, a);

#define VIEW_ACTIONX(ID,MAPID,_SLOT)\
	VIEW_ACTION(ICON_VIEW_##ID,MNU_##ID,TBR_##ID,MAPID,_SLOT)
	
void DLMUI_ViewOps::init()
{
  	QString dir = myAppMainWindow->getResourceDir() + QString( "/" );
  	QAction* a;
	
	VIEW_ACTIONX(FITALL,ViewFitAllId,fitAll)
		
	VIEW_ACTIONX(FITAREA,ViewFitAreaId,fitArea)
	VIEW_ACTIONX(ZOOM,ViewZoomId,zoom)
	VIEW_ACTIONX(PAN,ViewPanId,pan)
	//VIEW_ACTIONX(GLOBALPAN,ViewGlobalPanId,globalPan)
	VIEW_ACTIONX(ROTATION,ViewRotationId,rotation)
		
	VIEW_ACTIONX(FRONT,ViewFrontId,front)
	VIEW_ACTIONX(BACK,ViewBackId,back)
	VIEW_ACTIONX(TOP,ViewTopId,top)
	VIEW_ACTIONX(BOTTOM,ViewBottomId,bottom)
	VIEW_ACTIONX(LEFT,ViewLeftId,left)
	VIEW_ACTIONX(RIGHT,ViewRightId,right)
	
	VIEW_ACTIONX(AXO,ViewAxoId,axo)
	VIEW_ACTIONX(RESET,ViewResetId,reset)
	
	QActionGroup* ag = new QActionGroup( this );
	VIEW_ACTIONX(HLROFF,ViewHlrOffId,hlrOff)
    a->setCheckable( true );
  	ag->addAction(a);
 
  	VIEW_ACTIONX(HLRON,ViewHlrOnId,hlrOn)
 	a->setCheckable( true );
  	ag->addAction(a); 
}	


View* DLMUI_ViewOps::getActiveWndView()
{
	return myAppMainWindow->getActiveWndView();
}

void DLMUI_ViewOps::fitAll()		{	getActiveWndView()->fitAll();												}
void DLMUI_ViewOps::fitArea()		{	getActiveWndView()->setCurrentAction (View::CurAction3d_WindowZooming);		}
void DLMUI_ViewOps::zoom()			{  	getActiveWndView()->setCurrentAction (View::CurAction3d_DynamicZooming);	}
void DLMUI_ViewOps::pan()			{  	getActiveWndView()->setCurrentAction (View::CurAction3d_DynamicPanning);	}
void DLMUI_ViewOps::rotation()		{  	getActiveWndView()->setCurrentAction (View::CurAction3d_DynamicRotation);	}
/*	2026/4/23 11:25:53 这个globalPan的操作怪异，屏蔽
void DLMUI_ViewOps::globalPan()		
{
  // save the current zoom value
  //myCurZoom = getActiveWndView()->getView3d()->Scale();
  getActiveWndView()->scale();
  // Do a Global Zoom
  getActiveWndView()->getView3d()->FitAll();
  // Set the mode
  getActiveWndView()->setCurrentAction (View::CurAction3d_GlobalPanning);
}
*/
	
void DLMUI_ViewOps::front()			{	getActiveWndView()->getView3d()->SetProj( V3d_Yneg );		}
void DLMUI_ViewOps::back()			{  	getActiveWndView()->getView3d()->SetProj( V3d_Ypos );		}
void DLMUI_ViewOps::top()			{  	getActiveWndView()->getView3d()->SetProj( V3d_Zpos );		}
void DLMUI_ViewOps::bottom()		{  	getActiveWndView()->getView3d()->SetProj( V3d_Zneg );		}
void DLMUI_ViewOps::left()			{	getActiveWndView()->getView3d()->SetProj( V3d_Xneg );		}
void DLMUI_ViewOps::right()			{  	getActiveWndView()->getView3d()->SetProj( V3d_Xpos );		}
void DLMUI_ViewOps::axo()			{	getActiveWndView()->getView3d()->SetProj( V3d_XposYnegZpos );}
void DLMUI_ViewOps::reset()			{  	getActiveWndView()->getView3d()->Reset();					}

void DLMUI_ViewOps::hlrOff()		{	getActiveWndView()->switchHLR(false);						}
void DLMUI_ViewOps::hlrOn()			{	getActiveWndView()->switchHLR(true);						}

void View::switchHLR(bool on)
{
	if(myIsShadingMode==!on)
		return;
		
	myIsShadingMode = !on;
	QApplication::setOverrideCursor( Qt::WaitCursor );
  	myView->SetComputedMode (on);
  	myView->Redraw();
  	QApplication::restoreOverrideCursor();
}	

void View::fitAll()
{
	myView->FitAll();
	myView->ZFitAll();
	myView->Redraw();
}
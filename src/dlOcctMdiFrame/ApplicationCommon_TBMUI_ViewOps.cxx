#include "dlOcctMdiFrame/ApplicationCommon.h"
#include "dlOcctMdiFrame/View.h"
#include "dlOcctMdiFrame/MDIWindow.h"
#include <QApplication>

void ApplicationCommonWindow::createTBMUI_ViewOps()
{
	createTBMUI_ViewOps_init();
	myTBMUI_ViewOps = addToolBar( tr( "View Operations" ) );
	myTBMUI_ViewOps->addActions( myTBMUI_ViewOps_Actions );
	
	myTBMUI_ViewOps->hide();
}

#define VIEW_ACTION(ICON_ID,MNU_ID,TIP,MAPID,_SLOT)\
	a = new QAction( QPixmap( dir+QObject::tr(#ICON_ID) ), QObject::tr(#MNU_ID), this );\
	a->setToolTip( QObject::tr(#TIP) );\
	a->setStatusTip( QObject::tr(#TIP) );\
	connect( a, SIGNAL( triggered() ) , this, SLOT( _SLOT() ) );\
	myTBMUI_ViewOps_Actions.insert(MAPID, a);

#define VIEW_ACTIONX(ID,MAPID,_SLOT)\
	VIEW_ACTION(ICON_VIEW_##ID,MNU_##ID,TBR_##ID,MAPID,onTBMUI_ViewOps_##_SLOT)
	
	
void ApplicationCommonWindow::createTBMUI_ViewOps_init()
{
  	QString dir = ApplicationCommonWindow::getResourceDir() + QString( "/" );
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

void ApplicationCommonWindow::checkTBMUI_ViewOps(MDIWindow* w)
{
	if(w!=NULL)
	{
		myTBMUI_ViewOps->show();
		myTBMUI_ViewOps_Actions.at (ViewHlrOffId)->setChecked (w->getWndView()->isShadingMode());
		myTBMUI_ViewOps_Actions.at (ViewHlrOnId)->setChecked (!w->getWndView()->isShadingMode());
	}else
	{
		myTBMUI_ViewOps->hide();
	}
}

void ApplicationCommonWindow::onTBMUI_ViewOps_fitAll()		{	getActiveWndView()->fitAll();												}
void ApplicationCommonWindow::onTBMUI_ViewOps_fitArea()		{	getActiveWndView()->setCurrentAction (View::CurAction3d_WindowZooming);		}
void ApplicationCommonWindow::onTBMUI_ViewOps_zoom()		{  	getActiveWndView()->setCurrentAction (View::CurAction3d_DynamicZooming);	}
void ApplicationCommonWindow::onTBMUI_ViewOps_pan()			{  	getActiveWndView()->setCurrentAction (View::CurAction3d_DynamicPanning);	}
void ApplicationCommonWindow::onTBMUI_ViewOps_rotation()	{  	getActiveWndView()->setCurrentAction (View::CurAction3d_DynamicRotation);	}
/*	2026/4/23 11:25:53 这个globalPan的操作怪异，屏蔽
void ApplicationCommonWindow::onTBMUI_ViewOps_globalPan()		
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
	
void ApplicationCommonWindow::onTBMUI_ViewOps_front()			{	getActiveWndView()->getView3d()->SetProj( V3d_Yneg );		}
void ApplicationCommonWindow::onTBMUI_ViewOps_back()			{  	getActiveWndView()->getView3d()->SetProj( V3d_Ypos );		}
void ApplicationCommonWindow::onTBMUI_ViewOps_top()				{  	getActiveWndView()->getView3d()->SetProj( V3d_Zpos );		}
void ApplicationCommonWindow::onTBMUI_ViewOps_bottom()			{  	getActiveWndView()->getView3d()->SetProj( V3d_Zneg );		}
void ApplicationCommonWindow::onTBMUI_ViewOps_left()			{	getActiveWndView()->getView3d()->SetProj( V3d_Xneg );		}
void ApplicationCommonWindow::onTBMUI_ViewOps_right()			{  	getActiveWndView()->getView3d()->SetProj( V3d_Xpos );		}
void ApplicationCommonWindow::onTBMUI_ViewOps_axo()				{	getActiveWndView()->getView3d()->SetProj( V3d_XposYnegZpos );}
void ApplicationCommonWindow::onTBMUI_ViewOps_reset()			{  	getActiveWndView()->getView3d()->Reset();					}

void ApplicationCommonWindow::onTBMUI_ViewOps_hlrOff()			{	getActiveWndView()->switchHLR(false);						}
void ApplicationCommonWindow::onTBMUI_ViewOps_hlrOn()			{	getActiveWndView()->switchHLR(true);						}

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

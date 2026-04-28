#include "dlOcctMdiFrame/ApplicationCommon.h"
#include "dlOcctMdiFrame/View.h"
#include "dlOcctMdiFrame/MDIWindow.h"
#include <QApplication>

void ApplicationCommonWindow::createTBMUI_Raytrace()
{
	createTBMUI_Raytrace_init();
	// populate a tool bar with some actions
	myTBMUI_Raytrace = addToolBar( tr( "Ray-tracing Options" ) );
	myTBMUI_Raytrace->addActions( myTBMUI_Raytrace_Actions );
	
	myTBMUI_Raytrace->hide();
}

void ApplicationCommonWindow::checkTBMUI_Raytrace(MDIWindow* w)
{
	if(w!=NULL)
	{
		myTBMUI_Raytrace->show();
		myTBMUI_Raytrace_Actions.at (ToolRaytracingId)->setChecked (w->getWndView()->IsRaytracingMode());
		myTBMUI_Raytrace_Actions.at (ToolShadowsId)->setChecked (w->getWndView()->IsShadowsEnabled());
		myTBMUI_Raytrace_Actions.at (ToolReflectionsId)->setChecked (w->getWndView()->IsReflectionsEnabled());
		myTBMUI_Raytrace_Actions.at (ToolAntialiasingId)->setChecked (w->getWndView()->IsAntialiasingEnabled());
	}else
	{
		myTBMUI_Raytrace->hide();
	}	
}

void ApplicationCommonWindow::createTBMUI_Raytrace_init()
{
	QString dir = ApplicationCommonWindow::getResourceDir() + QString( "/" );
	
	QAction* a;
	
	a = new QAction( QPixmap( dir+QObject::tr("ICON_TOOL_RAYTRACING") ), QObject::tr("MNU_TOOL_RAYTRACING"), this );
	a->setToolTip( QObject::tr("TBR_TOOL_RAYTRACING") );
	a->setStatusTip( QObject::tr("TBR_TOOL_RAYTRACING") );
	a->setCheckable( true );
	//a->setChecked( false );
	connect( a, SIGNAL( triggered() ) , this, SLOT( onTBMUI_Raytrace() ) );
	myTBMUI_Raytrace_Actions.insert( ToolRaytracingId, a );
	
	a = new QAction( QPixmap( dir+QObject::tr("ICON_TOOL_SHADOWS") ), QObject::tr("MNU_TOOL_SHADOWS"), this );
	a->setToolTip( QObject::tr("TBR_TOOL_SHADOWS") );
	a->setStatusTip( QObject::tr("TBR_TOOL_SHADOWS") );
	a->setCheckable( true );
	//a->setChecked( true );
	connect( a, SIGNAL( triggered() ) , this, SLOT( onTBMUI_Raytrace() ) );
	myTBMUI_Raytrace_Actions.insert( ToolShadowsId, a );
	
	a = new QAction( QPixmap( dir+QObject::tr("ICON_TOOL_REFLECTIONS") ), QObject::tr("MNU_TOOL_REFLECTIONS"), this );
	a->setToolTip( QObject::tr("TBR_TOOL_REFLECTIONS") );
	a->setStatusTip( QObject::tr("TBR_TOOL_REFLECTIONS") );
	a->setCheckable( true );
	//a->setChecked( false );
	connect( a, SIGNAL( triggered() ) , this, SLOT( onTBMUI_Raytrace() ) );
	myTBMUI_Raytrace_Actions.insert( ToolReflectionsId, a );
	
	a = new QAction( QPixmap( dir+QObject::tr("ICON_TOOL_ANTIALIASING") ), QObject::tr("MNU_TOOL_ANTIALIASING"), this );
	a->setToolTip( QObject::tr("TBR_TOOL_ANTIALIASING") );
	a->setStatusTip( QObject::tr("TBR_TOOL_ANTIALIASING") );
	a->setCheckable( true );
	//a->setChecked( false );
	connect( a, SIGNAL( triggered() ) , this, SLOT( onTBMUI_Raytrace() ) );
	myTBMUI_Raytrace_Actions.insert( ToolAntialiasingId, a );  
}

void ApplicationCommonWindow::onTBMUI_Raytrace()
{
	QAction* aSentBy = (QAction*)sender();
	View* v = getActiveWndView();
	if (aSentBy == myTBMUI_Raytrace_Actions.at (ToolRaytracingId))
	{
		bool aState = myTBMUI_Raytrace_Actions.at (ToolRaytracingId)->isChecked(); 
		QApplication::setOverrideCursor (Qt::WaitCursor);
		if (aState)
			v->EnableRaytracing();
		else
			v->DisableRaytracing();
		QApplication::restoreOverrideCursor();
	}
	
	if (aSentBy == myTBMUI_Raytrace_Actions.at (ToolShadowsId))
	{
		bool aState = myTBMUI_Raytrace_Actions.at (ToolShadowsId)->isChecked(); 
		v->SetRaytracedShadows (aState);
	}
	
	if (aSentBy == myTBMUI_Raytrace_Actions.at (ToolReflectionsId))
	{
		bool aState = myTBMUI_Raytrace_Actions.at (ToolReflectionsId)->isChecked();
		v->SetRaytracedReflections (aState);
	}
	
	if (aSentBy == myTBMUI_Raytrace_Actions.at (ToolAntialiasingId))
	{
		bool aState = myTBMUI_Raytrace_Actions.at (ToolAntialiasingId)->isChecked();
		v->SetRaytracedAntialiasing (aState);
	}
}

void View::SetRaytracedShadows (bool theState)
{
  myView->ChangeRenderingParams().IsShadowEnabled = theState;

  myIsShadowsEnabled = theState;

  myContext->UpdateCurrentViewer();
}

void View::SetRaytracedReflections (bool theState)
{
  myView->ChangeRenderingParams().IsReflectionEnabled = theState;

  myIsReflectionsEnabled = theState;

  myContext->UpdateCurrentViewer();
}

void View::SetRaytracedAntialiasing (bool theState)
{
  myView->ChangeRenderingParams().IsAntialiasingEnabled = theState;

  myIsAntialiasingEnabled = theState;

  myContext->UpdateCurrentViewer();
}

void View::EnableRaytracing()
{
  if (!myIsRaytracing)
    myView->ChangeRenderingParams().Method = Graphic3d_RM_RAYTRACING;

  myIsRaytracing = true;

  myContext->UpdateCurrentViewer();
}

void View::DisableRaytracing()
{
  if (myIsRaytracing)
    myView->ChangeRenderingParams().Method = Graphic3d_RM_RASTERIZATION;

  myIsRaytracing = false;

  myContext->UpdateCurrentViewer();
}
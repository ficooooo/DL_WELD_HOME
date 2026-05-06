#include "dlOcctMdiFrame/DLMUI_Raytrace.h"
#include "dlOcctMdiFrame/View.h"
#include "dlOcctMdiFrame/MDIWindow.h"
#include <QApplication>

DLMUI_Raytrace::DLMUI_Raytrace(ApplicationCommonWindow* mainWnd,int id):DLModuleUIEntry(mainWnd,id)
{
	create();
}

DLMUI_Raytrace::~DLMUI_Raytrace()
{
}

void DLMUI_Raytrace::checkNoView(MDIWindow* /*w*/)
{
	myToolBar->hide();
}

void DLMUI_Raytrace::checkNewView(MDIWindow* /*w*/)
{
	myToolBar->show();
}

void DLMUI_Raytrace::checkViewActivated(MDIWindow* w)
{
	myActions.at (ToolRaytracingId)->setChecked (w->getWndView()->IsRaytracingMode());
	myActions.at (ToolShadowsId)->setChecked (w->getWndView()->IsShadowsEnabled());
	myActions.at (ToolReflectionsId)->setChecked (w->getWndView()->IsReflectionsEnabled());
	myActions.at (ToolAntialiasingId)->setChecked (w->getWndView()->IsAntialiasingEnabled());
}

void DLMUI_Raytrace::create()
{
	init();
	// populate a tool bar with some actions
	myToolBar = myAppMainWindow->addToolBar( tr( "Ray-tracing Options" ) );
	myToolBar->addActions( myActions );
	
	myToolBar->hide();
}

void DLMUI_Raytrace::init()
{
	QString dir = ApplicationCommonWindow::getResourceDir() + QString( "/" );
	
	QAction* a;
	
	a = new QAction( QPixmap( dir+QObject::tr("ICON_TOOL_RAYTRACING") ), QObject::tr("MNU_TOOL_RAYTRACING"), this );
	a->setToolTip( QObject::tr("TBR_TOOL_RAYTRACING") );
	a->setStatusTip( QObject::tr("TBR_TOOL_RAYTRACING") );
	a->setCheckable( true );
	//a->setChecked( false );
	connect( a, SIGNAL( triggered() ) , this, SLOT( raytraceSet() ) );
	myActions.insert( ToolRaytracingId, a );
	
	a = new QAction( QPixmap( dir+QObject::tr("ICON_TOOL_SHADOWS") ), QObject::tr("MNU_TOOL_SHADOWS"), this );
	a->setToolTip( QObject::tr("TBR_TOOL_SHADOWS") );
	a->setStatusTip( QObject::tr("TBR_TOOL_SHADOWS") );
	a->setCheckable( true );
	//a->setChecked( true );
	connect( a, SIGNAL( triggered() ) , this, SLOT( raytraceSet() ) );
	myActions.insert( ToolShadowsId, a );
	
	a = new QAction( QPixmap( dir+QObject::tr("ICON_TOOL_REFLECTIONS") ), QObject::tr("MNU_TOOL_REFLECTIONS"), this );
	a->setToolTip( QObject::tr("TBR_TOOL_REFLECTIONS") );
	a->setStatusTip( QObject::tr("TBR_TOOL_REFLECTIONS") );
	a->setCheckable( true );
	//a->setChecked( false );
	connect( a, SIGNAL( triggered() ) , this, SLOT( raytraceSet() ) );
	myActions.insert( ToolReflectionsId, a );
	
	a = new QAction( QPixmap( dir+QObject::tr("ICON_TOOL_ANTIALIASING") ), QObject::tr("MNU_TOOL_ANTIALIASING"), this );
	a->setToolTip( QObject::tr("TBR_TOOL_ANTIALIASING") );
	a->setStatusTip( QObject::tr("TBR_TOOL_ANTIALIASING") );
	a->setCheckable( true );
	//a->setChecked( false );
	connect( a, SIGNAL( triggered() ) , this, SLOT( raytraceSet() ) );
	myActions.insert( ToolAntialiasingId, a );  
}

void DLMUI_Raytrace::raytraceSet()
{
	QAction* aSentBy = (QAction*)sender();
	View* v = myAppMainWindow->getActiveWndView();
	if (aSentBy == myActions.at (ToolRaytracingId))
	{
		bool aState = myActions.at (ToolRaytracingId)->isChecked(); 
		QApplication::setOverrideCursor (Qt::WaitCursor);
		if (aState)
			v->EnableRaytracing();
		else
			v->DisableRaytracing();
		QApplication::restoreOverrideCursor();
	}
	
	if (aSentBy == myActions.at (ToolShadowsId))
	{
		bool aState = myActions.at (ToolShadowsId)->isChecked(); 
		v->SetRaytracedShadows (aState);
	}
	
	if (aSentBy == myActions.at (ToolReflectionsId))
	{
		bool aState = myActions.at (ToolReflectionsId)->isChecked();
		v->SetRaytracedReflections (aState);
	}
	
	if (aSentBy == myActions.at (ToolAntialiasingId))
	{
		bool aState = myActions.at (ToolAntialiasingId)->isChecked();
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
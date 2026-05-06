#include "dlOcctMdiFrame/DocumentCommon.h"
#include "dlOcctMdiFrame/ApplicationCommon.h"
#include "dlOcctMdiFrame/MDIWindow.h"
#include "dlOcctMdiFrame/View.h"
#include "dlOcctMdiFrame/DL_AIS_Scene_Floor.h"
#include "dlOcctMdiFrame/DLModuleUIEntry.h"

#include <QStatusBar>


#include <AIS_ViewCube.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <AIS_InteractiveObject.hxx>
#include <Graphic3d_NameOfMaterial.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <TCollection_AsciiString.hxx>

// =======================================================================
// function : Viewer
// purpose  :
// =======================================================================
Handle(V3d_Viewer) DocumentCommon::Viewer (const Standard_ExtString ,
                                           const Standard_CString ,
                                           const Standard_Real theViewSize,
                                           const V3d_TypeOfOrientation theViewProj,
                                           const Standard_Boolean theComputedMode,
                                           const Standard_Boolean theDefaultComputedMode )
{
	static Handle(OpenGl_GraphicDriver) aGraphicDriver;
	
	if (aGraphicDriver.IsNull())
	{
		Handle(Aspect_DisplayConnection) aDisplayConnection;
		aGraphicDriver = new OpenGl_GraphicDriver (aDisplayConnection);
	}
	
	Handle(V3d_Viewer) aViewer = new V3d_Viewer (aGraphicDriver);
	aViewer->SetDefaultViewSize (theViewSize);
	aViewer->SetDefaultViewProj (theViewProj);
	aViewer->SetComputedMode (theComputedMode);
	aViewer->SetDefaultComputedMode (theDefaultComputedMode);
	return aViewer;
}

DocumentCommon::DocumentCommon( const int theIndex, ApplicationCommonWindow* app )
	: QObject( app ),
	myApp( app ),
	myIndex( theIndex ),
	myNbMDIWindows( 0 )
{
	myDLModuleId = ApplicationCommonWindow::DLMUI_FileWndId;
		
	TCollection_ExtendedString a3DName ("Visu3D");
	
	myViewer = Viewer (a3DName.ToExtString(), "", 1000.0, V3d_XposYnegZpos, Standard_True, Standard_True);
	
	myViewer->SetDefaultLights();
	myViewer->SetLightOn();
	
	myContext = new AIS_InteractiveContext (myViewer);
}

DocumentCommon::~DocumentCommon()
{
	SCENE_COMPNENTS_LIST.clear();
	printf("DocumentCommon::~DocumentCommon()\n");
}

void DocumentCommon::initScene()
{
	Handle(AIS_ViewCube) VC = new AIS_ViewCube();
	SCENE_COMPNENTS_LIST.insert(DL_SCENE_VIEWCUBE,VC);
	myContext->Display(VC, Standard_False);
	
	int floorSize= getFloorSize();
	int floorGridFactor = getFloorGridSizeFactor();
	Handle(AIS_InteractiveObject) floor = new DL_AIS_Scene_Floor(floorSize,floorGridFactor,Quantity_NOC_GRAY58,3);
	SCENE_COMPNENTS_LIST.insert(DL_SCENE_FLOOR,floor);
	myContext->Display(floor,0,-1, Standard_False);
	
	Handle(AIS_InteractiveObject) floor1 = new DL_AIS_Scene_Floor(floorSize,floorGridFactor-2,Quantity_NOC_GRAY18,1);
	SCENE_COMPNENTS_LIST.insert(DL_SCENE_FLOOR_THIN,floor1);
	myContext->Display(floor1,0,-1, Standard_False);
}

void DocumentCommon::clearScene()
{
	for (int i = DL_VIEWOBJECT_COUNT; i < getNbSceneComponents() ; ++i)
	{
		Handle(AIS_InteractiveObject) obj = SCENE_COMPNENTS_LIST.take(i);
		myContext->Remove(obj, Standard_False);
	}
	
	fitAll();
}

/*
QMdiSubWindow *QMdiArea::addSubWindow(QWidget *widget, Qt::WindowFlags windowFlags = Qt::WindowFlags())
	Adds widget as a new subwindow to the MDI area. 
	If windowFlags are non-zero, they will override the flags set on the widget.
	The widget can be either a QMdiSubWindow 
		or another QWidget (in which case the MDI area will create a subwindow and set the widget as the internal widget).
		
void QMdiSubWindow::setWidget(QWidget *widget)
	Sets widget as the internal widget of this subwindow. 
	The internal widget is displayed in the center of the subwindow beneath the title bar.
	QMdiSubWindow takes temporary ownership of widget; you do not have to delete it. 
	Any existing internal widget will be removed and reparented to the root window.
*/

MDIWindow* DocumentCommon::onCreatMDIWindow()
{
  	QMdiArea* ws = myApp->getMdiArea();
  	MDIWindow* w = new MDIWindow (this, ws, 0);
    
  	if (!w)
    	return NULL;
    	
	ws->addSubWindow (w);
  	myMDIWindows.append (w);
	
  	connect( w, SIGNAL( message( const QString&, int ) ),myApp->statusBar(), SLOT( showMessage( const QString&, int ) ) );
  	connect( w, SIGNAL( sendCloseView( MDIWindow* ) ),this, SLOT( onCloseMDIWindow( MDIWindow* ) ) );
	
  	QString aName;
  	//TODO  2026/4/2 14:30:46
  	w->setWindowTitle( aName.sprintf( "Document %d:%d", myIndex, ++myNbMDIWindows ) );
  	QString dir = ApplicationCommonWindow::getResourceDir() + QString( "/" );
  
  	w->setWindowIcon( QPixmap( dir + QObject::tr("ICON_DOC") ) );
	
	w->showMaximized();
  	w->setFocus();
  	
  	return w;
}

//MDIWindow::close 窗口关闭
//File菜单>>Close  onCloseMDIWindow 调用activeSubWindow()->close();
//移除当前窗口，关闭当前文档的最后一个窗口时执行删除文档操作
void DocumentCommon::onCloseMDIWindow(MDIWindow* theView)
{
    myMDIWindows.removeOne(theView);
    ApplicationCommonWindow::getMdiArea()->removeSubWindow(theView);
    if( myMDIWindows.count() == 0 )
        emit sendCloseDocument( this );
    
    if(myApp->getNbWndView()<1)    
    {	
   		printf("--------------------DocumentCommon::onCloseMDIWindow--------------------\n");
    	DLModuleUIEntry::checkWindowActionAll(DLModuleUIEntry::CLOSE_VIEW,theView);    
    	printf("--------------------DocumentCommon::onCloseMDIWindow--------------------\n");
	}
}

void DocumentCommon::fitAll()
{
	QList<MDIWindow*>::iterator i;
	for ( i = myMDIWindows.begin(); i != myMDIWindows.end(); i++ )
		(*i)->getWndView()->fitAll();
}
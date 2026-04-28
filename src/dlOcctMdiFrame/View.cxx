#include "dlOcctMdiFrame/View.h"
#include "dlOcctMdiFrame/ApplicationCommon.h"
#include <dlOcctMdiFrame/OcctWindow.h>

#include <QApplication>
#include <QPainter>
#include <QMenu>
#include <QCursor>
#include <QFileInfo>
#include <QMouseEvent>
#include <QRubberBand>
#include <QMdiSubWindow>
#include <QStyleFactory>

#include <Graphic3d_GraphicDriver.hxx>
#include <Graphic3d_TextureEnv.hxx>
#include <Aspect_DisplayConnection.hxx>

namespace
{
	//! Map Qt buttons bitmask to virtual keys.
	Aspect_VKeyMouse qtMouseButtons2VKeys (Qt::MouseButtons theButtons)
	{
		Aspect_VKeyMouse aButtons = Aspect_VKeyMouse_NONE;
		if ((theButtons & Qt::LeftButton) != 0)
		{
			aButtons |= Aspect_VKeyMouse_LeftButton;
		}
		if ((theButtons & Qt::MiddleButton) != 0)
		{
			aButtons |= Aspect_VKeyMouse_MiddleButton;
		}
		if ((theButtons & Qt::RightButton) != 0)
		{
			aButtons |= Aspect_VKeyMouse_RightButton;
		}
		return aButtons;
	}

	//! Map Qt mouse modifiers bitmask to virtual keys.
	Aspect_VKeyFlags qtMouseModifiers2VKeys (Qt::KeyboardModifiers theModifiers)
	{
		Aspect_VKeyFlags aFlags = Aspect_VKeyFlags_NONE;
		if ((theModifiers & Qt::ShiftModifier) != 0)
		{
			aFlags |= Aspect_VKeyFlags_SHIFT;
		}
		if ((theModifiers & Qt::ControlModifier) != 0)
		{
			aFlags |= Aspect_VKeyFlags_CTRL;
		}
		if ((theModifiers & Qt::AltModifier) != 0)
		{
			aFlags |= Aspect_VKeyFlags_ALT;
		}
		return aFlags;
	}
}

QCursor* View::defCursor     = NULL;
QCursor* View::handCursor    = NULL;
QCursor* View::panCursor     = NULL;
//QCursor* View::globPanCursor = NULL;
QCursor* View::zoomCursor    = NULL;
QCursor* View::rotCursor     = NULL;

void View::initCursors()
{
	if(defCursor)
		return;
		
    defCursor = new QCursor( Qt::ArrowCursor );
    handCursor = new QCursor( Qt::PointingHandCursor );
    panCursor = new QCursor( Qt::SizeAllCursor );
    //globPanCursor = new QCursor( Qt::CrossCursor );
    zoomCursor = new QCursor( QPixmap( ApplicationCommonWindow::getResourceDir() + QString( "/" ) + QObject::tr( "ICON_CURSOR_ZOOM" ) ) );
    rotCursor = new QCursor( QPixmap( ApplicationCommonWindow::getResourceDir() + QString( "/" ) + QObject::tr( "ICON_CURSOR_ROTATE" ) ) );
}

View::View( Handle(AIS_InteractiveContext) theContext, QWidget* parent )
		: QWidget( parent ),
		  myIsRaytracing( false ),
		  myIsShadowsEnabled (true),
		  myIsReflectionsEnabled (false),
		  myIsAntialiasingEnabled (false),
		  myIsShadingMode(true),
		  myIsEnvironmentMapped(false)
{
	myContext = theContext;
	myCurZoom = 0;
	
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_DeleteOnClose);
	
	myDefaultGestures = myMouseGestureMap;
	myCurrentMode = CurAction3d_Nothing;
	setMouseTracking( true );
	
	initCursors();
	
	setBackgroundRole( QPalette::NoRole );//NoBackground );
	// set focus policy to threat QContextMenuEvent from keyboard  
	setFocusPolicy( Qt::StrongFocus );
	init();
}

View::~View()
{
	printf("View::~View()\n");
}

void View::init()
{
	if ( myView.IsNull() )
    	myView = myContext->CurrentViewer()->CreateView();
	
  	Handle(OcctWindow) hWnd = new OcctWindow ( this );
  	myView->SetWindow (hWnd);
  	if ( !hWnd->IsMapped() )
  	{
    	hWnd->Map();
  	}
  	myView->SetBackgroundColor (Quantity_NOC_BLACK);
  	myView->MustBeResized();

  	if (myIsRaytracing)
    	myView->ChangeRenderingParams().Method = Graphic3d_RM_RAYTRACING;
}

void View::paintEvent( QPaintEvent *  )
{
	myView->InvalidateImmediate();
  	FlushViewEvents (myContext, myView, true);
}

void View::resizeEvent( QResizeEvent * )
{
//  QApplication::syncX();
	if( !myView.IsNull() )
	{
		myView->MustBeResized();
	}
}

void View::OnSelectionChanged (const Handle(AIS_InteractiveContext)& ,
                               const Handle(V3d_View)& )
{
	//ApplicationCommonWindow::getApplication()->onSelectionChanged();
	emit selectionChanged();
}


/*!
  Get paint engine for the OpenGL viewer. [ virtual public ]
*/
QPaintEngine* View::paintEngine() const
{
  return 0;
}

void View::mousePressEvent (QMouseEvent* theEvent)
{
	const Graphic3d_Vec2i aPnt (theEvent->pos().x(), theEvent->pos().y());
	const Aspect_VKeyFlags aFlags = qtMouseModifiers2VKeys (theEvent->modifiers());
	if (!myView.IsNull()
		&& UpdateMouseButtons (aPnt,qtMouseButtons2VKeys (theEvent->buttons()),aFlags,false))
	{
		update();
	}
	myClickPos = aPnt;
}

void View::mouseReleaseEvent (QMouseEvent* theEvent)
{
	const Graphic3d_Vec2i aPnt (theEvent->pos().x(), theEvent->pos().y());
	const Aspect_VKeyFlags aFlags = qtMouseModifiers2VKeys (theEvent->modifiers());
	if (!myView.IsNull()
		&& UpdateMouseButtons (aPnt,qtMouseButtons2VKeys (theEvent->buttons()),	aFlags,	false))
	{
		update();
	}
	
	//if (myCurrentMode == CurAction3d_GlobalPanning)
	//{
	//	myView->Place (aPnt.x(), aPnt.y(), myCurZoom);
	//}
	
	if (myCurrentMode != CurAction3d_Nothing)
	{
		setCurrentAction (CurAction3d_Nothing);
	}
	if (theEvent->button() == Qt::RightButton
		&& (aFlags & Aspect_VKeyFlags_CTRL) == 0
		&& (myClickPos - aPnt).cwiseAbs().maxComp() <= 4)
	{
		Popup (aPnt.x(), aPnt.y());
	}
}

void View::mouseMoveEvent (QMouseEvent* theEvent)
{
	const Graphic3d_Vec2i aNewPos (theEvent->pos().x(), theEvent->pos().y());
	if (!myView.IsNull()
		&& UpdateMousePosition (aNewPos,
	                        qtMouseButtons2VKeys (theEvent->buttons()),
	                        qtMouseModifiers2VKeys (theEvent->modifiers()),
	                        false))
	{
		update();
	}
}

//==============================================================================
//function : wheelEvent
//purpose  :
//==============================================================================
void View::wheelEvent (QWheelEvent* theEvent)
{
	const Graphic3d_Vec2i aPos (theEvent->pos().x(), theEvent->pos().y());
	if (!myView.IsNull()
		&& UpdateZoom (Aspect_ScrollDelta (aPos, theEvent->delta() / 8)))
	{
		update();
	}
}

void View::setCurrentAction (CurrentAction3d theAction)
{
	myCurrentMode = theAction;
	myMouseGestureMap.Clear();
	switch (myCurrentMode)
	{
		case CurAction3d_Nothing:
		{
			myMouseGestureMap = myDefaultGestures;
			setCursor( *defCursor );
			break;
		}
		case CurAction3d_DynamicZooming:
		{
			myMouseGestureMap.Bind (Aspect_VKeyMouse_LeftButton, AIS_MouseGesture_Zoom);
			setCursor( *zoomCursor );
			break;
		}
		//case CurAction3d_GlobalPanning:		{break;	}
		case CurAction3d_WindowZooming:
		{
			myMouseGestureMap.Bind (Aspect_VKeyMouse_LeftButton, AIS_MouseGesture_ZoomWindow);
			setCursor( *handCursor );
			break;
		}
		case CurAction3d_DynamicPanning:
		{
			myMouseGestureMap.Bind (Aspect_VKeyMouse_LeftButton, AIS_MouseGesture_Pan);
			setCursor( *panCursor );
			break;
		}
		case CurAction3d_DynamicRotation:
		{
			myMouseGestureMap.Bind (Aspect_VKeyMouse_LeftButton, AIS_MouseGesture_RotateOrbit);
			setCursor( *rotCursor );
			break;
		}
		default:
		{
			setCursor( *defCursor );
			break;
		}
	}
}

bool View::dump(Standard_CString theFile)
{
  return myView->Dump(theFile);
}
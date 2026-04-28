#include "dlOcctMdiFrame/ApplicationCommon.h"
#include "dlOcctMdiFrame/View.h"
#include "dlOcctMdiFrame/DocumentCommon.h"
#include "dlOcctMdiFrame/MDIWindow.h"

#include <QMenuBar>
#include <QColorDialog>
#include <QFileDialog>

void ApplicationCommonWindow::createTBMUI_BackgroundSet()
{
	//如果是一个固定的弹出式菜单，可以创建Menu备用
	//本系统假设所有右键弹出菜单都是上下文相关动态创建   2026/4/24 9:07:35
	createTBMUI_BackgroundSet_init();
	myTBMUI_BackgroundSet_PopupMenu = menuBar()->addMenu( QObject::tr( "MNU_BACKGROUNDSET" ) );
	myTBMUI_BackgroundSet_PopupMenu->addAction( myTBMUI_BackgroundSet_Actions.at( BackgroundSet_ColorId ) );
	myTBMUI_BackgroundSet_PopupMenu->addAction( myTBMUI_BackgroundSet_Actions.at( BackgroundSet_EnvironmentMapId ) );
	
	//myTBMUI_BackgroundSet_PopupMenu->hide();
	menuBar()->removeAction( myTBMUI_BackgroundSet_PopupMenu->menuAction() );
}	

void ApplicationCommonWindow::createTBMUI_BackgroundSet_init()
{
	QAction* a = new QAction( QObject::tr("MNU_CH_BACK"), this );
	a->setToolTip( QObject::tr("TBR_CH_BACK") );
	connect( a, SIGNAL( triggered() ), this, SLOT( onTBMUI_BackgroundSet_Color() ) );
	myTBMUI_BackgroundSet_Actions.insert( BackgroundSet_ColorId, a );
					
	a = new QAction( QObject::tr("MNU_CH_ENV_MAP"), this );
	a->setToolTip( QObject::tr("TBR_CH_ENV_MAP") );
	connect( a, SIGNAL( triggered() ), this, SLOT( onTBMUI_BackgroundSet_EnvironmentMap() ) );
	a->setCheckable( true );
	myTBMUI_BackgroundSet_Actions.insert( BackgroundSet_EnvironmentMapId, a );
}

void ApplicationCommonWindow::checkTBMUI_BackgroundSet(MDIWindow* /*w*/)
{
	//对应弹出式菜单，不需要此函数，为保持一致性
}

void ApplicationCommonWindow::TBMUI_Popup_BackgroundSet()
{
	QMenu* myToolMenu = new QMenu( 0 );
			
	myToolMenu->addAction( myTBMUI_BackgroundSet_Actions.at( BackgroundSet_ColorId ) );
	myToolMenu->addAction( myTBMUI_BackgroundSet_Actions.at( BackgroundSet_EnvironmentMapId ) );
	
	myTBMUI_BackgroundSet_Actions.at( BackgroundSet_EnvironmentMapId )->setChecked(getActiveWndView()->isEnvironmentMapped());
	
	addContextItemInPopup(myToolMenu);
	
	myToolMenu->exec( QCursor::pos() );
	
	delete myToolMenu;
}	

//派生窗口类可重载此方法，实现更复杂的上下文右键菜单
void ApplicationCommonWindow::Popup( const int /*x*/, const int /*y*/ )
{
	QMdiSubWindow* w = getActiveWnd();
	DocumentCommon*  doc = getActiveDocument();
	
	if ( doc->getContext()->NbSelected() )
	{
		TBMUI_Popup_ModelSkin();
	}
	else
	{
		TBMUI_Popup_BackgroundSet();
	}
	
	if ( w )
		w->setFocus();
}

void ApplicationCommonWindow::onTBMUI_BackgroundSet_Color()
{
	getActiveWnd()->getWndView()->onBackground();
}	

void ApplicationCommonWindow::onTBMUI_BackgroundSet_EnvironmentMap()
{
	getActiveWnd()->getWndView()->onEnvironmentMap();
}

//右键弹出菜单
void View::Popup( const int x, const int y )
{
	ApplicationCommonWindow* stApp = ApplicationCommonWindow::getApplication();
	stApp->Popup(x,y);
}

void View::onBackground()
{
    QColor aColor ;
    Standard_Real R1;
    Standard_Real G1;
    Standard_Real B1;
    myView->BackgroundColor(Quantity_TOC_RGB,R1,G1,B1);
    aColor.setRgb((Standard_Integer)(R1 * 255), (Standard_Integer)(G1 * 255), (Standard_Integer)(B1 * 255));

    QColor aRetColor = QColorDialog::getColor(aColor);

    if( aRetColor.isValid() )
    {
        R1 = aRetColor.red()/255.;
        G1 = aRetColor.green()/255.;
        B1 = aRetColor.blue()/255.;
        myView->SetBackgroundColor(Quantity_TOC_RGB,R1,G1,B1);
    }
    myView->Redraw();
}

void View::onEnvironmentMap()
{
	//if (myBackMenu->actions().at(1)->isChecked())
	if (!isEnvironmentMapped())		
	{
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
														tr("All Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.tga)"));
		
		if(fileName=="")
			return;
	
		const TCollection_AsciiString anUtf8Path (fileName.toUtf8().data());
		Handle(Graphic3d_TextureEnv) aTexture = new Graphic3d_TextureEnv( anUtf8Path );
		myView->SetTextureEnv (aTexture);
	
		myIsEnvironmentMapped = !myIsEnvironmentMapped;
	}
	else
	{
		myView->SetTextureEnv (Handle(Graphic3d_TextureEnv)());
		myIsEnvironmentMapped = !myIsEnvironmentMapped;
	}
	
	myView->Redraw();
}
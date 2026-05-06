#include "dlOcctMdiFrame/DLMUI_BackgroundSet.h"
#include "dlOcctMdiFrame/View.h"
#include "dlOcctMdiFrame/DocumentCommon.h"
#include "dlOcctMdiFrame/MDIWindow.h"

#include <QMenuBar>
#include <QColorDialog>
#include <QFileDialog>

DLMUI_BackgroundSet::DLMUI_BackgroundSet(ApplicationCommonWindow* mainWnd,int id):DLModuleUIEntry(mainWnd,id)
{
	create();
}

DLMUI_BackgroundSet::~DLMUI_BackgroundSet()
{
}

void DLMUI_BackgroundSet::create()
{
	//如果是一个固定的弹出式菜单，可以创建Menu备用
	//本系统假设所有右键弹出菜单都是上下文相关动态创建   2026/4/24 9:07:35
	init();
	myMenu = myAppMainWindow->menuBar()->addMenu( QObject::tr( "MNU_BACKGROUNDSET" ) );
	myMenu->addAction( myActions.at( BGSet_ColorId ) );
	myMenu->addAction( myActions.at( BGSet_EnvironmentMapId ) );
	
	//myMenu->hide();
	myAppMainWindow->menuBar()->removeAction( myMenu->menuAction() );
}	

void DLMUI_BackgroundSet::init()
{
	QAction* a = new QAction( QObject::tr("MNU_CH_BACK"), this );
	a->setToolTip( QObject::tr("TBR_CH_BACK") );
	connect( a, SIGNAL( triggered() ), this, SLOT( onColor() ) );
	myActions.insert( BGSet_ColorId, a );
					
	a = new QAction( QObject::tr("MNU_CH_ENV_MAP"), this );
	a->setToolTip( QObject::tr("TBR_CH_ENV_MAP") );
	connect( a, SIGNAL( triggered() ), this, SLOT( onEnvironmentMap() ) );
	a->setCheckable( true );
	myActions.insert( BGSet_EnvironmentMapId, a );
}

void DLMUI_BackgroundSet::popup()
{
	QMenu* myToolMenu = new QMenu( 0 );
			
	myToolMenu->addAction( myActions.at( BGSet_ColorId ) );
	myToolMenu->addAction( myActions.at( BGSet_EnvironmentMapId ) );
	
	myActions.at( BGSet_EnvironmentMapId )->setChecked(myAppMainWindow->getActiveWndView()->isEnvironmentMapped());
	
	myAppMainWindow->addContextItemInPopup(myToolMenu);
	
	myToolMenu->exec( QCursor::pos() );
	
	delete myToolMenu;
}	

void DLMUI_BackgroundSet::onColor()
{
	myAppMainWindow->getActiveWndView()->onBackground();
}	

void DLMUI_BackgroundSet::onEnvironmentMap()
{
	myAppMainWindow->getActiveWndView()->onEnvironmentMap();
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
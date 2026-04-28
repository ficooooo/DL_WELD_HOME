#include "dlOcctMdiFrame/ApplicationCommon.h"
#include "dlOcctMdiFrame/DocumentCommon.h"
#include "dlOcctMdiFrame/Transparency.h"
#include "dlOcctMdiFrame/Material.h"
#include "dlOcctMdiFrame/MDIWindow.h"

#include <QSignalMapper>
#include <QApplication>
#include <QColor>
#include <QColorDialog>

#include <Graphic3d_GraphicDriver.hxx>

#define MATERIAL_ACTION(NAME,TIP,MAPID)\
	a = new QAction( QObject::tr(#NAME), this );\
  	a->setToolTip( QObject::tr(#TIP) );\
  	a->setStatusTip( QObject::tr(#TIP) );\
  	sm->setMapping( a,(int)MAPID );\
  	connect( a, SIGNAL( triggered() ), sm, SLOT( map() ) );\
  	myTBMUI_ModelSkin_Materials_Actions.insert( MAPID, a );

#define MATERIAL_ACTIONX(MATER_ID)\
	MATERIAL_ACTION(MNU_##MATER_ID,TBR_##MATER_ID,Graphic3d_NOM_##MATER_ID)

#define ModelSkin_ACTION(ICON_ID,MNU_ID,TIP,MAPID)\
	a = new QAction( QPixmap( dir+QObject::tr(#ICON_ID) ), QObject::tr(#MNU_ID), this );\
	a->setToolTip( QObject::tr(#TIP) );\
	a->setStatusTip( QObject::tr(#TIP) );\
	connect( a, SIGNAL( triggered() ) , this, SLOT( onTBMUI_ModelSkin() ) );\
	myTBMUI_ModelSkin_Actions.insert( MAPID, a );
	
#define ModelSkin_ACTIONX(ID,MAPID)\
	ModelSkin_ACTION(ICON_TOOL_##ID,MNU_TOOL_##ID,TBR_TOOL_##ID,MAPID)


void ApplicationCommonWindow::createTBMUI_ModelSkin()
{
	//---------------------------------------------------------------------------------------------、
	// populate a tool bar with some actions
	//主窗口Shape控制工具栏
	myTBMUI_ModelSkin = addToolBar( tr( "Shape Operations" ) );  
	QString dir = ApplicationCommonWindow::getResourceDir() + QString( "/" );
	QAction* a;
	
	ModelSkin_ACTIONX(WIRE,ToolWireframeId)
	ModelSkin_ACTIONX(SHAD,ToolShadingId)
	ModelSkin_ACTIONX(COLOR,ToolColorId)
	ModelSkin_ACTIONX(MATER,ToolMaterialId)
	ModelSkin_ACTIONX(TRANS,ToolTransparencyId)
	ModelSkin_ACTIONX(DEL,ToolDeleteId)
  //---------------------------------------------------------------------------------------------
  
  	QSignalMapper* sm = new QSignalMapper( this );
  	connect( sm, SIGNAL( mapped( int ) ), this, SLOT( onTBMUI_SetMaterial( int ) ) );
	
	MATERIAL_ACTIONX(BRASS)
	MATERIAL_ACTIONX(BRONZE)	
	MATERIAL_ACTIONX(COPPER) 
	MATERIAL_ACTIONX(GOLD)	
	MATERIAL_ACTIONX(PEWTER)	
	MATERIAL_ACTIONX(PLASTER)  
	MATERIAL_ACTIONX(PLASTIC)	
	MATERIAL_ACTIONX(SILVER)

	for ( int i = 0; i < myTBMUI_ModelSkin_Actions.size(); i++ )
		myTBMUI_ModelSkin->addAction( myTBMUI_ModelSkin_Actions.at( i ) );
	myTBMUI_ModelSkin->hide();
}

void ApplicationCommonWindow::checkTBMUI_ModelSkin_SelectionChanged(MDIWindow* w)
{
	if(w==NULL||w->getDocument()==NULL)
		return;
	
	Handle(AIS_InteractiveContext) context = w->getDocument()->getContext();
	
	bool OneOrMoreInShading = false;
	bool OneOrMoreInWireframe = false;
	
	int numSel = context->NbSelected();
	if ( numSel )
	{
		for ( context->InitSelected(); context->MoreSelected(); context->NextSelected() )
		{
			if ( context->IsDisplayed( context->SelectedInteractive(), 1 ) )
				OneOrMoreInShading = true;
			if ( context->IsDisplayed( context->SelectedInteractive(), 0 ) )
				OneOrMoreInWireframe = true;
		}
		myTBMUI_ModelSkin_Actions.at( ToolWireframeId )->setEnabled( OneOrMoreInShading );
		myTBMUI_ModelSkin_Actions.at( ToolShadingId )->setEnabled( OneOrMoreInWireframe );
		myTBMUI_ModelSkin_Actions.at( ToolColorId )->setEnabled( true );
		myTBMUI_ModelSkin_Actions.at( ToolMaterialId )->setEnabled( true );
		myTBMUI_ModelSkin_Actions.at( ToolTransparencyId )->setEnabled( OneOrMoreInShading );
		myTBMUI_ModelSkin_Actions.at( ToolDeleteId )->setEnabled( true );
	}
	else
	{
		myTBMUI_ModelSkin_Actions.at( ToolWireframeId )->setEnabled( false );
		myTBMUI_ModelSkin_Actions.at( ToolShadingId )->setEnabled( false );
		myTBMUI_ModelSkin_Actions.at( ToolColorId )->setEnabled( false );
		myTBMUI_ModelSkin_Actions.at( ToolMaterialId )->setEnabled( false );
		myTBMUI_ModelSkin_Actions.at( ToolTransparencyId )->setEnabled( false );
		myTBMUI_ModelSkin_Actions.at( ToolDeleteId )->setEnabled( false );
	}
}

void ApplicationCommonWindow::checkTBMUI_ModelSkin(MDIWindow* w)
{
	if(w!=NULL)
	{
		myTBMUI_ModelSkin->show();
	}else
	{
		myTBMUI_ModelSkin->hide();
	}
}

//2026/4/23 16:21:53 右键弹出菜单的统一模式，临时创建执行完即删除
//菜单所需的Action可预先创建
void ApplicationCommonWindow::TBMUI_Popup_ModelSkin()
{
	QMenu* myToolMenu = new QMenu( 0 );
	myToolMenu->addAction( myTBMUI_ModelSkin_Actions.at( ToolWireframeId ) );
	myToolMenu->addAction( myTBMUI_ModelSkin_Actions.at( ToolShadingId ) );
	myToolMenu->addAction( myTBMUI_ModelSkin_Actions.at( ToolColorId ) );
	
	QMenu* myMaterMenu = new QMenu( myToolMenu );
	QList<QAction*>* aMeterActions = &myTBMUI_ModelSkin_Materials_Actions;
	QString dir = ApplicationCommonWindow::getResourceDir() + QString( "/" );
	myMaterMenu = myToolMenu->addMenu( QPixmap( dir+QObject::tr("ICON_TOOL_MATER")), QObject::tr("MNU_MATER") );
	for ( int i = 0; i < aMeterActions->size(); i++ )
		myMaterMenu->addAction( aMeterActions->at( i ) );
	
	myToolMenu->addAction( myTBMUI_ModelSkin_Actions.at( ToolTransparencyId ) );
	myToolMenu->addAction( myTBMUI_ModelSkin_Actions.at( ToolDeleteId ) );
	
	addContextItemInPopup(myToolMenu);
	
	myToolMenu->exec( QCursor::pos() );
	
	delete myToolMenu;
}

void ApplicationCommonWindow::onTBMUI_ModelSkin()
{
	QAction* sentBy = (QAction*) sender();
	DocumentCommon* doc = getActiveDocument();
	
	if( sentBy == myTBMUI_ModelSkin_Actions.at( ToolWireframeId ) )
		doc->onWireframe();
	
	if( sentBy == myTBMUI_ModelSkin_Actions.at( ToolShadingId ) )
		doc->onShading();
	
	if( sentBy == myTBMUI_ModelSkin_Actions.at( ToolColorId ) )
		doc->onColor();
	
	if( sentBy == myTBMUI_ModelSkin_Actions.at( ToolMaterialId ) )
		doc->onMaterial();
	
	if( sentBy == myTBMUI_ModelSkin_Actions.at( ToolTransparencyId ) )
		doc->onTransparency();
	
	if( sentBy == myTBMUI_ModelSkin_Actions.at( ToolDeleteId ) )
		doc->onDelete();
}

void ApplicationCommonWindow::onTBMUI_SetMaterial( int theMaterial )
{
    DocumentCommon* doc = getActiveDocument();
    
    doc->onMaterial( theMaterial );
}

//-------------------------------------------- 文档对象中对可视化对象做操作
void DocumentCommon::onWireframe()
{
    QApplication::setOverrideCursor( Qt::WaitCursor );
    for( myContext->InitSelected(); myContext->MoreSelected(); myContext->NextSelected() )
        myContext->SetDisplayMode( myContext->SelectedInteractive(), 0, false );
        
    myContext->UpdateCurrentViewer();
    getApplication()->onSelectionChanged();
    
    QApplication::restoreOverrideCursor();
}

void DocumentCommon::onShading()
{
    QApplication::setOverrideCursor( Qt::WaitCursor );
    for( myContext->InitSelected(); myContext->MoreSelected(); myContext->NextSelected() )
        myContext->SetDisplayMode( myContext->SelectedInteractive(), 1, false );
    myContext->UpdateCurrentViewer();
    getApplication()->onSelectionChanged();
    QApplication::restoreOverrideCursor();
}

void DocumentCommon::onColor()
{
    QColor aColor ;
    myContext->InitSelected();
    Handle(AIS_InteractiveObject) Current = myContext->SelectedInteractive() ;
    if ( Current->HasColor () )
    {
        Quantity_Color aShapeColor;
        myContext->Color( Current, aShapeColor );
        aColor.setRgb( (Standard_Integer)(aShapeColor.Red() * 255), (Standard_Integer)(aShapeColor.Green() * 255),
                       (Standard_Integer)(aShapeColor.Blue() * 255));
    }
    else
        aColor.setRgb( 255, 255, 255 );

    QColor aRetColor = QColorDialog::getColor( aColor );
    if ( aRetColor.isValid() )
    {
        Quantity_Color color( aRetColor.red() / 255., aRetColor.green() / 255.,
                        aRetColor.blue() / 255., Quantity_TOC_RGB );
        for (; myContext->MoreSelected(); myContext->NextSelected() )
            myContext->SetColor( myContext->SelectedInteractive(), color, Standard_False);
        myContext->UpdateCurrentViewer();
    }
}

void DocumentCommon::onMaterial( int theMaterial )
{
    for ( myContext->InitSelected(); myContext->MoreSelected (); myContext->NextSelected () )
        myContext->SetMaterial( myContext->SelectedInteractive(), (Graphic3d_NameOfMaterial)theMaterial, Standard_False);
    myContext->UpdateCurrentViewer();
}

void DocumentCommon::onMaterial()
{
    DialogMaterial* m = new DialogMaterial();
    connect( m, SIGNAL( sendMaterialChanged( int ) ), this, SLOT( onMaterial( int ) ) );
    m->exec();
}

void DocumentCommon::onTransparency( int theTrans )
{
    for( myContext->InitSelected(); myContext->MoreSelected(); myContext->NextSelected() )
        myContext->SetTransparency (myContext->SelectedInteractive(), ((Standard_Real)theTrans) / 10.0, Standard_False);
    myContext->UpdateCurrentViewer();
}

void DocumentCommon::onTransparency()
{
    DialogTransparency* aDialog = new DialogTransparency();
    connect( aDialog, SIGNAL( sendTransparencyChanged( int ) ), this, SLOT( onTransparency( int ) ) );
    aDialog->exec();
}

void DocumentCommon::onDelete()
{
    myContext->EraseSelected (Standard_False);
    myContext->ClearSelected (Standard_False);
    myContext->UpdateCurrentViewer();
    getApplication()->onSelectionChanged();
}
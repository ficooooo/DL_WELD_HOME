#include "occtutorial\ApplicationTut.h"
#include "occtutorial\DocumentTut.h"
#include "dlOcctMdiFrame/MDIWindow.h"

#include <QApplication>
#include <QFileDialog>
#include <QStatusBar>
#include <QMdiSubWindow>
#include <QThread>

#include "dlOcctMdiFrame/DLMUI_FileWnd.h"
#include "dlOcctMdiFrame/DLMUI_ViewOps.h"
#include "dlOcctMdiFrame/DLMUI_ModelSkin.h"
#include "dlOcctMdiFrame/DLMUI_RayTrace.h"
#include "occtutorial/DLMUI_Bottle.h"
#include "dlOcctMdiFrame/DLMUI_BackgroundSet.h"

ApplicationTut::ApplicationTut()
    : ApplicationCommonWindow( )
{
	//主窗口基础菜单工具栏
	new DLMUI_FileWnd	(this,DLMUI_FileWndId);
	new DLMUI_ViewOps	(this,DLMUI_ViewOpsId);
	new DLMUI_ModelSkin	(this,DLMUI_ModelSkinId);
	new DLMUI_Raytrace	(this,DLMUI_RaytraceId);
	new DLMUI_Bottle	(this,DLMUI_BottleId);
	new DLMUI_BackgroundSet	(this,DLMUI_BackgroundSetId);
	
	DLModuleUIEntry::transMap2List();
	
  	statusBar()->showMessage( QObject::tr("INF_READY"), 5000 );
	resize( 1000, 700 );
}

ApplicationTut::~ApplicationTut()
{
}

DocumentCommon* ApplicationTut::createNewDocument()
{
  	return new DocumentTut( ++myNbDocuments, this );
}

void ApplicationTut::addContextItemInPopup(QMenu * menu)
{
	DLModuleUIEntry* entry = DLModuleUIEntry::getEntry(DLMUI_ViewOpsId);
	if(entry==NULL)
		return;
	
	menu->addSeparator();
	menu->addAction( entry->getAction( DLMUI_ViewOps::ViewFitAllId ) );
}	

//派生窗口类可重载此方法，实现更复杂的上下文右键菜单
void ApplicationTut::popup( const int x, const int y )
{
	QMdiSubWindow* w = getActiveWnd();
	DocumentCommon*  doc = getActiveDocument();
	
	if ( doc->getContext()->NbSelected() )
	{
		DLModuleUIEntry* entry = DLModuleUIEntry::getEntry(DLMUI_ModelSkinId);
		if(entry!=NULL)
			entry->popup();
	}
	else
	{
		DLModuleUIEntry* entry = DLModuleUIEntry::getEntry(DLMUI_BackgroundSetId);
		if(entry!=NULL)
			entry->popup();
	}
	
	if ( w )
		w->setFocus();
}

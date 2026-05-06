#include "dlOcctMdiFrame/DLModuleUIEntry.h"
#include "dlOcctMdiFrame/MDIWindow.h"
#include "dlOcctMdiFrame/View.h"
#include "dlOcctMdiFrame/ApplicationCommon.h"

QList<DLModuleUIEntry*>  DLModuleUIEntry::DLModuleUIEntryList;
QMap<int,DLModuleUIEntry*>  DLModuleUIEntry::DLModuleUIEntryMap;
	
void DLModuleUIEntry::clearGlobleList()
{
	DLModuleUIEntryMap.clear();
	int totalCount = DLModuleUIEntryList.size();
	for (int i = 0; i < totalCount ; ++i)
	{
		DLModuleUIEntry* obj = DLModuleUIEntryList.takeAt(0);
		delete obj;
	}	
}	

void DLModuleUIEntry::checkWindowActionAll(enum DOCUMENT_VIEW_ACTION eva,MDIWindow* w)
{
	for (int i = 0; i < DLModuleUIEntryList.size(); ++i)
	{
		DLModuleUIEntryList.at(i)->checkWindowAction(eva,w);
	}	
}

void DLModuleUIEntry::switchAll(bool show)
{
	for (int i = 0; i < DLModuleUIEntryList.size(); ++i)
	{
		DLModuleUIEntryList.at(i)->_switch(show);
	}
}	
	
void DLModuleUIEntry::_switch(bool show)	
{
	if(myToolBar == NULL)
		return;
		
	if ( show )
		myToolBar->show();
	else
		myToolBar->hide();
}	

void DLModuleUIEntry::transMap2List()
{
	DLModuleUIEntryList = DLModuleUIEntryMap.values();
}	
	
DLModuleUIEntry::DLModuleUIEntry(ApplicationCommonWindow* mainWnd,int id)
{
	myToolBar = NULL;
	myMenu = NULL;
	myAppMainWindow = mainWnd;
	myMdiArea = mainWnd->getMdiArea();
	myModuleID = id;
	
	DLModuleUIEntryMap.insert(id,this);
	std::cout<<DLModuleUIEntryMap.size()<<"\n";
    std::cout<<DLModuleUIEntryMap.value(id)<<"\n";		
	//DLModuleUIEntryList.insert(id,this);
}

DLModuleUIEntry::~DLModuleUIEntry()
{	
	myAppMainWindow = NULL;
	myMdiArea = NULL;
}

void DLModuleUIEntry::checkWindowAction(enum DOCUMENT_VIEW_ACTION eva,MDIWindow* w)
{
	if(eva==NO_VIEW)
	{	
		//初始状态由各Entry自行设定
		//this->checkNoView(NULL);
	}	
	else if(eva==NEW_DOCUMENT_VIEW)
	{
		this->checkNewView(w);
	}else if(eva==COPY_DOCUMENT_VIEW)
	{
		//checkViewActivated
	}	
	else if(eva==CLOSE_VIEW)		
	{
		this->checkNoView(NULL);
	}else if(eva==SWITCH_VIEW)
	{
		this->checkViewActivated(w);
	}else if(eva==VIEW_SELECTION_CHANGED)
		this->checkSelectionChanged(w);
}

//派生窗口类可重载此方法，实现更复杂的上下文右键菜单
void ApplicationCommonWindow::popup( const int /*x*/, const int /*y*/ )
{
	QMdiSubWindow* w = getActiveWnd();
	
	DLModuleUIEntry* entry = DLModuleUIEntry::getEntry(DLMUI_BackgroundSetId);

	if(entry!=NULL)
		entry->popup();
	
	if ( w )
		w->setFocus();
}

//View 中右键弹出菜单及功能实现
void View::Popup( const int x, const int y )
{
	ApplicationCommonWindow* stApp = ApplicationCommonWindow::getApplication();
	stApp->popup(x,y);
}
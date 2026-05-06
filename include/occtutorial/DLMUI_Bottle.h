#ifndef DLMUI_BOTTLE_H
#define DLMUI_BOTTLE_H

#include "dlOcctMdiFrame/DLModuleUIEntry.h"

class DLMUI_Bottle: public DLModuleUIEntry
{
	Q_OBJECT
	
	//最后两个是菜单顶级Menu对应的QAction , FilePreferencesId 未使用，导致出错
	enum { MakeBottleId, RotateBottleId };	
public:                      
  	DLMUI_Bottle(ApplicationCommonWindow* mainWnd,int id);
  	~DLMUI_Bottle();
	
public:
	virtual void                   	create();
	virtual void 					checkNoView(MDIWindow*);
	virtual void 					checkNewView(MDIWindow*);
	virtual void 					checkViewActivated(MDIWindow*);
public slots:
	void							makeBottle();
	void							rotBottle();
};
#endif
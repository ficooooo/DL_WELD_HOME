#ifndef DLMUI_MODELSKIN_H
#define DLMUI_MODELSKIN_H

#include "DLModuleUIEntry.h"

class DCL_DL_OCCTMDI_FRAME__CLASS DLMUI_ModelSkin: public DLModuleUIEntry
{
	Q_OBJECT
	
	enum { ToolWireframeId, ToolShadingId, ToolColorId, ToolMaterialId, ToolTransparencyId, ToolDeleteId };
public:                      
  	DLMUI_ModelSkin(ApplicationCommonWindow* mainWnd,int id);
  	~DLMUI_ModelSkin();
private:
	QList<QAction*>                 myActions_Materials;	
public:
	virtual void                   	create();
	
	virtual void 					checkNoView(MDIWindow*);
	virtual void 					checkNewView(MDIWindow*);	
	virtual void 					checkViewActivated(MDIWindow*);
	virtual void 					checkSelectionChanged(MDIWindow* w);
	
	virtual void 					popup();		
private:
	void 							init();
public slots:
	//显示控制：颜色材质...
  	void                            onModelSkin();	
  	void                            onSetMaterial( int theMaterial );
};
#endif
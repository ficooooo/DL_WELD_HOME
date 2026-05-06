#ifndef DLMUI_RAYTRACE_H
#define DLMUI_RAYTRACE_H

#include "DLModuleUIEntry.h"

class DCL_DL_OCCTMDI_FRAME__CLASS DLMUI_Raytrace: public DLModuleUIEntry
{
	Q_OBJECT
public:	
	enum RaytraceAction { ToolRaytracingId, ToolShadowsId, ToolReflectionsId, ToolAntialiasingId };
public:                      
  	DLMUI_Raytrace(ApplicationCommonWindow* mainWnd,int id);
  	~DLMUI_Raytrace();
	
public:
	virtual void                   	create();
	virtual void 					checkNoView(MDIWindow*);
	virtual void 					checkNewView(MDIWindow*);	
	virtual void 					checkViewActivated(MDIWindow*);
private:
	void							init();
public slots:
    void                          	raytraceSet();
};
#endif
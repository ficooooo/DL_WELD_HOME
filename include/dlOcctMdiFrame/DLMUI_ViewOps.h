#ifndef DLMUI_VIEWOPS_H
#define DLMUI_VIEWOPS_H

#include "DLModuleUIEntry.h"

class DCL_DL_OCCTMDI_FRAME__CLASS DLMUI_ViewOps: public DLModuleUIEntry
{
	Q_OBJECT
public:	
	enum ViewAction { ViewFitAllId, 
					  ViewFitAreaId, ViewZoomId, ViewPanId, /*ViewGlobalPanId,*/ ViewRotationId,  /*对应5个配套指针的动作过程*/
                      ViewFrontId, ViewBackId, ViewTopId, ViewBottomId, ViewLeftId, ViewRightId,
                      ViewAxoId, ViewResetId, ViewHlrOffId, ViewHlrOnId };
public:                      
  	DLMUI_ViewOps(ApplicationCommonWindow* mainWnd,int id);
  	~DLMUI_ViewOps();
	
public:
	virtual void                   	create();
	virtual void 					checkNoView(MDIWindow*);
	virtual void 					checkNewView(MDIWindow*);	
	virtual void 					checkViewActivated(MDIWindow*);
private:
	View*							getActiveWndView();
	void							init();
public slots:
    void                          	fitAll();
    void                          	fitArea();
    void                          	zoom();
    void                          	pan();
    //void                          globalPan();
    void                          	front();
    void                          	back();
    void                          	top();
    void                          	bottom();
    void                          	left();
    void                          	right();
    void                          	axo();
    void                          	rotation();
    void                          	reset();
    void                          	hlrOn();
    void                          	hlrOff();
};
#endif
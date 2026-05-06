#ifndef DLMUI_BACKGROUNDSETX_H
#define DLMUI_BACKGROUNDSETX_H

#include "DLModuleUIEntry.h"

class DCL_DL_OCCTMDI_FRAME__CLASS DLMUI_BackgroundSet: public DLModuleUIEntry
{
	Q_OBJECT
public:	
	enum BackgroundSet {BGSet_ColorId,BGSet_EnvironmentMapId };
public:                      
  	DLMUI_BackgroundSet(ApplicationCommonWindow* mainWnd,int id);
  	~DLMUI_BackgroundSet();
	
public:
	virtual void                   	create();
private:
	void							init();
public slots:
	void                          	onColor();
  	void                          	onEnvironmentMap();
  	
public:
	void 							popup();  	
};
#endif
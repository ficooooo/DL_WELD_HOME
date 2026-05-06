#ifndef DOCUMENTCOMMON_H
#define DOCUMENTCOMMON_H

#include "dllDeclaration.h"

#include <QObject>
#include <QMap>

#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>
#include <AIS_Shape.hxx>

class ApplicationCommonWindow;
class MDIWindow;


class DCL_DL_OCCTMDI_FRAME__CLASS DocumentCommon : public QObject
{
	Q_OBJECT
public:	
	//
	enum SCENE_COMPNENTS_ID_LIST	{ DL_SCENE_VIEWCUBE=0
										,DL_SCENE_FLOOR=1
										,DL_SCENE_FLOOR_THIN=2
										,DL_VIEWOBJECT_COUNT };		//在派生类中继续定义所用到的对象
public:
	DocumentCommon( const int, ApplicationCommonWindow* );
	~DocumentCommon();

	ApplicationCommonWindow*       	getApplication()	{	return myApp;		}
	Handle(AIS_InteractiveContext) 	getContext()		{  	return myContext;	}
	
  	void                           	removeViews();
	void                           	fitAll();

protected:
	int 							myDLModuleId;
	QMap<int,Handle(AIS_InteractiveObject)>	SCENE_COMPNENTS_LIST;
public:	
	virtual int						getNbSceneComponents()=0;
	Handle(AIS_InteractiveObject)	getAISIO(int id)	  const {	return SCENE_COMPNENTS_LIST.value(id);	}
	
signals:
	void                           	sendCloseDocument( DocumentCommon* );

public slots:
	virtual void                   	onCloseMDIWindow( MDIWindow* );
	virtual MDIWindow*            	onCreatMDIWindow();
public:
	virtual int						getFloorSize()				{	return 4;	}//4mX4m
	virtual int						getFloorGridSizeFactor()	{	return -1;	}//2^-1 *1000 500mm  
	virtual void					initScene();	
	virtual void					clearScene();
public slots:	
	virtual void                   	onMaterial();
  	virtual void                   	onMaterial( int );
	virtual void                   	onDelete();

	void                           	onWireframe();
	void                           	onShading();
	void                           	onColor();
	void                           	onTransparency();
	void                           	onTransparency( int );

private:
  	Handle(V3d_Viewer)             	Viewer (const Standard_ExtString theName,
                                         const Standard_CString theDomain,
                                         const Standard_Real theViewSize,
                                         const V3d_TypeOfOrientation theViewProj,
                                         const Standard_Boolean theComputedMode,
                                         const Standard_Boolean theDefaultComputedMode );

protected:
	//所属应用
	ApplicationCommonWindow*       	myApp;
	//本次文档计数器
	int                            	myIndex;
	//新建文档+WIndow>New View 创建的MDIWindow
	QList<MDIWindow*>              	myMDIWindows;
	int                            	myNbMDIWindows;
	//一个文档对应一个V3d_Viewer和一个AIS_InteractiveContext
	//属于同一文档的不同Window显示的数据相同，但可各自显示
	Handle(V3d_Viewer)             	myViewer;
	Handle(AIS_InteractiveContext) 	myContext;
	
friend class ApplicationCommonWindow;
};

#endif



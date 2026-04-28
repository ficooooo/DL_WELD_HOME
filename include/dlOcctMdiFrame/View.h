#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QAction>
#include <QList>

#include <AIS_InteractiveContext.hxx>
#include <AIS_ViewController.hxx>
#include <V3d_View.hxx>

class TopoDS_Shape;
class QRubberBand;

class View: public QWidget, protected AIS_ViewController
{
    Q_OBJECT
public:
    enum CurrentAction3d { CurAction3d_Nothing, 		CurAction3d_DynamicZooming,
                           CurAction3d_WindowZooming, 	CurAction3d_DynamicPanning,
                           CurAction3d_GlobalPanning, 	CurAction3d_DynamicRotation };
public:
	static QCursor* defCursor;
	static QCursor* handCursor;
	static QCursor* panCursor;
	//static QCursor* globPanCursor;
	static QCursor* zoomCursor;
	static QCursor* rotCursor;
private:
    void                          		initCursors();

public:    
    View( Handle(AIS_InteractiveContext) theContext, QWidget* parent );
    ~View();

    virtual void                  		init();
    bool                          		dump( Standard_CString theFile );
    virtual QPaintEngine*         		paintEngine() const;

//----------------------------------------------------------------------------------------------------------        
//Important：2026/4/23 16:13:55
//拾取操作处理函数映射到ApplicationCommonWindow的 OnSelectionChanged
//从AIS_InteractiveContext获取当前的拾取对象
protected: 
    //! Handle selection changed event.
    void 								OnSelectionChanged (const Handle(AIS_InteractiveContext)& theCtx,
                             								const Handle(V3d_View)& theView) Standard_OVERRIDE;
signals:
    void                          		selectionChanged();	
//----------------------------------------------------------------------------------------------------------    
private:
    Handle(V3d_View)                	myView;
    Handle(AIS_InteractiveContext)  	myContext;
public:
    Handle(V3d_View)&                   getView3d()				{	return myView;			}
    Handle(AIS_InteractiveContext)&     getContext()			{  	return myContext;		}
//----------------------------------------------------------------------------------------------------------    
private:
    //鼠标操控3d场景时相关变量
    AIS_MouseGestureMap             	myDefaultGestures;
    Graphic3d_Vec2i                 	myClickPos;
    CurrentAction3d                 	myCurrentMode;
    Standard_Real                   	myCurZoom;
    bool								myIsShadingMode;
    
public:	
	CurrentAction3d                     getCurrentMode()		{  	return myCurrentMode;	}
    void 								setCurrentAction (CurrentAction3d theAction);
    void								fitAll();
    void								switchHLR(bool on);		//Hidden Line Removal
    bool                          		isShadingMode()			{	return myIsShadingMode;			}  //和HLR on/off反 off=shading
	
protected:
    virtual void                  		paintEvent( QPaintEvent* );
    virtual void                  		resizeEvent( QResizeEvent* );
    virtual void                  		mousePressEvent( QMouseEvent* );
    virtual void                  		mouseReleaseEvent(QMouseEvent* );
    virtual void                  		mouseMoveEvent( QMouseEvent* );
    virtual void                  		wheelEvent (QWheelEvent* );    
//---------------------------------------------------------------------------
//当前View光线追踪相关的控制    
private:
    bool                            	myIsRaytracing;
    bool                            	myIsShadowsEnabled;
    bool                            	myIsReflectionsEnabled;
    bool                            	myIsAntialiasingEnabled;

public:
	void                          		EnableRaytracing();
    void                          		DisableRaytracing();
    void                          		SetRaytracedShadows (bool theState);
    void                          		SetRaytracedReflections (bool theState);
    void                          		SetRaytracedAntialiasing (bool theState);

    bool                          		IsRaytracingMode() const 		{ return myIsRaytracing; 			}
    bool                          		IsShadowsEnabled() const 		{ return myIsShadowsEnabled; 		}
    bool                          		IsReflectionsEnabled() const 	{ return myIsReflectionsEnabled; 	}
    bool                          		IsAntialiasingEnabled() const 	{ return myIsAntialiasingEnabled; 	}
//----------------------------------------------------------------------------------------------------------    
private:	
	bool								myIsEnvironmentMapped;
public:
	bool								isEnvironmentMapped()	{	return myIsEnvironmentMapped;	}
public:	//View中右键菜单弹出及背景色设置等两个方法
    void                                Popup( const int x, const int y );
    void                          		onBackground();
    void                          		onEnvironmentMap();
//---------------------------------------------------------------------------    
};

#endif



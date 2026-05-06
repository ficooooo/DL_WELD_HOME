#ifndef DL_AIS_SCENE_FLOOR_H
#define DL_AIS_SCENE_FLOOR_H

#include <AIS_InteractiveObject.hxx>

class DL_AIS_Scene_Floor : public AIS_InteractiveObject
{
  	DEFINE_STANDARD_RTTI_INLINE(DL_AIS_Scene_Floor, AIS_InteractiveObject)
public:
  	DL_AIS_Scene_Floor(int edgeSize_meter
									 , int grid_size_power
									 , Quantity_NameOfColor gridLineColor
									 , int gridLineSize) ;
private:
	int	n_EdgeSize_Meter;	  
	int n_GridSize_MMeter;
	int n_SideGridCount;
	Quantity_NameOfColor QNC_GridLineColor;
	int n_GridLineSize;
public:
  	virtual void Compute (const Handle(PrsMgr_PresentationManager)& thePrsMgr,
                        const Handle(Prs3d_Presentation)& thePrs,
                        const Standard_Integer theMode) override;
  	virtual void ComputeSelection(const Handle(SelectMgr_Selection)&,
                                  const Standard_Integer) override {}
  	virtual bool AcceptDisplayMode (const Standard_Integer theMode)  
  	{ 
  		return theMode==0; 
  	}
};

#endif

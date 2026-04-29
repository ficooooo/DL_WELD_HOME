#include "dlOcctMdiFrame/DL_AIS_Scene_Floor.h"

#include <PrsMgr_PresentationManager.hxx>
#include <Prs3d_Presentation.hxx>
#include <TopoDS_Shape.hxx>
#include <Graphic3d_ArrayOfSegments.hxx>
#include <gp_Pnt.hxx>
#include <Prs3d_LineAspect.hxx>

#include <math.h>

DL_AIS_Scene_Floor::DL_AIS_Scene_Floor(int edgeSize_meter
									 , int grid_size_power
									 , Quantity_NameOfColor gridLineColor
									 , int gridLineSize) 
{	
	n_EdgeSize_Meter = edgeSize_meter;
	n_GridSize_MMeter = (int)(1000*pow(2,grid_size_power));
	QNC_GridLineColor = gridLineColor;
	n_GridLineSize  = gridLineSize;
}

void DL_AIS_Scene_Floor::Compute (const Handle(PrsMgr_PresentationManager)& /*thePrsMgr*/,
                           const Handle(Prs3d_Presentation)& thePrs,
                           const Standard_Integer /*theMode*/)
{
	myDrawer->SetWireAspect (new Prs3d_LineAspect (QNC_GridLineColor, Aspect_TOL_SOLID, n_GridLineSize));
	
  	int x = n_EdgeSize_Meter*500;
  	int y = n_EdgeSize_Meter*500;
	int nEdgeGridCount = n_EdgeSize_Meter*1000/n_GridSize_MMeter;	
	
	//Graphic3d_ArrayOfSegments (Standard_Integer theMaxVertexs, Standard_Integer theMaxEdges, Graphic3d_ArrayFlags theArrayFlags)
	Handle(Graphic3d_ArrayOfSegments) aSegs = new Graphic3d_ArrayOfSegments ((nEdgeGridCount+1)*2*2, (nEdgeGridCount+1)*2 * 2, Graphic3d_ArrayFlags_None);
	for(int iy = 0;iy<=nEdgeGridCount;iy++)
	{
		aSegs->AddVertex (gp_Pnt (-x, -y+n_GridSize_MMeter*iy, 0.0));
		aSegs->AddVertex (gp_Pnt ( x, -y+n_GridSize_MMeter*iy, 0.0));
	}

	for(int ix = 0;ix<=nEdgeGridCount;ix++)
	{
		aSegs->AddVertex (gp_Pnt (-x+n_GridSize_MMeter*ix, -y, 0.0));
		aSegs->AddVertex (gp_Pnt (-x+n_GridSize_MMeter*ix, y, 0.0));
	}
	
	for(int iy = 0;iy<=nEdgeGridCount;iy++)
	{
		aSegs->AddEdges (iy*2+1, iy*2+2);
	}
	
	for(int ix = 0;ix<=nEdgeGridCount;ix++)
	{
		aSegs->AddEdges ((nEdgeGridCount+1)*2+ix*2+1, (nEdgeGridCount+1)*2+ix*2+2);	
	}

	Handle(Graphic3d_Group) aGroupSegs = thePrs->NewGroup();
	aGroupSegs->SetGroupPrimitivesAspect (myDrawer->WireAspect()->Aspect());
	aGroupSegs->AddPrimitiveArray (aSegs);
}


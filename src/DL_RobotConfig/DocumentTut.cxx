#include "DL_RobotConfig/DocumentTut.h"
#include "DL_RobotConfig/DL_RobotContext.h"
#include "dlModelContext/DL_Modeler.h"

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

#include <AIS_Shape.hxx>
#include <TopoDS_Shape.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

DocumentTut::DocumentTut( const int theIndex, ApplicationCommonWindow* app )
: DocumentCommon( theIndex, app ),
  myLastOpenResult(0)
{
}

DocumentTut::~DocumentTut()
{
}

DL_RobotContext* DocumentTut::getRobot()
{
	Handle(AIS_InteractiveContext) aContext = getContext();
	if (!myRobotContext)
	{
		myRobotContext.reset(new DL_RobotContext(aContext));
	}
	else
	{
		myRobotContext->setContext(aContext);
	}
	return myRobotContext.get();
}

int DocumentTut::openRobot(QWidget* theParent)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	const int aLoadResult = getRobot()->loadRobotDynamic(theParent);
	if (0 != aLoadResult)
	{
		myLastOpenResult = aLoadResult;
		fitAll();
	}
	QApplication::restoreOverrideCursor();
	return aLoadResult;
}

void DocumentTut::makeBottle()
{
	Handle(AIS_InteractiveContext) aCtx = getContext();

	for (V3d_ListOfView::Iterator aViewIter (aCtx->CurrentViewer()->ActiveViews()); aViewIter.More(); aViewIter.Next())
	{
		const Handle(V3d_View)& aView = aViewIter.Value();
		Graphic3d_RenderingParams& aParams = aView->ChangeRenderingParams();
		aParams.RenderResolutionScale = 2.0f;
	}
	
	const Handle(Prs3d_Drawer)& aDefDrawer = aCtx->DefaultDrawer();
	aDefDrawer->SetIsoOnTriangulation (true);
	
	QApplication::setOverrideCursor( Qt::WaitCursor );
	TopoDS_Shape aBottle=MakeBottle(50,70,30);
	Handle(AIS_Shape) AISBottle=new AIS_Shape(aBottle);
	
	getContext()->SetMaterial (AISBottle, Graphic3d_NameOfMaterial_Gold, Standard_False);
	getContext()->SetDisplayMode(AISBottle, 1, Standard_False);
	getContext()->Display(AISBottle, Standard_False);	

	fitAll();
	QApplication::restoreOverrideCursor();
}

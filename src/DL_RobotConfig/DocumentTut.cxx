#include "DL_RobotConfig/DocumentTut.h"
#include "DL_RobotConfig/DL_RobotContext.h"
#include "dlModelContext/DL_Modeler.h"

#include <QApplication>

#include <AIS_InteractiveObject.hxx>
#include <AIS_Shape.hxx>
#include <TopoDS_Shape.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

DocumentTut::DocumentTut(const int theIndex, ApplicationCommonWindow* app)
: DocumentCommon(theIndex, app),
  myLastOpenResult(0),
  myCurrentJointIndex(-1),
  mySceneUpperBound(DL_DYNAMIC_PART_BEGIN),
  myNextDynamicSceneId(DL_DYNAMIC_PART_BEGIN)
{
}

DocumentTut::~DocumentTut()
{
}

int DocumentTut::getNbSceneComponents()
{
	return mySceneUpperBound;
}

void DocumentTut::refreshSceneBounds()
{
	int anUpperBound = DL_DYNAMIC_PART_BEGIN;
	QMap<int, Handle(AIS_InteractiveObject)>::const_iterator anIt = SCENE_COMPNENTS_LIST.constEnd();
	while (anIt != SCENE_COMPNENTS_LIST.constBegin())
	{
		--anIt;
		if (anIt.key() >= DL_VIEWOBJECT_COUNT && !anIt.value().IsNull())
		{
			anUpperBound = anIt.key() + 1;
			break;
		}
	}

	mySceneUpperBound = anUpperBound;
	if (myNextDynamicSceneId > mySceneUpperBound)
		myNextDynamicSceneId = mySceneUpperBound < DL_DYNAMIC_PART_BEGIN ? DL_DYNAMIC_PART_BEGIN : mySceneUpperBound;
	if (myNextDynamicSceneId < DL_DYNAMIC_PART_BEGIN)
		myNextDynamicSceneId = DL_DYNAMIC_PART_BEGIN;
}

void DocumentTut::registerSceneObject(const int theId, const Handle(AIS_InteractiveObject)& theObject)
{
	if (theId < DL_VIEWOBJECT_COUNT)
		return;

	Handle(AIS_InteractiveObject) anOldObject = SCENE_COMPNENTS_LIST.value(theId);
	if (!anOldObject.IsNull() && anOldObject != theObject && !getContext().IsNull())
		getContext()->Remove(anOldObject, Standard_False);

	if (theObject.IsNull())
		SCENE_COMPNENTS_LIST.remove(theId);
	else
		SCENE_COMPNENTS_LIST.insert(theId, theObject);

	if (mySceneUpperBound < theId + 1)
		mySceneUpperBound = theId + 1;

	if (theId >= DL_DYNAMIC_PART_BEGIN && myNextDynamicSceneId <= theId)
		myNextDynamicSceneId = theId + 1;
}

int DocumentTut::registerDynamicSceneObject(const Handle(AIS_InteractiveObject)& theObject)
{
	const int anId = myNextDynamicSceneId++;
	registerSceneObject(anId, theObject);
	if (mySceneUpperBound < myNextDynamicSceneId)
		mySceneUpperBound = myNextDynamicSceneId;
	return anId;
}

void DocumentTut::removeSceneObject(const int theId)
{
	Handle(AIS_InteractiveObject) anObject = SCENE_COMPNENTS_LIST.take(theId);
	if (!anObject.IsNull() && !getContext().IsNull())
		getContext()->Remove(anObject, Standard_False);

	if (theId + 1 >= mySceneUpperBound)
		refreshSceneBounds();
}

void DocumentTut::clearDynamicSceneObjects()
{
	const int anUpperBound = mySceneUpperBound;
	for (int anId = DL_DYNAMIC_PART_BEGIN; anId < anUpperBound; ++anId)
		removeSceneObject(anId);

	myNextDynamicSceneId = DL_DYNAMIC_PART_BEGIN;
	refreshSceneBounds();
}

void DocumentTut::clearScene()
{
	if (myRobotContext)
		myRobotContext->clearSceneState();

	const int anUpperBound = mySceneUpperBound;
	for (int anId = DL_VIEWOBJECT_COUNT; anId < anUpperBound; ++anId)
		removeSceneObject(anId);

	myLastOpenResult = 0;
	myCurrentJointIndex = -1;
	mySceneUpperBound = DL_DYNAMIC_PART_BEGIN;
	myNextDynamicSceneId = DL_DYNAMIC_PART_BEGIN;

	if (!getContext().IsNull())
	{
		getContext()->ClearSelected(Standard_False);
		getContext()->UpdateCurrentViewer();
	}

	fitAll();
}

DL_RobotContext* DocumentTut::getRobot()
{
	Handle(AIS_InteractiveContext) aContext = getContext();
	if (!myRobotContext)
	{
		myRobotContext.reset(new DL_RobotContext(this, aContext));
	}
	else
	{
		myRobotContext->setDocument(this);
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

	for (V3d_ListOfView::Iterator aViewIter(aCtx->CurrentViewer()->ActiveViews()); aViewIter.More(); aViewIter.Next())
	{
		const Handle(V3d_View)& aView = aViewIter.Value();
		Graphic3d_RenderingParams& aParams = aView->ChangeRenderingParams();
		aParams.RenderResolutionScale = 2.0f;
	}

	const Handle(Prs3d_Drawer)& aDefDrawer = aCtx->DefaultDrawer();
	aDefDrawer->SetIsoOnTriangulation(true);

	QApplication::setOverrideCursor(Qt::WaitCursor);
	TopoDS_Shape aBottle = MakeBottle(50, 70, 30);
	Handle(AIS_Shape) AISBottle = new AIS_Shape(aBottle);

	registerSceneObject(DL_BOTTLE, AISBottle);

	getContext()->SetMaterial(AISBottle, Graphic3d_NameOfMaterial_Gold, Standard_False);
	getContext()->SetDisplayMode(AISBottle, 1, Standard_False);
	getContext()->Display(AISBottle, Standard_False);

	fitAll();
	QApplication::restoreOverrideCursor();
}

#ifndef DOCUMENTTUT_H
#define DOCUMENTTUT_H

#include "dlOcctMdiFrame\DocumentCommon.h"

#include <memory>

class AIS_InteractiveObject;
class DL_RobotContext;
class QWidget;

class DocumentTut : public DocumentCommon
{
	Q_OBJECT

public:
	enum SCENE_COMPNENTS_ID_LISTX
	{
		DL_BOTTLE = DL_VIEWOBJECT_COUNT,
		DL_STEP_PREVIEW,
		DL_ROD_0,
		DL_ROD_1,
		DL_ROD_2,
		DL_ROD_3,
		DL_ROD_4,
		DL_ROD_5,
		DL_ROD_6,
		DL_TOOL,
		DL_END_TRIHEDRON,
		DL_TRACE_LINE,
		DL_SPLIT_WHOLE,
		DL_DYNAMIC_PART_BEGIN
	};

public:
	DocumentTut(const int, ApplicationCommonWindow*);
	~DocumentTut();

	virtual int  getNbSceneComponents() override;
	virtual void clearScene() override;

	void               makeBottle();
	int                openRobot(QWidget* theParent);
	DL_RobotContext*   getRobot();
	int                lastOpenResult() const { return myLastOpenResult; }
	int                currentJointIndex() const { return myCurrentJointIndex; }
	void               setCurrentJointIndex(const int theIndex) { myCurrentJointIndex = theIndex; }

	void registerSceneObject(const int theId, const Handle(AIS_InteractiveObject)& theObject);
	int  registerDynamicSceneObject(const Handle(AIS_InteractiveObject)& theObject);
	void removeSceneObject(const int theId);
	void clearDynamicSceneObjects();

private:
	void refreshSceneBounds();

private:
	std::unique_ptr<DL_RobotContext> myRobotContext;
	int                              myLastOpenResult;
	int                              myCurrentJointIndex;
	int                              mySceneUpperBound;
	int                              myNextDynamicSceneId;
};

#endif

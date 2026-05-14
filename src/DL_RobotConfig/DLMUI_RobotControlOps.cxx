#include "DL_RobotConfig\DLMUI_RobotControlOps.h"

#include "DL_RobotConfig\DL_RobotCalcDialog.h"
#include "DL_RobotConfig\DL_RobotContext.h"
#include "DL_RobotConfig\DocumentTut.h"

#include "dlOcctMdiFrame\MDIWindow.h"

#include <QAction>
#include <QPixmap>

DLMUI_RobotControlOps::DLMUI_RobotControlOps(ApplicationCommonWindow* mainWnd, int id)
	: DLMUI_RobotModuleBase(mainWnd, id)
{
	create();
}

DLMUI_RobotControlOps::~DLMUI_RobotControlOps()
{
}

void DLMUI_RobotControlOps::create()
{
	QString aDir = myAppMainWindow->getResourceDir() + QString("/");
	QAction* anAction = NULL;

	myToolBar = myAppMainWindow->addToolBar(QObject::tr("Robot Control Operations"));

#define ROBOT_CONTROL_ACTION(MAPID, ICON_ID, TEXT_ID, TIP_ID, SHORTCUT_TEXT, SLOT_NAME) \
	anAction = new QAction(QPixmap(aDir + QObject::tr(ICON_ID)), QObject::tr(TEXT_ID), this); \
	anAction->setToolTip(QObject::tr(TEXT_ID)); \
	anAction->setStatusTip(QObject::tr(TIP_ID)); \
	anAction->setShortcut(QObject::tr(SHORTCUT_TEXT)); \
	connect(anAction, SIGNAL(triggered()), this, SLOT(SLOT_NAME())); \
	myActions.insert(MAPID, anAction); \
	myToolBar->addAction(anAction);

	ROBOT_CONTROL_ACTION(JointSelectId, "ICON_JOINT_SELECT", "TBR_JOINT_SELECT", "INF_JOINT_SELECT", "CTRL+S", onJointSelect)
	ROBOT_CONTROL_ACTION(JointForwardId, "ICON_JOINT_FWORD", "TBR_JOINT_FWORD", "INF_JOINT_FWORD", "CTRL+F", onJointForward)
	ROBOT_CONTROL_ACTION(JointBackwardId, "ICON_JOINT_BWORD", "TBR_JOINT_BWORD", "INF_JOINT_BWORD", "CTRL+B", onJointBackward)
	ROBOT_CONTROL_ACTION(RobotResetId, "ICON_ROBOT_RESET", "TBR_ROBOT_RESET", "INF_ROBOT_RESET", "CTRL+R", onRobotReset)
	ROBOT_CONTROL_ACTION(RobotCalcId, "ICON_ROBOT_CALC", "TBR_ROBOT_CALC", "INF_ROBOT_CALC", "CTRL+C", onRobotCalc)

#undef ROBOT_CONTROL_ACTION

	myToolBar->hide();
}

void DLMUI_RobotControlOps::checkNoView(MDIWindow*)
{
	syncDocumentUi(NULL);
}

void DLMUI_RobotControlOps::checkNewView(MDIWindow* theWindow)
{
	syncDocumentUi(theWindow == NULL ? NULL : qobject_cast<DocumentTut*>(theWindow->getDocument()));
}

void DLMUI_RobotControlOps::checkViewActivated(MDIWindow* theWindow)
{
	syncDocumentUi(theWindow == NULL ? NULL : qobject_cast<DocumentTut*>(theWindow->getDocument()));
}

void DLMUI_RobotControlOps::syncDocumentUi(DocumentTut* theDocument)
{
	if (myToolBar == NULL)
		return;

	const bool isRobotLoaded = (stateForDocument(theDocument) == RobotUi_RobotLoaded);
	myToolBar->setVisible(isRobotLoaded);
	myActions.at(JointSelectId)->setEnabled(isRobotLoaded);
	myActions.at(JointForwardId)->setEnabled(isRobotLoaded);
	myActions.at(JointBackwardId)->setEnabled(isRobotLoaded);
	myActions.at(RobotResetId)->setEnabled(isRobotLoaded);
	myActions.at(RobotCalcId)->setEnabled(isRobotLoaded);

	if (isRobotLoaded)
		showCurrentJointStatus(theDocument);
}

void DLMUI_RobotControlOps::onJointSelect()
{
	DocumentTut* aDocument = activeRobotDocument();
	if (aDocument == NULL || aDocument->lastOpenResult() <= 0)
		return;

	int aJointIndex = aDocument->currentJointIndex();
	if (aJointIndex < -1 || aJointIndex >= aDocument->lastOpenResult())
		aJointIndex = -1;

	++aJointIndex;
	if (aJointIndex == aDocument->lastOpenResult())
		aJointIndex = 0;

	aDocument->setCurrentJointIndex(aJointIndex);
	aDocument->getRobot()->setActiveJoint(aJointIndex);
	showCurrentJointStatus(aDocument);
}

void DLMUI_RobotControlOps::onJointForward()
{
	DocumentTut* aDocument = activeRobotDocument();
	if (aDocument == NULL || aDocument->lastOpenResult() <= 0)
		return;

	int aJointIndex = aDocument->currentJointIndex();
	if (aJointIndex < 0 || aJointIndex >= aDocument->lastOpenResult())
	{
		aJointIndex = 0;
		aDocument->setCurrentJointIndex(aJointIndex);
		aDocument->getRobot()->setActiveJoint(aJointIndex);
	}

	aDocument->getRobot()->moveJoint(aJointIndex, 1);
	showCurrentJointStatus(aDocument);
}

void DLMUI_RobotControlOps::onJointBackward()
{
	DocumentTut* aDocument = activeRobotDocument();
	if (aDocument == NULL || aDocument->lastOpenResult() <= 0)
		return;

	int aJointIndex = aDocument->currentJointIndex();
	if (aJointIndex < 0 || aJointIndex >= aDocument->lastOpenResult())
	{
		aJointIndex = 0;
		aDocument->setCurrentJointIndex(aJointIndex);
		aDocument->getRobot()->setActiveJoint(aJointIndex);
	}

	aDocument->getRobot()->moveJoint(aJointIndex, -1);
	showCurrentJointStatus(aDocument);
}

void DLMUI_RobotControlOps::onRobotReset()
{
	DocumentTut* aDocument = activeRobotDocument();
	if (aDocument == NULL)
		return;

	aDocument->getRobot()->resetRobot();
	showCurrentJointStatus(aDocument);
}

void DLMUI_RobotControlOps::onRobotCalc()
{
	DocumentTut* aDocument = activeRobotDocument();
	if (aDocument == NULL || aDocument->lastOpenResult() <= 0)
		return;

	if (myCalcDialog.isNull())
		myCalcDialog = new DL_RobotCalcDialog(myAppMainWindow);

	myCalcDialog->setDocument(aDocument);
	myCalcDialog->show();
	myCalcDialog->raise();
	myCalcDialog->activateWindow();
}

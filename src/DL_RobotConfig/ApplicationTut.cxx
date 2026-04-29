#include "DL_RobotConfig\ApplicationTut.h"
#include "DL_RobotConfig\DL_RobotContext.h"
#include "DL_RobotConfig\DocumentTut.h"

#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QMdiSubWindow>
#include <QStatusBar>

namespace
{
QString normalizedMenuText(QString theText)
{
	theText.remove(QLatin1Char('&'));
	return theText;
}
}

ApplicationTut::ApplicationTut()
	: ApplicationCommonWindow(),
	  myTBMUI_RobotOps(NULL),
	  myFileOpenAction(NULL),
	  nCurrentJoint_Index(0),
	  nMaxJoint_Count(0),
	  myRobotUiState(RobotUi_NoDocument)
{
	createTBMUI_FileMisc();
	createTBMUI_WindowPopup();
	createTBMUI_ModelSkin();
	createTBMUI_Raytrace();
	createTBMUI_ViewOps();
	createTBMUI_BackgroundSet();

	createFileOpenAction();
	createTBMUI_RobotOps();

	statusBar()->showMessage(QObject::tr("INF_READY"), 5000);
	resize(1000, 700);

	connect(getMdiArea(), SIGNAL(subWindowActivated(QMdiSubWindow*)),
	        this, SLOT(onRobotWindowActivated(QMdiSubWindow*)));
	updateRobotActions();
}

ApplicationTut::~ApplicationTut()
{
}

DocumentCommon* ApplicationTut::createNewDocument()
{
	return new DocumentTut(++myNbDocuments, this);
}

QMenu* ApplicationTut::findTopLevelMenu(const QString& theMenuText) const
{
	const QString aTarget = normalizedMenuText(theMenuText);
	QList<QAction*> aMenus = menuBar()->actions();
	for (int i = 0; i < aMenus.count(); ++i)
	{
		QMenu* aMenu = aMenus.at(i)->menu();
		if (aMenu != NULL && normalizedMenuText(aMenus.at(i)->text()) == aTarget)
		{
			return aMenu;
		}
	}
	return NULL;
}

void ApplicationTut::createFileOpenAction()
{
	QString dir = getResourceDir() + QString("/");
	QPixmap anOpenIcon(dir + QObject::tr("ICON_ROBOT_OPEN"));
	myFileOpenAction = new QAction(anOpenIcon, QObject::tr("MNU_ROBOT_OPEN"), this);
	myFileOpenAction->setToolTip(QObject::tr("TBR_ROBOT_OPEN"));
	myFileOpenAction->setStatusTip(QObject::tr("INF_ROBOT_OPEN"));
	myFileOpenAction->setShortcut(QObject::tr("CTRL+O"));
	connect(myFileOpenAction, SIGNAL(triggered()), this, SLOT(onFileOpen()));

	QMenu* aFileMenu = findTopLevelMenu(QObject::tr("MNU_FILE"));
	if (aFileMenu != NULL)
	{
		QList<QAction*> aFileActions = aFileMenu->actions();
		QAction* aBeforeAction = aFileActions.count() > 1 ? aFileActions.at(1) : NULL;
		if (aBeforeAction != NULL)
			aFileMenu->insertAction(aBeforeAction, myFileOpenAction);
		else
			aFileMenu->addAction(myFileOpenAction);
	}
}

#define ROBOT_ACTION(MAPID, ICON_ID, TEXT_ID, TIP_ID, SHORTCUT_TEXT, SLOT_NAME) \
	{ \
		QAction* anAction = new QAction(QPixmap(dir + QObject::tr(ICON_ID)), QObject::tr(TEXT_ID), this); \
		anAction->setToolTip(QObject::tr(TEXT_ID)); \
		anAction->setStatusTip(QObject::tr(TIP_ID)); \
		anAction->setShortcut(QObject::tr(SHORTCUT_TEXT)); \
		connect(anAction, SIGNAL(triggered()), this, SLOT(SLOT_NAME())); \
		myTBMUI_RobotOps_Actions.insert(MAPID, anAction); \
		myTBMUI_RobotOps->addAction(anAction); \
	}

void ApplicationTut::createTBMUI_RobotOps()
{
	QString dir = getResourceDir() + QString("/");
	myTBMUI_RobotOps = addToolBar(tr("Robot Operations"));

	ROBOT_ACTION(RobotDisasId, "ICON_ROBOT_DISAS", "TBR_ROBOT_DISAS", "INF_ROBOT_DISAS", "CTRL+D", onRobotDisas)
	ROBOT_ACTION(RobotWriteId, "ICON_ROBOT_WRITE", "TBR_ROBOT_WRITE", "INF_ROBOT_WRITE", "CTRL+SHIFT+W", onRobotWrite)
	myTBMUI_RobotOps->addSeparator();
	ROBOT_ACTION(JointSelectId, "ICON_JOINT_SELECT", "TBR_JOINT_SELECT", "INF_JOINT_SELECT", "CTRL+S", onJointSelect)
	ROBOT_ACTION(JointForwardId, "ICON_JOINT_FWORD", "TBR_JOINT_FWORD", "INF_JOINT_FWORD", "CTRL+F", onJointForward)
	ROBOT_ACTION(JointBackwardId, "ICON_JOINT_BWORD", "TBR_JOINT_BWORD", "INF_JOINT_BWORD", "CTRL+B", onJointBackward)
	ROBOT_ACTION(RobotResetId, "ICON_ROBOT_RESET", "TBR_ROBOT_RESET", "INF_ROBOT_RESET", "CTRL+R", onRobotReset)
	ROBOT_ACTION(RobotCalcId, "ICON_ROBOT_CALC", "TBR_ROBOT_CALC", "INF_ROBOT_CALC", "CTRL+C", onRobotCalc)

	myTBMUI_RobotOps->hide();
}

#undef ROBOT_ACTION

void ApplicationTut::checkTBMUIX(bool show)
{
	if (show)
	{
		setRobotUiState(RobotUi_EmptyDocument);
	}
	else
	{
		nCurrentJoint_Index = 0;
		nMaxJoint_Count = 0;
		setRobotUiState(RobotUi_NoDocument);
	}
}

DocumentTut* ApplicationTut::activeRobotDocument()
{
	return qobject_cast<DocumentTut*>(getActiveDocument());
}

DocumentTut* ApplicationTut::ensureRobotDocument()
{
	DocumentTut* aDocument = activeRobotDocument();
	if (aDocument != NULL)
	{
		return aDocument;
	}
	return qobject_cast<DocumentTut*>(onNewDoc());
}

ApplicationTut::RobotUiState ApplicationTut::stateForDocument(DocumentTut* theDocument) const
{
	if (theDocument == NULL)
	{
		return RobotUi_NoDocument;
	}

	const int aLastOpenResult = theDocument->lastOpenResult();
	if (aLastOpenResult < 0)
	{
		return RobotUi_StepPreview;
	}
	if (aLastOpenResult > 0)
	{
		return RobotUi_RobotLoaded;
	}
	return RobotUi_EmptyDocument;
}

void ApplicationTut::setRobotUiState(RobotUiState theState)
{
	myRobotUiState = theState;
	updateRobotActions();
}

void ApplicationTut::updateRobotActions()
{
	if (myTBMUI_RobotOps == NULL)
	{
		return;
	}

	const bool hasDocument = (myRobotUiState != RobotUi_NoDocument);
	const bool isStepPreview = (myRobotUiState == RobotUi_StepPreview);
	const bool isRobotLoaded = (myRobotUiState == RobotUi_RobotLoaded);

	myTBMUI_RobotOps->setVisible(hasDocument);
	if (!hasDocument)
	{
		return;
	}

	myTBMUI_RobotOps_Actions.at(RobotDisasId)->setEnabled(isStepPreview);
	myTBMUI_RobotOps_Actions.at(RobotWriteId)->setEnabled(true);
	myTBMUI_RobotOps_Actions.at(JointSelectId)->setEnabled(isRobotLoaded);
	myTBMUI_RobotOps_Actions.at(JointForwardId)->setEnabled(isRobotLoaded);
	myTBMUI_RobotOps_Actions.at(JointBackwardId)->setEnabled(isRobotLoaded);
	myTBMUI_RobotOps_Actions.at(RobotResetId)->setEnabled(isRobotLoaded);
	myTBMUI_RobotOps_Actions.at(RobotCalcId)->setEnabled(isRobotLoaded);
}

void ApplicationTut::showCurrentJointStatus()
{
	if (nMaxJoint_Count > 0)
	{
		statusBar()->showMessage(QString("Current Joint: J#") +
		                         QString::number(nCurrentJoint_Index + 1) +
		                         " of " + QString::number(nMaxJoint_Count));
	}
}

void ApplicationTut::onFileOpen()
{
	DocumentTut* aDocument = ensureRobotDocument();
	if (aDocument == NULL)
	{
		QMessageBox::warning(this, "Tips", "Please create an OCC 3D window first.");
		return;
	}

	statusBar()->showMessage(QObject::tr("INF_ROBOT_OPEN"), 5000);
	const int aLoadResult = aDocument->openRobot(this);
	if (aLoadResult > 0)
	{
		nMaxJoint_Count = aLoadResult;
		nCurrentJoint_Index = 0;
		aDocument->setCurrentJointIndex(nCurrentJoint_Index);
		aDocument->getRobot()->setActiveJoint(nCurrentJoint_Index);
		setRobotUiState(RobotUi_RobotLoaded);
		showCurrentJointStatus();
		return;
	}

	if (aLoadResult < 0)
	{
		nMaxJoint_Count = 0;
		nCurrentJoint_Index = 0;
		aDocument->setCurrentJointIndex(-1);
		setRobotUiState(RobotUi_StepPreview);
		statusBar()->showMessage("Single STEP preview loaded. Disassemble and Write XML are enabled.", 5000);
		return;
	}

	setRobotUiState(stateForDocument(aDocument));
	statusBar()->showMessage(QObject::tr("INF_DONE"), 3000);
}

void ApplicationTut::onRobotDisas()
{
	DocumentTut* aDocument = activeRobotDocument();
	if (aDocument == NULL)
	{
		QMessageBox::warning(this, "Tips", "Please open an OCC 3D window first.");
		return;
	}
	aDocument->getRobot()->disasRobot(this);
	statusBar()->showMessage(QObject::tr("INF_DONE"), 3000);
}

void ApplicationTut::onRobotWrite()
{
	DL_RobotContext::writeRobotXml(this);
	statusBar()->showMessage(QObject::tr("INF_DONE"), 3000);
}

void ApplicationTut::onJointSelect()
{
	if (nMaxJoint_Count <= 0)
	{
		return;
	}

	DocumentTut* aDocument = activeRobotDocument();
	if (aDocument == NULL)
	{
		return;
	}

	int aJointIndex = aDocument->currentJointIndex();
	if (aJointIndex < -1 || aJointIndex >= nMaxJoint_Count)
	{
		aJointIndex = -1;
	}

	++aJointIndex;
	if (aJointIndex == nMaxJoint_Count)
	{
		aJointIndex = 0;
	}

	nCurrentJoint_Index = aJointIndex;
	aDocument->setCurrentJointIndex(nCurrentJoint_Index);
	aDocument->getRobot()->setActiveJoint(nCurrentJoint_Index);
	showCurrentJointStatus();
}

void ApplicationTut::onJointForward()
{
	if (nMaxJoint_Count <= 0)
	{
		return;
	}
	DocumentTut* aDocument = activeRobotDocument();
	if (aDocument != NULL)
	{
		nCurrentJoint_Index = aDocument->currentJointIndex();
		if (nCurrentJoint_Index < 0 || nCurrentJoint_Index >= nMaxJoint_Count)
		{
			nCurrentJoint_Index = 0;
			aDocument->setCurrentJointIndex(nCurrentJoint_Index);
			aDocument->getRobot()->setActiveJoint(nCurrentJoint_Index);
		}
		aDocument->getRobot()->moveJoint(nCurrentJoint_Index, 1);
	}
}

void ApplicationTut::onJointBackward()
{
	if (nMaxJoint_Count <= 0)
	{
		return;
	}
	DocumentTut* aDocument = activeRobotDocument();
	if (aDocument != NULL)
	{
		nCurrentJoint_Index = aDocument->currentJointIndex();
		if (nCurrentJoint_Index < 0 || nCurrentJoint_Index >= nMaxJoint_Count)
		{
			nCurrentJoint_Index = 0;
			aDocument->setCurrentJointIndex(nCurrentJoint_Index);
			aDocument->getRobot()->setActiveJoint(nCurrentJoint_Index);
		}
		aDocument->getRobot()->moveJoint(nCurrentJoint_Index, -1);
	}
}

void ApplicationTut::onRobotReset()
{
	DocumentTut* aDocument = activeRobotDocument();
	if (aDocument != NULL)
	{
		aDocument->getRobot()->resetRobot();
		showCurrentJointStatus();
	}
}

void ApplicationTut::onRobotCalc()
{
	DocumentTut* aDocument = activeRobotDocument();
	if (aDocument != NULL)
	{
		aDocument->getRobot()->calcRobot();
	}
}

void ApplicationTut::onRobotWindowActivated(QMdiSubWindow* theWindow)
{
	Q_UNUSED(theWindow);
	DocumentTut* aDocument = activeRobotDocument();
	nMaxJoint_Count = (aDocument != NULL && aDocument->lastOpenResult() > 0)
	                ? aDocument->lastOpenResult()
	                : 0;
	nCurrentJoint_Index = (aDocument != NULL) ? aDocument->currentJointIndex() : 0;
	if (nMaxJoint_Count <= 0 || nCurrentJoint_Index < 0 || nCurrentJoint_Index >= nMaxJoint_Count)
	{
		nCurrentJoint_Index = 0;
	}
	setRobotUiState(stateForDocument(aDocument));
	if (aDocument != NULL && nMaxJoint_Count > 0)
	{
		aDocument->setCurrentJointIndex(nCurrentJoint_Index);
		showCurrentJointStatus();
	}
}

void ApplicationTut::addContextItemInPopup(QMenu* menu)
{
	menu->addSeparator();
	menu->addAction(myTBMUI_ViewOps_Actions.at(ViewFitAllId));
}

void ApplicationTut::Popup(const int x, const int y)
{
	ApplicationCommonWindow::Popup(x, y);
}

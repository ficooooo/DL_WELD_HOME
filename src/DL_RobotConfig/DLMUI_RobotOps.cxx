#include "DL_RobotConfig\DLMUI_RobotOps.h"

#include "DL_RobotConfig\DL_RobotCalcDialog.h"
#include "DL_RobotConfig\DL_RobotContext.h"
#include "DL_RobotConfig\DocumentTut.h"

#include "dlOcctMdiFrame\DLMUI_FileWnd.h"
#include "dlOcctMdiFrame\MDIWindow.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmap>
#include <QStatusBar>

namespace
{
QString normalizedMenuText(QString theText)
{
	theText.remove(QLatin1Char('&'));
	return theText;
}
}

DLMUI_RobotOps::DLMUI_RobotOps(ApplicationCommonWindow* mainWnd, int id)
	: DLModuleUIEntry(mainWnd, id)
{
	create();
}

DLMUI_RobotOps::~DLMUI_RobotOps()
{
}

void DLMUI_RobotOps::create()
{
	QString aDir = myAppMainWindow->getResourceDir() + QString("/");
	QAction* anAction = NULL;

	anAction = new QAction(QPixmap(aDir + QObject::tr("ICON_ROBOT_OPEN")), QObject::tr("MNU_ROBOT_OPEN"), this);
	anAction->setToolTip(QObject::tr("TBR_ROBOT_OPEN"));
	anAction->setStatusTip(QObject::tr("INF_ROBOT_OPEN"));
	anAction->setShortcut(QObject::tr("CTRL+O"));
	connect(anAction, SIGNAL(triggered()), this, SLOT(onFileOpen()));
	myActions.insert(RobotOpenId, anAction);

	myToolBar = myAppMainWindow->addToolBar(tr("Robot Operations"));

#define ROBOT_ACTION(MAPID, ICON_ID, TEXT_ID, TIP_ID, SHORTCUT_TEXT, SLOT_NAME) \
	anAction = new QAction(QPixmap(aDir + QObject::tr(ICON_ID)), QObject::tr(TEXT_ID), this); \
	anAction->setToolTip(QObject::tr(TEXT_ID)); \
	anAction->setStatusTip(QObject::tr(TIP_ID)); \
	anAction->setShortcut(QObject::tr(SHORTCUT_TEXT)); \
	connect(anAction, SIGNAL(triggered()), this, SLOT(SLOT_NAME())); \
	myActions.insert(MAPID, anAction); \
	myToolBar->addAction(anAction);

	ROBOT_ACTION(RobotDisasId, "ICON_ROBOT_DISAS", "TBR_ROBOT_DISAS", "INF_ROBOT_DISAS", "CTRL+D", onRobotDisas)
	ROBOT_ACTION(RobotWriteId, "ICON_ROBOT_WRITE", "TBR_ROBOT_WRITE", "INF_ROBOT_WRITE", "CTRL+SHIFT+W", onRobotWrite)
	myToolBar->addSeparator();
	ROBOT_ACTION(JointSelectId, "ICON_JOINT_SELECT", "TBR_JOINT_SELECT", "INF_JOINT_SELECT", "CTRL+S", onJointSelect)
	ROBOT_ACTION(JointForwardId, "ICON_JOINT_FWORD", "TBR_JOINT_FWORD", "INF_JOINT_FWORD", "CTRL+F", onJointForward)
	ROBOT_ACTION(JointBackwardId, "ICON_JOINT_BWORD", "TBR_JOINT_BWORD", "INF_JOINT_BWORD", "CTRL+B", onJointBackward)
	ROBOT_ACTION(RobotResetId, "ICON_ROBOT_RESET", "TBR_ROBOT_RESET", "INF_ROBOT_RESET", "CTRL+R", onRobotReset)
	ROBOT_ACTION(RobotCalcId, "ICON_ROBOT_CALC", "TBR_ROBOT_CALC", "INF_ROBOT_CALC", "CTRL+C", onRobotCalc)

#undef ROBOT_ACTION

	QMenu* aFileMenu = findTopLevelMenu(QObject::tr("MNU_FILE"));
	if (aFileMenu != NULL)
	{
		QList<QAction*> aFileActions = aFileMenu->actions();
		QAction* aBeforeAction = aFileActions.count() > 1 ? aFileActions.at(1) : NULL;
		if (aBeforeAction != NULL)
			aFileMenu->insertAction(aBeforeAction, myActions.at(RobotOpenId));
		else
			aFileMenu->addAction(myActions.at(RobotOpenId));
	}

	myToolBar->hide();
}

void DLMUI_RobotOps::checkNoView(MDIWindow*)
{
	syncDocumentUi(NULL);
}

void DLMUI_RobotOps::checkNewView(MDIWindow* theWindow)
{
	syncDocumentUi(theWindow == NULL ? NULL : qobject_cast<DocumentTut*>(theWindow->getDocument()));
}

void DLMUI_RobotOps::checkViewActivated(MDIWindow* theWindow)
{
	syncDocumentUi(theWindow == NULL ? NULL : qobject_cast<DocumentTut*>(theWindow->getDocument()));
}

QMenu* DLMUI_RobotOps::findTopLevelMenu(const QString& theMenuText) const
{
	const QString aTarget = normalizedMenuText(theMenuText);
	QList<QAction*> aMenus = myAppMainWindow->menuBar()->actions();
	for (int i = 0; i < aMenus.count(); ++i)
	{
		QMenu* aMenu = aMenus.at(i)->menu();
		if (aMenu != NULL && normalizedMenuText(aMenus.at(i)->text()) == aTarget)
			return aMenu;
	}
	return NULL;
}

DocumentTut* DLMUI_RobotOps::activeRobotDocument() const
{
	return qobject_cast<DocumentTut*>(myAppMainWindow->getActiveDocument());
}

DocumentTut* DLMUI_RobotOps::ensureRobotDocument()
{
	DocumentTut* aDocument = activeRobotDocument();
	if (aDocument != NULL)
		return aDocument;

	DLMUI_FileWnd* aFileWnd = static_cast<DLMUI_FileWnd*>(DLModuleUIEntry::getEntry(ApplicationCommonWindow::DLMUI_FileWndId));
	if (NULL == aFileWnd)
		return NULL;

	return qobject_cast<DocumentTut*>(aFileWnd->onNewDoc());
}

DLMUI_RobotOps::RobotUiState DLMUI_RobotOps::stateForDocument(DocumentTut* theDocument) const
{
	if (theDocument == NULL)
		return RobotUi_NoDocument;

	const int aLastOpenResult = theDocument->lastOpenResult();
	if (aLastOpenResult < 0)
		return RobotUi_StepPreview;
	if (aLastOpenResult > 0)
		return RobotUi_RobotLoaded;
	return RobotUi_EmptyDocument;
}

void DLMUI_RobotOps::syncDocumentUi(DocumentTut* theDocument)
{
	if (myToolBar == NULL)
		return;

	const RobotUiState aState = stateForDocument(theDocument);
	const bool hasDocument = (aState != RobotUi_NoDocument);
	const bool isStepPreview = (aState == RobotUi_StepPreview);
	const bool isRobotLoaded = (aState == RobotUi_RobotLoaded);

	myToolBar->setVisible(hasDocument);
	myActions.at(RobotDisasId)->setEnabled(isStepPreview);
	myActions.at(RobotWriteId)->setEnabled(hasDocument);
	myActions.at(JointSelectId)->setEnabled(isRobotLoaded);
	myActions.at(JointForwardId)->setEnabled(isRobotLoaded);
	myActions.at(JointBackwardId)->setEnabled(isRobotLoaded);
	myActions.at(RobotResetId)->setEnabled(isRobotLoaded);
	myActions.at(RobotCalcId)->setEnabled(isRobotLoaded);

	if (isRobotLoaded)
		showCurrentJointStatus(theDocument);
	else if (hasDocument)
		myAppMainWindow->statusBar()->showMessage(QObject::tr("INF_READY"), 3000);
}

void DLMUI_RobotOps::showCurrentJointStatus(DocumentTut* theDocument)
{
	if (theDocument == NULL)
		return;

	const int aJointCount = theDocument->lastOpenResult();
	const int aJointIndex = theDocument->currentJointIndex();
	if (aJointCount > 0 && aJointIndex >= 0 && aJointIndex < aJointCount)
	{
		myAppMainWindow->statusBar()->showMessage(QString("Current Joint: J#") +
		                                          QString::number(aJointIndex + 1) +
		                                          " of " + QString::number(aJointCount));
	}
}

void DLMUI_RobotOps::onFileOpen()
{
	DocumentTut* aDocument = ensureRobotDocument();
	if (aDocument == NULL)
	{
		QMessageBox::warning(myAppMainWindow, "Tips", "Please create an OCC 3D window first.");
		return;
	}

	myAppMainWindow->statusBar()->showMessage(QObject::tr("INF_ROBOT_OPEN"), 5000);
	const int aLoadResult = aDocument->openRobot(myAppMainWindow);
	if (aLoadResult > 0)
	{
		aDocument->setCurrentJointIndex(0);
		aDocument->getRobot()->setActiveJoint(0);
		syncDocumentUi(aDocument);
		return;
	}

	if (aLoadResult < 0)
	{
		aDocument->setCurrentJointIndex(-1);
		syncDocumentUi(aDocument);
		myAppMainWindow->statusBar()->showMessage("Single STEP preview loaded. Disassemble and Write XML are enabled.", 5000);
		return;
	}

	syncDocumentUi(aDocument);
	myAppMainWindow->statusBar()->showMessage(QObject::tr("INF_DONE"), 3000);
}

void DLMUI_RobotOps::onRobotDisas()
{
	DocumentTut* aDocument = activeRobotDocument();
	if (aDocument == NULL)
	{
		QMessageBox::warning(myAppMainWindow, "Tips", "Please open an OCC 3D window first.");
		return;
	}

	aDocument->getRobot()->disasRobot(myAppMainWindow);
	syncDocumentUi(aDocument);
	myAppMainWindow->statusBar()->showMessage(QObject::tr("INF_DONE"), 3000);
}

void DLMUI_RobotOps::onRobotWrite()
{
	DL_RobotContext::writeRobotXml(myAppMainWindow);
	myAppMainWindow->statusBar()->showMessage(QObject::tr("INF_DONE"), 3000);
}

void DLMUI_RobotOps::onJointSelect()
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

void DLMUI_RobotOps::onJointForward()
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

void DLMUI_RobotOps::onJointBackward()
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

void DLMUI_RobotOps::onRobotReset()
{
	DocumentTut* aDocument = activeRobotDocument();
	if (aDocument == NULL)
		return;

	aDocument->getRobot()->resetRobot();
	showCurrentJointStatus(aDocument);
}

void DLMUI_RobotOps::onRobotCalc()
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

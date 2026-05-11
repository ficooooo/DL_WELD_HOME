#include "DL_RobotConfig\DLMUI_RobotImportOps.h"

#include "DL_RobotConfig\DL_RobotContext.h"
#include "DL_RobotConfig\DocumentTut.h"

#include "dlOcctMdiFrame\MDIWindow.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QPixmap>
#include <QStatusBar>

DLMUI_RobotImportOps::DLMUI_RobotImportOps(ApplicationCommonWindow* mainWnd, int id)
	: DLMUI_RobotModuleBase(mainWnd, id)
{
	create();
}

DLMUI_RobotImportOps::~DLMUI_RobotImportOps()
{
}

void DLMUI_RobotImportOps::create()
{
	QString aDir = myAppMainWindow->getResourceDir() + QString("/");
	QAction* anAction = NULL;

	myToolBar = myAppMainWindow->addToolBar(tr("Robot Import Operations"));

#define ROBOT_MENU_ACTION(MAPID, ICON_ID, TEXT_ID, TIP_ID, SHORTCUT_TEXT, SLOT_NAME) \
	anAction = new QAction(QPixmap(aDir + QObject::tr(ICON_ID)), QObject::tr(TEXT_ID), this); \
	anAction->setToolTip(QObject::tr(TEXT_ID)); \
	anAction->setStatusTip(QObject::tr(TIP_ID)); \
	anAction->setShortcut(QObject::tr(SHORTCUT_TEXT)); \
	connect(anAction, SIGNAL(triggered()), this, SLOT(SLOT_NAME())); \
	myActions.insert(MAPID, anAction);

	ROBOT_MENU_ACTION(RobotOpenId, "ICON_ROBOT_OPEN", "MNU_ROBOT_OPEN", "INF_ROBOT_OPEN", "CTRL+O", onFileOpen)

#define ROBOT_IMPORT_ACTION(MAPID, ICON_ID, TEXT_ID, TIP_ID, SHORTCUT_TEXT, SLOT_NAME) \
	ROBOT_MENU_ACTION(MAPID, ICON_ID, TEXT_ID, TIP_ID, SHORTCUT_TEXT, SLOT_NAME) \
	myToolBar->addAction(anAction);

	ROBOT_IMPORT_ACTION(RobotDisasId, "ICON_ROBOT_DISAS", "TBR_ROBOT_DISAS", "INF_ROBOT_DISAS", "CTRL+D", onRobotDisas)
	ROBOT_IMPORT_ACTION(RobotWriteId, "ICON_ROBOT_WRITE", "TBR_ROBOT_WRITE", "INF_ROBOT_WRITE", "CTRL+SHIFT+W", onRobotWrite)

#undef ROBOT_IMPORT_ACTION
#undef ROBOT_MENU_ACTION

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

void DLMUI_RobotImportOps::checkNoView(MDIWindow*)
{
	syncDocumentUi(NULL);
}

void DLMUI_RobotImportOps::checkNewView(MDIWindow* theWindow)
{
	syncDocumentUi(theWindow == NULL ? NULL : qobject_cast<DocumentTut*>(theWindow->getDocument()));
}

void DLMUI_RobotImportOps::checkViewActivated(MDIWindow* theWindow)
{
	syncDocumentUi(theWindow == NULL ? NULL : qobject_cast<DocumentTut*>(theWindow->getDocument()));
}

void DLMUI_RobotImportOps::syncDocumentUi(DocumentTut* theDocument)
{
	if (myToolBar == NULL)
		return;

	const RobotUiState aState = stateForDocument(theDocument);
	const bool isStepPreview = (aState == RobotUi_StepPreview);
	const bool showImportToolbar = isStepPreview;
	const bool isEmptyDocument = (aState == RobotUi_EmptyDocument);

	myToolBar->setVisible(showImportToolbar);
	myActions.at(RobotOpenId)->setEnabled(true);
	myActions.at(RobotDisasId)->setEnabled(isStepPreview);
	myActions.at(RobotWriteId)->setEnabled(isStepPreview);

	if (isEmptyDocument)
		myAppMainWindow->statusBar()->showMessage(QObject::tr("INF_READY"), 3000);
}

void DLMUI_RobotImportOps::onFileOpen()
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
		refreshRobotUi();
		return;
	}

	if (aLoadResult < 0)
	{
		aDocument->setCurrentJointIndex(-1);
		refreshRobotUi();
		myAppMainWindow->statusBar()->showMessage("Single STEP preview loaded. Disassemble and Write XML are enabled.", 5000);
		return;
	}

	refreshRobotUi();
	myAppMainWindow->statusBar()->showMessage(QObject::tr("INF_DONE"), 3000);
}

void DLMUI_RobotImportOps::onRobotDisas()
{
	DocumentTut* aDocument = activeRobotDocument();
	if (aDocument == NULL)
	{
		QMessageBox::warning(myAppMainWindow, "Tips", "Please open an OCC 3D window first.");
		return;
	}

	aDocument->getRobot()->disasRobot(myAppMainWindow);
	refreshRobotUi();
	myAppMainWindow->statusBar()->showMessage(QObject::tr("INF_DONE"), 3000);
}

void DLMUI_RobotImportOps::onRobotWrite()
{
	DL_RobotContext::writeRobotXml(myAppMainWindow);
	myAppMainWindow->statusBar()->showMessage(QObject::tr("INF_DONE"), 3000);
}

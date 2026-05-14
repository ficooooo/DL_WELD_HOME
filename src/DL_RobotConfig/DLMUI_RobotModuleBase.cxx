#include "DL_RobotConfig\DLMUI_RobotModuleBase.h"

#include "DL_RobotConfig\DocumentTut.h"

#include "dlOcctMdiFrame\DLMUI_FileWnd.h"
#include "dlOcctMdiFrame\MDIWindow.h"

#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>

namespace
{
QString normalizedMenuText(QString theText)
{
	theText.remove(QLatin1Char('&'));
	return theText;
}
}

DLMUI_RobotModuleBase::DLMUI_RobotModuleBase(ApplicationCommonWindow* mainWnd, int id)
	: DLModuleUIEntry(mainWnd, id)
{
}

QMenu* DLMUI_RobotModuleBase::findTopLevelMenu(const QString& theMenuText) const
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

DocumentTut* DLMUI_RobotModuleBase::activeRobotDocument() const
{
	return qobject_cast<DocumentTut*>(myAppMainWindow->getActiveDocument());
}

DocumentTut* DLMUI_RobotModuleBase::ensureRobotDocument()
{
	DocumentTut* aDocument = activeRobotDocument();
	if (aDocument != NULL)
		return aDocument;

	DLMUI_FileWnd* aFileWnd = static_cast<DLMUI_FileWnd*>(DLModuleUIEntry::getEntry(ApplicationCommonWindow::DLMUI_FileWndId));
	if (NULL == aFileWnd)
		return NULL;

	return qobject_cast<DocumentTut*>(aFileWnd->onNewDoc());
}

DLMUI_RobotModuleBase::RobotUiState DLMUI_RobotModuleBase::stateForDocument(DocumentTut* theDocument) const
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

void DLMUI_RobotModuleBase::showCurrentJointStatus(DocumentTut* theDocument)
{
	if (theDocument == NULL)
		return;

	const int aJointCount = theDocument->lastOpenResult();
	const int aJointIndex = theDocument->currentJointIndex();
	if (aJointCount > 0 && aJointIndex >= 0 && aJointIndex < aJointCount)
	{
		myAppMainWindow->statusBar()->showMessage(
			QObject::tr("Current Joint: J#%1 of %2")
				.arg(aJointIndex + 1)
				.arg(aJointCount));
	}
}

void DLMUI_RobotModuleBase::refreshRobotUi()
{
	MDIWindow* aWindow = myAppMainWindow->getActiveWnd();
	if (aWindow != NULL)
	{
		DLModuleUIEntry::checkWindowActionAll(DLModuleUIEntry::SWITCH_VIEW, aWindow);
		return;
	}

	DLModuleUIEntry::checkWindowActionAll(DLModuleUIEntry::NO_VIEW, NULL);
}

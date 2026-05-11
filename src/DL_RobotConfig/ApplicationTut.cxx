#include "DL_RobotConfig\ApplicationTut.h"
#include "DL_RobotConfig\DocumentTut.h"
#include "DL_RobotConfig\DLMUI_RobotControlOps.h"
#include "DL_RobotConfig\DLMUI_RobotImportOps.h"

#include "dlOcctMdiFrame\DLMUI_BackgroundSet.h"
#include "dlOcctMdiFrame\DLMUI_FileWnd.h"
#include "dlOcctMdiFrame\DLMUI_ModelSkin.h"
#include "dlOcctMdiFrame\DLMUI_Raytrace.h"
#include "dlOcctMdiFrame\DLMUI_ViewOps.h"
#include "dlOcctMdiFrame\DLModuleUIEntry.h"
#include "dlOcctMdiFrame\MDIWindow.h"

#include <QMenu>
#include <QStatusBar>

ApplicationTut::ApplicationTut()
	: ApplicationCommonWindow()
{
	new DLMUI_FileWnd(this, DLMUI_FileWndId);
	new DLMUI_ViewOps(this, DLMUI_ViewOpsId);
	new DLMUI_ModelSkin(this, DLMUI_ModelSkinId);
	new DLMUI_Raytrace(this, DLMUI_RaytraceId);
	new DLMUI_BackgroundSet(this, DLMUI_BackgroundSetId);
	new DLMUI_RobotImportOps(this, DLMUI_RobotImportOpsId);
	new DLMUI_RobotControlOps(this, DLMUI_RobotControlOpsId);

	DLModuleUIEntry::transMap2List();

	statusBar()->showMessage(QObject::tr("INF_READY"), 5000);
	resize(1000, 700);
}

ApplicationTut::~ApplicationTut()
{
}

DocumentCommon* ApplicationTut::createNewDocument()
{
	return new DocumentTut(++myNbDocuments, this);
}

void ApplicationTut::addContextItemInPopup(QMenu* theMenu)
{
	DLModuleUIEntry* anEntry = DLModuleUIEntry::getEntry(DLMUI_ViewOpsId);
	if (NULL == anEntry)
		return;

	theMenu->addSeparator();
	theMenu->addAction(anEntry->getAction(DLMUI_ViewOps::ViewFitAllId));
}

void ApplicationTut::popup(const int, const int)
{
	MDIWindow* aWindow = getActiveWnd();
	DocumentCommon* aDoc = getActiveDocument();

	if (aDoc != NULL && aDoc->getContext()->NbSelected() > 0)
	{
		DLModuleUIEntry* anEntry = DLModuleUIEntry::getEntry(DLMUI_ModelSkinId);
		if (NULL != anEntry)
			anEntry->popup();
	}
	else
	{
		DLModuleUIEntry* anEntry = DLModuleUIEntry::getEntry(DLMUI_BackgroundSetId);
		if (NULL != anEntry)
			anEntry->popup();
	}

	if (aWindow != NULL)
		aWindow->setFocus();
}

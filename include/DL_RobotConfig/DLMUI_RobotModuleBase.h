#ifndef DLMUI_ROBOTMODULEBASE_H
#define DLMUI_ROBOTMODULEBASE_H

#include "dlOcctMdiFrame\DLModuleUIEntry.h"

class DocumentTut;
class QMenu;

class DLMUI_RobotModuleBase : public DLModuleUIEntry
{
public:
	enum RobotUiState
	{
		RobotUi_NoDocument,
		RobotUi_EmptyDocument,
		RobotUi_StepPreview,
		RobotUi_RobotLoaded
	};

protected:
	DLMUI_RobotModuleBase(ApplicationCommonWindow* mainWnd, int id);

	QMenu*       findTopLevelMenu(const QString& theMenuText) const;
	DocumentTut* activeRobotDocument() const;
	DocumentTut* ensureRobotDocument();
	RobotUiState stateForDocument(DocumentTut* theDocument) const;
	void         showCurrentJointStatus(DocumentTut* theDocument);
	void         refreshRobotUi();
};

#endif

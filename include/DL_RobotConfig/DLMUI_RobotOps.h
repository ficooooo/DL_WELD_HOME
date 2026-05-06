#ifndef DLMUI_ROBOTOPS_H
#define DLMUI_ROBOTOPS_H

#include "dlOcctMdiFrame\DLModuleUIEntry.h"

class DocumentCommon;
class DocumentTut;
class QMenu;

class DLMUI_RobotOps : public DLModuleUIEntry
{
	Q_OBJECT

public:
	enum RobotActionId
	{
		RobotOpenId = 0,
		RobotDisasId,
		RobotWriteId,
		JointSelectId,
		JointForwardId,
		JointBackwardId,
		RobotResetId,
		RobotCalcId
	};

	enum RobotUiState
	{
		RobotUi_NoDocument,
		RobotUi_EmptyDocument,
		RobotUi_StepPreview,
		RobotUi_RobotLoaded
	};

public:
	DLMUI_RobotOps(ApplicationCommonWindow* mainWnd, int id);
	~DLMUI_RobotOps();

public:
	virtual void create() override;
	virtual void checkNoView(MDIWindow*) override;
	virtual void checkNewView(MDIWindow*) override;
	virtual void checkViewActivated(MDIWindow*) override;

private:
	QMenu*       findTopLevelMenu(const QString& theMenuText) const;
	DocumentTut* activeRobotDocument() const;
	DocumentTut* ensureRobotDocument();
	RobotUiState stateForDocument(DocumentTut* theDocument) const;
	void         syncDocumentUi(DocumentTut* theDocument);
	void         showCurrentJointStatus(DocumentTut* theDocument);

public slots:
	void onFileOpen();
	void onRobotDisas();
	void onRobotWrite();
	void onJointSelect();
	void onJointForward();
	void onJointBackward();
	void onRobotReset();
	void onRobotCalc();
};

#endif

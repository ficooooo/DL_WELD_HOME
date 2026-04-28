#ifndef APPLICATIONTUT_H
#define APPLICATIONTUT_H

#include "dlOcctMdiFrame\ApplicationCommon.h"

class QAction;
class DocumentCommon;
class DocumentTut;
class QMdiSubWindow;

class ApplicationTut: public ApplicationCommonWindow
{
    Q_OBJECT
public:
	ApplicationTut();
	~ApplicationTut();

protected:
	virtual DocumentCommon* createNewDocument() override;

public:
	virtual void checkTBMUIX(bool show) override;

public slots:
	void onFileOpen();
	void onRobotDisas();
	void onRobotWrite();
	void onJointSelect();
	void onJointForward();
	void onJointBackward();
	void onRobotReset();
	void onRobotCalc();
	void onRobotWindowActivated(QMdiSubWindow* theWindow);

private:
	enum RobotActionId
	{
		RobotDisasId = 0,
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

	void createFileOpenAction();
	void createTBMUI_RobotOps();
	QMenu* findTopLevelMenu(const QString& theMenuText) const;
	DocumentTut* activeRobotDocument();
	DocumentTut* ensureRobotDocument();
	RobotUiState stateForDocument(DocumentTut* theDocument) const;
	void setRobotUiState(RobotUiState theState);
	void updateRobotActions();
	void showCurrentJointStatus();

private:
	QToolBar* myTBMUI_RobotOps;
	QList<QAction*> myTBMUI_RobotOps_Actions;
	QAction* myFileOpenAction;
	int nCurrentJoint_Index;
	int nMaxJoint_Count;
	RobotUiState myRobotUiState;

public:
	virtual void addContextItemInPopup(QMenu*) override;
	virtual void Popup(const int x, const int y) override;
};

#endif

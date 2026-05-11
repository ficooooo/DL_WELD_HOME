#ifndef DLMUI_ROBOTCONTROLOPS_H
#define DLMUI_ROBOTCONTROLOPS_H

#include "DL_RobotConfig\DLMUI_RobotModuleBase.h"

#include <QPointer>

class DL_RobotCalcDialog;
class MDIWindow;

class DLMUI_RobotControlOps : public DLMUI_RobotModuleBase
{
	Q_OBJECT

public:
	enum RobotControlActionId
	{
		JointSelectId = 0,
		JointForwardId,
		JointBackwardId,
		RobotResetId,
		RobotCalcId
	};

public:
	DLMUI_RobotControlOps(ApplicationCommonWindow* mainWnd, int id);
	~DLMUI_RobotControlOps();

public:
	virtual void create() override;
	virtual void checkNoView(MDIWindow*) override;
	virtual void checkNewView(MDIWindow*) override;
	virtual void checkViewActivated(MDIWindow*) override;

private:
	void syncDocumentUi(DocumentTut* theDocument);

private:
	QPointer<DL_RobotCalcDialog> myCalcDialog;

public slots:
	void onJointSelect();
	void onJointForward();
	void onJointBackward();
	void onRobotReset();
	void onRobotCalc();
};

#endif

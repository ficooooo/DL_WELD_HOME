#ifndef DLMUI_ROBOTIMPORTOPS_H
#define DLMUI_ROBOTIMPORTOPS_H

#include "DL_RobotConfig\DLMUI_RobotModuleBase.h"

class MDIWindow;

class DLMUI_RobotImportOps : public DLMUI_RobotModuleBase
{
	Q_OBJECT

public:
	enum RobotImportActionId
	{
		RobotOpenId = 0,
		RobotDisasId,
		RobotWriteId
	};

public:
	DLMUI_RobotImportOps(ApplicationCommonWindow* mainWnd, int id);
	~DLMUI_RobotImportOps();

public:
	virtual void create() override;
	virtual void checkNoView(MDIWindow*) override;
	virtual void checkNewView(MDIWindow*) override;
	virtual void checkViewActivated(MDIWindow*) override;

private:
	void syncDocumentUi(DocumentTut* theDocument);

public slots:
	void onFileOpen();
	void onRobotDisas();
	void onRobotWrite();
};

#endif

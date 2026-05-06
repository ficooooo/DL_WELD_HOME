#ifndef APPLICATIONTUT_H
#define APPLICATIONTUT_H

#include "dlOcctMdiFrame\ApplicationCommon.h"

class DocumentCommon;
class QMenu;

class ApplicationTut: public ApplicationCommonWindow
{
    Q_OBJECT
public:
	enum DLModuleUIEntryIdsX
	{
		DLMUI_RobotOpsId = DLMUI_Count
	};

public:
	ApplicationTut();
	~ApplicationTut();

public:
	virtual DocumentCommon* createNewDocument() override;
	virtual void            addContextItemInPopup(QMenu*) override;
	virtual void            popup(const int x, const int y) override;
};

#endif

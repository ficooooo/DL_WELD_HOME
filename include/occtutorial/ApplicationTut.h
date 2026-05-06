#ifndef APPLICATIONTUT_H
#define APPLICATIONTUT_H

#include "dlOcctMdiFrame\ApplicationCommon.h"

class ApplicationTut: public ApplicationCommonWindow
{
    Q_OBJECT
public:
	enum DLModuleUIEntryIdsX	{
									//派生类继续扩展此enum
									DLMUI_BottleId=DLMUI_Count		
								};
public:
	ApplicationTut();
	~ApplicationTut();
public:
	virtual DocumentCommon*         createNewDocument();

public:
	virtual void 		addContextItemInPopup(QMenu *);
	virtual	void		popup( const int x, const int y );
};

#endif


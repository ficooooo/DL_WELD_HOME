#ifndef APPLICATIONTUT_H
#define APPLICATIONTUT_H

#include "dlOcctMdiFrame\ApplicationCommon.h"

class ApplicationTut: public ApplicationCommonWindow
{
    Q_OBJECT
public:
	ApplicationTut();
	~ApplicationTut();

private:
	QToolBar*			myTBMUI_MakeBottle;
public:	
	virtual void		checkTBMUIX(bool show);
public slots:
	void				onTBMUI_MakeBottle();

private:
	void              	createTBMUI_MakeBottle();
public:
	virtual void 		addContextItemInPopup(QMenu *);
	virtual	void		Popup( const int x, const int y );
};

#endif

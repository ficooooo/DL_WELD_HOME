#ifndef DOCUMENTTUT_H
#define DOCUMENTTUT_H

#include "dlOcctMdiFrame\DocumentCommon.h"

#include <memory>

class ApplicationCommon;
class DL_RobotContext;

class DocumentTut : public DocumentCommon
{
	Q_OBJECT

public:
   DocumentTut( const int, ApplicationCommonWindow* );
	~DocumentTut();

	void                           makeBottle();
	int                            openRobot(QWidget* theParent);
	DL_RobotContext*               getRobot();
	int                            lastOpenResult() const { return myLastOpenResult; }

private:
	std::unique_ptr<DL_RobotContext> myRobotContext;
	int                              myLastOpenResult;
};

#endif

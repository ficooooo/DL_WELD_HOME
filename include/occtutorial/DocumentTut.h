#ifndef DOCUMENTTUT_H
#define DOCUMENTTUT_H

#include "dlOcctMdiFrame\DocumentCommon.h"


class DocumentTut : public DocumentCommon
{
	Q_OBJECT
public:
	enum SCENE_COMPNENTS_ID_LISTX	{ DL_BOTTLE=DL_VIEWOBJECT_COUNT
									,DL_VIEWOBJECT_COUNTX };
private:
	
	
public:
   DocumentTut( const int, ApplicationCommonWindow* );
	~DocumentTut();
	virtual int						getFloorSize()				{	return 1;	}
	virtual int						getFloorGridSizeFactor()	{	return -3;	}
	Handle(AIS_Shape)				makeBottle();
	int								getNbSceneComponents()		{	return DL_VIEWOBJECT_COUNTX;	}
};

#endif

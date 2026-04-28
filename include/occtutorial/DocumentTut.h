#ifndef DOCUMENTTUT_H
#define DOCUMENTTUT_H

#include "dlOcctMdiFrame\DocumentCommon.h"
//#include <Geom_BSplineSurface.hxx>

//class SurfConstruction;
class ApplicationCommon;

class DocumentTut : public DocumentCommon
{
	Q_OBJECT

public:
   DocumentTut( const int, ApplicationCommonWindow* );
	~DocumentTut();

	void                           makeBottle();

//private:
	//SurfConstruction*			   mySurfConstructor;
};

#endif

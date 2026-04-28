#ifndef MDIWINDOW_H
#define MDIWINDOW_H

#include "dllDeclaration.h"
#include <QMainWindow>
#include <QMdiSubWindow>

class DocumentCommon;
class View;

class DCL_DL_OCCTMDI_FRAME__CLASS MDIWindow: public QMdiSubWindow//QMainWindow
{
    Q_OBJECT

public:
  	MDIWindow( DocumentCommon* aDocument, QWidget* parent, Qt::WindowFlags wflags );
  	MDIWindow( View* aView, DocumentCommon* aDocument, QWidget* parent, Qt::WindowFlags wflags );
  	~MDIWindow();

  	virtual QSize		sizeHint() const;
signals:
  	void                message(const QString&, int );
	void               	sendCloseView(MDIWindow* theView);

public slots:
	void               	closeEvent(QCloseEvent* e);        
	void               	onWindowActivated ();
	void               	dump();
public:
	View*               getWndView()	{	return myView;		}
	DocumentCommon*     getDocument()	{	return myDocument;	}
protected:
  	DocumentCommon*     myDocument;
  	View*               myView;
};

#endif
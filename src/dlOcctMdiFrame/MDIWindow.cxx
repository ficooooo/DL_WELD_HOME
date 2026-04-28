#include "dlOcctMdiFrame/MDIWindow.h"

#include "dlOcctMdiFrame/View.h"
#include "dlOcctMdiFrame/DocumentCommon.h"
#include "dlOcctMdiFrame/ApplicationCommon.h"

#include <Standard_WarningsDisable.hxx>
#include <QFrame>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMainWindow> 
#include <QVBoxLayout>
#include <Standard_WarningsRestore.hxx>

MDIWindow::MDIWindow(View* aView,
                     DocumentCommon* aDocument, 
                     QWidget* parent, 
                     Qt::WindowFlags wflags )
: QMdiSubWindow( parent, wflags )
{
  	myView = aView;
  	myDocument = aDocument;
}

MDIWindow::MDIWindow( DocumentCommon* aDocument, QWidget* parent, Qt::WindowFlags wflags)
: QMdiSubWindow( parent, wflags )
{
	setAttribute(Qt::WA_DeleteOnClose);  //点击X关闭窗口，closeEvent后自动删除自己
	myDocument = aDocument;
	myView = new View (myDocument->getContext(), this);
	this->setWidget (myView);
	
	connect( myView, SIGNAL( selectionChanged() ), myDocument->getApplication(),  SLOT( onSelectionChanged() ) );
}

MDIWindow::~MDIWindow()
{
	printf("MDIWindow::~MDIWindow()\n");
}

void MDIWindow::closeEvent(QCloseEvent* )
{
	emit sendCloseView(this);
}

void MDIWindow::onWindowActivated ()
{
	//printf("MDIWindow::onWindowActivated ()  %s\n",windowTitle().toStdString().c_str());
	//getDocument()->getApplication()->onSelectionChanged();
}

void MDIWindow::dump()
{
	QString filter = "Images Files (*.bmp *.ppm *.png *.jpg *.tiff *.tga *.gif *.exr)";
	QFileDialog fd ( 0 );
	fd.setModal( true );
	fd.setNameFilter ( filter );
	fd.setWindowTitle( QObject::tr("INF_APP_EXPORT") );
	fd.setFileMode( QFileDialog::AnyFile );
	int ret = fd.exec(); 
	
	/* update the desktop after the dialog is closed */
	qApp->processEvents();
	
	QStringList fileNames;
	fileNames = fd.selectedFiles();
	
	QString file ( (ret == QDialog::Accepted && !fileNames.isEmpty() )? fileNames[0] : QString::null);
	if ( !file.isNull() )
	{
		QApplication::setOverrideCursor( Qt::WaitCursor );
		if ( !QFileInfo( file ).completeSuffix().length() )
			file += QString( ".bmp" );
		
		const TCollection_AsciiString anUtf8Path (file.toUtf8().data());
		
		bool res = myView->dump( anUtf8Path.ToCString() );
		QApplication::restoreOverrideCursor();                
		if ( !res )
		{
			QWidgetList list = qApp->allWidgets();
			QWidget* mainWidget = NULL;
			for( int i = 0; i < list.size(); ++i )
			{
				if( qobject_cast<ApplicationCommonWindow*>( list.at( i ) ) )
					mainWidget = qobject_cast<ApplicationCommonWindow*>( list.at( i ) );
			}
			
			QMessageBox::information ( mainWidget
								, QObject::tr("TIT_ERROR")
								, QObject::tr("INF_ERROR")
								, QObject::tr("BTN_OK")
								, QString::null, QString::null, 0, 0 );
			qApp->processEvents();
		}
	}
}

QSize MDIWindow::sizeHint() const
{
	return QSize( 450, 300 );
}

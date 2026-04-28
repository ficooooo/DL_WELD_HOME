#include "DL_RobotConfig\ApplicationTut.h"

#include <OSD_Environment.hxx>

#include <QApplication>
#include <QTranslator>
#include <cstdio>

int main ( int argc, char* argv[] )
{
	QApplication a( argc, argv );

	QString resDir = ApplicationTut::getResourceDir();
	
	//2026/4/21 8:36:17 按钮图片资源，文本翻译资源 ts>>qm文件
	//Important, 其他程序也需包含此部分
	//可分组管理文件
	//a != qApp
	GENERAL_TRANSLATOR_INSTALL(a,A,"Common-string",resDir)
	GENERAL_TRANSLATOR_INSTALL(a,B,"Common-icon",resDir)
	GENERAL_TRANSLATOR_INSTALL(a,C,"Tutorial-string",resDir)
	GENERAL_TRANSLATOR_INSTALL(a,D,"Tutorial-icon",resDir)
	
    QObject::connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

    ApplicationTut* mw = new ApplicationTut();
    mw->setWindowTitle( QObject::tr( "TIT_SAMPLE" ) );
    QString aResName( resDir + QString( "/" ) + QObject::tr( "ICON_SAMPLE" ) );
    	
    mw->setWindowIcon( QPixmap( aResName ) );
    mw->show();
    
    return a.exec();
}

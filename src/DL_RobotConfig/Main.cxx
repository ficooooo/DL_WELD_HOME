#include "DL_RobotConfig\ApplicationTut.h"

#include <OSD_Environment.hxx>

#include <QApplication>
#include <QTranslator>
#include <cstdio>

int main ( int argc, char* argv[] )
{
	QApplication a( argc, argv );
	std::printf("[DL_RobotConfig] launched from current build: %s %s\n", __DATE__, __TIME__);
	std::fflush(stdout);

	QString resDir = ApplicationTut::getResourceDir();
	
	//2026/4/21 8:36:17 按钮图片资源，文本翻译资源 ts>>qm文件
	//Important, 其他程序也需包含此部分
	//可分组管理文件
	//a != qApp
	GENERAL_TRANSLATOR_INSTALL(a,A,"Common-string",resDir)
	GENERAL_TRANSLATOR_INSTALL(a,B,"Common-icon",resDir)
	GENERAL_TRANSLATOR_INSTALL(a,C,"DL_RobotConfig-string",resDir)
	GENERAL_TRANSLATOR_INSTALL(a,D,"DL_RobotConfig-icon",resDir)
	
    QObject::connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

    ApplicationTut* mw = new ApplicationTut();
    mw->setWindowTitle( QObject::tr( "TIT_SAMPLE" ) + QString(" [%1 %2]").arg(__DATE__).arg(__TIME__) );
    QString aResName( resDir + QString( "/" ) + QObject::tr( "ICON_SAMPLE" ) );
    	
    mw->setWindowIcon( QPixmap( aResName ) );
    mw->show();
    
    return a.exec();
}

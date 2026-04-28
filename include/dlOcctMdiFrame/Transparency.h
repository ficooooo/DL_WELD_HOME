#ifndef DIALOGTRANSPARENCY_H
#define DIALOGTRANSPARENCY_H

#include <QDialog>
#include <QWidget>

class DialogTransparency : public QDialog
{
	Q_OBJECT
public:
	DialogTransparency ( QWidget * parent=0, Qt::WindowFlags f=0, bool modal=true );
	~DialogTransparency();
signals:
	void sendTransparencyChanged(int value);
};

#endif
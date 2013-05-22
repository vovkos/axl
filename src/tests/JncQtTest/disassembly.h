#ifndef _DISASSEMBLY_H
#define _DISASSEMBLY_H

class Disassembly : public QWidget
{
	Q_OBJECT

public:
	Disassembly(QWidget *parent);

	QSize sizeHint() const { return QSize(300, 50); }	
};


#endif


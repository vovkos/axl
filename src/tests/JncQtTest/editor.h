#ifndef EDITOR_H
#define EDITOR_H

class Editor : public QPlainTextEdit
{
	Q_OBJECT

public:
	Editor(QWidget *parent = 0);

	void highlightSingleLine(const QTextCursor &cursor, const QColor &back,
		QColor *fore = 0);

	void select(int startPos, int endPos);
	void selectLine(int line, bool isHighlighted = false);	

	int posFromLine(int line);
};

#endif
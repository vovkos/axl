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

	void appendFormat_va (const char* format, va_list va)
	{
		rtl::CString Text;
		Text.Format_va (format, va);

		moveCursor (QTextCursor::End);
		insertPlainText (QString::fromUtf8 (Text, Text.GetLength ()));
	}

	void appendFormat (const char* format, ...)
	{
		va_list va;
		va_start (va, format);
		appendFormat_va (format, va);
	}
};

#endif
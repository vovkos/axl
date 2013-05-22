#include "pch.h"
#include "output.h"
#include "mainwindow.h"
#include "mdichild.h"
#include "moc_output.cpp"

#define LINE_SELECTION_BACK		QColor(51, 51, 183)

Output::Output(QWidget *parent)
	: OutputBase(parent)
{
	setReadOnly(true);
	setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
	setLineWrapMode (QPlainTextEdit::NoWrap);
}
 
void Output::mouseDoubleClickEvent(QMouseEvent *e)
{
	int documentLine;
	QString filePath;

	if(parseLine(textCursor(), documentLine, filePath))
	{	
		MdiChild *child = GetMainWindow()->findMdiChild(filePath);
		if (child)
		{
			QColor foreColor(Qt::white);
			highlightSingleLine(textCursor(), LINE_SELECTION_BACK, &foreColor);

			child->selectLine(documentLine);
			child->setFocus();

			e->ignore();
		}
	}
	else
	{
		e->accept();
		OutputBase::mouseDoubleClickEvent(e);
	}
}

bool Output::parseLine(const QTextCursor &cursor, int &documentLine,
	QString &filePath)
{
	QString text = cursor.block().text();

	QRegExp regExp("\\((\\d)+,(\\d)+\\):");
	int pos = regExp.indexIn(text);

	if(pos == -1)
		return false;

	filePath = text.left(pos);

	QString matchedText = regExp.capturedTexts().first();
	QString lineNumber = matchedText.left(matchedText.indexOf(','));
	lineNumber.remove(0, 1);

	documentLine = lineNumber.toInt() - 1;

	return true;
}
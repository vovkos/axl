//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_GUI_QTRAGELSYNTAXHIGHLIGHTER_H

#include "axl_lex_Ragel.h"

namespace axl {
namespace gui {

//..............................................................................

template <typename T>
class QtRagelSyntaxHighlighter:
	public QSyntaxHighlighter,
	public lex::Ragel
{
public:
    explicit
	QtRagelSyntaxHighlighter(QObject* parent):
		QSyntaxHighlighter(parent)
	{
	}

	explicit
	QtRagelSyntaxHighlighter(QTextDocument* parent):
		QSyntaxHighlighter(parent)
	{
	}

    virtual
	void
	highlightBlock(const QString &text)
	{
		QByteArray data = text.toUtf8();

		Ragel::clear();
		static_cast<T*>(this)->init();
		Ragel::setSource(sl::StringRef(data.constData(), data.size()));
		static_cast<T*>(this)->exec();
	}

protected:
	void
	highlightLastToken(const QTextCharFormat& format)
	{
		setFormat(ts - m_begin, te - ts, format);
	}

	void
	highlightLastToken(const QColor& color)
	{
		setFormat(ts - m_begin, te - ts, color);
	}

	// to be implemented in lexer instance:

	// void
	// init ()
	// {
	// }

	// void
	// exec ()
	// {
	// }
};

//..............................................................................

} // namespace gui
} // namespace axl

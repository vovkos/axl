#include "pch.h"
#include "axl_gui_QtFont.h"
#include "axl_gui_QtEngine.h"

namespace axl {
namespace gui {

//.............................................................................

bool
getFontDescFromFontInfo (
	const QFontInfo& fontInfo,
	TFontDesc* fontDesc
	)
{
	QString familyName = fontInfo.family ();
	QByteArray familyNameUtf = familyName.toUtf8 ();	
	size_t length = familyNameUtf.size ();
	
	if (length >= countof (fontDesc->m_faceName))
		length = countof (fontDesc->m_faceName) - 1;
	
	memcpy (fontDesc->m_faceName, familyNameUtf, length);
	fontDesc->m_faceName [length] = 0;	
	fontDesc->m_pointSize = fontInfo.pointSize ();
	
	if (fontInfo.weight () >= QFont::Bold)
		fontDesc->m_flags |= EFontFlag_Bold;
	
	if (fontInfo.italic ())
		fontDesc->m_flags |= EFontFlag_Italic;
	
	if (fontInfo.underline ())
		fontDesc->m_flags |= EFontFlag_Underline;

	if (fontInfo.strikeOut ())
		fontDesc->m_flags |= EFontFlag_Strikeout;

	return true;
}

//.............................................................................

CQtFont::CQtFont ()
{
	m_engine = CQtEngine::getSingleton ();
}

TSize
CQtFont::calcTextSize_qt (const QString& string)
{
	QFontMetrics qtFontMetrics (m_qtFont);
		
	TSize size;
	size.m_width = qtFontMetrics.width (string);
	size.m_height = qtFontMetrics.height ();
	return size;
}

TSize
CQtFont::calcTextSize_utf8 (
	const utf8_t* text,
	size_t length
	)
{
	char buffer [256];
	rtl::CString_utf16 string (ref::EBuf_Stack, buffer, sizeof (buffer));
	string.copy (text, length);

	return calcTextSize_qt (QString ((const QChar*) string.cc (), string.getLength ()));
}

TSize
CQtFont::calcTextSize_utf32 (
	const utf32_t* text,
	size_t length
	)
{
	char buffer [256];
	rtl::CString_utf16 string (ref::EBuf_Stack, buffer, sizeof (buffer));
	string.copy (text, length);

	return calcTextSize_qt (QString ((const QChar*) string.cc (), string.getLength ()));
}


//.............................................................................

} // namespace gui
} // namespace axl

#include "pch.h"
#include "axl_gui_QtFont.h"
#include "axl_gui_QtEngine.h"

namespace axl {
namespace gui {

//.............................................................................

bool
GetFontDescFromFontInfo (
	const QFontInfo& FontInfo,
	TFontDesc* pFontDesc
	)
{
	QString FamilyName = FontInfo.family ();
	QByteArray FamilyNameUtf = FamilyName.toUtf8 ();	
	size_t Length = FamilyNameUtf.size ();
	
	if (Length >= countof (pFontDesc->m_FaceName))
		Length = countof (pFontDesc->m_FaceName) - 1;
	
	memcpy (pFontDesc->m_FaceName, FamilyNameUtf, Length);
	pFontDesc->m_FaceName [Length] = 0;	
	pFontDesc->m_PointSize = FontInfo.pointSize ();
	
	if (FontInfo.weight () >= QFont::Bold)
		pFontDesc->m_Flags |= EFontFlag_Bold;
	
	if (FontInfo.italic ())
		pFontDesc->m_Flags |= EFontFlag_Italic;
	
	if (FontInfo.underline ())
		pFontDesc->m_Flags |= EFontFlag_Underline;

	if (FontInfo.strikeOut ())
		pFontDesc->m_Flags |= EFontFlag_Strikeout;

	return true;
}

//.............................................................................

CQtFont::CQtFont ()
{
	m_pEngine = CQtEngine::GetSingleton ();
}

TSize
CQtFont::CalcTextSize_qt (const QString& String)
{
	QFontMetrics QtFontMetrics (m_QtFont);
		
	TSize Size;
	Size.m_Width = QtFontMetrics.width (String);
	Size.m_Height = QtFontMetrics.height ();
	return Size;
}

TSize
CQtFont::CalcTextSize_utf8 (
	const utf8_t* pText,
	size_t Length
	)
{
	char Buffer [256];
	rtl::CString_utf16 String (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	String.Copy (pText, Length);

	return CalcTextSize_qt (QString ((const QChar*) String.cc (), String.GetLength ()));
}

TSize
CQtFont::CalcTextSize_utf32 (
	const utf32_t* pText,
	size_t Length
	)
{
	char Buffer [256];
	rtl::CString_utf16 String (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	String.Copy (pText, Length);

	return CalcTextSize_qt (QString ((const QChar*) String.cc (), String.GetLength ()));
}


//.............................................................................

} // namespace gui
} // namespace axl

#include "pch.h"
#include "axl_gui_TextAttr.h"

namespace axl {
namespace gui {

//.............................................................................

void
TextAttr::parse (
	const char* string,
	const char** end
	)
{
	ColorAttr::parse (string, &string);

	while (isspace (*string))
		string++;

	if (*string == '+')
		m_fontFlags = parseFontFlagString (string + 1, &string);

	if (end)
		*end = string;
}

void
TextAttr::parseOverlay (
	const TextAttr& baseAttr,
	const char* string,
	const char** end
	)
{
	TextAttr overlayAttr;
	overlayAttr.parse (string, end);
	overlay (baseAttr, overlayAttr);
}

//.............................................................................

} // namespace gui
} // namespace axl

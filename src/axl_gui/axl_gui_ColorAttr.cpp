#include "pch.h"
#include "axl_gui_ColorAttr.h"

namespace axl {
namespace gui {

//.............................................................................

void
ColorAttr::parse (
	const char* string,
	const char** end
	)
{
	m_foreColor = parseColorString (string, &string);

	while (isspace (*string))
		string++;

	if (*string == '|')
		m_backColor = parseColorString (string + 1, &string);

	if (end)
		*end = string;
}

void
ColorAttr::parseOverlay (
	const ColorAttr& baseAttr,
	const char* string,
	const char** end
	)
{
	ColorAttr overlayAttr;
	overlayAttr.parse (string, end);
	overlay (baseAttr, overlayAttr);
}

//.............................................................................

} // namespace gui
} // namespace axl
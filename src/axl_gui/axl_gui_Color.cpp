#include "pch.h"
#include "axl_gui_Color.h"
#include "axl_mt_CallOnce.h"

namespace axl {
namespace gui {

//.............................................................................

uint_t
Palette::getColorRgb (uint_t color)
{
	if (!(color & ColorFlag_Index))
		return color;

	size_t i = color & ColorFlag_IndexMask;
	color = i < m_count ? m_colorArray [i] : -1;
			
	if (color & ColorFlag_Index) // allow two-staged index lookups
	{
		i = color & ColorFlag_IndexMask;
		color = i < m_count ? m_colorArray [i] : -1;
	}

	return color;
}

//.............................................................................

uint_t
parseColorString (
	const char* string,
	const char** end_o
	)
{
	if (!string)
	{
		if (end_o)
			*end_o = NULL;

		return StdColor_Black;
	}

	uint_t color = StdColor_Black;

	char* end;

	if (string[0] == '#')
	{
		uint_t rgb = strtol (string + 1, &end, 16);
		if (end != string)
			color = rgb;
	}
	else
	{
		uint_t index = strtol (string, &end, 10);
		if (end != string)
			color = ColorFlag_Index | index;
	}

	if (end_o)
		*end_o = end;

	return color;
}

//.............................................................................

} // namespace gui
} // namespace axl
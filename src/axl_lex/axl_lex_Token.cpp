#include "pch.h"
#include "axl_lex_ParseError.h"
#include "axl_sl_Unpacker.h"

namespace axl {
namespace lex {

//.............................................................................

void
LineCol::incrementalCount (
	const char* p,
	size_t length
	)
{
	if (length == -1)
		length = strlen_s (p);

	const char* end = p + length;
	const char* line = p;

	for (; p < end; p++)
		if (*p == '\n')
		{
			m_line++;
			line = p + 1;
		}

	m_col = (int) (p - line);
}	

//.............................................................................

} // namespace lex
} // namespace axl


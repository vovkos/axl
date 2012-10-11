
/* #line 1 "D:/Prj/Ninja/axl3/axl_lua/axl_st_Lexer.rl" */
#include "stdafx.h"
#include "axl_st_Lexer.h"

// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)

namespace axl { 
namespace st { 

//.............................................................................


/* #line 2 "axl_st_Lexer.cpp" */
static const char _axl_lua_st_actions[] = {
	0, 1, 1, 1, 2, 1, 3, 1, 
	5, 1, 6, 1, 8, 1, 9, 1, 
	10, 1, 12, 1, 13, 1, 14, 2, 
	0, 4, 2, 0, 7, 2, 0, 11
	
};

static const char _axl_lua_st_key_offsets[] = {
	0, 3, 9, 16, 17, 20
};

static const char _axl_lua_st_trans_keys[] = {
	10, 36, 37, 40, 95, 65, 90, 97, 
	122, 95, 48, 57, 65, 90, 97, 122, 
	123, 10, 123, 125, 10, 40, 41, 0
};

static const char _axl_lua_st_single_lengths[] = {
	3, 2, 1, 1, 3, 1
};

static const char _axl_lua_st_range_lengths[] = {
	0, 2, 3, 0, 0, 1
};

static const char _axl_lua_st_index_offsets[] = {
	0, 4, 9, 14, 16, 20
};

static const char _axl_lua_st_trans_targs[] = {
	0, 1, 3, 0, 0, 2, 2, 2, 
	0, 2, 2, 2, 2, 0, 0, 0, 
	4, 4, 4, 4, 5, 5, 5, 0, 
	0, 0, 0
};

static const char _axl_lua_st_trans_actions[] = {
	29, 0, 0, 17, 15, 0, 0, 0, 
	21, 0, 0, 0, 0, 19, 13, 21, 
	23, 5, 5, 7, 26, 9, 11, 21, 
	19, 21, 0
};

static const char _axl_lua_st_to_state_actions[] = {
	1, 0, 0, 0, 1, 1
};

static const char _axl_lua_st_from_state_actions[] = {
	3, 0, 0, 0, 3, 3
};

static const char _axl_lua_st_eof_trans[] = {
	0, 26, 25, 26, 0, 0
};

static const int axl_lua_st_start = 0;
static const int axl_lua_st_first_final = 0;
static const int axl_lua_st_error = -1;

static const int axl_lua_st_en_user_code = 4;
static const int axl_lua_st_en_user_data = 5;
static const int axl_lua_st_en_main = 0;


/* #line 69 "D:/Prj/Ninja/axl3/axl_lua/axl_st_Lexer.rl" */


//.............................................................................

void 
CLexer::Init ()
{
	
/* #line 66 "axl_st_Lexer.cpp" */
	{
	cs = axl_lua_st_start;
	ts = 0;
	te = 0;
	act = 0;
	}

/* #line 77 "D:/Prj/Ninja/axl3/axl_lua/axl_st_Lexer.rl" */
}

bool
CLexer::Exec ()
{
	
/* #line 72 "axl_st_Lexer.cpp" */
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
_resume:
	_acts = _axl_lua_st_actions + _axl_lua_st_from_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 2:
/* #line 1 "NONE" */
	{ts = p;}
	break;
/* #line 89 "axl_st_Lexer.cpp" */
		}
	}

	_keys = _axl_lua_st_trans_keys + _axl_lua_st_key_offsets[cs];
	_trans = _axl_lua_st_index_offsets[cs];

	_klen = _axl_lua_st_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _axl_lua_st_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
_eof_trans:
	cs = _axl_lua_st_trans_targs[_trans];

	if ( _axl_lua_st_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _axl_lua_st_actions + _axl_lua_st_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 29 "D:/Prj/Ninja/axl3/axl_lua/axl_st_Lexer.rl" */
	{ NewLine (p + 1); }
	break;
	case 3:
/* #line 41 "D:/Prj/Ninja/axl3/axl_lua/axl_st_Lexer.rl" */
	{te = p+1;{ CreateToken (ts [0]); }}
	break;
	case 4:
/* #line 42 "D:/Prj/Ninja/axl3/axl_lua/axl_st_Lexer.rl" */
	{te = p+1;}
	break;
	case 5:
/* #line 43 "D:/Prj/Ninja/axl3/axl_lua/axl_st_Lexer.rl" */
	{te = p+1;}
	break;
	case 6:
/* #line 51 "D:/Prj/Ninja/axl3/axl_lua/axl_st_Lexer.rl" */
	{te = p+1;{ CreateToken (ts [0]); }}
	break;
	case 7:
/* #line 52 "D:/Prj/Ninja/axl3/axl_lua/axl_st_Lexer.rl" */
	{te = p+1;}
	break;
	case 8:
/* #line 53 "D:/Prj/Ninja/axl3/axl_lua/axl_st_Lexer.rl" */
	{te = p+1;}
	break;
	case 9:
/* #line 61 "D:/Prj/Ninja/axl3/axl_lua/axl_st_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_OpenCode); }}
	break;
	case 10:
/* #line 62 "D:/Prj/Ninja/axl3/axl_lua/axl_st_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_OpenData); }}
	break;
	case 11:
/* #line 64 "D:/Prj/Ninja/axl3/axl_lua/axl_st_Lexer.rl" */
	{te = p+1;}
	break;
	case 12:
/* #line 65 "D:/Prj/Ninja/axl3/axl_lua/axl_st_Lexer.rl" */
	{te = p+1;}
	break;
	case 13:
/* #line 63 "D:/Prj/Ninja/axl3/axl_lua/axl_st_Lexer.rl" */
	{te = p;p--;{ CreateStringToken (EToken_Data, 1); }}
	break;
	case 14:
/* #line 65 "D:/Prj/Ninja/axl3/axl_lua/axl_st_Lexer.rl" */
	{te = p;p--;}
	break;
/* #line 192 "axl_st_Lexer.cpp" */
		}
	}

_again:
	_acts = _axl_lua_st_actions + _axl_lua_st_to_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 1:
/* #line 1 "NONE" */
	{ts = 0;}
	break;
/* #line 203 "axl_st_Lexer.cpp" */
		}
	}

	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	if ( _axl_lua_st_eof_trans[cs] > 0 ) {
		_trans = _axl_lua_st_eof_trans[cs] - 1;
		goto _eof_trans;
	}
	}

	}

/* #line 83 "D:/Prj/Ninja/axl3/axl_lua/axl_st_Lexer.rl" */

	bool Result = cs != axl_lua_st_error;
	if (!Result)
	{
		err::SetStringError ("lexer error");
		return false;
	}

	return Result;
}

int
CLexer::GetMachineState (ELexerMachine Machine)
{
	switch (Machine)
	{
	case ELexerMachine_Main:
		return axl_lua_st_en_main;

	case ELexerMachine_UserCode:
		return axl_lua_st_en_user_code;

	case ELexerMachine_UserData:
		return axl_lua_st_en_user_data;

	default:
		ASSERT (false);
		return axl_lua_st_en_main;
	}
}

//.............................................................................

} // namespace st { 
} // namespace axl { 

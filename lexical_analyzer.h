#pragma once
#include <iostream>
#include "vector.h"
#include <fstream>
#include <string.h>
#include <vector>
#include <map>
#include <list>
#include <tuple>
#include <stack>

enum Lexem {
	lex_semicolon_mark,
	lex_dim,
	lex_as,
	lex_int,
	lex_vec,
	lex_set,
	lex_to,
	lex_while,
	lex_do,
	lex_for,
	lex_if,
	lex_then,
	lex_else,
	lex_cin_mark,
	lex_cout_mark,
	lex_cin,
	lex_cout,
	lex_mark,
	lex_colon_mark,
	lex_comma_mark,
	lex_goto_mark,
	lex_switch,
	lex_case,
	lex_default,
	lex_exception,
	lex_remark,
	lex_begin,
	lex_end,
	lex_scalarprod,
	lex_subvec,
	lex_shiftr,
	lex_shiftl,
	lex_at,
	lex_conc,
	lex_equal_mark,
	lex_variable,
	lex_constant,
	lex_aur_op,
	lex_relation,
	lex_error,
	lex_end_mark,
	lex_round_bracket_open,
	lex_round_bracket_close
};
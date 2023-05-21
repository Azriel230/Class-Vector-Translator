#pragma once
#include <string>
#include <set>
#include <map>
#include <fstream>
#include <iostream>
#include <utility>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>

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

class Syntactical_Symbol
{
public:
	bool m_is_terminal_symbol = false;
	size_t m_id_symbol = lex_error;
	std::string m_name_symbol;

public:
	friend bool operator<(const Syntactical_Symbol& obj1, const Syntactical_Symbol& obj2)
	{
		return obj1.m_id_symbol < obj2.m_id_symbol;
	}
	friend bool operator==(const Syntactical_Symbol& obj1, const Syntactical_Symbol& obj2)
	{
		return obj1.m_id_symbol == obj2.m_id_symbol;
	}
};

class CF_Grammar
{
protected:
	std::map<std::string, size_t> m_alphabet;

	std::map<std::string, size_t> m_terminals;
	std::map<std::string, size_t> m_non_terminals;

	struct Grammar_rule
	{
		Syntactical_Symbol m_non_terminal;
		std::vector<Syntactical_Symbol> m_right_part;
	};

	std::multimap<Syntactical_Symbol, Grammar_rule> m_rules;

public:
	CF_Grammar(const std::string filename);
	
	void Fill_terminal_symbols();
	void print_rules();
	size_t get_id(const std::string& symbol);

	std::set<Syntactical_Symbol> FIRST1(const Syntactical_Symbol& S);
	std::set<Syntactical_Symbol> FIRST1_REC(const Syntactical_Symbol& S, std::set<Syntactical_Symbol>& seen);
};
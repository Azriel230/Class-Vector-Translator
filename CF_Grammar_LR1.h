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
#include <list>
#include <stack>
#include "vector.h"

// Перечисление содержит константы для обозначения классов символьных лексем: БУКВА, ЦИФРА, АР_ОП, ОТНОШ, ПРОБЕЛ, LF, ;, :, ,, [, ], (, ), -, ОШИБКА, КОНЕЦ.
enum SymbolicTokenClass {
	Letter,
	Digit,
	Aur_OP,
	Relation,
	Space,
	LF,
	Semicolon,
	Colon,
	Comma,
	OpenSquareBracket,
	CloseSquareBracket,
	OpenRoundBracket,
	CloseRoundBracket,
	Error,
	EndOfFile
};

// Перечисление содержит константы для обозначения состояний автомата и одну дополнительную константу s_Stop.
enum States {
	start_lex,
	read_keyword,
	read_rel,
	var,
	constant,
	cr_rel,
	const_vec1,
	const_vec2,
	cout_mark1,
	remark1,
	remark2,
	error1,
	kw_var,
	kw_var_bracket,
	kw_var_default,
	goto_space,
	goto_mark1,
	goto_mark2,
	kw_var_bracket2,
	kw_var_to,
	arif,
	s_STOP
};

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

struct Variable
{
	std::string name;
	int type; //0 - int; 1 - vec; -1 - mark
	int integer_value;
	Vector vector_value;
};

class Syntactical_Symbol
{
public:
	bool m_is_terminal;
	std::string m_name;
	size_t m_id;

	Syntactical_Symbol(const Syntactical_Symbol& obj)
	{
		m_is_terminal = obj.m_is_terminal;
		m_name = obj.m_name;
		m_id = obj.m_id;
	}

	Syntactical_Symbol(bool flag = false, std::string name = "epsilon", size_t id = -1)
	{
		m_is_terminal = flag;
		m_name = name;
		m_id = id;
	}

	Syntactical_Symbol& operator= (const Syntactical_Symbol& obj)
	{
		m_is_terminal = obj.m_is_terminal;
		m_name = obj.m_name;
		m_id = obj.m_id;
		return *this;
	}

	friend bool operator<(const Syntactical_Symbol& obj1, const Syntactical_Symbol& obj2)
	{
		return obj1.m_id < obj2.m_id;
	}
	friend bool operator==(const Syntactical_Symbol& obj1, const Syntactical_Symbol& obj2)
	{
		return obj1.m_id == obj2.m_id;
	}
};

class Grammar_rule
{
public:
	Syntactical_Symbol m_left_part; //non terminal
	std::vector<Syntactical_Symbol> m_right_part;
	Grammar_rule()
	{
		m_left_part = Syntactical_Symbol();
		m_right_part = std::vector<Syntactical_Symbol>();
	}
	Grammar_rule(const Grammar_rule& obj)
	{
		m_left_part = obj.m_left_part;
		m_right_part = obj.m_right_part;
	}
};

//Действия для очередного шага
enum Actions { TRANSFER, CONVOLUTION, ACCEPT, ERROR }; //ПЕРЕНОС, СВЕРТКА, ДОСТУП, ОШИБКА

struct Action
{
	Actions act;
	Grammar_rule rule_conv; //правило по которому идет свертка
};

struct LR1_Situation
{
	Syntactical_Symbol m_left_part;
	std::vector<Syntactical_Symbol> m_rp_before_point;
	std::vector<Syntactical_Symbol> m_rp_after_point;
	std::vector<Syntactical_Symbol> m_avan_chain;
	bool friend operator== (const LR1_Situation& obj1, const LR1_Situation& obj2)
	{
		if (obj1.m_left_part != obj2.m_left_part) return false;
		if (obj1.m_rp_before_point != obj2.m_rp_before_point) return false;
		if (obj1.m_rp_after_point != obj2.m_rp_after_point) return false;
		if (obj1.m_avan_chain.size() != obj2.m_avan_chain.size()) return false;
		for (int i = 0; i < obj1.m_avan_chain.size(); i++)
			if (find(obj1.m_avan_chain.begin(), obj1.m_avan_chain.end(), obj2.m_avan_chain[i]) == obj1.m_avan_chain.end()) return false;
		return true;
	}
};

//Множество LR1 ситуаций
struct TableLR1
{
	std::vector<LR1_Situation> m_situations;
	bool friend operator== (const TableLR1& obj1, const TableLR1& obj2)
	{
		if (obj1.m_situations.size() != obj2.m_situations.size()) return false;
		for (int i = 0; i < obj1.m_situations.size(); i++)
			if (obj1.m_situations[i] != obj2.m_situations[i]) return false;
		return true;
	}
};

class CF_Grammar
{
public:
	std::map<std::string, Syntactical_Symbol> m_terminals;
	std::map<std::string, Syntactical_Symbol> m_non_terminals;
	std::multimap<Syntactical_Symbol, Grammar_rule> m_rules;
	std::list<std::pair<Syntactical_Symbol, int>> m_input_word; //lexem list
	std::vector <std::pair<Syntactical_Symbol, TableLR1>> m_magazine;
	TableLR1 m_curr_table;
	std::vector<std::string> m_ActionListStr = { "TRANSFER", "CONVOLUTION", "ACCEPT", "ERROR" };
	bool m_synth_error_flag;

	int m_countOfLR1Situations;

	CF_Grammar(std::string filename);
	//~CF_Grammar();

	void Fill_terminal_symbols();
	void print_rules();
	size_t get_id(const std::string& symbol);

	std::set<Syntactical_Symbol> FIRST1(const Syntactical_Symbol& S);
	std::set<Syntactical_Symbol> FIRST1_REC(const Syntactical_Symbol& S, std::set<Syntactical_Symbol>& seen);
	std::vector<Syntactical_Symbol> FOLLOW(const Syntactical_Symbol& symbol);

	Action get_action(TableLR1 table, Syntactical_Symbol symbol);
	void GOTO(TableLR1 table, Syntactical_Symbol symbol);
	void GenerateTableLR1(TableLR1 table);
	void PrepareInputWord(const std::list<std::tuple<Lexem, long long int, int>>& lexem_list);
	void synt_parse();
	/////////////////////////

	void print_situation(LR1_Situation sit);
	void print_rule(Grammar_rule rule);
	void FixGenerateTableLR1(TableLR1& table);
};
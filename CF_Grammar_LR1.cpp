#include "CF_Grammar_LR1.h"
#include <string>

void CF_Grammar::Fill_terminal_symbols()
{
	m_terminals.emplace("semicolon_mark", lex_semicolon_mark);
	m_terminals.emplace("dim", lex_dim);
	m_terminals.emplace("as", lex_as);
	m_terminals.emplace("int", lex_int);
	m_terminals.emplace("vec", lex_vec);
	m_terminals.emplace("set", lex_set);
	m_terminals.emplace("to", lex_to);
	m_terminals.emplace("while", lex_while);
	m_terminals.emplace("do", lex_do);
	m_terminals.emplace("for", lex_for);
	m_terminals.emplace("if", lex_if);
	m_terminals.emplace("then", lex_then);
	m_terminals.emplace("else", lex_else);
	m_terminals.emplace("cin_mark", lex_cin_mark);
	m_terminals.emplace("cout_mark", lex_cout_mark);
	m_terminals.emplace("cin", lex_cin);
	m_terminals.emplace("cout", lex_cout);
	m_terminals.emplace("mark", lex_mark);
	m_terminals.emplace("colon_mark", lex_colon_mark);
	m_terminals.emplace("comma_mark", lex_comma_mark);
	m_terminals.emplace("goto_mark", lex_goto_mark);
	m_terminals.emplace("switch", lex_switch);
	m_terminals.emplace("case", lex_case);
	m_terminals.emplace("default", lex_default);
	m_terminals.emplace("exception", lex_exception);
	m_terminals.emplace("remark", lex_remark);
	m_terminals.emplace("begin", lex_begin);
	m_terminals.emplace("end", lex_end);
	m_terminals.emplace("scalarprod", lex_scalarprod);
	m_terminals.emplace("subvec", lex_subvec);
	m_terminals.emplace("shiftr", lex_shiftr);
	m_terminals.emplace("shiftl", lex_shiftl);
	m_terminals.emplace("at", lex_at);
	m_terminals.emplace("conc", lex_conc);
	m_terminals.emplace("equal_mark", lex_equal_mark);
	m_terminals.emplace("variable", lex_variable);
	m_terminals.emplace("constant", lex_constant);
	m_terminals.emplace("aur_op", lex_aur_op);
	m_terminals.emplace("relation", lex_relation);
	m_terminals.emplace("end_mark", lex_end_mark);
	m_terminals.emplace("round_bracket_open", lex_round_bracket_open);
	m_terminals.emplace("round_bracket_close", lex_round_bracket_close);
}

CF_Grammar::CF_Grammar(const std::string filename)
{
	Grammar_rule current_rule;
	Syntactical_Symbol symbol;
	std::string word;

	Fill_terminal_symbols();

	size_t index_non_term_sym = m_terminals.size();

	std::ifstream input_file(filename);
	if (!input_file)
	{
		std::cout << "In CF_grammar::CF_grammar error openning " << filename << std::endl;
		return;
	}

	while (input_file.peek() != EOF)
	{
		input_file >> current_rule.m_non_terminal.m_name_symbol;
		if (current_rule.m_non_terminal.m_name_symbol[0] != '{')
		{
			std::cout << "Wrong input format";
			input_file.close();
			exit(EXIT_FAILURE);
		}

		current_rule.m_non_terminal.m_is_terminal_symbol = false;

		if (m_non_terminals.contains(current_rule.m_non_terminal.m_name_symbol))
			current_rule.m_non_terminal.m_id_symbol = m_non_terminals[current_rule.m_non_terminal.m_name_symbol];
		else
		{
			current_rule.m_non_terminal.m_id_symbol = index_non_term_sym;
			m_non_terminals.emplace(current_rule.m_non_terminal.m_name_symbol, index_non_term_sym);
			index_non_term_sym++;
		}

		while (input_file.peek() != '\n' && input_file.peek() != EOF)          //÷икл идЄт до конца строки, считыва€ слова и определ€€, €вл€ютс€ ли они термилалами, или нет
		{
			input_file >> word;
			symbol.m_name_symbol = word;

			if (word[0] != '{') 
			{
				symbol.m_is_terminal_symbol = true;
				if (!m_non_terminals.contains(word)) //double check
				{
					std::cout << "unexpected terminal";
					input_file.close();
					exit(EXIT_FAILURE);
				}

				symbol.m_id_symbol = m_non_terminals[word];
			}
			else
			{
				symbol.m_is_terminal_symbol = false;
				if (m_non_terminals.contains(word))
					symbol.m_id_symbol = m_non_terminals[word];
				else
				{
					symbol.m_id_symbol = index_non_term_sym;
					m_non_terminals.emplace(word, index_non_term_sym);
					index_non_term_sym++;
				}
			}
			current_rule.m_right_part.push_back(symbol);

			while (input_file.peek() == ' ')
				input_file.ignore();
		}

		m_rules.emplace(current_rule.m_non_terminal, current_rule);
		current_rule.m_right_part.clear();

	}

	input_file.close();
	
}

void CF_Grammar::print_rules()
{
	for (auto it : m_rules)
	{
		std::cout << it.second.m_non_terminal.m_name_symbol << " -> ";
		for (auto that : it.second.m_right_part)
			std::cout << that.m_name_symbol << ' ';
		std::cout << std::endl;
	}
}

size_t CF_Grammar::get_id(const std::string& symbol) //double check
{
	if (m_non_terminals.contains(symbol))
		return m_non_terminals[symbol];
	return 0;
}

std::set<Syntactical_Symbol> CF_Grammar::FIRST1(const Syntactical_Symbol& S)
{
	std::set<Syntactical_Symbol> seen;

	return FIRST1_REC(S, seen);
}

std::set<Syntactical_Symbol> CF_Grammar::FIRST1_REC(const Syntactical_Symbol& S, std::set<Syntactical_Symbol>& seen)
{
	std::set<Syntactical_Symbol> first_symbols; ///////// <n> -> aboba;   <n> -> <t>aboba

	size_t rules_amount = m_rules.count(S);
	auto it = m_rules.find(S);

	for (size_t i = 0; it != m_rules.end() && i < rules_amount; ++i, ++it)
	{
		if ((*it).second.m_right_part[0].m_is_terminal_symbol)
			first_symbols.insert((*it).second.m_right_part[0]);
		else if (seen.find((*it).second.m_right_part[0]) == seen.end())
		{
			seen.insert((*it).second.m_right_part[0]);
			first_symbols.merge(FIRST1_REC((*it).second.m_right_part[0], seen));
		}
	}

	return first_symbols;
}
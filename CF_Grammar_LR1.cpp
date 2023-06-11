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
#include "CF_Grammar_LR1.h"

void CF_Grammar::Fill_terminal_symbols()
{
	m_terminals.emplace("constant", Syntactical_Symbol(true, "constant", 0));
	m_terminals.emplace("variable", Syntactical_Symbol(true, "variable", 1));
	m_terminals.emplace("int", Syntactical_Symbol(true, "int", 2));
	m_terminals.emplace("vec", Syntactical_Symbol(true, "vec", 3));
	m_terminals.emplace("==", Syntactical_Symbol(true, "==", 4));
	m_terminals.emplace("!=", Syntactical_Symbol(true, "!=", 5));
	m_terminals.emplace("<", Syntactical_Symbol(true, "<", 6));
	m_terminals.emplace(">", Syntactical_Symbol(true, ">", 7));
	m_terminals.emplace("<=", Syntactical_Symbol(true, "<=", 8));
	m_terminals.emplace(">=", Syntactical_Symbol(true, ">=", 9));
	m_terminals.emplace("+", Syntactical_Symbol(true, "+", 10));
	m_terminals.emplace("-", Syntactical_Symbol(true, "-", 11));
	m_terminals.emplace("*", Syntactical_Symbol(true, "*", 12));
	m_terminals.emplace("/", Syntactical_Symbol(true, "/", 13));
	m_terminals.emplace("%", Syntactical_Symbol(true, "%", 14));
	m_terminals.emplace("=", Syntactical_Symbol(true, "=", 15));
	m_terminals.emplace("dim", Syntactical_Symbol(true, "dim", 16));
	m_terminals.emplace("as", Syntactical_Symbol(true, "as", 17));
	m_terminals.emplace("set", Syntactical_Symbol(true, "set", 18));
	m_terminals.emplace("to", Syntactical_Symbol(true, "to", 19));
	m_terminals.emplace("while", Syntactical_Symbol(true, "while", 20));
	m_terminals.emplace("do", Syntactical_Symbol(true, "do", 21));
	m_terminals.emplace("for", Syntactical_Symbol(true, "for", 22));
	m_terminals.emplace("if", Syntactical_Symbol(true, "if", 23));
	m_terminals.emplace("then", Syntactical_Symbol(true, "then", 24));
	m_terminals.emplace("else", Syntactical_Symbol(true, "else", 25));
	m_terminals.emplace("<-", Syntactical_Symbol(true, "<-", 26));
	m_terminals.emplace("->", Syntactical_Symbol(true, "->", 27));
	m_terminals.emplace("cin", Syntactical_Symbol(true, "cin", 28));
	m_terminals.emplace("cout", Syntactical_Symbol(true, "cout", 29));
	m_terminals.emplace("mark_goto", Syntactical_Symbol(true, "mark_goto", 30)); //����� ���� L:
	m_terminals.emplace("goto", Syntactical_Symbol(true, "goto", 31));
	m_terminals.emplace("switch", Syntactical_Symbol(true, "switch", 32));
	m_terminals.emplace("case", Syntactical_Symbol(true, "case", 33));
	m_terminals.emplace("default", Syntactical_Symbol(true, "default", 34));
	m_terminals.emplace("exception", Syntactical_Symbol(true, "exception", 35));
	m_terminals.emplace("begin", Syntactical_Symbol(true, "begin", 36));
	m_terminals.emplace("end", Syntactical_Symbol(true, "end", 37));
	m_terminals.emplace("scalarprod", Syntactical_Symbol(true, "scalarprod", 38));
	m_terminals.emplace("subvec", Syntactical_Symbol(true, "subvec", 39));
	m_terminals.emplace("shiftr", Syntactical_Symbol(true, "shiftr", 40));
	m_terminals.emplace("shiftl", Syntactical_Symbol(true, "shiftl", 41));
	m_terminals.emplace("at", Syntactical_Symbol(true, "at", 42));
	m_terminals.emplace("conc", Syntactical_Symbol(true, "conc", 43));
	m_terminals.emplace("(", Syntactical_Symbol(true, "(", 44));
	m_terminals.emplace(")", Syntactical_Symbol(true, ")", 45));
	m_terminals.emplace(";", Syntactical_Symbol(true, ";", 46));
	m_terminals.emplace(":", Syntactical_Symbol(true, ":", 47));
	m_terminals.emplace(",", Syntactical_Symbol(true, ",", 48));
	m_terminals.emplace("remark", Syntactical_Symbol(true, "remark", 49)); //?
	m_terminals.emplace("error", Syntactical_Symbol(true, "error", 50));
	m_terminals.emplace("mark_end", Syntactical_Symbol(true, "mark_end", 51));

	m_terminals.emplace("epsilon", Syntactical_Symbol(true, "epsilon", -1));

	m_terminals.emplace("a", Syntactical_Symbol(true, "a", 60));
	m_terminals.emplace("b", Syntactical_Symbol(true, "b", 61));
}
//m_terminals.emplace(";", lex_semicolon_mark);
//m_terminals.emplace("dim", lex_dim);
//m_terminals.emplace("as", lex_as);
//m_terminals.emplace("int", lex_int);
//m_terminals.emplace("vec", lex_vec);
//m_terminals.emplace("set", lex_set);
//m_terminals.emplace("to", lex_to);
//m_terminals.emplace("while", lex_while);
//m_terminals.emplace("do", lex_do);
//m_terminals.emplace("for", lex_for);
//m_terminals.emplace("if", lex_if);
//m_terminals.emplace("then", lex_then);
//m_terminals.emplace("else", lex_else);
//m_terminals.emplace("<-", lex_cin_mark);
//m_terminals.emplace("->", lex_cout_mark);
//m_terminals.emplace("cin", lex_cin);
//m_terminals.emplace("cout", lex_cout);
//m_terminals.emplace("mark", lex_mark); //					???????????????
//m_terminals.emplace(":", lex_colon_mark);
//m_terminals.emplace(",", lex_comma_mark);
//m_terminals.emplace("goto", lex_goto_mark);
//m_terminals.emplace("switch", lex_switch);
//m_terminals.emplace("case", lex_case);
//m_terminals.emplace("default", lex_default);
//m_terminals.emplace("exception", lex_exception);
//m_terminals.emplace("remark", lex_remark);//				??????? (<<<REMARK>>>)
//m_terminals.emplace("begin", lex_begin);
//m_terminals.emplace("end", lex_end);
//m_terminals.emplace("scalarprod", lex_scalarprod);
//m_terminals.emplace("subvec", lex_subvec);
//m_terminals.emplace("shiftr", lex_shiftr);
//m_terminals.emplace("shiftl", lex_shiftl);
//m_terminals.emplace("at", lex_at);
//m_terminals.emplace("conc", lex_conc);
//m_terminals.emplace("=", lex_equal_mark);
//m_terminals.emplace("variable", lex_variable);//			????
//m_terminals.emplace("constant", lex_constant);//			????
//m_terminals.emplace("aur_op", lex_aur_op);
//m_terminals.emplace("relation", lex_relation);
//m_terminals.emplace("EOF", lex_end_mark);
//m_terminals.emplace("(", lex_round_bracket_open);
//m_terminals.emplace(")", lex_round_bracket_close);
//}

CF_Grammar::CF_Grammar(std::string filename)
{
	std::ifstream input_file(filename);
	if (!input_file.is_open())
	{
		std::cout << "In CF_grammar::CF_grammar error openning " << filename << std::endl;
		exit(EXIT_FAILURE);
	}

	Fill_terminal_symbols();

	Grammar_rule current_rule;
	Syntactical_Symbol current_symbol;
	std::string word;
	size_t index_non_term_sym = 100;

	while (input_file.peek() != EOF)
	{
		//����� ����� �������
		input_file >> current_rule.m_left_part.m_name;
		if (current_rule.m_left_part.m_name[0] != '{' || current_rule.m_left_part.m_name[current_rule.m_left_part.m_name.length() - 1] != '}')
		{
			std::cout << "Wrong input format";
			input_file.close();
			exit(EXIT_FAILURE);
		}

		current_rule.m_left_part.m_is_terminal = false;
		if (m_non_terminals.contains(current_rule.m_left_part.m_name))
			current_rule.m_left_part.m_id = m_non_terminals[current_rule.m_left_part.m_name].m_id;
		else
		{
			current_rule.m_left_part.m_id = index_non_term_sym;
			m_non_terminals.emplace(current_rule.m_left_part.m_name, Syntactical_Symbol(false, current_rule.m_left_part.m_name, current_rule.m_left_part.m_id));
			index_non_term_sym++;
		}

		//������ ����� �������
		while (input_file.peek() != '\n' && input_file.peek() != EOF)          //���� ��� �� ����� ������, �������� ����� � ���������, �������� �� ��� �����������, ��� ���
		{
			input_file >> word;
			current_symbol.m_name = word;

			if (word[0] != '{')
			{
				current_symbol.m_is_terminal = true;
				if (!m_terminals.contains(word))
				{
					std::cout << "unexpected terminal";
					input_file.close();
					exit(EXIT_FAILURE);
				}

				current_symbol.m_id = m_terminals[word].m_id;
			}
			else
			{
				current_symbol.m_is_terminal = false;
				if (m_non_terminals.contains(word))
					current_symbol.m_id = m_non_terminals[word].m_id;
				else
				{
					current_symbol.m_id = index_non_term_sym;
					m_non_terminals.emplace(word, current_symbol);
					index_non_term_sym++;
				}
			}
			current_rule.m_right_part.push_back(current_symbol);

			while (input_file.peek() == ' ')
				input_file.ignore();
		}

		m_rules.emplace(current_rule.m_left_part, current_rule);
		current_rule.m_right_part.clear();
	}

	input_file.close();
}

void CF_Grammar::print_rules()
{
	for (auto it : m_rules)
	{
		std::cout << it.second.m_left_part.m_name << " -> ";
		for (auto that : it.second.m_right_part)
			std::cout << that.m_name << ' ';
		std::cout << std::endl;
	}
}

size_t CF_Grammar::get_id(const std::string& symbol) //double check
{
	if (m_non_terminals.contains(symbol))
		return m_non_terminals[symbol].m_id;
	return 0;
}

std::set<Syntactical_Symbol> CF_Grammar::FIRST1(const Syntactical_Symbol& S)
{
	std::set<Syntactical_Symbol> seen;

	if (S.m_is_terminal == true && m_terminals.contains(S.m_name))
	{
		std::set<Syntactical_Symbol> result;
		result.insert(S);
		return result;
	}

	return FIRST1_REC(S, seen);
}

std::set<Syntactical_Symbol> CF_Grammar::FIRST1_REC(const Syntactical_Symbol& S, std::set<Syntactical_Symbol>& seen)
{
	std::set<Syntactical_Symbol> first_symbols;

	size_t rules_amount = m_rules.count(S);
	auto it = m_rules.find(S);

	for (size_t i = 0; it != m_rules.end() && i < rules_amount; ++i, ++it)
	{
		if ((*it).second.m_right_part[0].m_is_terminal)
			first_symbols.insert((*it).second.m_right_part[0]);
		else if (seen.find((*it).second.m_right_part[0]) == seen.end())
		{
			seen.insert((*it).second.m_right_part[0]);
			first_symbols.merge(FIRST1_REC((*it).second.m_right_part[0], seen));
		}
	}

	return first_symbols;
}

Action CF_Grammar::get_action(TableLR1 table, Syntactical_Symbol symbol)
{
	Action resultAct;
	for (int i = 0; i < table.m_situations.size(); i++)
	{
		//���� ������ ������ � ����� ����� ������ ���
		if ((symbol == Syntactical_Symbol(true, "epsilon", -1)) && table.m_situations[i].m_left_part == Syntactical_Symbol(false, "{S'}", 100))
		{
			resultAct.act = ACCEPT;
			return resultAct;
		}
		//���� ������ ����� ������� ������� ������ �� �����
		else if (!table.m_situations[i].m_rp_after_point.empty() && symbol == table.m_situations[i].m_rp_after_point[0] && symbol != "epsilon")
		{
			resultAct.act = TRANSFER;
			return resultAct;
		}
		for (int j = 0; j < table.m_situations[i].m_avan_chain.size(); j++)
		{
			//���� ������ �� ����� ������ ������, � ��� ������ �� �����������
			if (table.m_situations[i].m_rp_after_point.empty() && (symbol == table.m_situations[i].m_avan_chain[j] || symbol == "epsilon"))
			{
				resultAct.act = CONVOLUTION;
				int rnum = 0;
				for (auto& it : m_rules)
				{
					if (it.second.m_left_part == table.m_situations[i].m_left_part && it.second.m_right_part == table.m_situations[i].m_rp_before_point)
					{
						resultAct.rule_conv.m_left_part = it.second.m_left_part;
						resultAct.rule_conv.m_right_part = it.second.m_right_part;
					}
					rnum++;
				}

				return resultAct;
			}
		}
	}
	resultAct.act = ERROR;
	return resultAct;
}

void CF_Grammar::GOTO(TableLR1 table, Syntactical_Symbol symbol)
{
	TableLR1 newTable;
	LR1_Situation currSituation;
	//���� �� ���� LR1 ��������� � ���� ����� ��� ��, � ������� ������� symbol ����� ������ ����� ����� �����
	for (int i = 0; i < table.m_situations.size(); i++)
	{
		currSituation = table.m_situations[i];
		if (!table.m_situations[i].m_rp_after_point.empty() && table.m_situations[i].m_rp_after_point[0] == symbol)
		{
			currSituation.m_rp_before_point.push_back(symbol);
			currSituation.m_rp_after_point.erase(currSituation.m_rp_after_point.begin());
			newTable.m_situations.push_back(currSituation);
		}
	}
	if (newTable.m_situations.size() > 0)
	{
		FixGenerateTableLR1(newTable);
		GenerateTableLR1(newTable);
	}
}

void CF_Grammar::GenerateTableLR1(TableLR1 table)
{
	LR1_Situation currSituation;
	std::set<Syntactical_Symbol> tempAvanChain; //�����, ����� � ���� �������� ��������� FIRST, ����� ����� �������������� � ��������� ����-�������

	//�������� ��������� ������� ��� ����������� S'
	if (table.m_situations.empty())
	{
		Syntactical_Symbol S(false, "{S'}", 100);
		currSituation.m_left_part = m_rules.find(S)->second.m_left_part;
		currSituation.m_rp_after_point = m_rules.find(S)->second.m_right_part;
		currSituation.m_avan_chain.push_back(Syntactical_Symbol(true, "epsilon", -1));
		table.m_situations.push_back(currSituation);
		FixGenerateTableLR1(table);
		GenerateTableLR1(table);
	}
	else //��������� ������ 
	{
		bool TableChangedFlag = false;
		for (int i = 0; i < table.m_situations.size(); i++)
		{
			//���� ����� ����� ����� �������������� ������
			if (!table.m_situations[i].m_rp_after_point.empty() && m_non_terminals.contains(table.m_situations[i].m_rp_after_point[0].m_name))
			{
				//���� ������� ��� ������� �����������
				for (auto& it : m_rules)
				{
					if (table.m_situations[i].m_rp_after_point[0] == it.second.m_left_part)
					{
						currSituation.m_left_part = it.second.m_left_part;
						currSituation.m_rp_after_point = it.second.m_right_part;

						//������ ����-�������
						//���� ����� ����� ��� �������, �� ������ �������� ����-�������, ����� ����� FIRST �� ������� 
						if (table.m_situations[i].m_rp_after_point.size() == 1) //changed .empty()
						{
							currSituation.m_avan_chain = table.m_situations[i].m_avan_chain;
						}
						else
						{
							tempAvanChain = FIRST1(table.m_situations[i].m_rp_after_point[1]);
							for (auto& avan_it : tempAvanChain)
							{
								if (std::find(currSituation.m_avan_chain.begin(), currSituation.m_avan_chain.end(), avan_it) == currSituation.m_avan_chain.end())
									currSituation.m_avan_chain.push_back(avan_it); //����� ���� ����� ��������� ������ ����� ?
							}
						}

						TableChangedFlag = true;
						//���� �� ���� ��������� � ������� � �������, ���� �� ����� ��� ������ ��� ����������
						for (int s = 0; s < table.m_situations.size(); s++)
						{
							if (table.m_situations[s] == currSituation)
							{
								TableChangedFlag = false;
								break;
							}
						}
						//���� �� ����, �� ���������� �� � �������
						if (TableChangedFlag)
						{
							table.m_situations.push_back(currSituation);
						}
					}
				}
			}
		}
		if (TableChangedFlag)
		{
			FixGenerateTableLR1(table);
			GenerateTableLR1(table);
		}
		else
		{
			FixGenerateTableLR1(table);
			m_curr_table = table;
		}
	}
}

std::vector<Syntactical_Symbol> CF_Grammar::FOLLOW(const Syntactical_Symbol& symbol)
{
	std::vector<Syntactical_Symbol> AlreadyCountedLeftParts;
	std::vector<Syntactical_Symbol> result;

	for (auto& it : m_rules)
	{
		for (int i = 0; i < it.second.m_right_part.size(); i++)
		{
			if ((it.second.m_right_part[i] == symbol) && (it.second.m_right_part.size() > static_cast<unsigned long long>(i) + 1))
			{
				std::set<Syntactical_Symbol> midresfirst = FIRST1(it.second.m_right_part[static_cast<std::vector<Syntactical_Symbol, std::allocator<Syntactical_Symbol>>::size_type>(i) + 1]);
				std::vector<Syntactical_Symbol> midres;
				for (auto& item : midresfirst)
					midres.push_back(item);
				for (int s = 0; s < midres.size(); s++)
				{
					if (std::find(result.begin(), result.end(), midres[s]) == result.end())
						result.push_back(midres[s]);
				}
				break;
			}
			else if ((it.second.m_right_part[i] == symbol) && (it.second.m_right_part.size() == i) && (symbol != it.second.m_left_part))
			{
				if (std::find(AlreadyCountedLeftParts.begin(), AlreadyCountedLeftParts.end(), it.second.m_left_part) == AlreadyCountedLeftParts.end())
				{
					std::vector<Syntactical_Symbol> midres = FOLLOW(it.second.m_left_part);
					for (int s = 0; s < midres.size(); s++)
					{
						if (std::find(result.begin(), result.end(), midres[s]) == result.end())
							result.push_back(midres[s]);
					}
					AlreadyCountedLeftParts.push_back(it.second.m_left_part);
				}
			}
		}
	}
	return result;
}

void CF_Grammar::PrepareInputWord(const std::list<std::tuple<Lexem, long long int, int>>& lexem_list)
{
	std::map<long long int, bool> declaredStuff;
	std::set<std::string> markedStuff;
	std::set<std::string> gotoStuff;
	for (auto it = lexem_list.begin(); it != lexem_list.end(); ++it)
	{
		switch (std::get<0>(*it))
		{
		case lex_semicolon_mark:
		{
			m_input_word.emplace_back(m_terminals[";"], std::get<2>(*it));
			break;
		}
		case lex_dim:
		{
			//���������, ������� �� ���������� ����������
			auto nextIter = std::next(it, 1);
			if (declaredStuff.find(std::get<1>(*nextIter)) == declaredStuff.end())
			{
				declaredStuff[std::get<1>(*nextIter)] = true;
				m_input_word.emplace_back(m_terminals["dim"], std::get<2>(*it));
			}
			else
			{
				std::cout << "SYNTH BLOCK: VARIABLE " << ((Variable*)(std::get<1>(*it)))->name << " DECLARED TWICE" << std::endl;
				m_synth_error_flag = true;
				return;
			}

			break;
		}
		case lex_as:
		{
			m_input_word.emplace_back(m_terminals["as"], std::get<2>(*it));
			break;
		}
		case lex_int:
		{
			m_input_word.emplace_back(m_terminals["int"], std::get<2>(*it));
			break;
		}
		case lex_vec:
		{
			m_input_word.emplace_back(m_terminals["vec"], std::get<2>(*it));
			break;
		}
		case lex_set:
		{
			m_input_word.emplace_back(m_terminals["set"], std::get<2>(*it));
			break;
		}
		case lex_to:
		{
			m_input_word.emplace_back(m_terminals["to"], std::get<2>(*it));
			break;
		}
		case lex_while:
		{
			m_input_word.emplace_back(m_terminals["while"], std::get<2>(*it));
			break;
		}
		case lex_do:
		{
			m_input_word.emplace_back(m_terminals["do"], std::get<2>(*it));
			break;
		}
		case lex_for:
		{
			m_input_word.emplace_back(m_terminals["for"], std::get<2>(*it));
			break;
		}
		case lex_if:
		{
			m_input_word.emplace_back(m_terminals["if"], std::get<2>(*it));
			break;
		}
		case lex_then:
		{
			m_input_word.emplace_back(m_terminals["then"], std::get<2>(*it));
			break;
		}
		case lex_else:
		{
			m_input_word.emplace_back(m_terminals["else"], std::get<2>(*it));
			break;
		}
		case lex_cin_mark:
		{
			m_input_word.emplace_back(m_terminals["<-"], std::get<2>(*it));
			break;
		}
		case lex_cout_mark:
		{
			m_input_word.emplace_back(m_terminals["->"], std::get<2>(*it));
			break;
		}
		case lex_cin:
		{
			m_input_word.emplace_back(m_terminals["cin"], std::get<2>(*it));
			break;
		}
		case lex_cout:
		{
			m_input_word.emplace_back(m_terminals["cout"], std::get<2>(*it));
			break;
		}
		case lex_mark: //����� L:
		{
			if (markedStuff.insert(((Variable*)std::get<1>(*it))->name).second)
			{
				m_input_word.emplace_back(m_terminals["mark_goto"], std::get<2>(*it));
			}
			else
			{
				std::cout << "SYNTH BLOCK: MARK " << ((Variable*)(std::get<1>(*it)))->name << " SET TWICE" << std::endl;
				m_synth_error_flag = true;
				return;
			}
			break;
		}
		case lex_colon_mark:
		{
			m_input_word.emplace_back(m_terminals[":"], std::get<2>(*it));
			break;
		}
		case lex_comma_mark:
		{
			m_input_word.emplace_back(m_terminals[","], std::get<2>(*it));
			break;
		}
		case lex_goto_mark:
		{
			gotoStuff.insert(((Variable*)(std::get<1>(*it)))->name);
			m_input_word.emplace_back(m_terminals["goto"], std::get<2>(*it));
			break;
		}
		case lex_switch:
		{
			m_input_word.emplace_back(m_terminals["switch"], std::get<2>(*it));
			break;
		}
		case lex_case:
		{
			m_input_word.emplace_back(m_terminals["case"], std::get<2>(*it));
			break;
		}
		case lex_default:
		{
			m_input_word.emplace_back(m_terminals["default"], std::get<2>(*it));
			break;
		}
		case lex_exception:
		{
			m_input_word.emplace_back(m_terminals["exception"], std::get<2>(*it));
			break;
		}
		case lex_remark:
		{
			m_input_word.emplace_back(m_terminals["remark"], std::get<2>(*it));
			break;
		}
		case lex_begin:
		{
			m_input_word.emplace_back(m_terminals["begin"], std::get<2>(*it));
			break;
		}
		case lex_end:
		{
			m_input_word.emplace_back(m_terminals["end"], std::get<2>(*it));
			break;
		}
		case lex_scalarprod:
		{
			m_input_word.emplace_back(m_terminals["scalarprod"], std::get<2>(*it));
			break;
		}
		case lex_subvec:
		{
			m_input_word.emplace_back(m_terminals["subvec"], std::get<2>(*it));
			break;
		}
		case lex_shiftr:
		{
			m_input_word.emplace_back(m_terminals["shiftr"], std::get<2>(*it));
			break;
		}
		case lex_shiftl:
		{
			m_input_word.emplace_back(m_terminals["shiftl"], std::get<2>(*it));
			break;
		}
		case lex_at:
		{
			m_input_word.emplace_back(m_terminals["at"], std::get<2>(*it));
			break;
		}
		case lex_conc:
		{
			m_input_word.emplace_back(m_terminals["conc"], std::get<2>(*it));
			break;
		}
		case lex_equal_mark:
		{
			m_input_word.emplace_back(m_terminals["="], std::get<2>(*it));
			break;
		}
		case lex_variable:
		{
			if (declaredStuff.find(std::get<1>(*it)) != declaredStuff.end())
			{
				m_input_word.emplace_back(m_terminals["variable"], std::get<2>(*it));
			}
			else
			{
				std::cout << "SYNTH BLICK: ERROR! VARIABLE " << ((Variable*)(std::get<1>(*it)))->name << " IS NOT INITIALIZED!" << std::endl;
				m_synth_error_flag = true;
				return;
			}
			break;
		}
		case lex_constant:
		{
			m_input_word.emplace_back(m_terminals["constant"], std::get<2>(*it));
			break;
		}
		case lex_aur_op:
		{
			std::string aurOper;
			aurOper += (char)(std::get<1>(*it));
			m_input_word.emplace_back(m_terminals[aurOper], std::get<2>(*it));
			break;
		}
		case lex_relation:
		{
			switch (std::get<1>(*it))
			{
			case(1): {m_input_word.emplace_back(m_terminals["=="], std::get<2>(*it)); break; }
			case(2): {m_input_word.emplace_back(m_terminals["!="], std::get<2>(*it)); break; }
			case(3): {m_input_word.emplace_back(m_terminals["<"], std::get<2>(*it)); break; }
			case(4): {m_input_word.emplace_back(m_terminals[">"], std::get<2>(*it)); break; }
			case(5): {m_input_word.emplace_back(m_terminals["<="], std::get<2>(*it)); break; }
			case(6): {m_input_word.emplace_back(m_terminals[">="], std::get<2>(*it)); break; }
			default:
				break;
			}
			break;
		}
		case lex_error:
		{
			std::cout << "SYNTH BLOCK: ERROR! (in prepareInputWord function" << std::endl;
			m_synth_error_flag = true;
			return;
			break;
		}
		case lex_end_mark:
		{
			m_input_word.emplace_back(m_terminals["mark_end"], std::get<2>(*it));
			break;
		}
		case lex_round_bracket_open:
		{
			m_input_word.emplace_back(m_terminals["("], std::get<2>(*it));
			break;
		}
		case lex_round_bracket_close:
		{
			m_input_word.emplace_back(m_terminals[")"], std::get<2>(*it));
			break;
		}
		}
	}

	for (auto& it : gotoStuff)
	{
		if (markedStuff.find(it) == markedStuff.end())
		{
			std::cout << "SYNTH BLOCK: MARK " << it << " HADNT BEEN SET" << std::endl;
			m_synth_error_flag = true;
			return;
		}
	}
}

void CF_Grammar::synt_parse()
{
	if (m_synth_error_flag)
		return;

	Action currentActionMain;
	GenerateTableLR1(m_curr_table);
	FixGenerateTableLR1(m_curr_table);
	m_magazine.emplace_back(Syntactical_Symbol(true, "epsilon", -1), m_curr_table);
	auto it = m_input_word.begin();
	while (it != m_input_word.end())
	{
		Action currAction = get_action(m_curr_table, std::get<0>(*it));

		//������ ��� ������:
		if (currAction.act == CONVOLUTION)
		{
			std::cout << std::endl << "Current Symbol: " << std::get<0>(*it).m_name << "  Current Action: " << m_ActionListStr[currAction.act] << " by rule: ";
			print_rule(currAction.rule_conv);
			std::cout << "  CurrentTable Situation = " << m_curr_table.m_situations.size() << std::endl;
		}
		else
		{
			std::cout << std::endl << "Current Symbol: " << std::get<0>(*it).m_name << "  Current Action: " << m_ActionListStr[currAction.act];
			std::cout << "  CurrentTable Situation = " << m_curr_table.m_situations.size() << std::endl;
		}
		std::cout << "Current Table: " << std::endl;
		for (int i = 0; i < m_curr_table.m_situations.size(); i++)
		{
			std::cout << std::endl;
			print_situation(m_curr_table.m_situations[i]);
			std::cout << std::endl;
		}
		std::cout << std::endl;
		//����� ������ ��� ������

		if (currAction.act == TRANSFER)
		{
			GOTO(m_curr_table, std::get<0>(*it));
			//FixGenerateTableLR1(m_curr_table);
			m_magazine.emplace_back(std::get<0>(*it), m_curr_table);
			++it;
		}
		else if (currAction.act == CONVOLUTION)
		{
			m_magazine.resize(m_magazine.size() - currAction.rule_conv.m_right_part.size());
			m_curr_table = m_magazine.back().second;
			GOTO(m_curr_table, currAction.rule_conv.m_left_part);
			//FixGenerateTableLR1(m_curr_table);
			m_magazine.emplace_back(currAction.rule_conv.m_left_part, m_curr_table);
		}
		else if (currAction.act == ERROR)
		{
			std::cout << "SYNTH BLOCK: ERROR IN STR: " << std::get<1>(*it) << std::endl;
			break;
		}

		currentActionMain.act = currAction.act;
		currentActionMain.rule_conv = currAction.rule_conv;
	}
	
	//������� mark_end
	it--;
	Action currAction = get_action(m_curr_table, std::get<0>(*it));
	if (currAction.act == TRANSFER)
	{
		GOTO(m_curr_table, std::get<0>(*it));
		//FixGenerateTableLR1(m_curr_table);
		m_magazine.emplace_back(std::get<0>(*it), m_curr_table);
		++it;
	}

	//������� ���������
	if (currentActionMain.act != ERROR)
	{
		while (true)
		{
			Action currAction = get_action(m_curr_table, Syntactical_Symbol(true, "epsilon", -1));
			if (currAction.act == TRANSFER)
			{
				GOTO(m_curr_table, std::get<0>(*it));
				//FixGenerateTableLR1(m_curr_table);
				m_magazine.emplace_back(std::get<0>(*it), m_curr_table);
				++it;
			}
			else if (currAction.act == CONVOLUTION)
			{
				m_magazine.resize(m_magazine.size() - currAction.rule_conv.m_right_part.size());
				m_curr_table = m_magazine.back().second;
				GOTO(m_curr_table, currAction.rule_conv.m_left_part);
				//FixGenerateTableLR1(m_curr_table);
				m_magazine.emplace_back(currAction.rule_conv.m_left_part, m_curr_table);
			}
			else if (currAction.act == ACCEPT)
			{
				std::cout << "CORRECT PROGRAM!"; //������
				return;
			}
			else if (currAction.act == ERROR)
			{
				std::cout << "SYNTH BLOCK: ERROR IN STR: " << std::get<1>(*it) << std::endl;
				break;
			}
		}
	}
}

void CF_Grammar::print_situation(LR1_Situation sit)
{
	std::cout << '[' << sit.m_left_part.m_name << " -> ";
	for (int i = 0; i < sit.m_rp_before_point.size(); i++)
		std::cout << sit.m_rp_before_point[i].m_name << ' ';
	std::cout << ".";
	for (int i = 0; i < sit.m_rp_after_point.size(); i++)
		std::cout << sit.m_rp_after_point[i].m_name << ' ';
	std::cout << ",";
	for (int i = 0; i < sit.m_avan_chain.size(); i++)
		std::cout << sit.m_avan_chain[i].m_name << "|";
	if (sit.m_avan_chain.empty())
		std::cout << "EPSILON";
	std::cout << ']';
}

void CF_Grammar::print_rule(Grammar_rule rule)
{
	std::cout << rule.m_left_part.m_name << " -> ";
	for (int i = 0; i < rule.m_right_part.size(); i++)
		std::cout << rule.m_right_part[i].m_name;
}

void CF_Grammar::FixGenerateTableLR1(TableLR1& table)
{
	TableLR1 tempTable;
	for (auto it1 = table.m_situations.begin(); it1 != table.m_situations.end(); it1++)
	{
		for (auto it2 = table.m_situations.begin(); it2 != table.m_situations.end(); it2++)
		{
			if ((*it1).m_left_part == (*it2).m_left_part && (*it1).m_rp_before_point == (*it2).m_rp_before_point)
			{
				if ((*it1).m_rp_after_point == (*it2).m_rp_after_point && (*it1).m_avan_chain != (*it2).m_avan_chain)
				{
					for (int s = 0; s < (*it2).m_avan_chain.size(); s++)
					{
						if (std::find((*it1).m_avan_chain.begin(), (*it1).m_avan_chain.end(), (*it2).m_avan_chain[s]) == (*it1).m_avan_chain.end())
							(*it1).m_avan_chain.push_back((*it2).m_avan_chain[s]);
					}
					table.m_situations.erase(it2--);
				}
			}
		}
	}
}
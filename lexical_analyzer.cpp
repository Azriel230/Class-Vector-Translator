#include <iostream>
#include "vector.h"
#include <fstream>
#include <string.h>
#include <vector>
#include <map>
#include <list>
#include <tuple>
#include <stack>

// Перечисление содержит константы для обозначения классов символьных лексем: БУКВА, ЦИФРА, АР_ОП, ОТНОШ, ПРОБЕЛ, LF, ;, :, ,, [, ], (, ), -, ОШИБКА, КОНЕЦ.
enum SymbolicTokenClass {	Letter,
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
							EndOfFile	};

// Перечисление содержит константы для обозначения состояний автомата и одну дополнительную константу s_Stop.
enum States {	start_lex,
				read_keyword,
				read_rel,
				variable,
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
				s_STOP	};

enum Lexem {	semicolon_mark,
				dim,
				as,
				int_lex,
				vec,
				set,
				to,
				while_lex,
				do_lex,
				for_lex,
				if_lex,
				then_lex,
				else_lex,
				cin_mark,
				cout_mark,
				cin,
				cout,
				mark,
				colon_mark,
				comma_mark,
				goto_mark_lex,
				switch_lex,
				case_lex,
				default_lex,
				exception,
				remark,
				begin,
				end,
				scalarprod,
				subvec,
				shiftr,
				shiftl,
				at,
				conc,
				equal_mark,
				variable_lex,
				constant_lex,
				aur_op,
				relation,
				err,
				end_mark,
				round_bracket_open,
				round_bracket_close	};

struct Variable
{
	std::string name;
	int type; //0 - int; 1 - vec; -1 - mark
	int integer_value;
	Vector vector_value;
};

// Структура для представления символьной лексемы.
struct SymbolicToken
{
	SymbolicTokenClass token_class; // Класс символьной лексемы.
	int value;                      // Значение символьной лексемы.
};

// Глобальные переменные, используемые процедурами автомата.
SymbolicToken s;    // Символьная лексема.

long long int reg_nT_pointer; //Регистр указателя. содержит указатель для лексем PUSH и POP
int reg_number;    // Регистр числа. используется для вычисления констант
int reg_relation; //Регистр отношения. хранит информацию о первом символе отношения
std::string reg_var_name; //Регистр переменной. накапливает имя переменной
int reg_detection = 0; //Регистр обнаружения. хранит номер позиции в таблице обнаружения для поиска ключевых слов.
int reg_line_num = 1; //Номер строки. хранит номер текущей строки в программе.
Lexem reg_class; //Регистр класса. служит для хранения класса лексемы
int reg_value; //Регистр значения. хранит значения лексем
Vector reg_vector; //Регистр вектора, хранит его значение
int reg_count_vector = 1; //Регистр количества векторов
int reg_remark = 0; // Регистр комментария

int error_state = 0; //запоминает состояние, вызвавшее ошибку
int error_symbolic_token_class = 0; //запоминает символ, который вызвал ошибку

std::map<std::string, Variable> nameTable;
std::list<std::tuple<Lexem, long long int, int>> lexem_list;
std::list<std::string> error_messages_list; //список ошибок

std::vector<int>beg_vector(26);
void init_beg_vect()
{
	beg_vector[0] = 3;   // a
	beg_vector[1] = 55;  // b
	beg_vector[2] = 25;  // c
	beg_vector[3] = 1;   // d
	beg_vector[4] = 22;  // e
	beg_vector[5] = 16;  // f
	beg_vector[6] = 30;  // g
	beg_vector[8] = 4;   // i
	beg_vector[18] = 8;  // s
	beg_vector[19] = 10; // t
	beg_vector[21] = 6;  // v
	beg_vector[22] = 11; // w
}

const int state_number = 21; // Число состояний (без s_Stop).
const int class_number = 15; // Число символов входного алфавита.

typedef int (*function_pointer)();
function_pointer table[state_number][class_number];

std::vector<std::tuple<char, int, function_pointer>> detect_table(83);

/* Функция транслитерации.
 * ch - символ
 * Возвращаемое значение - результат транслитерации символа ch
 */
SymbolicToken transliterator(int ch)
{
	SymbolicToken s;
	s.value = 0;
	if (isdigit(ch))
	{
		s.token_class = Digit;
		s.value = ch - '0';
	}
	else if ((ch > 64 && ch < 91) || (ch > 96 && ch < 123))
	{
		s.token_class = Letter;
		s.value = ch;
	}
	else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
	{
		s.token_class = Aur_OP;
		s.value = ch;
	}
	else if (ch == '=' || ch == '!' || ch == '<' || ch == '>')
	{
		s.token_class = Relation;
		s.value = ch;
	}
	else if (ch == ' ' || ch == '\t')
	{
		s.token_class = Space;
	}
	else if (ch == '\n' || ch == '\r\n')
	{
		s.token_class = LF;
	}
	else if (ch == EOF)
	{
		s.token_class = EndOfFile;
	}
	else if (ch == ';')
	{
		s.token_class = Semicolon;
		s.value = ch;
	}
	else if (ch == ':')
	{
		s.token_class = Colon;
		s.value = ch;
	}
	else if (ch == ',')
	{
		s.token_class = Comma;
		s.value = ch;
	}
	else if (ch == '[')
	{
		s.token_class = OpenSquareBracket;
		s.value = ch;
	}
	else if (ch == ']')
	{
		s.token_class = CloseSquareBracket;
		s.value = ch;
	}
	else if (ch == '(')
	{
		s.token_class = OpenRoundBracket;
		s.value = ch;
	}
	else if (ch == ')')
	{
		s.token_class = CloseRoundBracket;
		s.value = ch;
	}
	else
	{
		s.token_class = Error;
	}
	return s;
}

bool const_flag = 0;
bool vector_flag = 0;
bool err_flag = 0;

void create_lexem() //процедура создания лексемы
{
	if (reg_class == constant_lex || reg_class == variable_lex || reg_class == mark || reg_class == goto_mark_lex)
	{
		std::tuple<Lexem, long long int, int> tup(reg_class, reg_nT_pointer, reg_line_num);
		lexem_list.push_back(tup);
	}
	else if (reg_class == aur_op)
	{
		std::tuple<Lexem, long long int, int> tup(reg_class, reg_value, reg_line_num);
		lexem_list.push_back(tup);
	}
	else if (reg_class == relation)
	{
		std::tuple<Lexem, long long int, int> tup(reg_class, reg_relation, reg_line_num);
		lexem_list.push_back(tup);
	}
	else if (reg_class == dim || reg_class == as || reg_class == int_lex || reg_class == vec || reg_class == set || reg_class == to)
	{
		std::tuple<Lexem, long long int, int> tup(reg_class, -1, reg_line_num);
		lexem_list.push_back(tup);
	}
	else if (reg_class == while_lex || reg_class == do_lex || reg_class == for_lex || reg_class == if_lex || reg_class == then_lex || reg_class == else_lex)
	{
		std::tuple<Lexem, long long int, int> tup(reg_class, -1, reg_line_num);
		lexem_list.push_back(tup);
	}
	else if (reg_class == cin_mark || reg_class == cout_mark || reg_class == cin || reg_class == cout || reg_class == colon_mark || reg_class == comma_mark)
	{
		std::tuple<Lexem, long long int, int> tup(reg_class, -1, reg_line_num);
		lexem_list.push_back(tup);
	}
	else if (reg_class == switch_lex || reg_class == case_lex || reg_class == default_lex || reg_class == exception)
	{
		std::tuple<Lexem, long long int, int> tup(reg_class, -1, reg_line_num);
		lexem_list.push_back(tup);
	}
	else if (reg_class == begin || reg_class == end || reg_class == equal_mark || reg_class == end_mark || reg_class == semicolon_mark)
	{
		std::tuple<Lexem, long long int, int> tup(reg_class, -1, reg_line_num);
		lexem_list.push_back(tup);
	}
	else if (reg_class == scalarprod || reg_class == subvec || reg_class == shiftr || reg_class == shiftl || reg_class == at || reg_class == conc)
	{
		std::tuple<Lexem, long long int, int> tup(reg_class, -1, reg_line_num);
		lexem_list.push_back(tup);
	}
	else if (reg_class == round_bracket_open || reg_class == round_bracket_close || reg_class == remark)
	{
		std::tuple<Lexem, long long int, int> tup(reg_class, -1, reg_line_num);
		lexem_list.push_back(tup);
	}
	else if (reg_class == err)
	{
		std::tuple<Lexem, long long int, int> tup(reg_class, reg_line_num, reg_line_num);
		lexem_list.push_back(tup);
	}

	//используя регистры: класса, указателя, значения, отношения, номер строки
	// сформировать лексему и кинуть в список
	//нужно создать тройку (класс, значение, номер строки).
}

//int Error1() //процедура обработки ошибки
//{
//	reg_class = err;
//	create_lexem();
//	err_flag = 1;
//	return error1;
//}

bool err_flag_lf = false;
std::string err_message;
int err_flag_helpformessage = -1; //-1: Флаг опущен; 1: Флаг поднят для случая 1; 2: Флаг поднят для случая 2; i: Флаг поднят для случая i;

int Error1()
{
	//ищем нужное сообщение об ошибочке
	if (error_state == start_lex) //состояние начала считывания лексемы
	{
		err_message = "A forbidden character was entered as the first character of a lexem";
	}
	else if (error_state == read_keyword) //состояние считывания ключевого слова
	{
		err_message = "An illegal character was encountered while reading a keyword";
	}
	else if (error_state == read_rel) //состояние считывания отношения
	{
		if (error_symbolic_token_class == Aur_OP)
		{
			err_message = "Wrong character <cin> entered, expected character: -";
		}
		else if (error_symbolic_token_class == Relation)
		{
			if (err_flag_helpformessage == 1)
			{
				err_message = "Read the wrong two-character relation";
			}
			else if (err_flag_helpformessage == 2)
			{
				err_message = "Error when reading a comment character ( after < entered a character other than = )";
			}
		}
		else if (error_symbolic_token_class == Space || error_symbolic_token_class == LF || error_symbolic_token_class == OpenRoundBracket)
		{
			err_message = "Read the wrong two-character relation";
		}
		else
		{
			err_message = "Invalid character entered in relation lexem";
		}
	}
	else if (error_state == variable) //состояние считывания переменной
	{
		err_message = "Invalid character entered in variable lexem";
	}
	else if (error_state == constant) //состояние считывания константы
	{
		if (error_symbolic_token_class == Letter)
		{
			err_message = "Error: constant lexem can't starts with letter";
		}
		else
		{
			err_message = "Invalid character entered in constant lexem";
		}
	}
	else if (error_state == cr_rel) //состояние создания лексемы отношения
	{
		err_message = "Invalid character entered after relation lexem";
	}
	else if (error_state == const_vec1) //состояние, в котором считывается первая цифра каждой координаты вектора
	{
		err_message = "Invalid character entered in start const_vector lexem. Expected: digit";
	}
	else if (error_state == const_vec2) //состояние, в котором считывается координата вектора
	{
		err_message = "Invalid character entered in coordinate of const_vector lexem. Expected: digit or space-char or ] ";
	}
	else if (error_state == cout_mark1) //состояние, в котором отслеживается правильный ввод символа вывода
	{
		err_message = "Invalid character entered in cout_mark lexem. Expected: > ";
	}
	else if (error_state == remark1) //состояние, в котором отслеживается правильный ввод символа начала комментария
	{
		err_message = "Invalid character entered in start_comment_mark symbols. Expected: < ";
	}
	else if (error_state == remark2) //состояние считывания комментария
	{
		err_message = "File ended before closing remark";
	}
	else if (error_state == kw_var) //состояние считывания символа после ключевого слова
	{
		err_message = "Invalid character entered after keyword. Expected: letter, digit, space, lf or ; ";
	}
	else if (error_state == kw_var_bracket) //состояние считывания символа после ключевого слова (скобка)
	{
		err_message = "Invalid character entered after keyword. Expected: letter, digit, lf or ( ";
	}
	else if (error_state == kw_var_default) //состояние считывания символа после ключевого слова default (:)
	{
		err_message = "Invalid character entered after default. Expected: letter, digit, lf or : ";
	}
	else if (error_state == goto_space) //состояние считывания символа после ключевого слова goto (пробел)
	{
		err_message = "Invalid character entered after goto. Expected: letter, digit or space";
	}
	else if (error_state == goto_mark1) //состояние, в котором нужно считать первую букву метки
	{
		err_message = "Invalid character entered at start of mark-name .Expected: letter";
	}
	else if (error_state == goto_mark2) //состояние, в котором нужно считать имя метки
	{
		err_message = "Invalid character entered at start of mark-name .Expected: letter, digit, space, lf or ; ";
	}
	else if (error_state == kw_var_bracket2) //состояние считывания символа после ключевого слова (скобка)
	{
		err_message = "Invalid character entered after keyword. Expected: letter, digit, lf or ( ";
	}
	else if (error_state == kw_var_to) //состояние считывания символа после ключевого слова to
	{
		err_message = "Invalid character entered after to. Expected: letter, digit, lf, [ or ( ";
	}
	else if (error_state == arif)
	{
		if (error_symbolic_token_class == Aur_OP)
		{
			err_message = "Error: no two-character arithmetic operations";
		}
		else
		{
			err_message = "Invalid character entered after keyword. Expected: letter, digit, relation, space, lf or ( ";
		}
	}

	error_messages_list.push_back(err_message); //закидываем сообщение об ошибке в список ошибок

	reg_class = err;
	create_lexem();

	if (err_flag_lf)
	{
		reg_line_num++;
		err_flag_lf = 0;
		err_flag = 1;
		return start_lex;
	}
	err_flag = 1;
	return error1;
}


std::string ITOA(int num)
{
	std::string str;
	for (int i = floor(log10(num)); i >= 0; --i)
	{
		str.push_back((num / (int)pow(10, i) % 10) + '0');
	}
	return str;
}

void add_constant()
{
	if (const_flag == 0 && vector_flag == 0) //check?
		return;

	Variable var;
	if (const_flag == 1)
	{
		var.type = 0;
		var.name = ITOA(reg_number);
		var.integer_value = reg_number;
	}
	else
	{
		var.type = 1;
		var.name = "#vec" + ITOA(reg_count_vector);
		++reg_count_vector;
		var.vector_value = reg_vector;
	}

	bool flag = false;
	for (auto& it : nameTable)
	{
		if ((std::get<0>(it) == var.name) && ((std::get<1>(it).integer_value == var.integer_value) || (std::get<1>(it).vector_value == var.vector_value)))
		{
			flag = true;
			reg_nT_pointer = reinterpret_cast<long long int>(&nameTable[var.name]);
			return;
		}
	}

	if (!flag)
		nameTable.insert(std::pair<std::string, Variable>(var.name, var));
	reg_nT_pointer = reinterpret_cast<long long int>(&nameTable[var.name]);

	const_flag = 0;
	vector_flag = 0;
}

void add_variable()
{
	if (reg_var_name == "dim" || reg_var_name == "as" || reg_var_name == "int" || reg_var_name == "vec" || reg_var_name == "set" || reg_var_name == "to")
	{
		Error1();
		return;
	}
	else if (reg_var_name == "while" || reg_var_name == "do" || reg_var_name == "for" || reg_var_name == "if" || reg_var_name == "then" || reg_var_name == "else")
	{
		Error1();
		return;
	}
	else if (reg_var_name == "cin" || reg_var_name == "cout" || reg_var_name == "goto" || reg_var_name == "switch" || reg_var_name == "case" || reg_var_name == "default")
	{
		Error1();
		return;
	}
	else if (reg_var_name == "exception" || reg_var_name == "begin" || reg_var_name == "end")
	{
		Error1();
		return;
	}
	else if (reg_var_name == "scalarprod" || reg_var_name == "subvec" || reg_var_name == "shiftr" || reg_var_name == "shiftl" || reg_var_name == "at" || reg_var_name == "conc")
	{
		Error1();
		return;
	}

	Variable var;
	var.name = reg_var_name;
	var.type = 0;
	bool flag = false;
	for (auto& it : nameTable)
	{
		if ((std::get<0>(it) == var.name) && (std::get<1>(it).integer_value == var.integer_value) && (std::get<1>(it).type == var.type))
		{
			flag = true;
			reg_nT_pointer = reinterpret_cast<long long int>(&nameTable[var.name]);
			return;
		}
	}
	if (!flag)
		nameTable.insert(std::pair<std::string, Variable>(var.name, var));
	reg_nT_pointer = reinterpret_cast<long long int>(&nameTable[var.name]);
}

void add_mark()
{
	if (reg_var_name == "dim" || reg_var_name == "as" || reg_var_name == "int" || reg_var_name == "vec" || reg_var_name == "set" || reg_var_name == "to")
	{
		Error1();
		return;
	}
	else if (reg_var_name == "while" || reg_var_name == "do" || reg_var_name == "for" || reg_var_name == "if" || reg_var_name == "then" || reg_var_name == "else")
	{
		Error1();
		return;
	}
	else if (reg_var_name == "cin" || reg_var_name == "cout" || reg_var_name == "goto" || reg_var_name == "switch" || reg_var_name == "case" || reg_var_name == "default")
	{
		Error1();
		return;
	}
	else if (reg_var_name == "exception" || reg_var_name == "begin" || reg_var_name == "end")
	{
		Error1();
		return;
	}
	else if (reg_var_name == "scalarprod" || reg_var_name == "subvec" || reg_var_name == "shiftr" || reg_var_name == "shiftl" || reg_var_name == "at" || reg_var_name == "conc")
	{
		Error1();
		return;
	}

	Variable var;
	var.name = '~' + reg_var_name; //имя метки будет начинаться с ~, нужно над этим еще подумать
	var.type = -1;
	bool flag = false;
	for (auto& it : nameTable)
	{
		if ((std::get<0>(it) == var.name) && (std::get<1>(it).integer_value == var.integer_value) && (std::get<1>(it).type == var.type))
		{
			flag = true;
			reg_nT_pointer = reinterpret_cast<long long int>(&nameTable[var.name]);
			return;
		}
	}
	if (!flag)
		nameTable.insert(std::pair<std::string, Variable>(var.name, var));
	reg_nT_pointer = reinterpret_cast<long long int>(&nameTable[var.name]);
}

//Процедуры автомата

int start_lex_Space()
{
	return start_lex;
}
int start_lex_LF()
{
	reg_line_num++;
	return start_lex;
}
int start_lex_CrEmpty()
{
	reg_class = semicolon_mark;
	create_lexem();
	return start_lex;
}
int start_lex_CrRoundBracket1()
{
	reg_class = round_bracket_open;
	create_lexem();
	return start_lex;
}
int start_lex_CrRoundBracket2()
{
	reg_class = round_bracket_close;
	create_lexem();
	return start_lex;
}
int cr_rel_Rem()
{
	if (s.value == '=')
	{
		if (reg_relation == '=')
		{
			reg_relation = 1;
		}
		else if (reg_relation == '!')
		{
			reg_relation = 2;
		}
		else if (reg_relation == '<')
		{
			reg_relation = 5;
		}
		else if (reg_relation == '>')
		{
			reg_relation = 6;
		}
		else
		{
			err_flag_helpformessage = 1; //первый случай: ошибка при чтении отношения
			return Error1();
		}
		create_lexem();
		return cr_rel;
	}
	else if (s.value == '<')
	{
		if (reg_relation == '<')
		{
			return remark1;
		}
		else
		{
			err_flag_helpformessage = 2; //второй случай: ошибка при чтении символа комментария ( после < ввели символ, отличный от = )
			return Error1();
		}
	}
	else
		return Error1();
}
int start_lex_CrRel()
{
	if (reg_relation == '!')
		return Error1();
	if (reg_relation == '<')
		reg_relation = 3;
	else if (reg_relation == '>')
		reg_relation = 4;

	if (reg_relation == '=')
	{
		reg_class = equal_mark;
		reg_relation = 7;
	}

	create_lexem();
	return start_lex;
}
int start_lex_CrRel_LF()
{
	if (reg_relation == '!')
		return Error1();
	if (reg_relation == '<')
		reg_relation = 3;
	else if (reg_relation == '>')
		reg_relation = 4;

	if (reg_relation == '=')
	{
		reg_class = equal_mark;
		reg_relation = 7;
	}

	create_lexem();
	reg_line_num++;
	return start_lex;
}
int start_lex_CrRelBracket1()
{
	if (reg_relation == '!')
		return Error1();
	if (reg_relation == '<')
		reg_relation = 3;
	else if (reg_relation == '>')
		reg_relation = 4;

	if (reg_relation == '=')
	{
		reg_class = equal_mark;
		reg_relation = 7;
	}

	create_lexem();

	reg_class = round_bracket_open;
	create_lexem();

	return start_lex;
}
int start_lex_CrVar()
{
	reg_class = variable_lex;
	add_variable();
	create_lexem();
	return start_lex;
}
int start_lex_CrVar_LF()
{
	reg_class = variable_lex;
	add_variable();
	create_lexem();
	reg_line_num++;
	return start_lex;
}
int start_lex_CrVar_Bracket2()
{
	reg_class = variable_lex;
	add_variable();
	create_lexem();
	reg_class = round_bracket_close;
	create_lexem();
	return start_lex;
}
int start_lex_CrConst_Bracket2()
{
	reg_class = constant_lex;
	add_constant();
	create_lexem();
	reg_class = round_bracket_close;
	create_lexem();
	return start_lex;
}
int start_lex_CrVar_Comma()
{
	reg_class = variable_lex;
	add_variable();
	create_lexem();
	reg_class = comma_mark;
	create_lexem();
	return start_lex;
}
int start_lex_CrConst_Comma()
{
	reg_class = constant_lex;
	add_constant();
	create_lexem();
	reg_class = comma_mark;
	create_lexem();
	return start_lex;
}
int start_lex_CrConst()
{
	reg_class = constant_lex;
	const_flag = 1;
	add_constant();
	create_lexem();
	return start_lex;
}
int start_lex_CrConst_LF()
{
	reg_class = constant_lex;
	const_flag = 1;
	add_constant();
	create_lexem();
	reg_line_num++;
	return start_lex;
}
int start_lex_CrLex() //после отношения просто перейти на старт
{
	return start_lex;
}
int start_lex_CrLex_LF() //после отношения увеличить к-во строк на 1 и перейти на старт
{
	reg_line_num++;
	return start_lex;
}
int start_lex_CrKW()
{
	create_lexem();
	return start_lex;
}
int start_lex_CrKW_LF()
{
	create_lexem();
	reg_line_num++;
	return start_lex;
}
int start_lex_CrKWEmpty()
{
	create_lexem();
	reg_class = semicolon_mark;
	create_lexem();
	return start_lex;
}
int start_lex_CrConstColon()
{
	reg_class = constant_lex;
	add_constant();
	create_lexem();
	reg_class = colon_mark;
	create_lexem();
	return start_lex;
}
int start_lex_CrVarEmpty()
{
	reg_class = variable_lex;
	add_variable();
	create_lexem();
	reg_class = semicolon_mark;
	create_lexem();
	return start_lex;
}
int start_lex_CrConstEmpty()
{
	reg_class = constant_lex;
	add_constant();
	create_lexem();
	reg_class = semicolon_mark;
	create_lexem();
	return start_lex;
}
int arif_CrVarArif() //////////////////////////////////////////
{
	reg_class = variable_lex;
	add_variable();
	create_lexem();
	reg_class = aur_op;
	reg_value = s.value;
	return arif;
}
int read_rel_CrVarRel()
{
	reg_class = variable_lex;
	add_variable();
	create_lexem();
	reg_class = relation;
	reg_relation = s.value;
	return read_rel;
}
int read_keyword_CrRel()
{
	if (reg_relation == '!')
		return Error1();
	if (reg_relation == '<')
		reg_relation = 3;
	else if (reg_relation == '>')
		reg_relation = 4;

	if (reg_relation == '=')
	{
		reg_class = equal_mark;
		reg_relation = 7;
	}
	create_lexem();

	reg_var_name = (s.value);

	if ((s.value > 96 && s.value < 123))
		reg_detection = beg_vector[s.value - (size_t)97];
	if (reg_detection == 0)
	{
		return variable;
	}
	return read_keyword;
}
int constant_CrRel()
{
	if (reg_relation == '!')
		return Error1();
	if (reg_relation == '<')
		reg_relation = 3;
	else if (reg_relation == '>')
		reg_relation = 4;

	if (reg_relation == '=')
	{
		reg_class = equal_mark;
		reg_relation = 7;
	}
	create_lexem();

	const_flag = 1;
	vector_flag = 0;
	reg_number = s.value;
	return constant;
}
int arif_CrConstArif() ////////////
{
	reg_class = constant_lex;
	const_flag = 1;
	add_constant();
	create_lexem();

	reg_class = aur_op;
	reg_value = s.value;
	return arif;
}
int read_rel_CrConstRel()
{
	reg_class = constant_lex;
	const_flag = 1;
	add_constant();
	create_lexem();

	reg_class = relation;
	reg_relation = s.value;
	return read_rel;
}
int kw_var_bracket_CrSw()
{
	reg_class = switch_lex;
	return kw_var_bracket;
}
int kw_var_default_CrDefault()
{
	reg_class = default_lex;
	return kw_var_default;
}
int kw_var_bracket_CrScalarProd()
{
	reg_class = scalarprod;
	return kw_var_bracket;
}
int kw_var_bracket_CrSubvec()
{
	reg_class = subvec;
	return kw_var_bracket;
}
int kw_var_bracket_CrShiftR()
{
	reg_class = shiftr;
	return kw_var_bracket;
}
int kw_var_bracket_CrShiftL()
{
	reg_class = shiftl;
	return kw_var_bracket;
}
int kw_var_bracket_CrAt()
{
	reg_class = at;
	return kw_var_bracket;
}
int kw_var_bracket_CrConc()
{
	reg_class = conc;
	return kw_var_bracket;
}
int start_lex_CrLex_Colon()
{
	create_lexem();
	reg_class = colon_mark;
	create_lexem();
	return start_lex;
}
int start_lex_CrLex_Bracket1()
{
	create_lexem();
	reg_class = round_bracket_open;
	create_lexem();
	return start_lex;
}
int read_keyword_A()
{
	reg_var_name = (s.value);

	if ((s.value > 96 && s.value < 123))
		reg_detection = beg_vector[s.value - (size_t)97];
	if (reg_detection == 0)
	{
		return variable;
	}
	return read_keyword;
}
int read_keyword_B()
{
	reg_detection++;
	return read_keyword;
}
int read_rel_A()
{
	reg_class = relation;
	reg_relation = s.value;
	return read_rel;
}
int variable_A()
{
	reg_var_name = (s.value);
    return variable;
}
int variable_B()
{
	if (s.token_class == Digit)
		reg_var_name += ITOA(s.value);
	else
		reg_var_name += s.value;
	return variable;
}
int constant_A()
{
	const_flag = 1;
	vector_flag = 0;
	reg_number = s.value;
	return constant;
}
int constant_B()
{
	reg_number = 10 * reg_number + s.value;
	return constant;
}
int arif_CrArif()
{
	reg_class = aur_op;
	reg_value = s.value;
	//create_lexem();
	return arif;
}
int read_keyword_CrArif()
{
	create_lexem();
	reg_var_name = (s.value);

	if ((s.value > 96 && s.value < 123))
		reg_detection = beg_vector[s.value - (size_t)97];
	if (reg_detection == 0)
	{
		return variable;
	}
	return read_keyword;
}
int constant_CrArif()
{
	create_lexem();
	const_flag = 1;
	vector_flag = 0;
	reg_number = s.value;
	return constant;
}
int start_lex_CrArif()
{
	create_lexem();
	return start_lex;
}
int start_lex_CrArif_LF()
{
	create_lexem();
	reg_line_num++;
	return start_lex;
}
int start_lex_CrArif_CrRBracket1()
{
	create_lexem();
	reg_class = round_bracket_open;
	create_lexem();
	return start_lex;
}
int start_lex_CrCoutMark()
{
	if (s.value == '>')
	{
		reg_class = cout_mark;
		create_lexem();
		return start_lex;
	}
	else
		return Error1();
}
int start_lex_CrColon()
{
	reg_class = colon_mark;
	create_lexem();
	return start_lex;
}
int start_lex_CrComma()
{
	reg_class = comma_mark;
	create_lexem();
	return start_lex;
}
int start_lex_CrMark()
{
	reg_class = mark;
	add_mark();
	create_lexem();
	return start_lex;
}
int start_lex_CrGTMark()
{
	reg_class = goto_mark_lex;
	add_mark();
	create_lexem();
	return start_lex;
}
int start_lex_CrGTMark_LF()
{
	reg_class = goto_mark_lex;
	add_mark();
	create_lexem();
	reg_line_num++;
	return start_lex;
}
int start_lex_CrGTMarkEmpty()
{
	reg_class = goto_mark_lex;
	add_mark();
	create_lexem();
	reg_class = semicolon_mark;
	create_lexem();
	return start_lex;
}
int start_lex_CrCinMark()
{
	if (s.value == '-')
	{
		reg_class = cin_mark;
		create_lexem();
		return start_lex;
	}
	else
		return Error1();
}
int start_lex_CrConstVec()
{
	reg_class = constant_lex;
	reg_vector.push_back(reg_number);
	add_constant();
	create_lexem();
	reg_vector.Clear();
	return start_lex;
}
int kw_var_CrDim()
{
	reg_class = dim;
	return kw_var;
}
int kw_var_CrAs()
{
	reg_class = as;
	return kw_var;
}
int kw_var_CrInt()
{
	reg_class = int_lex;
	return kw_var;
}
int kw_var_CrVec()
{
	reg_class = vec;
	return kw_var;
}
int kw_var_CrSet()
{
	reg_class = set;
	return kw_var;
}
int kw_var_to_CrTo()
{
	reg_class = to;
	return kw_var_to;
}
int kw_var_bracket2_CrWhile()
{
	reg_class = while_lex;
	return kw_var_bracket2;
}
int kw_var_CrDo()
{
	reg_class = do_lex;
	return kw_var;
}
int kw_var_CrFor()
{
	reg_class = for_lex;
	return kw_var;
}
int kw_var_bracket2_CrIf()
{
	reg_class = if_lex;
	return kw_var_bracket2;
}
int kw_var_CrThen()
{
	reg_class = then_lex;
	return kw_var;
}
int kw_var_CrElse()
{
	reg_class = else_lex;
	return kw_var;
}
int kw_var_CrCin()
{
	reg_class = cin;
	return kw_var;
}
int kw_var_CrCout()
{
	reg_class = cout;
	return kw_var;
}
int kw_var_CrCase()
{
	reg_class = case_lex;
	return kw_var;
}
int kw_var_CrException()
{
	reg_class = exception;
	return kw_var;
}
int kw_var_CrBegin()
{
	reg_class = begin;
	return kw_var;
}
int kw_var_CrEnd()
{
	reg_class = end;
	return kw_var;
}
int goto_space_CrGoto()
{
	reg_class = goto_mark_lex;
	return goto_space;
}
int goto_mark1_A()
{
	return goto_mark1;
}
int goto_mark2_A()
{
	reg_var_name = (s.value);
	return goto_mark2;
}
int goto_mark2_B()
{
	if (s.token_class == Digit)
		reg_var_name += ITOA(s.value);
	else
		reg_var_name += s.value;
	return goto_mark2;
}
int const_vec1_A()
{
	vector_flag = 1;
	const_flag = 0;
	return const_vec1;
}
int const_vec2_A()
{
	reg_number = s.value;
	return const_vec2;
}
int const_vec2_B()
{
	reg_number = 10 * reg_number + s.value;
	return const_vec2;
}
int const_vec1_B()
{
	reg_vector.push_back(reg_number);
	return const_vec1;
}
int const_vec1_CrLexTo()
{
	create_lexem();
	vector_flag = 1;
	const_flag = 0;
	return const_vec1;
}
int cout_mark_A()
{
	return cout_mark;
}
int remark2_A() // НУЖНО ПОДУМАТЬ ЧТО ДЕЛАТЬ С НОМЕРОМ СТРОКИ У ЛЕКСЕМЫ КОММЕНТ НА СТАДИИ ДЕБАГА!!!
{
	if (s.value == '<')
	{
		reg_class = remark;
		return remark2;
	}
	else
		return Error1();
}
int remark2_C()
{
	return remark2;
}
int remark2_B()
{
	if (s.value == '>')
		reg_remark++;
	else
		reg_remark = 0;

	if (reg_remark == 3)
	{
		create_lexem();
		return start_lex;
	}
	return remark2;
}
int remark2_C_LF()
{
	reg_line_num++;
	return remark2;
}
int error1_All()
{
	return error1;
}
int error1_LF()
{
	err_flag_lf = true;
	return Error1();
}
int EXIT1()
{
	reg_class = end_mark;
	create_lexem();
	return s_STOP;
}
int EXIT2()
{
	if (reg_relation == '!')
	{
		reg_class = err;
		err_flag = 1;
	}
	create_lexem();
	reg_class = end_mark;
	create_lexem();
	return s_STOP;
}
int EXIT3() 
{
	reg_class = variable_lex;
	add_variable();
	create_lexem();
	reg_class = end_mark;
	create_lexem();
	return s_STOP;
}
int EXIT4()
{
	reg_class = constant_lex;
	add_constant();
	create_lexem();
	reg_class = end_mark;
	create_lexem();
	return s_STOP;
}
int EXIT5()
{
	reg_class = err;
	err_flag = 1;
	create_lexem();
	err_message = "File ended before closing remark";
	error_messages_list.push_back(err_message);
	reg_class = end_mark;
	create_lexem();
	return s_STOP;
}
int EXIT6()
{
	create_lexem();
	reg_class = end_mark;
	create_lexem();
	return s_STOP;
}
int Rec_Keyword()
{
	if (reg_detection == 0)
	{
		reg_var_name += (s.value);
		return variable;
	}
	if (s.value == std::get<0>(detect_table[(reg_detection - (size_t)1)]))//
	{
		reg_var_name += (s.value);
		return (*std::get<2>(detect_table[(reg_detection - (size_t)1)]))();
	}
	else
	{
		reg_detection = std::get<1>(detect_table[(reg_detection - (size_t)1)]);
		if (reg_detection == 0)
		{
			reg_var_name += (s.value);
			return variable;
		}
		Rec_Keyword();
	}
}

void init_detect_table()
{
	detect_table[0] = std::tuple<char, int, function_pointer>('i', 15, read_keyword_B);
	detect_table[1] = std::tuple<char, int, function_pointer>('m', 0, kw_var_CrDim); //dim
	detect_table[2] = std::tuple<char, int, function_pointer>('s', 81, kw_var_CrAs); //as
	detect_table[3] = std::tuple<char, int, function_pointer>('n', 18, read_keyword_B);
	detect_table[4] = std::tuple<char, int, function_pointer>('t', 0, kw_var_CrInt); //int
	detect_table[5] = std::tuple<char, int, function_pointer>('e', 0, read_keyword_B);
	detect_table[6] = std::tuple<char, int, function_pointer>('c', 0, kw_var_CrVec); //vec
	detect_table[7] = std::tuple<char, int, function_pointer>('e', 33, read_keyword_B);
	detect_table[8] = std::tuple<char, int, function_pointer>('t', 0, kw_var_CrSet); //set
	detect_table[9] = std::tuple<char, int, function_pointer>('o', 19, kw_var_to_CrTo); //to
	detect_table[10] = std::tuple<char, int, function_pointer>('h', 0, read_keyword_B);
	detect_table[11] = std::tuple<char, int, function_pointer>('i', 0, read_keyword_B);
	detect_table[12] = std::tuple<char, int, function_pointer>('l', 0, read_keyword_B);
	detect_table[13] = std::tuple<char, int, function_pointer>('e', 0, kw_var_bracket2_CrWhile); //while
	detect_table[14] = std::tuple<char, int, function_pointer>('o', 41, kw_var_CrDo); //do
	detect_table[15] = std::tuple<char, int, function_pointer>('o', 0, read_keyword_B);
	detect_table[16] = std::tuple<char, int, function_pointer>('r', 0, kw_var_CrFor); //for
	detect_table[17] = std::tuple<char, int, function_pointer>('f', 0, kw_var_bracket2_CrIf); //if
	detect_table[18] = std::tuple<char, int, function_pointer>('h', 0, read_keyword_B);
	detect_table[19] = std::tuple<char, int, function_pointer>('e', 0, read_keyword_B);
	detect_table[20] = std::tuple<char, int, function_pointer>('n', 0, kw_var_CrThen); //then
	detect_table[21] = std::tuple<char, int, function_pointer>('l', 47, read_keyword_B);
	detect_table[22] = std::tuple<char, int, function_pointer>('s', 0, read_keyword_B);
	detect_table[23] = std::tuple<char, int, function_pointer>('e', 0, kw_var_CrElse); //else
	detect_table[24] = std::tuple<char, int, function_pointer>('i', 27, read_keyword_B);
	detect_table[25] = std::tuple<char, int, function_pointer>('n', 0, kw_var_CrCin); //cin
	detect_table[26] = std::tuple<char, int, function_pointer>('o', 38, read_keyword_B);
	detect_table[27] = std::tuple<char, int, function_pointer>('u', 82, read_keyword_B);
	detect_table[28] = std::tuple<char, int, function_pointer>('t', 0, kw_var_CrCout); //cout
	detect_table[29] = std::tuple<char, int, function_pointer>('o', 0, read_keyword_B);
	detect_table[30] = std::tuple<char, int, function_pointer>('t', 0, read_keyword_B);
	detect_table[31] = std::tuple<char, int, function_pointer>('o', 0, goto_space_CrGoto); //goto
	detect_table[32] = std::tuple<char, int, function_pointer>('w', 61, read_keyword_B);
	detect_table[33] = std::tuple<char, int, function_pointer>('i', 0, read_keyword_B);
	detect_table[34] = std::tuple<char, int, function_pointer>('t', 0, read_keyword_B);
	detect_table[35] = std::tuple<char, int, function_pointer>('c', 0, read_keyword_B);
	detect_table[36] = std::tuple<char, int, function_pointer>('h', 0, kw_var_bracket_CrSw); //switch
	detect_table[37] = std::tuple<char, int, function_pointer>('a', 0, read_keyword_B);
	detect_table[38] = std::tuple<char, int, function_pointer>('s', 0, read_keyword_B);
	detect_table[39] = std::tuple<char, int, function_pointer>('e', 0, kw_var_CrCase); //case
	detect_table[40] = std::tuple<char, int, function_pointer>('e', 0, read_keyword_B);
	detect_table[41] = std::tuple<char, int, function_pointer>('f', 0, read_keyword_B);
	detect_table[42] = std::tuple<char, int, function_pointer>('a', 0, read_keyword_B);
	detect_table[43] = std::tuple<char, int, function_pointer>('u', 0, read_keyword_B);
	detect_table[44] = std::tuple<char, int, function_pointer>('l', 0, read_keyword_B);
	detect_table[45] = std::tuple<char, int, function_pointer>('t', 0, kw_var_default_CrDefault); //default
	detect_table[46] = std::tuple<char, int, function_pointer>('x', 59, read_keyword_B);
	detect_table[47] = std::tuple<char, int, function_pointer>('c', 0, read_keyword_B);
	detect_table[48] = std::tuple<char, int, function_pointer>('e', 0, read_keyword_B);
	detect_table[49] = std::tuple<char, int, function_pointer>('p', 0, read_keyword_B);
	detect_table[50] = std::tuple<char, int, function_pointer>('t', 0, read_keyword_B);
	detect_table[51] = std::tuple<char, int, function_pointer>('i', 0, read_keyword_B);
	detect_table[52] = std::tuple<char, int, function_pointer>('o', 0, read_keyword_B);
	detect_table[53] = std::tuple<char, int, function_pointer>('n', 0, kw_var_CrException); //exception
	detect_table[54] = std::tuple<char, int, function_pointer>('e', 0, read_keyword_B);
	detect_table[55] = std::tuple<char, int, function_pointer>('g', 0, read_keyword_B);
	detect_table[56] = std::tuple<char, int, function_pointer>('i', 0, read_keyword_B);
	detect_table[57] = std::tuple<char, int, function_pointer>('n', 0, kw_var_CrBegin); //begin
	detect_table[58] = std::tuple<char, int, function_pointer>('n', 0, read_keyword_B);
	detect_table[59] = std::tuple<char, int, function_pointer>('d', 0, kw_var_CrEnd); //end
	detect_table[60] = std::tuple<char, int, function_pointer>('c', 70, read_keyword_B);
	detect_table[61] = std::tuple<char, int, function_pointer>('a', 0, read_keyword_B);
	detect_table[62] = std::tuple<char, int, function_pointer>('l', 0, read_keyword_B);
	detect_table[63] = std::tuple<char, int, function_pointer>('a', 0, read_keyword_B);
	detect_table[64] = std::tuple<char, int, function_pointer>('r', 0, read_keyword_B);
	detect_table[65] = std::tuple<char, int, function_pointer>('p', 0, read_keyword_B);
	detect_table[66] = std::tuple<char, int, function_pointer>('r', 0, read_keyword_B);
	detect_table[67] = std::tuple<char, int, function_pointer>('o', 0, read_keyword_B);
	detect_table[68] = std::tuple<char, int, function_pointer>('d', 0, kw_var_bracket_CrScalarProd); //scalarprod
	detect_table[69] = std::tuple<char, int, function_pointer>('u', 75, read_keyword_B);
	detect_table[70] = std::tuple<char, int, function_pointer>('b', 0, read_keyword_B);
	detect_table[71] = std::tuple<char, int, function_pointer>('v', 0, read_keyword_B);
	detect_table[72] = std::tuple<char, int, function_pointer>('e', 0, read_keyword_B);
	detect_table[73] = std::tuple<char, int, function_pointer>('c', 0, kw_var_bracket_CrSubvec); //subvec
	detect_table[74] = std::tuple<char, int, function_pointer>('h', 0, read_keyword_B);
	detect_table[75] = std::tuple<char, int, function_pointer>('i', 0, read_keyword_B);
	detect_table[76] = std::tuple<char, int, function_pointer>('f', 0, read_keyword_B);
	detect_table[77] = std::tuple<char, int, function_pointer>('t', 0, read_keyword_B);
	detect_table[78] = std::tuple<char, int, function_pointer>('r', 80, kw_var_bracket_CrShiftR); //shiftr
	detect_table[79] = std::tuple<char, int, function_pointer>('l', 0, kw_var_bracket_CrShiftL); //shiftl
	detect_table[80] = std::tuple<char, int, function_pointer>('t', 0, kw_var_bracket_CrAt); //at
	detect_table[81] = std::tuple<char, int, function_pointer>('n', 0, read_keyword_B);
	detect_table[82] = std::tuple<char, int, function_pointer>('c', 0, kw_var_bracket_CrConc); //conc
}

void initialize_table()
{
	for (int i = 0; i < state_number; i++)
	{
		for (int j = 0; j < class_number; j++)
		{
			table[i][j] = Error1;
		}
	}

	// Далее заполняются ячейки для остальных процедур.

	table[start_lex][Letter] = read_keyword_A;
	table[start_lex][Digit] = constant_A;
	table[start_lex][Aur_OP] = arif_CrArif; 
	table[start_lex][Relation] = read_rel_A;
	table[start_lex][Space] = start_lex_Space; 
	table[start_lex][LF] = start_lex_LF; 
	table[start_lex][Semicolon] = start_lex_CrEmpty; 
	table[start_lex][Colon] = start_lex_CrColon;
	table[start_lex][Comma] = start_lex_CrComma; 
	table[start_lex][OpenSquareBracket] = const_vec1_A; 
	table[start_lex][OpenRoundBracket] = start_lex_CrRoundBracket1;
	table[start_lex][CloseRoundBracket] = start_lex_CrRoundBracket2;
	table[start_lex][EndOfFile] = EXIT1;

	table[read_keyword][Letter] = Rec_Keyword;
	table[read_keyword][Digit] = variable_B;
	table[read_keyword][Aur_OP] = arif_CrVarArif;
	table[read_keyword][Relation] = read_rel_CrVarRel;
	table[read_keyword][Space] = start_lex_CrVar;
	table[read_keyword][LF] = start_lex_CrVar_LF;
	table[read_keyword][Semicolon] = start_lex_CrVarEmpty;
	table[read_keyword][Colon] = start_lex_CrMark;
	table[read_keyword][Comma] = start_lex_CrVar_Comma;
	table[read_keyword][CloseRoundBracket] = start_lex_CrVar_Bracket2;
	table[read_keyword][EndOfFile] = EXIT3;

	table[read_rel][Letter] = read_keyword_CrRel;
	table[read_rel][Digit] = constant_CrRel;
	table[read_rel][Aur_OP] = start_lex_CrCinMark;
	table[read_rel][Relation] = cr_rel_Rem; 
	table[read_rel][Space] = start_lex_CrRel;
	table[read_rel][LF] = start_lex_CrRel_LF; 
	table[read_rel][OpenRoundBracket] = start_lex_CrRelBracket1;
	table[read_rel][EndOfFile] = EXIT2;

	table[variable][Letter] = variable_B;
	table[variable][Digit] = variable_B;
	table[variable][Aur_OP] = arif_CrVarArif;
	table[variable][Relation] = read_rel_CrVarRel;
	table[variable][Space] = start_lex_CrVar;
	table[variable][LF] = start_lex_CrVar_LF;
	table[variable][Semicolon] = start_lex_CrVarEmpty;
	table[variable][Colon] = start_lex_CrMark;
	table[variable][Comma] = start_lex_CrVar_Comma;
	table[variable][CloseRoundBracket] = start_lex_CrVar_Bracket2;
	table[variable][EndOfFile] = EXIT3;

	table[constant][Digit] = constant_B;
	table[constant][Aur_OP] = arif_CrConstArif;
	table[constant][Relation] = read_rel_CrConstRel;
	table[constant][Space] = start_lex_CrConst;
	table[constant][LF] = start_lex_CrConst_LF;
	table[constant][Semicolon] = start_lex_CrConstEmpty;
	table[constant][Colon] = start_lex_CrConstColon;
	table[constant][Comma] = start_lex_CrConst_Comma;
	table[constant][CloseRoundBracket] = start_lex_CrConst_Bracket2;
	table[constant][EndOfFile] = EXIT4;

	table[cr_rel][Letter] = variable_A;
	table[cr_rel][Digit] = constant_A; 
	table[cr_rel][Space] = start_lex_CrLex;
	table[cr_rel][LF] = start_lex_CrLex_LF; 
	table[cr_rel][OpenRoundBracket] = start_lex_CrRoundBracket1;
	table[cr_rel][EndOfFile] = EXIT1;

	table[const_vec1][Digit] = const_vec2_A;
	table[const_vec1][LF] = error1_LF;

	table[const_vec2][Digit] = const_vec2_B;
	table[const_vec2][Space] = const_vec1_B; 
	table[const_vec2][LF] = error1_LF; 
	table[const_vec2][CloseSquareBracket] = start_lex_CrConstVec;

	table[cout_mark][Relation] = start_lex_CrCoutMark;
	table[cout_mark][LF] = error1_LF;

	table[remark1][Relation] = remark2_A;
	table[remark1][LF] = error1_LF;

	table[remark2][Letter] = remark2_C;
	table[remark2][Digit] = remark2_C;
	table[remark2][Aur_OP] = remark2_C; 
	table[remark2][Relation] = remark2_B;
	table[remark2][Space] = remark2_C;
	table[remark2][LF] = remark2_C;
	table[remark2][Semicolon] = remark2_C;
	table[remark2][Colon] = remark2_C;
	table[remark2][Comma] = remark2_C; 
	table[remark2][OpenSquareBracket] = remark2_C;
	table[remark2][CloseSquareBracket] = remark2_C;
	table[remark2][OpenRoundBracket] = remark2_C;
	table[remark2][CloseRoundBracket] = remark2_C; 
	table[remark2][Error] = remark2_C;
	table[remark2][EndOfFile] = EXIT5;

	table[error1][Letter] = error1_All;
	table[error1][Digit] = error1_All;
	table[error1][Aur_OP] = error1_All;
	table[error1][Relation] = error1_All;
	table[error1][Space] = start_lex_Space;
	table[error1][LF] = start_lex_LF; 
	table[error1][Semicolon] = error1_All;
	table[error1][Colon] = error1_All;
	table[error1][Comma] = error1_All; 
	table[error1][OpenSquareBracket] = error1_All;
	table[error1][CloseSquareBracket] = error1_All;
	table[error1][OpenRoundBracket] = error1_All;
	table[error1][CloseRoundBracket] = error1_All;
	table[error1][Error] = error1_All;
	table[error1][EndOfFile] = EXIT1;

	table[kw_var][Letter] = variable_B;
	table[kw_var][Digit] = variable_B;
	table[kw_var][Space] = start_lex_CrKW; 
	table[kw_var][LF] = start_lex_CrKW_LF; 
	table[kw_var][Semicolon] = start_lex_CrKWEmpty;
	table[kw_var][EndOfFile] = EXIT6;

	table[kw_var_bracket][Letter] = variable_B; 
	table[kw_var_bracket][Digit] = variable_B; 
	table[kw_var_bracket][LF] = error1_LF; 
	table[kw_var_bracket][OpenRoundBracket] = start_lex_CrLex_Bracket1;

	table[kw_var_default][Letter] = variable_B;
	table[kw_var_default][Digit] = variable_B;
	table[kw_var_default][LF] = error1_LF; 
	table[kw_var_default][Colon] = start_lex_CrLex_Colon;

	table[goto_space][Letter] = variable_B;
	table[goto_space][Digit] = variable_B;
	table[goto_space][Space] = goto_mark1_A;
	table[goto_space][LF] = error1_LF;

	table[goto_mark1][Letter] = goto_mark2_A;
	table[goto_mark1][LF] = error1_LF;

	table[goto_mark2][Letter] = goto_mark2_B; 
	table[goto_mark2][Digit] = goto_mark2_B; 
	table[goto_mark2][Space] = start_lex_CrGTMark;
	table[goto_mark2][LF] = start_lex_CrGTMark_LF;
	table[goto_mark2][Semicolon] = start_lex_CrGTMarkEmpty;
	//EXTT?

	table[kw_var_bracket2][Letter] = variable_B;
	table[kw_var_bracket2][Digit] = variable_B;
	table[kw_var_bracket2][Space] = start_lex_CrKW;
	table[kw_var_bracket2][LF] = start_lex_CrKW_LF;
	table[kw_var_bracket2][Semicolon] = start_lex_CrKWEmpty;
	table[kw_var_bracket2][OpenRoundBracket] = start_lex_CrLex_Bracket1;
	table[kw_var_bracket2][EndOfFile] = EXIT6;

	table[kw_var_to][Letter] = variable_B;
	table[kw_var_to][Digit] = variable_B;
	table[kw_var_to][Space] = start_lex_CrKW;
	table[kw_var_to][LF] = start_lex_CrKW_LF;
	table[kw_var_to][Semicolon] = start_lex_CrKWEmpty;
	table[kw_var_to][OpenSquareBracket] = const_vec1_CrLexTo;
	table[kw_var_to][OpenRoundBracket] = start_lex_CrLex_Bracket1;
	table[kw_var_to][EndOfFile] = EXIT6;

	table[arif][Letter] = read_keyword_CrArif;
	table[arif][Digit] = constant_CrArif;
	table[arif][Relation] = start_lex_CrCoutMark;
	table[arif][Space] = start_lex_CrArif;
	table[arif][LF] = start_lex_CrArif_LF;
	table[arif][OpenRoundBracket] = start_lex_CrArif_CrRBracket1;
	table[arif][EndOfFile] = EXIT6;
}

void parse(const char* filename)
{
	std::ifstream in(filename);
	if (!in)
	{
		std::cout << "File didn't open " << filename << std::endl;
		return;
	}
	int ch;             // Входной символ.
	int state = start_lex;   // Текущее состояние автомата.
	while (state != s_STOP)
	{
		// s_Stop - техническое состояние. Когда автомат попадает в s_Stop, цикл останавливается.
		ch = in.get();
		s = transliterator(ch);
		error_state = state;
		error_symbolic_token_class = s.token_class;
		state = (*table[state][s.token_class])();
	}

	in.close();
}

const char* relation_table(long long int ch)
{
	switch (ch)
	{
	case(1): {return "=="; }
	case(2): {return "!="; }
	case(3): {return "<"; }
	case(4): {return ">"; }
	case(5): {return "<="; }
	case(6): {return ">="; }
	case(61): {return "="; } //возможно нужно будет добавить метки <<< & >>>, но это не точно
	default:
		break;
	}
	return 0;
}

void printNameTable()
{
	std::cout << "1) Variables:" << std::endl;
	std::cout << "|name" << "\t" << "|value " << std::endl;
	for (auto& it : nameTable)
	{
		if (std::get<0>(it)[0] > '9')
		{
			std::cout << '|' << std::get<0>(it) << "\t" << '|' << std::get<1>(it).integer_value << std::endl;
		}
	}
	std::cout << std::endl;
}

void printConstTable()
{
	std::cout << "2) Constants:" << std::endl;
	std::cout << "|value " << "\t|" << std::endl;
	for (auto& it : nameTable)
	{
		if (std::get<0>(it)[0] > '0' && std::get<0>(it)[0] < '9')
		{
			std::cout << '|' << std::get<1>(it).integer_value << "\t|" << std::endl;
		}
	}
	std::cout << std::endl;

	std::cout << "3) Vectors:" << std::endl;
	std::cout << "value: " << "\t|" << std::endl;
	for (auto& it : nameTable)
	{
		if (std::get<0>(it)[0] == '#')
		{
			std::cout << std::get<1>(it).vector_value << std::endl;
		}
	}
	std::cout << std::endl;
}

void printMarkTable() //скорее всего нужно будет убрать столбец значения
{
	std::cout << "4) Marks:" << std::endl;
	std::cout << "|name" << "\t" << "|value " << std::endl;
	for (auto& it : nameTable)
	{
		if (std::get<0>(it)[0] == '~')
		{
			std::cout << '|' << std::get<0>(it) << "\t" << '|' << std::get<1>(it).integer_value << std::endl;
		}
	}
	std::cout << std::endl;
}

void printLexemList()
{
	std::cout << "5) Lexems:" << std::endl;
	for (auto it = lexem_list.begin(); it != lexem_list.end(); ++it)
	{
		switch (std::get<0>(*it))
		{
		case semicolon_mark:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << ';' << std::endl;
			break;
		}
		case dim:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "dim" << std::endl;
			break;
		}
		case as:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "as" << std::endl;
			break;
		}
		case int_lex:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "int" << std::endl;
			break;
		}
		case vec:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "vec" << std::endl;
			break;
		}
		case set:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "set" << std::endl;
			break;
		}
		case to:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "to" << std::endl;
			break;
		}
		case while_lex:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "while" << std::endl;
			break;
		}
		case do_lex:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "do" << std::endl;
			break;
		}
		case for_lex:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "for" << std::endl;
			break;
		}
		case if_lex:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "if" << std::endl;
			break;
		}
		case then_lex:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "then" << std::endl;
			break;
		}
		case else_lex:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "else" << std::endl;
			break;
		}
		case cin_mark:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "<-" << std::endl;
			break;
		}
		case cout_mark:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "->" << std::endl;
			break;
		}
		case cin:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "cin" << std::endl;
			break;
		}
		case cout:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "cout" << std::endl;
			break;
		}
		case mark:
		{
			std::cout << std::get<2>(*it) << ": ";
			//std::cout << std::get<1>(*it) << std::endl;
			if (((Variable*)std::get<1>(*it))->name[0] == '~') //double check this cringe
			{
				for (int i = 1; i < ((Variable*)std::get<1>(*it))->name.length(); i++)
					std::cout << ((Variable*)std::get<1>(*it))->name[i];
				std::cout << ' ' << std::get<1>(*it) << std::endl;
			}
			break;
		}
		case colon_mark:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << ':' << std::endl;
			break;
		}
		case comma_mark:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << ',' << std::endl;
			break;
		}
		case goto_mark_lex: //mega double check this bruh
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "goto" << ' ';
			if (((Variable*)std::get<1>(*it))->name[0] == '~') //double check this cringe
			{
				for (int i = 1; i < ((Variable*)std::get<1>(*it))->name.length(); i++)
					std::cout << ((Variable*)std::get<1>(*it))->name[i];
				std::cout << ' ' << std::get<1>(*it) << std::endl;
			}
			break;
		}
		case switch_lex:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "switch" << std::endl;
			break;
		}
		case case_lex:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "case" << std::endl;
			break;
		}
		case default_lex:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "default" << std::endl;
			break;
		}
		case exception:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "exception" << std::endl;
			break;
		}
		case remark:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "remark" << std::endl;
			break;
		}
		case begin:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "begin" << std::endl;
			break;
		}
		case end:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "end" << std::endl;
			break;
		}
		case scalarprod:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "scalarprod" << std::endl;
			break;
		}
		case subvec:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "subvec" << std::endl;
			break;
		}
		case shiftr:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "shiftr" << std::endl;
			break;
		}
		case shiftl:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "shiftl" << std::endl;
			break;
		}
		case at:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "at" << std::endl;
			break;
		}
		case conc:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "conc" << std::endl;
			break;
		}
		case equal_mark:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << '=' << std::endl;
			break;
		}
		case variable_lex:
		{
			std::cout << std::get<2>(*it) << ": ";
			if (((Variable*)std::get<1>(*it))->name[0] != '~' || ((Variable*)std::get<1>(*it))->name[0] != '#')
				std::cout << ((Variable*)std::get<1>(*it))->name << ' ';
			std::cout << ((Variable*)std::get<1>(*it))->integer_value << std::endl;
			break;
		}
		case constant_lex:
		{
			std::cout << std::get<2>(*it) << ": ";
			if (((Variable*)std::get<1>(*it))->name[0] >= '0' && ((Variable*)std::get<1>(*it))->name[0] <= '9')
			{
				std::cout << ((Variable*)std::get<1>(*it))->name << std::endl;
			}
			else if (((Variable*)std::get<1>(*it))->name[0] == '#')
			{
				//std::string temp;
				//for (int i = 1; i < ((Variable*)std::get<1>(*it))->name.length(); i++)
				//	std::cout << ((Variable*)std::get<1>(*it))->name[i];
				//std::cout << ' ';
				std::cout << ((Variable*)std::get<1>(*it))->vector_value << std::endl;
			}
			break;
		}
		case aur_op:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << (char)std::get<1>(*it) << std::endl;
			break;
		}
		case relation:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << relation_table(std::get<1>(*it)) << std::endl;
			break;
		}
		case err: //need improved
		{
			std::cout << "error in line" << ' ';
			std::cout << std::get<1>(*it) << std::endl;
			break;
		}
		case end_mark:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << "endoffile" << std::endl;
			break;
		}
		case round_bracket_open:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << '(' << std::endl;
			break;
		}
		case round_bracket_close:
		{
			std::cout << std::get<2>(*it) << ": ";
			std::cout << ')' << std::endl;
			break;
		}
		default:
			break;
		}
	}
}

void PrintErrorList()
{
	std::cout << "6) Error's messages:" << std::endl;
	for (auto it = lexem_list.begin(); it != lexem_list.end(); ++it)
	{
		if (std::get<0>(*it) == err)
		{
			std::cout << "error in line" << ' ';
			std::cout << std::get<1>(*it) << ":   " << error_messages_list.front() << std::endl;
			error_messages_list.pop_front(); //мб по другому надо
		}
	}
}

int main(int argc, char* argv[])
{
	//if (argc != 2)
	//{
	//    cout << "Формат: ./имя-исполняемого-файла имя-входного-файла" << endl;
	//    return 1;
	//}

	// Поскольку автомат не меняется, то таблица инициализируется один раз.
	init_beg_vect();
	init_detect_table();
	initialize_table();
	parse("LIV1.txt");

	std::cout << "\t" << "<Name Table>" << std::endl;
	printNameTable();
	printConstTable();
	printMarkTable();
	printLexemList();
	std::cout << std::endl;
	PrintErrorList();
	//parse(argv[1]);

	return 0;
}
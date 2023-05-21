//#include <iostream>
//#include "vector.h"
//#include <fstream>
//#include <string.h>
//#include <vector>
//#include <map>
//#include <list>
//#include <tuple>
//#include <stack>
//
//
//// Перечисление содержит константы для обозначения классов символьных лексем: БУКВА, ЦИФРА, АР_ОП, ОТНОШ, ПРОБЕЛ, LF, ;, ОШИБКА, КОНЕЦ.
//enum SymbolicTokenClass { Letter, Digit, Aur_OP, Relation, Space, LF, Semicolon, Error, EndOfFile, OpenBracket, CloseBracket, };
//
//// Перечисление содержит константы для обозначения состояний автомата и одну дополнительную константу s_Stop.
//enum States { s_A1, s_A2, s_B1, s_C1, s_D1, s_E1, s_E2, s_E3, s_F1, s_F2, s_F3, s_G1, s_H1, s_I1, s_I2, s_J1, s_K1, s_K2, s_STOP};
//
//enum Lexem { push, pop, ar_op, relat, jmp, ji, read, write, End, comment, err, mark_end, scalarproduct, subvector, shiftr, shiftl, at, conc};
//
//struct variable
//{
//    std::string name;
//    int type; //0 - int; 1 - Vector
//    int integer_value;
//    Vector vector_value;
//};
//
//// Структура для представления символьной лексемы.
//struct SymbolicToken
//{
//    SymbolicTokenClass token_class; // Класс символьной лексемы.
//    int value;                      // Значение символьной лексемы.
//};
//
//// Глобальные переменные, используемые процедурами автомата.
//SymbolicToken s;    // Символьная лексема.
//
//long long int reg_nT_pointer; //Регистр указателя. содержит указатель для лексем PUSH и POP
//int reg_number;    // Регистр числа. используется для вычисления констант
//int reg_relation; //Регистр отношения. хранит информацию о первом символе отношения
//std::string reg_var_name; //Регистр переменной. накапливает имя переменной
//int reg_detection = 0; //Регистр обнаружения. хранит номер позиции в таблице обнаружения для поиска ключевых слов.
//int reg_line_num = 1; //Номер строки. хранит номер текущей строки в программе.
//Lexem reg_class; //Регистр класса. служит для хранения класса лексемы
//int reg_value; //Регистр значения. хранит значения лексем
//Vector reg_vector; //Регистр вектора, хранит его значение
//int reg_count_vector = 1; //Регистр количества векторов
//
//std::map<std::string, variable> nameTable;
//std::list<std::tuple<Lexem, long long int, int>> lexem_list;
//
//std::stack<variable> main_stack;
//
//std::vector<int>beg_vector(26);
//void init_beg_vect()
//{
//    beg_vector[0] = 44;
//    beg_vector[2] = 45;
//    beg_vector[4] = 1;
//    beg_vector[9] = 3;
//    beg_vector[15] = 6;
//    beg_vector[17] = 11;
//    beg_vector[18] = 18;
//    beg_vector[22] = 14;
//}
//
//// преобразование типов
////long long int reg = (long long int)указатель
//// auto reg_ = (variable*)reg;
//
//const int state_number = 18; // Число состояний (без s_Stop).
//const int class_number = 11; // Число символов входного алфавита.
//
//typedef int (*function_pointer)();
//function_pointer table[state_number][class_number];
//
//std::vector<std::tuple<char, int, function_pointer>> detect_table(47);
//
//
///* Функция транслитерации.
// * ch - символ
// * Возвращаемое значение - результат транслитерации символа ch
// */
//SymbolicToken transliterator(int ch)
//{
//    SymbolicToken s;
//    s.value = 0;
//    if (isdigit(ch))
//    {
//        s.token_class = Digit;
//        s.value = ch - '0';
//    }
//    else if ((ch > 64 && ch < 91) || (ch > 96 && ch < 123))
//    {
//        s.token_class = Letter;
//        s.value = ch;
//    }
//    else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
//    {
//        s.token_class = Aur_OP;
//        s.value = ch;
//    }
//    else if (ch == '=' || ch == '!' || ch == '<' || ch == '>')
//    {
//        s.token_class = Relation;
//        s.value = ch;
//    }
//    else if (ch == ' ' || ch == '\t')
//    {
//        s.token_class = Space;
//    }
//    else if (ch == '\n' || ch == '\r\n')
//    {
//        s.token_class = LF;
//    }
//    else if (ch == EOF)
//    {
//        s.token_class = EndOfFile;
//    }
//    else if (ch == ';')
//    {
//        s.token_class = Semicolon;
//        s.value = ch;
//    }
//    else if (ch == '[')
//    {
//        s.token_class = OpenBracket;
//        s.value = ch;
//    }
//    else if (ch == ']')
//    {
//        s.token_class = CloseBracket;
//        s.value = ch;
//    }
//    else
//    {
//        s.token_class = Error;
//    }
//    return s;
//}
//
//
//bool const_flag = 0;
//bool vector_flag = 0;
//bool err_flag = 0;
//
//
////Важные процедуры, не связанные с состояниями
//void create_lexem()
//{
//    if (reg_class == push || reg_class == pop)
//    {
//        std::tuple<Lexem, long long int, int> tup(reg_class, reg_nT_pointer, reg_line_num);
//        lexem_list.push_back(tup);
//    }
//    else if (reg_class == ar_op)
//    {
//        std::tuple<Lexem, long long int, int> tup(reg_class, reg_value, reg_line_num);
//        lexem_list.push_back(tup);
//    }
//    else if (reg_class == relat)
//    {
//        std::tuple<Lexem, long long int, int> tup(reg_class, reg_relation, reg_line_num);
//        lexem_list.push_back(tup);
//    }
//    else if (reg_class == jmp || reg_class == ji)
//    {
//        std::tuple<Lexem, long long int, int> tup(reg_class, reg_number, reg_line_num);
//        lexem_list.push_back(tup);
//    }
//    else if (reg_class == read || reg_class == write || reg_class == End || reg_class == comment || reg_class == mark_end)
//    {
//        std::tuple<Lexem, long long int, int> tup(reg_class, -1, reg_line_num);
//        lexem_list.push_back(tup);
//    }
//    else if (reg_class == scalarproduct || reg_class == subvector || reg_class == shiftr || reg_class == shiftl || reg_class == at || reg_class == conc)
//    {
//        std::tuple<Lexem, long long int, int> tup(reg_class, -1, reg_line_num);
//        lexem_list.push_back(tup);
//    }
//    else if (reg_class == err)
//    {
//        std::tuple<Lexem, long long int, int> tup(reg_class, reg_line_num, reg_line_num);
//        lexem_list.push_back(tup);
//    }
//
//    //используя регистры: класса, указателя, значения, отношения, номер строки
//    // сформировать лексему и кинуть в список
//    //нужно создать тройку (класс, значение, номер строки).
//}
//
//int Error1()
//{
//    reg_class = err;
//    create_lexem();
//    err_flag = 1;
//    return s_J1;
//}
//
//std::string ITOA(int num)
//{
//    std::string str;
//    for (int i = floor(log10(num)); i >= 0; --i)
//    {
//        str.push_back((num / (int)pow(10, i) % 10) + '0');
//    }
//    return str;
//}
//
//void add_constant()
//{
//    if (const_flag == 0 && vector_flag == 0) //check?
//        return;
//
//    variable var;
//    if (const_flag == 1)
//    {
//        var.type = 0;
//        var.name = ITOA(reg_number);
//        var.integer_value = reg_number;
//    }   
//    else
//    {
//        var.type = 1;
//        var.name = "#vec" + ITOA(reg_count_vector);
//        ++reg_count_vector;
//        var.vector_value = reg_vector;
//    }
//
//    bool flag = false;
//    for (auto& it : nameTable)
//    {
//        if ((std::get<0>(it) == var.name) && ((std::get<1>(it).integer_value == var.integer_value) || (std::get<1>(it).vector_value == var.vector_value)))
//        {
//            flag = true;
//            reg_nT_pointer = reinterpret_cast<long long int>(&nameTable[var.name]);
//            return;
//        }
//    }
//
//    if (!flag)
//        nameTable.insert(std::pair<std::string, variable>(var.name, var));
//    reg_nT_pointer = reinterpret_cast<long long int>(&nameTable[var.name]);
//}
//
//void add_variable()
//{
//    if (reg_var_name == "push" || reg_var_name == "pop" || reg_var_name == "jmp" || reg_var_name == "ji")
//    {
//        Error1();
//        return;
//    }
//    else if (reg_var_name == "end" || reg_var_name == "write" || reg_var_name == "read")
//    {
//        Error1();
//        return;
//    }
//    else if (reg_var_name == "scalarproduct" || reg_var_name == "subvector" || reg_var_name == "shiftr" || reg_var_name == "shiftl" || reg_var_name == "at" || reg_var_name == "conc")
//    {
//        Error1();
//        return;
//    }
//
//    variable var;
//    var.name = reg_var_name;
//    var.type = 0;
//    bool flag = false;
//    for (auto& it : nameTable)
//    {
//        if ((std::get<0>(it) == var.name) && (std::get<1>(it).integer_value == var.integer_value) && (std::get<1>(it).type == var.type))
//        {
//            flag = true;
//            reg_nT_pointer = reinterpret_cast<long long int>(&nameTable[var.name]);
//            return;
//        }
//    }
//    if (!flag)
//        nameTable.insert(std::pair<std::string, variable>(var.name, var));
//    reg_nT_pointer = reinterpret_cast<long long int>(&nameTable[var.name]);
//}
//
////Процедуры автомата
//
//int A1a()
//{
//    create_lexem();
//    reg_line_num++;
//    return s_A1;
//}
//int A1b()
//{
//    reg_line_num++;
//    return s_A1;
//}
//int A2a()
//{
//    reg_line_num++;
//    return s_A2;
//}
//int A2b()
//{
//    create_lexem();
//    reg_line_num++;
//    return s_A2;
//}
//int A2c()
//{
//    add_constant();
//    create_lexem();
//    reg_line_num++;
//    return s_A2;
//}
//int A2d()
//{
//    add_variable();
//    if (reg_class != err)
//        create_lexem();
//    reg_line_num++;
//    return s_A2;
//}
//int A2e()
//{
//    if (reg_relation == '!') Error1();
//    if (reg_relation == '=') reg_relation = 1;
//    else if (reg_relation == '<') reg_relation = 3;
//    else if (reg_relation == '>') reg_relation = 4;
//    create_lexem();
//    reg_line_num++;
//    return s_A2;
//}
//int A2f()
//{
//    reg_class = err;
//    err_flag = 1;
//    create_lexem();
//    reg_line_num++;
//    return s_A2;
//}
//int B1a()
//{
//    reg_detection = beg_vector[s.value - (size_t)97];
//    if (reg_detection == 0)
//        Error1();
//    return s_B1;
//
//}
//int B1b()
//{
//    ++reg_detection;
//    return s_B1;
//}
//int C1a()
//{
//    reg_class = ar_op;
//    reg_value = s.value;
//    create_lexem();
//    return s_C1;
//}
//int C1b()
//{
//    reg_class = End;
//    create_lexem();
//    return s_C1;
//}
//int C1c()
//{
//    reg_class = read;
//    create_lexem();
//    return s_C1;
//}
//int C1d()
//{
//    reg_class = write;
//    create_lexem();
//    return s_C1;
//}
//int C1e()
//{
//    add_constant();
//    create_lexem();
//    return s_C1;
//}
//int C1f()
//{
//    add_variable();
//    create_lexem();
//    return s_C1;
//}
//int C1g()
//{
//    if (reg_relation == '!')
//        Error1();
//    if (reg_relation == '=')
//        reg_relation = 1;
//    else if (reg_relation == '<')
//        reg_relation = 3;
//    else if (reg_relation == '>')
//        reg_relation = 4;
//
//    create_lexem();
//    return s_C1;
//}
//int C1h()
//{
//    if (s.value == '=')
//    {
//        if (reg_relation == '!')
//        {
//            reg_relation = 2;
//        }
//        else if (reg_relation == '<')
//        {
//            reg_relation = 5;
//        }
//        else if (reg_relation == '>')
//        {
//            reg_relation = 6;
//        }
//        else
//            return Error1();
//
//        create_lexem();
//        return s_C1;
//    }
//    else
//        return Error1();
//}
//int C1i()
//{
//    reg_class = scalarproduct;
//    create_lexem();
//    return s_C1;
//}
//int C1j()
//{
//    reg_class = subvector;
//    create_lexem();
//    return s_C1;
//}
//int C1k()
//{
//    reg_class = shiftr;
//    create_lexem();
//    return s_C1;
//}
//int C1l()
//{
//    reg_class = shiftl;
//    create_lexem();
//    return s_C1;
//}
//int C1m()
//{
//    reg_class = at;
//    create_lexem();
//    return s_C1;
//}
//int C1n()
//{
//    reg_class = conc;
//    create_lexem();
//    return s_C1;
//}
//int C1t()
//{
//    reg_vector.push_back(reg_number);
//    add_constant();
//    create_lexem();
//    reg_vector.Clear();
//    return s_C1;
//}
//int D1a()
//{
//    reg_class = relat;
//    reg_relation = s.value;
//    return s_D1;
//}
//int E1a()
//{
//    reg_class = push;
//    const_flag = 1;
//    return s_E1;
//}
//int E2a()
//{
//    reg_class = ji;
//    const_flag = 0;
//    return s_E2;
//}
//int E2b()
//{
//    reg_class = jmp;
//    const_flag = 0;
//    return s_E2;
//}
//int E3a()
//{
//    reg_class = pop;
//    return s_E3;
//}
//int G1a()
//{
//    reg_number = s.value;
//    return s_G1;
//}
//int G1b()
//{
//    reg_number = 10 * reg_number + s.value;
//    return s_G1;
//}
//int H1a()
//{
//    reg_var_name = (s.value);
//    return s_H1;
//}
//int H1b()
//{
//    if (s.token_class == Digit)
//        reg_var_name += ITOA(s.value);
//    else
//        reg_var_name += s.value;
//    return s_H1;
//}
//int I1a()
//{
//    reg_class = comment;
//    return s_I1;
//}
//int I2a()
//{
//    reg_class = comment;
//    return s_I2;
//}
//int I2b()
//{
//    add_constant();
//    create_lexem();
//    reg_class = comment;
//    return s_I2;
//}
//int I2c()
//{
//    add_variable();
//    create_lexem();
//    reg_class = comment;
//    return s_I2;
//}
//int I2d()
//{
//    if (reg_relation == '!')
//        Error1();
//    create_lexem();
//    reg_class = comment;
//    return s_I2;
//
//}
//int EXIT1()
//{
//    reg_class = mark_end;
//    create_lexem();
//    return s_STOP;
//}
//int EXIT2()
//{
//    if (reg_relation == '!')
//    {
//        reg_class = err;
//        err_flag = 1;
//    }
//    create_lexem();
//    reg_class = mark_end;
//    create_lexem();
//    return s_STOP;
//}
//int EXIT3()
//{
//    add_constant();
//    create_lexem();
//    reg_class = mark_end;
//    create_lexem();
//    return s_STOP;
//}
//int EXIT4()
//{
//    add_variable();
//    create_lexem();
//    reg_class = mark_end;
//    create_lexem();
//    return s_STOP;
//}
//int p_A1()
//{
//    return s_A1;
//}
//int p_A2()
//{
//    reg_line_num++;
//    return s_A2;
//}
//int p_C1()
//{
//    return s_C1;
//}
//int p_F1()
//{
//    return s_F1;
//}
//int p_F2()
//{
//    return s_F2;
//}
//int p_F3()
//{
//    return s_F3;
//}
//int p_I1()
//{
//    return s_I1;
//}
//int p_I2()
//{
//    return s_I2;
//}
//int p_J1()
//{
//    return s_J1;
//}
//int M1()
//{
//    if (reg_detection == 0)
//        return s_J1;
//    if (s.value == std::get<0>(detect_table[(reg_detection - (size_t)1)]))//
//        return (*std::get<2>(detect_table[(reg_detection - (size_t)1)]))();
//    else
//    {
//        reg_detection = std::get<1>(detect_table[(reg_detection - (size_t)1)]);
//        if (reg_detection == 0)
//            Error1();
//        M1();
//    }
//}
//int K1a()
//{
//    vector_flag = 1;
//    const_flag = 0;
//    return s_K1;
//}
//int K2a()
//{
//    reg_number = s.value;
//    return s_K2;
//}
//int K2b()
//{
//    reg_number = 10 * reg_number + s.value;
//    return s_K2;
//}
//int K1b()
//{
//    reg_vector.push_back(reg_number);
//    return s_K1;
//}
//
//void init_detect_table()
//{
//    detect_table[0] = std::tuple<char, int, function_pointer>('n', 0, B1b);
//    detect_table[1] = std::tuple<char, int, function_pointer>('d', 0, C1b);
//    detect_table[2] = std::tuple<char, int, function_pointer>('i', 4, E2a);
//    detect_table[3] = std::tuple<char, int, function_pointer>('m', 0, B1b);
//    detect_table[4] = std::tuple<char, int, function_pointer>('p', 0, E2b);
//    detect_table[5] = std::tuple<char, int, function_pointer>('o', 8, B1b);
//    detect_table[6] = std::tuple<char, int, function_pointer>('p', 0, E3a);
//    detect_table[7] = std::tuple<char, int, function_pointer>('u', 0, B1b);
//    detect_table[8] = std::tuple<char, int, function_pointer>('s', 0, B1b);
//    detect_table[9] = std::tuple<char, int, function_pointer>('h', 0, E1a);
//    detect_table[10] = std::tuple<char, int, function_pointer>('e', 0, B1b);
//    detect_table[11] = std::tuple<char, int, function_pointer>('a', 0, B1b);
//    detect_table[12] = std::tuple<char, int, function_pointer>('d', 0, C1c);
//    detect_table[13] = std::tuple<char, int, function_pointer>('r', 0, B1b);
//    detect_table[14] = std::tuple<char, int, function_pointer>('i', 0, B1b);
//    detect_table[15] = std::tuple<char, int, function_pointer>('t', 0, B1b);
//    detect_table[16] = std::tuple<char, int, function_pointer>('e', 0, C1d);
//    detect_table[17] = std::tuple<char, int, function_pointer>('c', 30, B1b);
//    detect_table[18] = std::tuple<char, int, function_pointer>('a', 0, B1b);
//    detect_table[19] = std::tuple<char, int, function_pointer>('l', 0, B1b);
//    detect_table[20] = std::tuple<char, int, function_pointer>('a', 0, B1b);
//    detect_table[21] = std::tuple<char, int, function_pointer>('r', 0, B1b);
//    detect_table[22] = std::tuple<char, int, function_pointer>('p', 0, B1b);
//    detect_table[23] = std::tuple<char, int, function_pointer>('r', 0, B1b);
//    detect_table[24] = std::tuple<char, int, function_pointer>('o', 0, B1b);
//    detect_table[25] = std::tuple<char, int, function_pointer>('d', 0, B1b);
//    detect_table[26] = std::tuple<char, int, function_pointer>('u', 0, B1b);
//    detect_table[27] = std::tuple<char, int, function_pointer>('c', 0, B1b);
//    detect_table[28] = std::tuple<char, int, function_pointer>('t', 0, C1i);
//    detect_table[29] = std::tuple<char, int, function_pointer>('u', 38, B1b);
//    detect_table[30] = std::tuple<char, int, function_pointer>('b', 0, B1b);
//    detect_table[31] = std::tuple<char, int, function_pointer>('v', 0, B1b);
//    detect_table[32] = std::tuple<char, int, function_pointer>('e', 0, B1b);
//    detect_table[33] = std::tuple<char, int, function_pointer>('c', 0, B1b);
//    detect_table[34] = std::tuple<char, int, function_pointer>('t', 0, B1b);
//    detect_table[35] = std::tuple<char, int, function_pointer>('o', 0, B1b);
//    detect_table[36] = std::tuple<char, int, function_pointer>('r', 0, C1j);
//    detect_table[37] = std::tuple<char, int, function_pointer>('h', 0, B1b);
//    detect_table[38] = std::tuple<char, int, function_pointer>('i', 0, B1b);
//    detect_table[39] = std::tuple<char, int, function_pointer>('f', 0, B1b);
//    detect_table[40] = std::tuple<char, int, function_pointer>('t', 0, B1b);
//    detect_table[41] = std::tuple<char, int, function_pointer>('r', 43, C1k);
//    detect_table[42] = std::tuple<char, int, function_pointer>('l', 0, C1l);
//    detect_table[43] = std::tuple<char, int, function_pointer>('t', 0, C1m);
//    detect_table[44] = std::tuple<char, int, function_pointer>('o', 0, B1b);
//    detect_table[45] = std::tuple<char, int, function_pointer>('n', 0, B1b);
//    detect_table[46] = std::tuple<char, int, function_pointer>('c', 0, C1n);
//}
//
//void initialize_table()
//{
//    for (int i = 0; i < state_number; i++)
//    {
//        for (int j = 0; j < class_number; j++)
//        {
//            table[i][j] = Error1;
//        }
//    }
//    // Далее заполняются ячейки для остальных процедур.
//    table[s_A1][Letter] = B1a; table[s_A1][Aur_OP] = C1a; table[s_A1][Relation] = D1a;
//    table[s_A1][Space] = p_A1; table[s_A1][LF] = A1b; table[s_A1][Semicolon] = I1a;
//
//    table[s_A2][Letter] = B1a; table[s_A2][Aur_OP] = C1a; table[s_A2][Relation] = D1a;
//    table[s_A2][Space] = p_A2; table[s_A2][LF] = A2a; table[s_A2][Semicolon] = I2a; table[s_A2][EndOfFile] = EXIT1;
//
//    table[s_B1][Letter] = M1; table[s_B1][LF] = A2f;
//
//    table[s_C1][Space] = p_C1; table[s_C1][LF] = A2a; table[s_C1][Semicolon] = I2a; table[s_C1][EndOfFile] = EXIT1;
//
//    table[s_D1][Relation] = C1h; table[s_D1][Space] = C1g; table[s_D1][LF] = A2e; table[s_D1][Semicolon] = I2d; table[s_D1][EndOfFile] = EXIT2;
//
//    table[s_E1][Space] = p_F1; table[s_E1][LF] = A2f;
//
//    table[s_E2][Space] = p_F2; table[s_E2][LF] = A2f;
//    table[s_E3][Space] = p_F3; table[s_E3][LF] = A2f;
//
//    table[s_F1][Letter] = H1a; table[s_F1][Digit] = G1a; table[s_F1][Space] = p_F1; table[s_F1][LF] = A2f;
//
//    table[s_F2][Digit] = G1a; table[s_F2][Space] = p_F2; table[s_F2][LF] = A2f;
//
//    table[s_F3][Letter] = H1a; table[s_F3][Space] = p_F3; table[s_F3][LF] = A2f;
//
//    table[s_G1][Digit] = G1b; table[s_G1][Space] = C1e; table[s_G1][LF] = A2c; table[s_G1][Semicolon] = I2b; table[s_G1][EndOfFile] = EXIT3;
//
//    table[s_H1][Letter] = H1b; table[s_H1][Digit] = H1b; table[s_H1][Space] = C1f; table[s_H1][LF] = A2d; table[s_H1][Semicolon] = I2c; table[s_H1][EndOfFile] = EXIT4;
//
//    for (int i = 0; i < 11; ++i)
//    {
//        if (i == LF)
//        {
//            table[s_I1][i] = A1a;
//            table[s_I2][i] = A2b;
//            table[s_J1][i] = A2a;
//        }
//        else
//        {
//            table[s_I1][i] = p_I1;
//            table[s_I2][i] = p_I2;
//            table[s_J1][i] = p_J1;
//        }
//    }
//
//    table[s_I2][EndOfFile] = EXIT1;
//    table[s_J1][EndOfFile] = EXIT1;
//
//    table[s_F1][OpenBracket] = K1a;
//    table[s_K1][Digit] = K2a;
//    table[s_K2][Digit] = K2b; table[s_K2][Space] = K1b; table[s_K2][CloseBracket] = C1t;
//}
//
//void parse(const char* filename)
//{
//    std::ifstream in(filename);
//    if (!in)
//    {
//        std::cout << "File didn't open " << filename << std::endl;
//        return;
//    }
//    int ch;             // Входной символ.
//    int state = s_A1;   // Текущее состояние автомата.
//    while (state != s_STOP)
//    {
//        // s_Stop - техническое состояние. Когда автомат попадает в s_Stop, цикл останавливается.
//        ch = in.get();
//        s = transliterator(ch);
//        state = (*table[state][s.token_class])();
//    }
//
//    in.close();
//}
//
//const char* relation_table(long long int ch)
//{
//    switch (ch)
//    {
//    case(1): {return "="; }
//    case(2): {return "!="; }
//    case(3): {return "<"; }
//    case(4): {return ">"; }
//    case(5): {return "<="; }
//    case(6): {return ">="; }
//    default:
//        break;
//    }
//    return 0;
//}
//
//void printNameTable()
//{
//    std::cout << "1) Variables:" << std::endl;
//    std::cout << "|name" << "\t" << "|value " << std::endl;
//    for (auto& it : nameTable)
//    {
//        if (std::get<0>(it)[0] > '9')
//        {
//            std::cout << '|' << std::get<0>(it) << "\t" << '|' << std::get<1>(it).integer_value << std::endl;
//        }
//    }
//    std::cout << std::endl;
//}
//
//void printConstTable()
//{
//    std::cout << "2) Constants:" << std::endl;
//    std::cout << "|value " << "\t|" << std::endl;
//    for (auto& it : nameTable)
//    {
//        if (std::get<0>(it)[0] > '0' && std::get<0>(it)[0] < '9')
//        {
//            std::cout << '|' << std::get<1>(it).integer_value << "\t|" << std::endl;
//        }
//    }
//    std::cout << std::endl;
//
//    std::cout << "3) Vectors:" << std::endl;
//    std::cout << "value: " << std::endl;
//    for (auto& it : nameTable)
//    {
//        if (std::get<0>(it)[0] == '#')
//        {
//            std::cout << std::get<1>(it).vector_value << std::endl;
//        }
//    }
//    std::cout << std::endl;
//}
//
//void printLexemList()
//{
//    for (auto it = lexem_list.begin(); it != lexem_list.end(); ++it)
//    {
//
//        switch (std::get<0>(*it))
//        {
//        case 0:
//        {
//            std::cout << std::get<2>(*it) << ": ";
//            std::cout << "push" << ' ';
//            if (((variable*)std::get<1>(*it))->name[0] > '9' || ((variable*)std::get<1>(*it))->name[0] == '#')
//                std::cout << ((variable*)std::get<1>(*it))->name << ' ';
//            std::cout << ((variable*)std::get<1>(*it))->integer_value << std::endl;
//            break;
//        }
//        case 1:
//        {
//            std::cout << std::get<2>(*it) << ": ";
//            std::cout << "pop" << ' ';
//            std::cout << ((variable*)std::get<1>(*it))->name << std::endl;
//            break;
//        }
//        case 2:
//        {
//            std::cout << std::get<2>(*it) << ": ";
//            std::cout << (char)std::get<1>(*it) << std::endl;
//            break;
//        }
//        case 3:
//        {
//            std::cout << std::get<2>(*it) << ": ";
//            std::cout << relation_table(std::get<1>(*it)) << std::endl;
//            break;
//        }
//        case 4:
//        {
//            std::cout << std::get<2>(*it) << ": ";
//            std::cout << "jmp" << ' ';
//            std::cout << std::get<1>(*it) << std::endl;
//            break;
//        }
//        case 5:
//        {
//            std::cout << std::get<2>(*it) << ": ";
//            std::cout << "ji" << ' ';
//            std::cout << std::get<1>(*it) << std::endl;
//            break;
//        }
//        case 6:
//        {
//            std::cout << std::get<2>(*it) << ": ";
//            std::cout << "read" << std::endl;
//            break;
//        }
//        case 7:
//        {
//            std::cout << std::get<2>(*it) << ": ";
//            std::cout << "write" << std::endl;
//            break;
//        }
//        case 8:
//        {
//            std::cout << std::get<2>(*it) << ": ";
//            std::cout << "end" << std::endl;
//            break;
//        }
//        case 9:
//        {
//            std::cout << std::get<2>(*it) << ": ";
//            std::cout << "comment" << std::endl;
//            break;
//        }
//        case 10:
//        {
//            std::cout << "error in line" << ' ';
//            std::cout << std::get<1>(*it) << std::endl;
//            break;
//        }
//        case 11:
//        {
//            std::cout << std::get<2>(*it) << ": ";
//            std::cout << "endoffile" << std::endl;
//            break;
//
//        }
//        default:
//            break;
//        };
//
//    }
//
//}
//
//void interpretator()
//{
//    if (err_flag)
//    {
//        std::cout << "There are errors in programm" << std::endl;
//        return;
//    }
//    for (auto lex = lexem_list.begin(); lex != lexem_list.end(); ++lex)
//    {
//        switch (std::get<0>(*lex))// при нагрузке учесть поля type и разность типов
//        {
//        case 0:// push
//        {
//            main_stack.push(*(variable*)std::get<1>(*lex));
//            break;
//        }
//        case 1:// pop
//        {
//            if (main_stack.top().type == 0)
//                (*(variable*)std::get<1>(*lex)).integer_value = main_stack.top().integer_value;
//            else
//                (*(variable*)std::get<1>(*lex)).vector_value = main_stack.top().vector_value;
//            (*(variable*)std::get<1>(*lex)).type = main_stack.top().type;
//
//            main_stack.pop();
//            break;
//        }
//        case 2:// aurethmetic operation
//        {
//            auto secondVar = main_stack.top();
//            main_stack.pop();
//            auto firstVar = main_stack.top();
//            main_stack.pop();
//
//            if (firstVar.type != secondVar.type)
//            {
//                std::cout << "Error in interpretator: Type mismatch!" << std::endl;
//                return;
//            }
//
//            variable res;
//            res.name = "";
//            switch (std::get<1>(*lex))// при нагрузке учитывать типы
//            {
//            case 37: // %
//            {
//                if (firstVar.type == 1 || secondVar.type == 1)
//                {
//                    std::cout << "Error in interpretator: You can't use % on a vector!" << std::endl;
//                    return;
//                }
//                res.integer_value = (firstVar.integer_value) % (secondVar.integer_value);
//                res.type = 0;
//                main_stack.push(res);
//
//                break;
//            }
//            case 42: // *
//            {
//                if (firstVar.type == 0 && secondVar.type == 0)
//                {
//                    res.integer_value = (firstVar.integer_value) * (secondVar.integer_value);
//                    res.type = 0;
//                }
//                else
//                {
//                    res.vector_value = (firstVar.vector_value) * (secondVar.vector_value);
//                    res.type = 1;
//                }
//                main_stack.push(res);
//
//                break;
//            }
//            case 43: // +
//            {
//                if (firstVar.type == 0 && secondVar.type == 0)
//                {
//                    res.integer_value = (firstVar.integer_value) + (secondVar.integer_value);
//                    res.type = 0;
//                }
//                else
//                {
//                    res.vector_value = (firstVar.vector_value) + (secondVar.vector_value);
//                    res.type = 1;
//                }
//                main_stack.push(res);
//
//                break;
//            }
//            case 45: // -
//            {
//                if (firstVar.type == 0 && secondVar.type == 0)
//                {
//                    res.integer_value = (firstVar.integer_value) - (secondVar.integer_value);
//                    res.type = 0;
//                }
//                else
//                {
//                    res.vector_value = (firstVar.vector_value) - (secondVar.vector_value);
//                    res.type = 1;
//                }
//                main_stack.push(res);
//
//                break;
//            }
//            case 47: // /
//            {
//                if (firstVar.type == 0 && secondVar.type == 0)
//                {
//                    res.integer_value = (firstVar.integer_value) / (secondVar.integer_value);
//                    res.type = 0;
//                }
//                else
//                {
//                    res.vector_value = (firstVar.vector_value) / (secondVar.vector_value);
//                    res.type = 1;
//                }
//                main_stack.push(res);
//
//                break;
//            }
//            default:
//                break;
//            };
//            break;
//        }
//        case 3:// relations
//        {
//            auto secondVar = main_stack.top();
//            main_stack.pop();
//            auto firstVar = main_stack.top();
//            main_stack.pop();
//
//            if (firstVar.type != secondVar.type)
//            {
//                std::cout << "Error in interpretator: Type mismatch!" << std::endl;
//                return;
//            }
//
//            variable res;
//            res.name = "";
//            res.type = 0;
//            switch (std::get<1>(*lex))// при нагрузке учитывать типы
//            {
//            case 1: // =
//            {
//                if (firstVar.type == 0 && secondVar.type == 0)
//                    res.integer_value = (firstVar.integer_value) == (secondVar.integer_value);
//                else
//                    res.integer_value = (firstVar.vector_value) == (secondVar.vector_value);
//                main_stack.push(res);
//
//                break;
//            }
//            case 2: // !=
//            {
//                if (firstVar.type == 0 && secondVar.type == 0)
//                    res.integer_value = (firstVar.integer_value) != (secondVar.integer_value);
//                else
//                    res.integer_value = (firstVar.vector_value) != (secondVar.vector_value);
//                main_stack.push(res);
//
//                break;
//            }
//            case 3: // <
//            {
//                if (firstVar.type == 1 || secondVar.type == 1)
//                {
//                    std::cout << "Error in interpretator: You can't use < on a vector!" << std::endl;
//                    return;
//                }
//                res.integer_value = (firstVar.integer_value) < (secondVar.integer_value);
//                main_stack.push(res);
//
//                break;
//            }
//            case 4: // >
//            {
//                if (firstVar.type == 1 || secondVar.type == 1)
//                {
//                    std::cout << "Error in interpretator: You can't use > on a vector!" << std::endl;
//                    return;
//                }
//                res.integer_value = (firstVar.integer_value) > (secondVar.integer_value);
//                main_stack.push(res);
//
//                break;
//            }
//            case 5: // <=
//            {
//                if (firstVar.type == 1 || secondVar.type == 1)
//                {
//                    std::cout << "Error in interpretator: You can't use <= on a vector!" << std::endl;
//                    return;
//                }
//                res.integer_value = (firstVar.integer_value) <= (secondVar.integer_value);
//                main_stack.push(res);
//
//                break;
//            }
//            case 6: // >=
//            {
//                if (firstVar.type == 1 || secondVar.type == 1)
//                {
//                    std::cout << "Error in interpretator: You can't use >= on a vector!" << std::endl;
//                    return;
//                }
//                res.integer_value = (firstVar.integer_value) >= (secondVar.integer_value);
//                main_stack.push(res);
//
//                break;
//            }
//            default:
//                break;
//            };
//            break;
//        }
//        case 4:// jmp
//        {
//            long long int line_num = std::get<1>(*lex);
//            for (auto lexSearch = lexem_list.begin(); lexSearch != lexem_list.end(); ++lexSearch)// double-check on this bad boy
//            {
//                if (std::get<2>(*lexSearch) >= line_num)
//                {
//                    lex = lexSearch;
//                    --lex;
//                    break;
//                }
//            }
//            break;
//        }
//        case 5:// ji
//        {
//            if (main_stack.top().integer_value == 0)
//            {
//                main_stack.pop();
//                break;
//            }
//            long long int line_num = std::get<1>(*lex);
//            main_stack.pop();
//            for (auto lexSearch = lexem_list.begin(); lexSearch != lexem_list.end(); ++lexSearch)// double-check on this bad boy
//            {
//                if (std::get<2>(*lexSearch) >= line_num)
//                {
//                    lex = lexSearch;
//                    --lex;
//                    break;
//                }
//            }
//            break;
//        }
//        case 6:// read
//        {
//            variable var;
//
//            if (std::cin.peek() == '[')
//            {
//                Vector vec;
//                std::cin >> vec;
//                var.name = "#vec";
//                var.vector_value = vec;
//                var.type = 1;
//            }
//            else
//            {
//                int input = 0;
//                std::cin >> input;
//                if (input < 0)
//                {
//                    std::cout << "Non-natural number has been inputed" << std::endl;
//                    return;
//                }
//                var.name = "";
//                var.integer_value = input;
//                var.type = 0;
//            }
//            std::cout << std::endl;
//
//            main_stack.push(var);
//
//            break;
//        }
//        case 7:// write
//        {
//            if (main_stack.top().type == 0)
//                std::cout << main_stack.top().integer_value << std::endl;// учесть типы
//            else 
//                std::cout << main_stack.top().vector_value << std::endl;
//
//            main_stack.pop();
//
//            break;
//        }
//        case 8:// end
//        {
//            return;
//            break;
//        }
//        //case 10:
//        //{
//        //   std::cout << "err" << ' ';
//        //   std::cout << std::get<1>(*it) << ' ';
//        //   std::cout << std::get<2>(*it) << std::endl;
//        //   break;
//        //}
//        case 11:// EndOfFile 
//        {
//            return;
//            break;
//        }
//        case 12: //scalarproduct
//        {
//            auto secondVec = main_stack.top();
//            main_stack.pop();
//            auto firstVec = main_stack.top();
//            main_stack.pop();
//
//            if (secondVec.type != 1 || firstVec.type != 1)
//            {
//                std::cout << "Error in interpretator: function scalarproduct has bad arguments (wrong types)" << std::endl;
//                return;
//            }
//
//            variable res;
//            res.name = "";
//            res.type = 0;
//            res.integer_value = ScalarProduct(firstVec.vector_value, secondVec.vector_value);
//            main_stack.push(res);
//
//            break;
//        }
//        case 13: //subvector
//        {
//            auto secondVar = main_stack.top();
//            main_stack.pop();
//            auto firstVar = main_stack.top();
//            main_stack.pop();
//            auto vec = main_stack.top();
//            main_stack.pop();
//            
//            if (secondVar.type != 0 || firstVar.type != 0 || vec.type != 1)
//            {
//                std::cout << "Error in interpretator: function subvector has bad arguments (wrong types)" << std::endl;
//                return;
//            }
//
//            variable subVec;
//            subVec.name = "";
//            subVec.type = 1;
//            subVec.vector_value = vec.vector_value.SubVector(firstVar.integer_value, secondVar.integer_value);
//
//            main_stack.push(subVec);
//
//            break;
//        }
//        case 14: //shiftr
//        {
//            auto num = main_stack.top();
//            main_stack.pop();
//            auto vec = main_stack.top();
//            main_stack.pop();
//
//            if (num.type != 0 || vec.type != 1)
//            {
//                std::cout << "Error in interpretator: function shiftr has bad arguments (wrong types)" << std::endl;
//                return;
//            }
//
//            variable res;
//            res.name = "";
//            res.type = 1;
//            res.vector_value = vec.vector_value >>= num.integer_value;
//
//            main_stack.push(res);
//
//            break;
//        }
//        case 15: //shiftl
//        {
//            auto num = main_stack.top();
//            main_stack.pop();
//            auto vec = main_stack.top();
//            main_stack.pop();
//
//            if (num.type != 0 || vec.type != 1)
//            {
//                std::cout << "Error in interpretator: function shiftl has bad arguments (wrong types)" << std::endl;
//                return;
//            }
//
//            variable res;
//            res.name = "";
//            res.type = 1;
//            res.vector_value = vec.vector_value <<= num.integer_value;
//
//            main_stack.push(res);
//
//            break;
//        }
//        case 16: //at
//        {
//            auto num = main_stack.top();
//            main_stack.pop();
//            auto vec = main_stack.top();
//            main_stack.pop();
//
//            if (num.type != 0 || vec.type != 1)
//            {
//                std::cout << "Error in interpretator: function at has bad arguments (wrong types)" << std::endl;
//                return;
//            }
//
//            variable res;
//            res.name = "";
//            res.type = 0;
//            res.integer_value = vec.vector_value[num.integer_value];
//            if (res.integer_value < 0)
//            {
//                std::cout << "Error in interpretator: non-natural number has pushed in stack";
//                return;
//            }
//
//            main_stack.push(res);
//
//            break;
//        }
//        case 17: // conc
//        {
//            auto vec2 = main_stack.top();
//            main_stack.pop();
//            auto vec1 = main_stack.top();
//            main_stack.pop();
//
//            if (vec1.type != 1 || vec2.type != 1)
//            {
//                std::cout << "Error in interpretator: function conc has bad arguments (wrong types)";
//                return;
//            }
//
//            variable res;
//            res.name = "";
//            res.type = 1;
//            res.vector_value = vec1.vector_value += vec2.vector_value;
//
//            main_stack.push(res);
//
//            break;
//        }
//        default:
//            break;
//        };
//    }
//}
//
//
////Вызов программы имеет вид ./имя-исполняемого-файла имя-входного-файла.
//int main(int argc, char* argv[])
//{
//    //if (argc != 2)
//    //{
//    //    cout << "Формат: ./имя-исполняемого-файла имя-входного-файла" << endl;
//    //    return 1;
//    //}
//
//    // Поскольку автомат не меняется, то таблица инициализируется один раз.
//    init_beg_vect();
//    init_detect_table();
//    initialize_table();
//    parse("test.txt");
//
//    std::cout << "\t" << "<Name Table>" << std::endl;
//    printNameTable();
//    printConstTable();
//    printLexemList();
//    interpretator();
//    //parse(argv[1]);
//
//    return 0;
//}
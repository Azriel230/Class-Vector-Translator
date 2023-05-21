
//#include "vector.h"
//#include <iostream>
//
//int main()
//{
//	Vector vec1;
//	std::cin >> vec1;
//	std::cout << std::endl << "vec1: " << vec1 << std::endl;
//	//int a[] = { 1,2,3,4,5 };
//	//Vector vec1(a, 5);
//
//	Vector vec2;
//	std::cin >> vec2;
//	std::cout << std::endl << "vec2: " << vec2 << std::endl;
//
//	//vec1 += vec2;
//	//std::cout << std::endl << "vec1: " << vec1 << std::endl;
//
//	//std::cout << std::endl;
//	//Vector vec;
//	//vec = vec1 + vec2;
//	//std::cout << std::endl << "vec: " << vec << std::endl;
//	//std::cout << "scalar prod = " << ScalarProduct(vec1, vec2);
//	//std::cout << vec1[0] << vec1[1] << vec1[2];
//	//std::cout << vec1[0] << vec1[1] << vec1[2];
//	//std::cout << "vec1: " << vec1 << std::endl;
//	std::cout << (vec1 == vec2) << std::endl;
//	//vec << 1;
//	//std::cout << std::endl <<= "vec1: " << vec << std::endl;
//	//std::cout << "subvec1 = " << vec1.SubVector(2, 4);
//
//	return 0;
//}

//int main()
//{
//	Vector vec;
//	vec.push_back(10);
//	vec.push_back(0);
//	vec.push_back(0);
//	vec.push_back(4);
//	std::cout << vec;
//}

#include "CF_Grammar_LR1.h"

int main()
{
	CF_Grammar bob("LR1Grammar.txt");
	bob.print_rules();

	std::cout << std::endl;

	Syntactical_Symbol A;
	A.m_is_terminal_symbol = false;
	A.m_name_symbol = "";
	A.m_id_symbol = bob.get_id("{S}");

	auto bobik = bob.FIRST1(A);

	for (auto it : bobik)
		std::cout << it.m_name_symbol << " ";


	return 0;
}
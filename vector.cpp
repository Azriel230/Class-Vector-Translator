#include "vector.h"
#include <iostream>

void Vector::Copy(const Vector& obj)
{
	if (obj.head == nullptr) //если список пустой, то копировать нечего
	{
		head = nullptr;
		size = obj.size;
		return;
	}
	size = obj.size;

	head = new Node;  //создали новую голову списка
	head->data = obj.head->data;
	head->num = obj.head->num;

	Node* tail = head;  //переменная для движения по списку и его дальнейшего создания
	Node* temp = obj.head->next;  //переменная для движения по старому списку 

	while (temp != nullptr) //пока есть что копировать
	{
		tail->next = new Node; //создаем новый узел
		tail = tail->next; //сдвигаемся в него

		tail->data = temp->data; //копируем данные
		tail->num = temp->num;  //копируем номер

		temp = temp->next; //идем дальше по старому списку
	}

	tail->next = nullptr; //закончили создание списка, хвост указывает на nullptr
}

void Vector::Clear()
{
	while (head)
	{
		Node* temp = head;
		head = head->next;
		delete temp;
	}
	size = 0;
}

Vector::Vector()
{
	size = 0;
	head = nullptr;
}

Vector::Vector(int* arr, int arrSize)
{
	size = arrSize;
	if (arrSize == 0)
	{
		head = nullptr;
		return;
	}

	head = nullptr;
	int index = 0;
	while (index < arrSize)
	{
		if (arr[index] != 0)
		{
			head = new Node;
			head->data = arr[index];
			head->num = index;
			head->next = nullptr;
			break;
		}
		index++;
	}

	Node* temp = head;
	while (index < arrSize)
	{
		if (arr[index] != 0)
		{
			temp->next = new Node;
			temp = temp->next;
			temp->data = arr[index];
			temp->num = index;
			temp->next = nullptr;
		}
		index++;
	}
}

Vector::Vector(const Vector& obj)
{
	Copy(obj);
}

Vector::~Vector()
{
	Clear();
	//std::cout << "Destructor!" << std::endl;
}

Vector& Vector::operator=(const Vector& obj)
{
	if (this == &obj)
		return *this;
	Clear();
	Copy(obj);
	return *this;
}

Node* Vector::Search(int key) const
{
	if ((head == nullptr) || (head->next == nullptr))
		return head;

	Node* temp = head;

	while ((temp->num != key) && (temp->next != nullptr))
	{
		temp = temp->next;
	}

	return temp;
}

void Vector::DeleteNode(int key)
{
	if (head == nullptr)
		return;
	Node* temp = head;
	temp = Search(key);

	if (temp == nullptr)
		return;
	else if (temp == head)
	{
		head = head->next;
		delete temp;
	}
	else
	{
		Node* tempPrev = head;
		while (tempPrev->next != temp)
			tempPrev = tempPrev->next;
		tempPrev->next = temp->next;
		delete temp;
	}
}

void Vector::push_back(int number_)
{
	if (number_ == 0)
	{
		++size;
		return;
	}
	if (head == nullptr)
	{
		head = new Node;
		head->data = number_;
		head->num = size;
		head->next = nullptr;
		++size;
		return;
	}

	Node* tempVec = head;
	while (tempVec->next != nullptr)
	{
		tempVec = tempVec->next;
	}

	tempVec->next = new Node;
	tempVec = tempVec->next;

	tempVec->num = size;
	tempVec->data = number_;
	tempVec->next = nullptr;

	++size;
}

//поэлементные арифметические операции
Vector Vector::operator+(const Vector& obj)
{
	Vector vec = *this;

	vec.size = std::max(size, obj.size);

	Node* objTemp = obj.head;
	Node* vecTemp = vec.head;

	while (objTemp != nullptr)
	{
		if (vec.head == nullptr)
		{
			vec.head = new Node;
			vec.head->data = objTemp->data;
			vec.head->num = objTemp->num;
			vec.head->next = nullptr;
			objTemp = objTemp->next;
			continue;
		}

		vecTemp = vec.Search(objTemp->num);
		if (vecTemp->num == objTemp->num)
		{
			vecTemp->data += objTemp->data;
			if (vecTemp->data == 0)
				vec.DeleteNode(vecTemp->num);
		}
		else
		{
			if(objTemp->data != 0)
			{
				vecTemp->next = new Node;
				vecTemp = vecTemp->next;
				vecTemp->data = objTemp->data;
				vecTemp->num = objTemp->num;
				vecTemp->next = nullptr;
			}
		}

		objTemp = objTemp->next;
	}

 	return vec;
}

Vector Vector::operator-(const Vector& obj)
{
	Vector vec = *this;

	vec.size = std::max(size, obj.size);

	Node* objTemp = obj.head;
	Node* vecTemp = vec.head;

	while (objTemp != nullptr)
	{
		if (vec.head == nullptr)
		{
			vec.head = new Node;
			vec.head->data = -objTemp->data;
			vec.head->num = objTemp->num;
			vec.head->next = nullptr;
			objTemp = objTemp->next;
			continue;
		}

		vecTemp = vec.Search(objTemp->num);
		if (vecTemp->num == objTemp->num)
		{
			vecTemp->data -= objTemp->data;
			if (vecTemp->data == 0)
				vec.DeleteNode(vecTemp->num);
		}
		else
		{
			if (objTemp->data != 0)
			{
				vecTemp->next = new Node;
				vecTemp = vecTemp->next;
				vecTemp->data = -objTemp->data;
				vecTemp->num = objTemp->num;
				vecTemp->next = nullptr;
			}
		}

		objTemp = objTemp->next;
	}

	return vec;
}

Vector Vector::operator*(const Vector& obj)
{
	if ((head == nullptr) && (obj.head == nullptr))
		return *this;
	if ((head == nullptr) && (obj.head != nullptr))
		return *this;
	if ((head != nullptr) && (obj.head == nullptr))
		return obj;

	Vector vec;
	vec.size = std::max(size, obj.size);

	Node* temp = head;
	Node* objTemp = obj.head;

	for (int i = 0; i < vec.size; i++)
	{
		temp = Search(i);
		objTemp = obj.Search(i);
		if ((temp->num == i) && (objTemp->num == i))
		{
			vec.head = new Node;
			vec.head->data = temp->data * objTemp->data;
			vec.head->num = i;
			vec.head->next = nullptr;
			break;
		}
	}

	if (vec.head == nullptr) //если не смогли создать голову, то вектор - нуль-вектор
		return vec;

	Node* vecTemp = vec.head;

	for (int i = vec.head->num + 1; i < vec.size; i++)
	{
		temp = Search(i);
		objTemp = obj.Search(i);
		if ((temp->num == i) && (objTemp->num == i))
		{
			vecTemp->next = new Node;
			vecTemp = vecTemp->next;
			vecTemp->data = temp->data * objTemp->data;
			vecTemp->num = i;
			vecTemp->next = nullptr;
		}
	}

	return vec;
}

Vector Vector::operator/(const Vector& obj)
{
	if (obj.size < size) //если вектор-делитель имеет меньшую длину, чем вектор-делимое (тк разница в векторах дополняется нулями)
	{
		std::cout << "ERROR! Attempt to divide by zero!" << std::endl;
		exit(EXIT_FAILURE);
	}
	if ((head == nullptr) && (obj.head == nullptr))
	{
		std::cout << "ERROR! Attempt to divide by zero!" << std::endl;
		exit(EXIT_FAILURE);
	}
	if ((head == nullptr) && (obj.head != nullptr))
	{
		Vector vec;
		vec.size = std::max(size, obj.size);
		return vec;
	}
	if ((head != nullptr) && (obj.head == nullptr))
	{
		std::cout << "ERROR! Attempt to divide by zero!" << std::endl;
		exit(EXIT_FAILURE);
	}

	int delRes = 0; //результат деления элемента первого вектора на второй

	Vector vec;
	vec.size = std::max(size, obj.size);

	Node* temp = head;
	Node* objTemp = obj.head;

	for (int i = 0; i < vec.size; i++)
	{
		temp = Search(i);
		objTemp = obj.Search(i);
		if (objTemp->num != i)
		{
			std::cout << "ERROR! Attempt to divide by zero!" << std::endl;
			exit(EXIT_FAILURE);
		}
		if ((temp->num == i) && (objTemp->num == i))
		{
			delRes = temp->data / objTemp->data;
			if (delRes != 0)
			{
				vec.head = new Node;
				vec.head->data = delRes;
				vec.head->num = i;
				vec.head->next = nullptr;
				break;
			}
		}
	}

	if (vec.head == nullptr) //если не смогли создать голову, то вектор - нуль-вектор
		return vec;

	Node* vecTemp = vec.head;

	for (int i = vec.head->num + 1; i < vec.size; i++)
	{
		temp = Search(i);
		objTemp = obj.Search(i);
		if (objTemp->num != i)
		{
			std::cout << "ERROR! Attempt to divide by zero!" << std::endl;
			exit(EXIT_FAILURE);
		}
		if ((temp->num == i) && (objTemp->num == i))
		{
			delRes = temp->data / objTemp->data;
			if (delRes != 0)
			{
				vecTemp->next = new Node;
				vecTemp = vecTemp->next;
				vecTemp->data = delRes;
				vecTemp->num = i;
				vecTemp->next = nullptr;
			}
		}
	}

	return vec;
}

int Vector::operator[](int index)
{
	if ((index < 0) || (index >= size))
	{
		std::cout << "Error! Out of vector range!" << std::endl;
		exit(EXIT_FAILURE);
	}

	Node* temp = Search(index);
	if (temp->num == index)
		return temp->data;
	else
		return 0;
}

bool Vector::operator==(const Vector& obj) const
{
	if (size != obj.size)
		return false;
	if ((head == nullptr) && (obj.head == nullptr))
		return true;
	if (((head == nullptr) && (obj.head != nullptr)) || ((head != nullptr) && (obj.head == nullptr)))
		return false;
	
	Node* temp = head;
	Node* objTemp = obj.head;

	for (int i = 0; i < size; i++)
	{
		temp = Search(i);
		objTemp = obj.Search(i);

		if ((temp->num == i) && (objTemp->num == i))
		{
			if (temp->data != objTemp->data)
				return false;
		}

		if (((temp->num == i) && (objTemp->num != i)) || ((temp->num != i) && (objTemp->num == i)))
			return false;
	}

	return true;
}

bool Vector::operator!=(const Vector& obj) const
{
	return !(*this == obj);
}

Vector& Vector::operator+=(const Vector& obj) //конкатенация
{
	if ((head == nullptr) && (obj.head == nullptr))
	{
		size += obj.size;
		return *this;
	}

	Node* objTemp = obj.head;

	if ((head == nullptr) && (obj.head != nullptr))
	{
		head = new Node;
		head->data = objTemp->data;
		head->num = objTemp->num + size;
		head->next = nullptr;
		objTemp = objTemp->next;
	}

	Node* temp = head;
	while (temp->next != nullptr)
		temp = temp->next;

	while (objTemp != nullptr)
	{
		temp->next = new Node;
		temp = temp->next;
		temp->data = objTemp->data;
		temp->num = objTemp->num + size;
		temp->next = nullptr;

		objTemp = objTemp->next;
	}

	size += obj.size;

	return *this;
}

Vector& Vector::operator>>=(const int number) //сдвиг вектора вправо
{
	if (head == nullptr)
		return *this;

	Vector vec;
	vec.size = size;
	
	Node* temp = head;
	while ((vec.head == nullptr) && (temp != nullptr))
	{
		if (temp->num + number < size)
		{
			vec.head = new Node;
			vec.head->data = temp->data;
			vec.head->num = temp->num + number;
			vec.head->next = nullptr;
		}
		temp = temp->next;
	}

	Node* vecTemp = vec.head;
	while (temp != nullptr)
	{
		if (temp->num + number < size)
		{
			vecTemp->next = new Node;
			vecTemp = vecTemp->next;
			vecTemp->data = temp->data;
			vecTemp->num = temp->num + number;
			vecTemp->next = nullptr;
		}
		temp = temp->next;
	}

	*this = vec;
	vec.Clear();

	return *this;
}

Vector& Vector::operator<<=(const int number)
{
	if (head == nullptr)
		return *this;

	Vector vec;
	vec.size = size;

	Node* temp = head;
	while ((vec.head == nullptr) && (temp != nullptr))
	{
		if (temp->num - number >= 0)
		{
			vec.head = new Node;
			vec.head->data = temp->data;
			vec.head->num = temp->num - number;
			vec.head->next = nullptr;
		}
		temp = temp->next;
	}

	Node* vecTemp = vec.head;
	while (temp != nullptr)
	{
		if (temp->num + number >= 0)
		{
			vecTemp->next = new Node;
			vecTemp = vecTemp->next;
			vecTemp->data = temp->data;
			vecTemp->num = temp->num - number;
			vecTemp->next = nullptr;
		}
		temp = temp->next;
	}

	*this = vec;
	vec.Clear();

	return *this;
}

int ScalarProduct(const Vector& vec1, const Vector& vec2)
{
	if (vec1.size != vec2.size) //если размеры не совпадают - ошибка
	{
		std::cout << "Error! Vector sizes do not match" << std::endl;
		exit(EXIT_FAILURE);
	}

	if ((vec1.head == nullptr) || (vec2.head == nullptr))
		return 0;

	int result = 0;
	Node* vec1Temp = vec1.head;
	Node* vec2Temp = vec2.head;

	for (int i = 0; i < vec1.size; i++)
	{
		vec1Temp = vec1.Search(i);
		vec2Temp = vec2.Search(i);

		if ((vec1Temp->num == i) && (vec2Temp->num == i))
			result += vec1Temp->data * vec2Temp->data;
	}

	return result;
}

Vector Vector::SubVector(const int firstIndex, const int lastIndex)
{
	if ((firstIndex < 0) || (lastIndex >= size))
	{
		std::cout << "Error! Out of vector range!";
		exit(EXIT_FAILURE);
	}
	if (firstIndex > lastIndex)
	{
		std::cout << "Error! Range entered incorrectly!";
		exit(EXIT_FAILURE);
	}

	Vector vec;
	vec.size = lastIndex - firstIndex + 1;


	if (head == nullptr)
	{
		return vec;
	}

	Node* temp = nullptr;

	for (int i = firstIndex; i <= lastIndex; i++)
	{
		temp = Search(i);
		if (temp->num == i)
		{
			vec.head = new Node;
			vec.head->data = temp->data;
			vec.head->num = i-firstIndex;
			vec.head->next = nullptr;
			break;
		}
	}
	
	if (vec.head == nullptr)
		return vec;

	Node* vecTemp = vec.head;
	for (int i = vec.head->num + 1; i <= lastIndex; i++)
	{
		temp = Search(i);
		if (temp->num == i)
		{
			vecTemp->next = new Node;
			vecTemp = vecTemp->next;
			vecTemp->data = temp->data;
			vecTemp->num = i-firstIndex;
			vecTemp->next = nullptr;
		}
	}

	return vec;
}

std::ostream& operator<<(std::ostream& stream, const Vector& obj)
{
	stream << '[';
	if (obj.head != nullptr)
	{
		for (int i = 0; i < obj.size; i++)
		{
			Node* temp = obj.head;
			while ((temp->num != i) && (temp->next != nullptr))
				temp = temp->next;

			if (temp->num == i)
				stream << temp->data;
			else
				stream << '0';

			if (i < obj.size - 1)
				stream << ' ';
		}
	}
	else
	{
		for (int i = 0; i < obj.size; i++)
		{
			stream << '0';
			if (i < obj.size - 1)
				stream << ' ';
		}
	}

	stream << ']';
	return stream;
}

std::istream& operator>>(std::istream& stream, Vector& obj)
{
	obj.Clear();

	while ((stream.peek() == ' ') || (stream.peek() == '\n'))
		stream.ignore();

	if (stream.peek() != '[')
	{
		std::cout << "Error! Invalid vector input" << std::endl; 
		exit(EXIT_FAILURE);
	}
	else
		stream.ignore();

	if (stream.peek() == ']') 
	{
		stream.ignore();
		return stream;
	}

	int n = 0; //индекс координат вектора
	int tempdata = 0; //координата вектора
	obj.head = nullptr;

	while (stream.peek() != EOF) //пропускаем все нулевые координаты вектора и накручиваем индексы
	{
		stream >> tempdata; //получаем первую координату вектора
		if (stream.fail())
		{
			std::cout << "Error! You didn't enter a number" << std::endl;  //ошибка ввода: было введено не число
			exit(EXIT_FAILURE);
		}

		if (tempdata != 0) //нашли ненулевую координату вектора, создаем голову списка
		{
			obj.head = new Node;
			obj.head->data = tempdata;
			obj.head->num = n;
			obj.head->next = nullptr;
			n++;
			break;
		}
		else
		{
			n++;
			if (stream.peek() == ']') //если это случилось здесь, значит ввели нуль-вектор
			{
				stream.ignore();
				obj.size = n;
				return stream;
			}

			if (stream.peek() != ' ')
			{
				std::cout << "Error! You entered an invalid character" << std::endl; 
				exit(EXIT_FAILURE);
			}
			else
				stream.ignore();
		}
	}

	Node* tail = obj.head;
	while (stream.peek() != EOF)
	{
		if (stream.peek() == ']') //вектор закончился
		{
			stream.ignore();
			tail->next = nullptr;
			obj.size = n;
			return stream;
		}
		if (stream.peek() != ' ')
		{
			std::cout << "Error! You entered an invalid character" << std::endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			stream.ignore();
			stream >> tempdata;
			if (stream.fail())
			{
				std::cout << "Error! You didn't enter a number" << std::endl;
				exit(EXIT_FAILURE);
			}
			if (tempdata == 0) //если координата равна нулю, то пропускаем узел списка, увеличив индекс, иначе создаем узел
			{
				n++;
				continue;
			}
			else
			{
				tail->next = new Node;
				tail = tail->next;
				tail->data = tempdata;
				tail->num = n;
				tail->next = nullptr;
				n++;
			}
		}
	}
	return stream;
}

Iterator Vector::begin()
{
	Iterator it;
	it.ptr = head;
	return it;
}

Iterator Vector::end()
{
	Iterator it;
	it.ptr = nullptr;
	return it;
}

int& Iterator::operator*()
{
	return ptr->data;
}

Iterator& Iterator::operator++()
{
	ptr = ptr->next;
	return *this;
}

Iterator Iterator::operator++(int)
{
	Iterator old = *this;
	ptr = ptr->next;
	return old;
}

bool Iterator::operator==(const Iterator& it)
{
	return (ptr == it.ptr);
}

bool Iterator::operator!=(const Iterator& it)
{
	return (ptr != it.ptr);
}
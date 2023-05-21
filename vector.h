#pragma once
#include <iostream>

struct Node 
{
	int num;	//�����
	int data;	//��������
	Node* next; //��������� �� ��������� ����
};

class Iterator;

class Vector //������ � ���� ���������������� ������ ��������� ��������� ���� (�����, ��������)
{  
	int size;
    Node* head;
	void Copy(const Vector& obj);
public:
	void Clear();

	Vector();

	Vector(int* arr, int arrSize);

	Vector(const Vector& obj);

	~Vector();

	Vector& operator=(const Vector& obj);

	friend std::ostream& operator<<(std::ostream& stream, const Vector& obj);
	friend std::istream& operator>>(std::istream& stream, Vector& obj);

	Node* Search(int key) const; //����� ���������� � ������ ��������
	void DeleteNode(int key);

	//������������ �������������� ��������: 1) ������������ ��������, ���������, ���������, ������� ������� �� ������. 								
	//�������, ��� ���� � ���� �������� ������ �����, �� ������� �������� ������
	Vector operator+(const Vector& obj);
	Vector operator-(const Vector& obj);
	Vector operator*(const Vector& obj);
	Vector operator/(const Vector& obj);

	friend int ScalarProduct(const Vector& vec1, const Vector& vec2); //��������� ������������

	int operator[](int index);

	bool operator==(const Vector& obj) const;
	bool operator!=(const Vector& obj) const;

	Vector& operator+=(const Vector& obj);

	Vector& operator>>=(const int number);
	Vector& operator<<=(const int number);

	Vector SubVector(const int firstIndex, const int lastIndex);

	Iterator begin();
	Iterator end();

	void push_back(int number_);
};

class Iterator 
{
	Node* ptr; //��������� �� ����
public:
	int& operator*();
	Iterator& operator++();
	Iterator operator++(int);
	bool operator==(const Iterator& it);
	bool operator!=(const Iterator& it);

	friend class Vector;
};
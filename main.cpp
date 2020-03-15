#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include <conio.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <windows.h>
#include <algorithm>
using namespace std;

class Aviasales;
class Date;
class List;
class Node;
class Time;

void addTicket(List*, Aviasales*);
char* strFill() {
	int len = 1;
	char* tmpStr = nullptr;
	char* resultStr = new char[len];
	while (true)
	{
		resultStr[len - 1] = _getch();
		if (resultStr[len - 1] == '\0')
		{
			continue;
		}
		if (resultStr[len - 1] == 13)
		{
			resultStr[len - 1] = '\0';
			break;
		}
		if (resultStr[len - 1] == '\b')
		{
			printf("%c", resultStr[len - 1]);
			printf("%c", ' ');
			printf("%c", resultStr[len - 1]);
			tmpStr = new char[len - 1];
			for (int i = 0; i < len - 2; i++)
			{
				tmpStr[i] = resultStr[i];
			}
			delete[] resultStr;
			resultStr = tmpStr;
			len--;
		}
		else
		{
			printf("%c", resultStr[len - 1]);
			tmpStr = new char[len + 1];
			for (int i = 0; i < len; i++)
			{
				tmpStr[i] = resultStr[i];
			}
			delete[] resultStr;
			resultStr = tmpStr;
			len++;
		}
	}
	return resultStr;
}
char* readCharsBeforeSeparator(FILE* from, char separator) {
	char* str = nullptr;
	char tmpCh = ' ';
	int strlen = 0;
	do
	{
		fscanf(from, "%c", &tmpCh);
		char* tmpStr = new char[strlen + 1];
		for (int i = 0; i < strlen; i++)
		{
			tmpStr[i] = str[i];
		}
		tmpStr[strlen] = tmpCh;
		strlen++;
		delete[] str;
		str = tmpStr;
	} while (tmpCh != separator);

	str[strlen - 1] = '\0';
	return str;
}
void sortArr(List*);
bool needToSwap(Node*, Node*, int);

class Date
{
private:
	int day, month;

public:
	Date() = default;
	Date(int _day, int _month)
	{
		day = _day;
		month = _month;
	}
	int getDay() {
		return day;
	}
	int getMonth() {
		return month;
	}
};
class Time
{
private:
	int hour, minute;

public:
	Time() = default;
	Time(int _hour, int _minute)
	{
		hour = _hour;
		minute = _minute;
	}

	int getHour() {
		return hour;
	}

	int getMinute() {
		return minute;
	}
};
class Aviasales
{
private:
	Date dt;
	Time time;
	char* nameOfAirfield;
	int travelTime;
	bool IsBreak;
public:
	Aviasales();
	Aviasales(Date&, Time&, char*, int, bool);
	Aviasales(const Aviasales&);
	~Aviasales();
	Aviasales& operator=(const Aviasales& other)
	{
		this->dt = other.dt;
		this->time = other.time;
		this->nameOfAirfield = new char[strlen(other.nameOfAirfield) + 1];
		strcpy_s(nameOfAirfield, strlen(other.nameOfAirfield) + 1, other.nameOfAirfield);
		this->travelTime = other.travelTime;
		this->IsBreak = other.IsBreak;
		return *this;
	}

	void setDate(const Date& _dt) {
		this->dt = _dt;
	}
	void setTime(const Time& _time) {
		this->time = _time;
	}
	void setNameOfAirfield(const char* _nameOfAirfield) {
		strcpy(this->nameOfAirfield, _nameOfAirfield);
	}
	void setTravelTime(int _travelTime) {
		this->travelTime = _travelTime;
	}
	void setIsBreak(bool isBreak) {
		IsBreak = isBreak;
	}
	Date& getDate() {
		return dt;
	}
	Time& getTime() {
		return time;
	}
	char* getNameOfAirfield() {
		return nameOfAirfield;
	}
	int getTravelTime() {
		return travelTime;
	}
	bool isBreak() {
		return IsBreak;
	}
	static Aviasales* readTicket(FILE* from) {
		int dd, MM, hh, mm, tt;
		fscanf(from, "%d.%d;%d:%d;", &dd, &MM, &hh, &mm);
		auto date = new Date(dd, MM);
		auto time = new Time(hh, mm);
		char* nameOfAirfield = readCharsBeforeSeparator(from, ';');
		fscanf(from, "%d;\n", &tt);
		bool isBreak = tt > 4;
		return new Aviasales(*date, *time, nameOfAirfield, tt, isBreak);
	}
	static Aviasales* readTicket() {
		int dd, MM, hh, mm, tt;
		printf("Введите дату в формате XX.XX\n");
		scanf("%d.%d", &dd, &MM);
		printf("Введите время в формате XX:XX\n");
		scanf("%d:%d", &hh, &mm);
		printf("Введите название аэропорта\n");
		char* nameOfAirfield = strFill();
		printf("\nВведите время в пути\n");
		scanf("%d", &tt);
		bool isBreak = tt > 4;
		auto date = new Date(dd, MM);
		auto time = new Time(hh, mm);
		return new Aviasales(*date, *time, nameOfAirfield, tt, isBreak);
	}

	static Aviasales* readFromFile(List* myList) {
		FILE* file;
		if ((file = fopen("database.txt", "r")) == NULL)
		{
			printf("Файл не найден!\n");
			exit(0);
		}
		if (getc(file) == EOF)
		{
			printf("Файл пуст!");
			exit(0);
		}
		Aviasales* readTickets = nullptr;

		fclose(file);
		file = fopen("database.txt", "r");
		while (!feof(file))
		{
			addTicket(myList, readTicket(file));
		}
		fclose(file);
		return readTickets;
	}
};
class Node
{
public:
	Aviasales* data;
	Node* prev;
	Node* next;
	Node* current;
	Node(Aviasales* data)
	{
		this->data = data;
		prev = nullptr;
		next = nullptr;
	}
	Node(Aviasales* data, Node* next, Node* prev)
	{
		this->data = data;
		this->prev = prev;
		this->next = next;
	}
};
class List
{
	Node* head;
	Node* tail;
	int size = 0;
public:
	List()
	{
		head = nullptr;
		tail = nullptr;
	}
	~List()
	{
		while (head)
		{
			tail = head->next;
			delete head;
			head = tail;
		}
	}

	void setValueSize(int _size) {
		this->size = _size;
	}							// Да-да, size можно было реализовать иначе, но это мои тараканы :)
	int getSize() {
		return size;
	}
	void sort()
	{
		printf("Выберите поле для сортировки: \n1 - Дата\n2 - Время\n3 - Название аэропорта\n4 - Время в пути\n5 - Наличие завтрака\n6 - Верунться назад\n\n");
		int choice = 0;
		scanf("%d", &choice);
		if (choice == 6)
		{
			return;
		}
		else
		{
			Node* left = head;
			Node* right = head->next;
			while (left->next)
			{
				while (right)
				{
					if (!needToSwap(left, right, choice))
					{
						Aviasales* r = left->data;
						left->data = right->data;
						right->data = r;
					}
					right = right->next;
				}
				left = left->next;
				right = left->next;
			}
		}
		printf("\nБаза данных успешно отсортированна!\n");
	}
	void pushBack(Aviasales* value)
	{
		Node* temp = new Node(value, nullptr, tail);
		tail = temp;
		if (head == nullptr)
		{
			head = temp;
		}
		else
		{
			Node* current = head;
			while (current->next)
			{
				current = current->next;
			}
			current->next = temp;
			temp->prev = current;
		}
		size++;
	}
	void saveDB() {
		FILE* file = fopen("database.txt", "w");

		Node* current = head;
		while (current != NULL)
		{
			fprintf(
				file,
				"%.2i.%.2i;%.2i:%.2i;%s;%d;\n",
				current->data->getDate().getDay(),
				current->data->getDate().getMonth(),
				current->data->getTime().getHour(),
				current->data->getTime().getMinute(),
				current->data->getNameOfAirfield(),
				current->data->getTravelTime());
			current = current->next;
		}
		fclose(file);
	}
	void print() {
		Node* current = head;
		while (current != NULL) {
			printf(
				"%.2i.%.2i; %.2i:%.2i; %s; %d; ",
				current->data->getDate().getDay(),
				current->data->getDate().getMonth(),
				current->data->getTime().getHour(),
				current->data->getTime().getMinute(),
				current->data->getNameOfAirfield(),
				current->data->getTravelTime());
			current->data->isBreak() ? printf("Завтрак включен\n") : printf(" Без завтрака\n");
			current = current->next;
		}
	}
	int deleteList(int index)
	{
		if (head == nullptr)
		{
			printf("Cписок пуст\n");
			return 0;
		}
		else if (index == 1)
		{
			Node* tmp = head;
			if (tmp->next == nullptr)
			{
				delete head;
				head = nullptr;
				size--;
				return 0;
			}
			else
			{
				tmp = tmp->next;
				delete head;
				head = tmp;
				size--;
				return 0;
			}
		}
		else if (index == getSize())
		{
			Node* current = tail;
			current = current->prev;
			delete current->next;
			current->next = nullptr;
			size--;
			tail = current;
			return 0;
		}
		else
		{
			Node* current = head;
			for (int i = index; i > 1; i--)
				current = current->next;
			if (current == head) head = current->next;
			current->prev->next = current->next;
			current->next->prev = current->prev;
			delete current;
			size--;
		}
		printf("Элемент удален!\n");
	}
};

Aviasales::Aviasales()
{
	Time nullTime(0, 0);
	Date nullDate(1, 1);
	this->dt = nullDate;
	this->time = nullTime;
	this->nameOfAirfield = nullptr;
	this->travelTime = 0;
	this->IsBreak = 0;
}
Aviasales::Aviasales(Date& _dt, Time& _time, char* _nameOfAirfield, int _travelTime, bool _isBreak)
{
	this->dt = _dt;
	this->time = _time;
	this->nameOfAirfield = _nameOfAirfield;
	this->travelTime = _travelTime;
	this->IsBreak = _isBreak;
}
Aviasales::Aviasales(const Aviasales& other)
{
	this->dt = other.dt;
	this->time = other.time;
	this->nameOfAirfield = new char[strlen(other.nameOfAirfield) + 1];
	strcpy_s(nameOfAirfield, strlen(other.nameOfAirfield) + 1, other.nameOfAirfield);
	this->travelTime = other.travelTime;
	this->IsBreak = other.IsBreak;
}
Aviasales::~Aviasales()
{
	if (nameOfAirfield != nullptr)
	{
		delete[] nameOfAirfield;
	}
}

bool needToSwap(Node* left, Node* right, int choice) {
	if (choice == 3) {
		return strcmp(left->data->getNameOfAirfield(), right->data->getNameOfAirfield()) < 0;
	}
	else if (choice == 2) {
		if (left->data->getTime().getHour() == right->data->getTime().getHour())
			return (left->data->getTime().getMinute() < right->data->getTime().getMinute());
		else
			return (left->data->getTime().getHour() < right->data->getTime().getHour());
	}
	else if (choice == 1) {
		if (left->data->getDate().getMonth() == right->data->getDate().getMonth())
			return (left->data->getDate().getDay() < right->data->getDate().getDay());
		else
			return (left->data->getDate().getMonth() < right->data->getDate().getMonth());
	}
	else if (choice == 4) {
		return (left->data->getTravelTime() < right->data->getTravelTime());
	}
	else if (choice == 5) {
		return (left->data->getTravelTime() > right->data->getTravelTime());
	}
}
void addTicket(List* temp, Aviasales* toAdd) {
	temp->pushBack(toAdd);
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int menu = 0;
	int ticketsCount = 0;
	List* myList = new List();
	while (true)
	{
		printf("\n\n---Выберите желаемое действие--- \n 1. Добавление элемента\n 2. Чтение базы данных из файла\n 3. Вывод базы на экран \n 4. Сортировка базы данных \n 5. Записать данные в файл \n 6. Удаление элемента  \n 7. Выход из программы \n");
		scanf("%d", &menu);
		switch (menu)
		{
		case 1: // Добавление элемента
			printf("\nДобавление элемента:\n--------------------------------\n");
			addTicket(myList, Aviasales::readTicket());
			printf("Информация успешно загружена!");
			break;
		case 2: // Чтение из файла
			printf("\nЧтение базы данных:\n--------------------------------\n");
			if (myList->getSize() == 0)
			{
				Aviasales::readFromFile(myList);
				printf("Информация успешно загружена!");
			}
			else
			{
				delete myList;
				myList = new List();
				myList->setValueSize(0);
				Aviasales::readFromFile(myList);
				printf("Информация успешно загружена!");
			};
			break;
		case 3: // Вывод на экран
			printf("\nВывод базы данных на экран:\n--------------------------------\n");
			if (myList->getSize() == 0)
			{
				printf("Вы не выбрали базу данных!");
			}
			else
			{
				myList->print();
			};
			break;
		case 4: // Сортировка
			printf("\nСортировка базы данных:\n--------------------------------\n");
			if (myList->getSize() == 0)
			{
				printf("Вы не выбрали базу данных!");
			}
			else
			{
				myList->sort();
				myList->print();
			};
			break;
		case 5: // Сохранение
			printf("\nЗапись базы данных в файл:\n--------------------------------\n");
			if (myList->getSize() == 0)
			{
				printf("Вы не выбрали базу данных!");
			}
			else
			{
				myList->saveDB();
				printf("База данных успешно записана в файл!\n");
			};
			break;
		case 6: // Удаление элемента
			printf("\nУдаление элемента:\n--------------------------------\n");
			if (myList->getSize() == 0)
			{
				printf("База данных пуста!");
			}
			else
			{
				int tickNumber = -1;
				printf("Введите номер билета\n");
				scanf("%d", &tickNumber);
				if (tickNumber > myList->getSize() || tickNumber < 1)
				{
					printf("\nВы ввели несуществующий номер!");
				}
				else
				{
					myList->deleteList(tickNumber);
					printf("База данных успешно обновлена!");
				};
			};
			break;
		case 7: // Выход
			printf("Вы успешно завершили работу программы.");
			return(0);
			break;
		default:
			printf("Такой функции не существует.");
			break;
		};
	}
	return 0;
}

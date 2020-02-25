#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include <conio.h>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <cstring>
using namespace std;

class Date;
class Time;
class Aviasales;
class Node;

void addTicket(Aviasales*&, int&, Aviasales*);
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
void deleteTicket(Aviasales*&, int, int&);
void saveDB(Aviasales*, int);
void sortArr(Aviasales*, int);
void printTicketArr(Aviasales*, int);
bool needToSwap(Aviasales*, Aviasales*, int);

class Node
{
public:
	Aviasales* data;
	Node* prev;
	Node* next;
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
	friend class Aviasales;
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

	
	void setSize() {
		this->size++;
	}
	void pushBack(Aviasales* value)
	{
		Node* temp = new Node(value);
		if (head == nullptr)
		{
			temp->prev = nullptr;
			head = tail = temp;
		}
		else
		{
			Node* tail = head;
			while (tail->next != nullptr)
			{
				tail = tail->next;
			}
			tail->next = temp;
			temp->prev = tail;
		}
		setSize();
	}
	void print(Aviasales* temp)
	{
		Node* current = head;
		while (current)
		{
			cout << current->data->getDate << endl;
			current = current->next;
		}
	}
};

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

	static Aviasales* readFromFile(int& size) {
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
			addTicket(readTickets, size, readTicket(file));
		}
		fclose(file);
		return readTickets;
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


bool needToSwap(Aviasales* left, Aviasales* right, int choice) {
	if (choice == 3) {
		return strcmp(left->getNameOfAirfield(), right->getNameOfAirfield()) < 0;
	}
	else if (choice == 2) {
		if (left->getTime().getHour() == right->getTime().getHour())
			return (left->getTime().getMinute() < right->getTime().getMinute());
		else
			return (left->getTime().getHour() < right->getTime().getHour());
	}
	else if (choice == 1) {
		if (left->getDate().getMonth() == right->getDate().getMonth())
			return (left->getDate().getDay() < right->getDate().getDay());
		else
			return (left->getDate().getMonth() < right->getDate().getMonth());
	}
	else if (choice == 4) {
		return (left->getTravelTime() < right->getTravelTime());
	}
	else if (choice == 5) {
		return (left->getTravelTime() > right->getTravelTime());
	}
}
void printTicketArr(Aviasales* ticketArr, int arrSize) {
	for (int i = 0; i < arrSize; i++)
	{
		printf(
			"%.2i.%.2i; %.2i:%.2i; %s; %d; ",
			ticketArr[i].getDate().getDay(),
			ticketArr[i].getDate().getMonth(),
			ticketArr[i].getTime().getHour(),
			ticketArr[i].getTime().getMinute(),
			ticketArr[i].getNameOfAirfield(),
			ticketArr[i].getTravelTime()
		);
		ticketArr[i].isBreak() == 1 ? printf("Завтрак включён\n") : printf("Без завтрака\n");
	}
}
void sortArr(Aviasales* ticketArr, int arrSize) {
	printf("Выберите поле для сортировки: \n1 - Дата\n2 - Время\n3 - Название аэропорта\n4 - Время в пути\n5 - Наличие завтрака\n6 - Верунться назад\n\n");
	int choice = 0;
	scanf("%d", &choice);
	if (choice == 6)
	{
		return;
	}
	else
	{
		for (int i = 0; i < arrSize; i++)
		{
			for (int j = arrSize - 1; j > i; j--)
			{
				if (needToSwap(&ticketArr[j], &ticketArr[j - 1], choice))
				{
					Aviasales _temp = ticketArr[j];
					ticketArr[j] = ticketArr[j - 1];
					ticketArr[j - 1] = _temp;
					ticketArr[j - 1].setNameOfAirfield(_temp.getNameOfAirfield());
				}
			}
		}
		printf("\nБаза данных успешно отсортированна!\n");
		printTicketArr(ticketArr, arrSize);
	};
}
void saveDB(Aviasales* ticketArr, int arrSize) {
	FILE* file = fopen("database.txt", "w");
	for (int i = 0; i < arrSize; i++)
	{
		fprintf(
			file,
			"%.2i.%.2i;%.2i:%.2i;%s;%d;\n",
			ticketArr[i].getDate().getDay(),
			ticketArr[i].getDate().getMonth(),
			ticketArr[i].getTime().getHour(),
			ticketArr[i].getTime().getMinute(),
			ticketArr[i].getNameOfAirfield(),
			ticketArr[i].getTravelTime()
		);
	}
	fclose(file);
}
void deleteTicket(Aviasales*& ticketArr, int tickNum, int& tickCount) {
	tickNum--;
	tickCount--;
	Aviasales* newArr = new Aviasales[tickCount];
	for (int i = tickNum; i < tickCount; i++)
	{
		ticketArr[i].setDate(ticketArr[i + 1].getDate());
		ticketArr[i].setIsBreak(ticketArr[i + 1].isBreak());
		ticketArr[i].setTime(ticketArr[i + 1].getTime());
		ticketArr[i].setTravelTime(ticketArr[i + 1].getTravelTime());
		ticketArr[i].setNameOfAirfield(ticketArr[i + 1].getNameOfAirfield());
	}
	for (int i = 0; i < tickCount; i++)
		newArr[i] = ticketArr[i];
	ticketArr = newArr;
}

void addTicket(Aviasales*& ticketArr, int& tickCount, Aviasales* toAdd) {
	Aviasales* newArr = new Aviasales[tickCount + 1];
	List* temp;
	for (int i = 0; i < tickCount; i++)
	{
		newArr[i] = ticketArr[i];
	}
	newArr[tickCount] = *toAdd;
	tickCount++;
	ticketArr = newArr;
//	temp->pushBack(newArr);
}

int main()
{
	List* list = new List();
	Aviasales* temp = new Aviasales();
	Date date(5, 10);
	Time time(10, 40);

	temp->setTravelTime(4);
	temp->setIsBreak(1);
	temp->setDate(date);
	temp->setTime(time);
	list->pushBack(temp);

	Aviasales* temp1 = new Aviasales();
	Date date1(5, 10);
	Time time1(10, 40);

	temp->setTravelTime(3);
	temp->setIsBreak(2);
	temp->setDate(date1);
	temp->setTime(time1);
	list->pushBack(temp1);


	list->print();

	return 0;
}
//
//int main()
//{
//	SetConsoleCP(1251);
//	SetConsoleOutputCP(1251);
//	int menu = 0;
//	bool isClear = 1;
//	int ticketsCount = 0;
//	Aviasales* tickets = nullptr;
//	while (true)
//	{
//		printf("\n\n---Выберите желаемое действие--- \n 1. Добавление элемента\n 2. Чтение базы данных из файла\n 3. Вывод базы на экран \n 4. Сортировка базы данных \n 5. Записать данные в файл \n 6. Удаление элемента  \n 7. Выход из программы \n");
//		scanf("%d", &menu);
//		switch (menu)
//		{
//		case 1: // Добавление элемента
//			printf("\nДобавление элемента:\n--------------------------------\n");
//			addTicket(tickets, ticketsCount, Aviasales::readTicket());
//			printf("Информация успешно загружена!");
//			isClear = 0;
//			break;
//		case 6: // Удаление элемента
//			printf("\nУдаление элемента:\n--------------------------------\n");
//			if (isClear)
//			{
//				printf("База данных пуста!");
//			}
//			else
//			{
//				int tickNumber = -1;
//				printf("Введите номер билета\n");
//				scanf("%d", &tickNumber);
//				if (tickNumber > ticketsCount || tickNumber < 1)
//				{
//					printf("\nВы ввели несуществующий номер!");
//				}
//				else
//				{
//					deleteTicket(tickets, tickNumber, ticketsCount);
//					printf("База данных успешно обновлена!");
//				};
//			};
//			break;
//
//		case 2: // Чтение из файла
//			printf("\nЧтение базы данных:\n--------------------------------\n");
//			if (isClear)
//			{
//				tickets = Aviasales::readFromFile(ticketsCount);
//				printf("Информация успешно загружена!");
//				isClear = 0;
//			}
//			else
//			{
//				tickets = nullptr;
//				ticketsCount = 0;
//				tickets = Aviasales::readFromFile(ticketsCount);
//				printf("Информация успешно загружена!");
//				isClear = 0;
//			};
//			break;
//		case 3: // Вывод на экран
//			printf("\nВывод базы данных на экран:\n--------------------------------\n");
//			if (isClear)
//			{
//				printf("Вы не выбрали базу данных!");
//			}
//			else
//			{
//				printTicketArr(tickets, ticketsCount);
//			};
//			break;
//		case 4: // Сортировка
//			printf("\nСортировка базы данных:\n--------------------------------\n");
//			if (isClear)
//			{
//				printf("Вы не выбрали базу данных!");
//			}
//			else
//			{
//				sortArr(tickets, ticketsCount);
//			};
//			break;
//		case 5: // Сохранение
//			printf("\nЗапись базы данных в файл:\n--------------------------------\n");
//			if (isClear)
//			{
//				printf("Вы не выбрали базу данных!");
//			}
//			else
//			{
//				saveDB(tickets, ticketsCount);
//				printf("База данных успешно записана в файл!\n");
//			};
//			break;
//		case 7: // Выход
//			printf("Вы успешно завершили работу программы.");
//			return(0);
//			break;
//		default:
//			printf("Такой функции не существует.");
//			break;
//		};
//	}
//	return 0;
//}

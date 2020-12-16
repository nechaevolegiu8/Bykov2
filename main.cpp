#include <iostream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <unordered_map>
#include <vector>
#include <set>
#include <locale.h>
#include <windows.h>

using namespace std;
class Users;

class Assoc { // Класс задания связи должности в структурном подразделении
public:
	double stavka; // Ставка
	string work; // Должность
	string department; // Название подразделения (если ключ) 
	Assoc(double st, string wrk, string depName) {
		stavka = st; work = wrk; department = depName;
	}
};

class User { // Класс сотрудников
public:
	static Users* db;
	string name; // ФИО (ключ)
	int year; // Год рождения
	int Snils; // СНИЛС (ключ)

	User(string nam = "", int ye = 0, int snils = -1) { // Конструктор
		name = nam;
		year = ye;
		Snils = snils;
	}

	~User() { // Деструктор
		connections.clear();
	}

	void print() { // Вывод информации
		cout << "ФИО: " << name + "; Год рождения: " + to_string(year) + "; СНИЛС: " + to_string(Snils) << endl;
		cout << "Должности:" << endl;
		for (Assoc* pos : connections) cout << pos->work + "; " + pos->department + "; ставка: " + to_string(pos->stavka) << endl;
	}

	void setWork(string depart, string workName, double stav); // Назначение на должность
	vector<Assoc*> connections; // Связи пользователей с подразделениями
};

class Department { // Класс структурных подразделений
public:
	set<string> employees;  // Список сотрудников  
	string name; // Название (ключ)
	string description; // Описание 
	
	Department(string _name = "", string _story = "") { // Конструктор
		name = _name; description = _story;
	}

	void print() { // Вывод информации
		cout << "Название: " << name << "; Описание: " + description << endl;
		cout << "Cотрудники подразделения:" << endl;
		for (string pos : employees)	cout << pos << endl;
	}
};

class Users { // Класс базы данных
public:
	unordered_map<string, User*> byName; // Сотрудники (ФИО)
	unordered_map<int, User*> bySnils; // Сотрудники (СНИЛС)
	unordered_map<string, Department*> departments; // Структурные подразделения
	
	Users() {} // Конструктор

	void printDep()	{ // Вывод всех структурных подразделений
		for (auto it = departments.begin(); it != departments.end(); ++it) it->second->print(); 
	}

	void printEmpl() { // Вывод всех сотрудников
		for (auto it = byName.begin(); it != byName.end(); ++it) it->second->print();
	}

	User* findName(string name)	{ // Поиск полных данных о сотруднике (сотрудниках) (поиск по ФИО)
		auto it = byName.find(name);
		if (it == byName.end())	return nullptr;
		else return it->second; 
	}

	void printName(string name) { // Вывод полных данных о сотруднике (сотрудниках) (поиск по ФИО)
		cout << "Поиск сотрудника по ФИО: " << name << endl;
		User* user = findName(name);
		if (!user) cout << "Сотрудника с таким ФИО нет в БД" << endl; 
		else user->print(); 
	}

	User* findSnils(int snils) { // Поиск полных данных о сотруднике (сотрудниках) (поиск по СНИЛС)
		auto it = bySnils.find(snils);
		if (it == bySnils.end()) return nullptr;
		else return it->second;
	}

	void printSnils(int snils) { // Вывод полных данных о сотруднике (сотрудниках) (поиск по СНИЛС)
		cout << "Поиск сотрудника по СНИЛС: " << snils << endl;
		User* user = findSnils(snils);
		if (!user) cout << "Сотрудника с таким СНИЛС нет в БД" << endl;
		else user->print();
	}

	void changeNameSnils(int snils, string newName) { // Поиск по СНИЛС и изменение некоторых данных о сотруднике (изменение ФИО, должности в структурном подразделении)
		User* user = findSnils(snils);
		if (!user) cout << "Cотрудника с таким СНИЛС нет в БД" << endl;
		else
		{
			User* user2 = findName(newName);
			if (user2) cout << "Сотрудник с таким ФИО уже есть в БД" << endl;
			else {
				cout << "Изменение успешно Старое ФИО: " << user->name << ", новое ФИО: " << newName << endl;
				user->name = newName;
			}
		}
	}

	void deleteEmplSnils(int snils) { // Поиск по СНИЛС и удаление сотрудника из БД
		User* user = findSnils(snils);
		if (!user) cout << "Сотрудника с таким СНИЛС нет в БД" << endl;
		else {
			byName.erase(user->name);
			bySnils.erase(snils);
			for (auto sv : user->connections) departments[sv->department]->employees.erase(user->name);
			delete user;
			cout << "Удаление успешно" << endl;
		}
	}

	void changeWorkSnils(int snils, string depart, string workName, double newStav) { // Поиск по СНИЛС и изменение ставки некоторой должности в некотором структурном подразделении
		User* user = findSnils(snils);
		if (!user) cout << "Сотрудника с таким СНИЛС нет в БД" << endl;
		else {
			cout << "ФИО сотрудника: " << user->name << endl;
			Assoc* connection = nullptr;
			for (auto sv : user->connections) {
				if (sv->department == depart && sv->work == workName) {
					connection = sv;
					break;
				}
			}
			if (!connection) cout << "Не найдено соответствующей должности" << endl;
			else {
				cout << "Структурное подразделение: " << depart << ", должность: " << workName << endl;
				cout << "Изменение успешно. Старая ставка: " << connection->stavka << ", новая ставка: " << newStav << endl;
				connection->stavka = newStav;
			}
		}
	}

	void printDepartment(string department) {	// Вывод структурного подразделения
		cout << "Поиск структурного подразделения по названию: " << department << endl;
		Department* dep = findDepartment(department);
		if (!dep) cout << "Структурного подразделения с таким названием нет в БД" << endl;
		else dep->print();
	}

	Department* findDepartment(string department) { // Поиск по названию структурного подразделения
		auto it = departments.find(department);
		if (it == departments.end()) return nullptr;
		else return it->second;
	}

	void addUser(User* user) { // Добавление сотрудника
		if (findSnils(user->Snils) || findName(user->name)) {
			cout << "Сотрудник с таким ФИО/СНИЛС уже есть в БД" << endl;
			return;
		}
		byName[user->name] = user;
		bySnils[user->Snils] = user;
		cout << "Добавление успешно" << endl;
	}

	void addDepart(Department* depart) { // Добавление структурного подразделение
		if (findDepartment(depart->name)) {
			cout << "Структурное подразделение с таким названием уже есть в БД" << endl;
			return;
		}
		departments[depart->name] = depart;
		cout << "Добавление успешно" << endl;
	}

	void changeDepartment(string department, string new_name, string new_story) { // Поиск по названию и изменение некоторых данных о структурном подразделении (новое название и краткое описание)
		Department* dep = findDepartment(department);
		if (!dep) cout << "Cтруктурного подразделения с таким названием нет в БД" << endl;
		else {
			cout << "Структурное подразделение: " << department << endl;
			cout << "Новое название: " << new_name << ", новое описание: " << new_story << endl;
			dep->name = new_name;
			dep->description = new_story;
		}
	}
};

Users* User::db = new Users();

void User::setWork(string depart, string workName, double stav) { // Назначение сотрудника на должность
	if (!db->findDepartment(depart)) cout << "Подразделения с заданным названием не существует." << endl;
	connections.push_back(new Assoc(stav, workName, depart));
	db->departments[depart]->employees.insert(name);
}


void help() { // Вывод списка доступных команд
	cout << endl << "1) Добавление нового сотрудника" << endl << "2) Добавление нового структурного подразделения" << endl << "3) Назначение сотрудника на должность" 
		<< endl << "4) Вывод всех сотрудников" << endl << "5) Вывод всех структурных подразделений" << endl << "6) Вывод данных о сотруднике по ФИО" 
		<< endl << "7) Вывод данных о сотрудника по СНИЛС" << endl << "8) Поиск сотрудника по СНИЛС; Изменение ФИО" << endl << "9) Изменение ставки должности сотрудника" 
		<< endl << "10) Поиск сотрудника по СНИЛС; Удаление его из базы данных" << endl << "11) Вывод данных о структурном подразделение по названию" 
		<< endl << "12) Изменение названия и описания структурного подразделения" << endl << "0) Выход" << endl << endl;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	string name, department, department2, description, work;
	Department* newDepart = nullptr;
	int command, year, snils;
	User* newUser = nullptr;
	Users* db = User::db;
	double stavka;
	
	do
	{
		help();
		cout << "------------------------------------" << endl;
		cout << "Команда: ";
		cin >> command;
		switch (command)
		{
		case 0:
			break;
		case 1:
			cout << "ФИО: ";
			cin.ignore(); getline(cin, name);
			cout << "Год рождения: "; cin >> year;
			cout << "СНИЛС: "; cin >> snils;
			newUser = new User(name, year, snils);
			db->addUser(newUser);
			break;
		case 2:
			cout << "Название подразделения: ";
			cin.ignore(); getline(cin, department);
			cout << "Описание подразделения: ";
			getline(cin, description);
			newDepart = new Department(department, description);
			db->addDepart(newDepart);
			break;
		case 3:
			cout << "СНИЛС: "; cin >> snils;
			cout << "Название подразделения: ";
			cin.ignore(); getline(cin, department);
			cout << "Название должности: ";
			getline(cin, work);
			cout << "Ставка: "; cin >> stavka;
			newUser = db->findSnils(snils);
			if (!newUser) cout << "Сотрудника с данным СНИЛС в базе данных нет" << endl;
			else newUser->setWork(department, work, stavka); 
			break;
		case 4:
			db->printEmpl();
			break;
		case 5:
			db->printDep();
			break;
		case 6:
			cout << "ФИО: ";
			cin.ignore(); getline(cin, name);
			db->printName(name);
			break;
		case 7:
			cout << "СНИЛС: ";
			cin >> snils;
			db->printSnils(snils);
			break;
		case 8:
			cout << "СНИЛС: "; cin >> snils;
			cout << "Новое ФИО: ";
			cin.ignore(); getline(cin, name);
			db->changeNameSnils(snils, name);
			break;
		case 9:
			cout << "СНИЛС: "; cin >> snils;
			cout << "Название подразделения: ";
			cin.ignore(); getline(cin, department);
			cout << "Название должности: ";	getline(cin, work);
			cout << "Новая ставка: "; cin >> stavka;
			db->changeWorkSnils(snils, department, work, stavka);
			break;
		case 10:
			cout << "СНИЛС: "; cin >> snils;
			db->deleteEmplSnils(snils);
			break;
		case 11:
			cout << "Название подразделения: ";
			cin.ignore(); getline(cin, department);
			db->printDepartment(department);
			break;
		case 12:
			cout << "Название подразделения: ";
			cin.ignore(); getline(cin, department);
			cout << "Новое название подразделения: ";
			getline(cin, department2);
			cout << "Новое описание подразделения: ";
			getline(cin, description);
			db->changeDepartment(department, department2, description);
			break;
		default:
			cout << "Некорректный ввод" << endl;
		}
		cout << "------------------------------------" << endl;
	} while (cin && (command != 0));

	return 0;
}

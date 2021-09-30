#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>
#include <windows.h>
#include <iomanip>
#define ALPHABET_LEN 255 
#define NOT_FOUND patlen 
#define maxxx(aa, bb) ((aa < bb) ? bb : aa)

using namespace std;

struct Passengers
{
    string passport; // номер паспорта 
    string placedate; // место и дата выдачи паспорта
    string fullname; // Фамилия Имя Отчество
    string birthdate;  // дата рождения
};

struct Hash_Table
{
    string key; // номер паспорта 
    Passengers data; //данные
    bool status = NULL;  // состояние (NULL- пусто; false - удалено; true - есть данные)
};

struct Flights
{
    string fly_num; // номер авиарейса 
    string airline_name; // название авиакомпании
    string depart_port;  // аэропорт отправления
    string arriv_port; // аэропорт прибытия
    string date; // дата отправления 
    string time; // время отправления
    int total_seats = 0; // всего мест
    int avail_seats = 0;  // свободных мест
};

struct Tickets
{
    string passport; // номер паспорта 
    string fly_num; // номер авиарейса 
    string bilet_num; // номер билета
};

struct list_tickets
{
    Tickets data; // поле данных
    list_tickets* next; // указатель на следующий элемент
    list_tickets* prev; // указатель на предыдущий элемент
};

struct node    // описание узла
{
    Flights data;   // информационное поле
    int height;
    node* left; // указатель на левое поддерево
    node* right; // указатель на правое поддерево
};

Hash_Table* hash_table = new Hash_Table[2];//хэш-таблица с данными о пассажирах
int size_of_tab = 2;
string flight_for_tickets[10];
int num_of_pass = 0;
node* new_node;
node* find_node;
struct node* find_arriv_port(struct node* airlines, string arriv_port);
int adress = 0;
int num_flights = 10;
int num_tickets = 150;
list_tickets* head = NULL;
list_tickets* cur = NULL;
list_tickets* fastSort(list_tickets* head);
void add_to_hash(Passengers p);
bool find_pass(char key[]);
void del_pass(int num);
void del_gar_pass(list_tickets* head, string passport);
void del_ticket(string ticket);
void input(char[12]);

int h(char key[11], int size)
{
    int h = 0, sum = 0;
    for (int i = 0; i < 11; i++)
    {
        sum += (int)((((int)key[i]) / (3 * (i + 1)))) * (int)key[i];
    }
    sum *= sum * 2;
    sum = sum % 1000000;
    h = (int)(sum / 100);
    h = h % size;
    return h;
}

bool valid_date(string date)
{
    if ((date.length() < 9) || ((int)date[2] != 46) || ((int)date[5] != 46))
    {
        return false;
    }
    else
    {
        for (int i = 0; i < date.length(); i++)
        {
            if ((i == 2) || (i == 5))
            {
                continue;
            }
            if (((int)date[i] >= 48) && ((int)date[i] <= 57))
            {
                continue;
            }
            else
            {
                return false;
            }
        }
    }
    int DayNo = (date[0] - 48) * 10 + (date[1] - 48);
    int Month = (date[3] - 48) * 10 + (date[4] - 48);
    int Year = (date[6] - 48) * 1000 + (date[7] - 48) * 100 + (date[8] - 48) * 10 + (date[9] - 48);
    int days_in_month[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
    if (Year % 4 == 0)
        days_in_month[2] = 29;
    if ((Month < 1) || (Month > 12) || (Year > 2021))
        return false;
    if ((DayNo < 1) || (DayNo > days_in_month[Month]))
        return false;
    return true;
}

bool valid_time(string time)
{
    if ((time.length() < 4) || ((int)time[2] != 58))
    {
        return false;
    }
    else
    {
        for (int i = 0; i < time.length(); i++)
        {

            switch (i)
            {
            case 0:
            {
                if (((int)time[i] >= 48) && ((int)time[i] <= 50))
                {
                    break;
                }
                else
                {
                    return false;
                }
            }
            case 1:
            {
                if ((int)time[i - 1] == 50)
                {
                    if (((int)time[i] >= 48) && ((int)time[i] <= 51))
                    {
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (((int)time[i - 1] == 48) || ((int)time[i - 1] == 49))
                {
                    if (((int)time[i] >= 48) && ((int)time[i] <= 57))
                    {
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            case 2:
            {
                break;
            }
            case 3:
            {
                if (((int)time[i] >= 48) && ((int)time[i] <= 53))
                {
                    break;
                }
                else
                {
                    return false;
                }
            }
            case 4:
            {
                if (((int)time[i] >= 48) && ((int)time[i] <= 57))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

            }
        }
    }
}

bool check_Name(string name)
{
    if ((((int)name[0] > 64) && ((int)name[0] < 90)) || (((int)name[0] + 256 > 127) && ((int)name[0] + 256 < 160))) //Проверка первой буквы на верхний регистр
    {
        for (int i = 1; i < name.length(); i++)
        {
            if ((int)name[i - 1] == 45) //Проверка буквы после тире на верхний регистр
            {
                if ((((int)name[i] > 65) && ((int)name[i] < 90)) || (((int)name[i] + 256 > 128) && ((int)name[i] + 256 < 159)))
                {
                    continue;
                }
                else
                {
                    return false;
                }
            }
            if (((int)name[i] >= 97) && ((int)name[i] <= 122)) //Проверка на английские буквы
            {
                continue;
            }
            if (((int)name[i] + 256 >= 160) && ((int)name[i] + 256 <= 175)) //Проверка на русские буквы ч.1
            {
                continue;
            }
            if (((int)name[i] + 256 >= 224) && ((int)name[i] + 256 <= 239)) //Проверка на русские буквы ч.2
            {
                continue;
            }
            if ((int)name[i] == 45) //Проверка на тире
            {
                continue;
            }

            else
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool check_air_Name(string name)
{
    if (name.length() == 3)
    {
        for (int i = 0; i < 3; i++)
        {
            if (((int)name[i] >= 65) && ((int)name[i] <= 90))
            {
                continue;
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

string decode_kirill(string kirill)
{
    for (int i = 0; i < kirill.length(); i++)
    {
        if ((int)kirill[i] == 45)
        {
            kirill[i] = (char)((int)kirill[i]);
            continue;
        }
        if ((int)kirill[i] > -33)
        {
            kirill[i] = (char)((int)kirill[i] + 272);
        }

        else
        {
            kirill[i] = (char)((int)kirill[i] + 320);
        }
    }
    return kirill;
}

void add_pass() //регистрация нового пассажира 
{
    Passengers pass;
    string place, date, fio, surname, name, otchestvo, birthday;
    char key[12] = { NULL };
    bool correct = true;
    do
    {
        if (correct == true)
            cout << "Введите номер паспорта формата НННН-СССССС ";
        input(key);
        adress = h(key, size_of_tab);
        correct = true;
        if (find_pass(key) == true)
        {
            cout << "Такой номер пасспорта уже есть в базе данных. Повторите ввод" << endl;
            correct = false;
            continue;
        }
        else
        {
            while ((hash_table[adress].status != NULL) && (correct == true))
            {
                if (hash_table[adress].key == key)
                    correct = false;
                else
                    adress += 2;
            }
        }
        continue;
    } while (correct == false);
    adress = h(key, size_of_tab);
    cout << "Введите место выдачи паспорта: ";
    while (!(cin >> place) || ((check_Name(place) != true)))
    {
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
    }
    if ((int)place[0] <= 0)
    {
        place = decode_kirill(place);
    }
    cout << "Введите дату выдачи паспорта в формате дд.мм.гггг: ";
    cin >> date;

    while (valid_date(date) != true)
    {
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
        cin >> date;
    }
    int passYear = (date[6] - 48) * 1000 + (date[7] - 48) * 100 + (date[8] - 48) * 10 + (date[9] - 48);
    while ((passYear < 1960) || (valid_date(date) != true))
    {
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
        cin >> date;
        passYear = (date[6] - 48) * 1000 + (date[7] - 48) * 100 + (date[8] - 48) * 10 + (date[9] - 48);
    }
    cout << "Введите Имя: ";
    while (!(cin >> name) || (check_Name(name) != true))
    {
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
    }
    if ((int)name[0] <= 0)
    {
        name = decode_kirill(name);
    }
    cout << "Введите Фамилию: ";
    while (!(cin >> surname) || (check_Name(surname) != true))
    {
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
    }
    if ((int)surname[0] <= 0)
    {
        surname = decode_kirill(surname);
    }
    cout << "Введите Отчество: ";
    while (!(cin >> otchestvo) || (check_Name(otchestvo) != true))
    {
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
    }
    if ((int)otchestvo[0] <= 0)
    {
        otchestvo = decode_kirill(otchestvo);
    }
    fio = surname + " " + name + " " + otchestvo;
    cout << "Введите дату рождения: ";
    cin >> birthday;
    while (!(valid_date(birthday)))
    {
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
        cin >> birthday;
    }
    int birthYear = (birthday[6] - 48) * 1000 + (birthday[7] - 48) * 100 + (birthday[8] - 48) * 10 + (birthday[9] - 48);
    while (((passYear - birthYear) < 14) || !(valid_date(birthday)) || (birthYear < 1910))
    {
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
        cin >> birthday;
        birthYear = (birthday[6] - 48) * 1000 + (birthday[7] - 48) * 100 + (birthday[8] - 48) * 10 + (birthday[9] - 48);
    }
    pass.passport = key;
    if (place.length() < 8)
    {
        pass.placedate = place + "\t" + "\t" + date;
    }
    else
    {
        pass.placedate = place + "\t" + date;
    }
    pass.fullname = fio;
    pass.birthdate = birthday;
    cout << "Данные о пассажире добавлены." << endl;
    add_to_hash(pass);
}

void Resize()
{
    const int elem_num = size_of_tab * 2;
    Hash_Table* new_Tab = new Hash_Table[elem_num];
    for (int i = 0; i < size_of_tab; ++i)
    {
        if ((hash_table[i].status != NULL) || (hash_table[i].status != false))
        {
            int adress = 0;
            char key[12] = {  };
            strcpy(key, hash_table[i].key.c_str());
            adress = h(key, elem_num);
            int collisions = 0;
            while (new_Tab[adress].status == true)
            {
                adress = adress + 2 * collisions + 3 * collisions * collisions;
                while (adress > size_of_tab)
                {
                    adress = adress % size_of_tab;
                }
                collisions++;
            }
            new_Tab[adress] = hash_table[i];
        }
    }
    size_of_tab = elem_num;
    swap(new_Tab, hash_table);
    delete[] new_Tab;
}

void add_to_hash(Passengers p)
{
    if (size_of_tab / 2 < num_of_pass)
    {
        Resize();
    }
    int adress = 0;
    int collisions = 0;
    char key[12] = {  };
    strcpy(key, p.passport.c_str());
    adress = h(key, size_of_tab);
    while (true)
    {
        if (hash_table[adress].status != true)
        {
            num_of_pass++;
            hash_table[adress].key = key;
            hash_table[adress].data.passport = key;
            hash_table[adress].data.placedate = p.placedate;
            hash_table[adress].data.fullname = p.fullname;
            hash_table[adress].data.birthdate = p.birthdate;
            hash_table[adress].status = true;
            break;
        }
        else
        {
            if (collisions == 5)
            {
                Resize();
                collisions = 0;
            }
            else
            {
                adress = adress + 2 * collisions + 3 * collisions * collisions;
                while (adress > size_of_tab - 1)
                {
                    adress = adress % size_of_tab;
                }
                collisions++;
            }

        }
    }
}

void del_pass() //удаление из хэш таблицы
{
    char key[12] = { 0 };
    bool correct = true;
    cout << "Введите номер паспорта в формате CCCC-НННННН ";
    input(key);
    adress = h(key, size_of_tab);
    bool flag = true;
    do
    {
        if (hash_table[adress].key == key)
        {
            del_gar_pass(head, hash_table[adress].key);
            hash_table[adress].status = false;
            adress = 0;
            break;
        }
        else
        {
            adress += 2;
        }
    } while (adress < size_of_tab - 1);
    if (adress >= size_of_tab)
        flag = false;
    if (flag == false)
        cout << "Пассажир не найден" << endl;
    else
    {

        cout << "Данные о пассажире удалены" << endl;

    }
}

void input(char key[11]) //проверка номера паспорта
{
    bool correct = true;
    char smth[256];
    do
    {
        cin >> smth;
        if (smth[11] != NULL)
        {
            correct = false;
            cout << "Введенные данные некорректны. Введите еще раз: " << endl;
            continue;
        }
        else
        {
            for (int i = 0; i < 11; i++)
            {
                key[i] = smth[i];
            }
            for (int i = 0; i < 11; i++)
            {
                if (i != 4)
                {
                    if (((int)key[i] < 48) || ((int)key[i] > 57))
                    {
                        correct = false;
                        cout << "Введенные данные некорректны. Введите еще раз: " << endl;
                        break;
                    }
                }
                else
                {
                    if (((int)key[4] != 45))
                    {
                        correct = false;
                        cout << "Введенные данные некорректны. Введите еще раз: " << endl;
                        break;
                    }
                }
                if (i == 10)
                {
                    correct = true;
                }
            }
        }

    } while (correct == false);
}

bool find_pass(char key[]) //поиск по номеру паспорта
{
    int collisions = 0;
    int copy_size = size_of_tab;
    do
    {
        if (hash_table[adress].key == key)
        {
            return true;
        }
        else
        {
            if (collisions == 5)
            {
                collisions = 0;
                copy_size /= 1.25;
                adress = h(key, copy_size);
                continue;
            }
            adress = adress + 2 * collisions + 3 * collisions * collisions;
            while (adress > copy_size - 1)
            {
                adress = adress % copy_size;
            }
            collisions++;
        }
    } while (copy_size != 4);
    if (hash_table[0].key == key)
    {
        return NULL;
    }
    return false;
}

void find_fio() //поиск по ФИО
{
    string name, surname, otchestvo, fio;
    cout << "Введите Имя: ";
    while (!(cin >> name) || (check_Name(name) != true))
    {
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
    }
    if ((int)name[0] <= 0)
    {
        name = decode_kirill(name);
    }
    cout << "Введите Фамилию: ";
    while (!(cin >> surname) || (check_Name(surname) != true))
    {
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
    }
    if ((int)surname[0] <= 0)
    {
        surname = decode_kirill(surname);
    }
    cout << "Введите Отчество: ";
    while (!(cin >> otchestvo) || (check_Name(otchestvo) != true))
    {
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
    }
    if ((int)otchestvo[0] <= 0)
    {
        otchestvo = decode_kirill(otchestvo);
    }
    fio = surname + " " + name + " " + otchestvo;
    bool flag = false;
    for (int i = 0; i < size_of_tab - 1; i++)
    {
        if ((hash_table[i].status == NULL) || (hash_table[i].status == false))
        {
            continue;
        }
        else
        {
            if (hash_table[i].data.fullname == fio)
            {
                cout << "Пассажир найден" << endl;
                cout << "№ паспорта" << "\t" << "Место и дата регистрации паспорта" << "\t" << "Фамилия Имя Отчество" << "\t\t" << "Дата рождения" << endl;
                cout << hash_table[i].key << "\t" << hash_table[i].data.placedate << "\t\t" << hash_table[i].data.fullname << "\t";
                if (hash_table[i].data.fullname.length() > 23)

                {
                    cout << hash_table[i].data.birthdate << endl;
                }
                else
                {
                    cout << "\t" << hash_table[i].data.birthdate << endl;
                }
                flag = true;
                break;
            }
        }

    }
    if (!flag)
    {
        cout << "Пассажир не найден" << endl;
    }
}

int into_int(string str)
{
    int fly_num = 0;
    int mnozhitel = 100;
    for (int i = 4; i < 7; ++i)
    {
        fly_num += (str[i] - 48) * mnozhitel;
        mnozhitel /= 10;
    };
    return fly_num;
}

int height(node* p)
{
    int t;
    if (p == NULL)
    {
        return -1;
    }
    else
    {
        t = p->height;
        return t;
    }
}

int maximum(int value1, int value2)
{
    return ((value1 > value2) ? value1 : value2);
}

node* srl(node* p1)
{
    node* p2 = new node;
    p2 = p1->left;
    p1->left = p2->right;
    p2->right = p1;
    p1->height = maximum(height(p1->left), height(p1->right)) + 1;
    p2->height = maximum(height(p2->left), p1->height) + 1;
    return p2;
}

node* srr(node* p1)
{
    node* p2 = new node;
    p2 = p1->right;
    p1->right = p2->left;
    p2->left = p1;
    p1->height = maximum(height(p1->left), height(p1->right)) + 1;
    p2->height = maximum(p1->height, height(p2->right)) + 1;
    return p2;
}

node* drl(node* p1)
{
    p1->left = srr(p1->left);
    return srl(p1);
}

node* drr(node* p1)
{
    p1->right = srl(p1->right);
    return srr(p1);
}

node** Balance(node** p, Flights d)
{
    if ((height((*p)->left) - height((*p)->right)) == 2)
            {
                if (into_int(d.fly_num) < into_int((*p)->left->data.fly_num))
                {
                    *p = srl(*p);
                }
                else
                {
                    *p = drl(*p);
                }
            }
    if ((height((*p)->right) - height((*p)->left)) == 2)
    {
        if (into_int(d.fly_num) > into_int((*p)->data.fly_num))
        {
            *p = srr(*p);
        }
        else
        {
            *p = drr(*p);
        }
    }
    return p;
}

void insert(node** p, Flights d)
{
    if (*p == NULL)
    {
        *p = new node;
        (*p)->data = d;
        (*p)->left = NULL;
        (*p)->right = NULL;
        (*p)->height = 0;
    }
    else
    {
        if (into_int(d.fly_num) < into_int((*p)->data.fly_num))
        {
            insert(&(*p)->left, d);
            p = Balance(p, d);
        }
        else if (into_int(d.fly_num) > into_int((*p)->data.fly_num))
        {
            insert(&(*p)->right, d);
            p = Balance(p, d);
        }
    }
    int m, n, dd;
    m = height((*p)->left);
    n = height((*p)->right);
    dd = maximum(m, n);
    (*p)->height = dd + 1;
}

void printsim(const node* p, bool truth) // распечатка дерева в симметричном порядке
{
    if (p != NULL)//  если дерево не пусто
    {
        if (truth)
        {
            printsim(p->left, true);   //  печатаем левое поддерево 
            cout << p->data.fly_num << "  " << p->data.airline_name;
            if (p->data.airline_name.length() >= 7)
            {
                cout << "\t " << p->data.depart_port;
            }
            else
            {
                cout << "\t\t " << p->data.depart_port;
            }
            if ((p->data.depart_port.length() >= 7) && (p->data.depart_port.length() < 12))
            {
                cout << "\t\t " << p->data.arriv_port;
            }
            else
            {
                if (p->data.depart_port.length() > 12)
                {
                    cout << "\t " << p->data.arriv_port;
                }
                else
                {
                    cout << "\t\t\t " << p->data.arriv_port;
                }
            }
            if ((p->data.arriv_port.length() >= 7) && (p->data.arriv_port.length() < 12))
            {
                cout << "\t " << p->data.date << "   " << p->data.time << "\t" << p->data.total_seats << "\t     " << p->data.avail_seats << endl;
            }
            else
            {
                if (p->data.arriv_port.length() > 12)
                {
                    cout << " " << p->data.date << "   " << p->data.time << "\t" << p->data.total_seats << "\t     " << p->data.avail_seats << endl;
                }
                else
                {
                    cout << "\t\t " << p->data.date << "   " << p->data.time << "\t" << p->data.total_seats << "\t     " << p->data.avail_seats << endl;

                }
            }
            printsim(p->right, true);  //  печатаем правое поддерево
        }
        else
        {
            cout << p->data.fly_num << "  " << p->data.airline_name;
            if (p->data.airline_name.length() > 7)
            {
                cout << "\t " << p->data.depart_port;
            }
            else
            {
                cout << "\t\t " << p->data.depart_port;
            }
            if ((p->data.depart_port.length() > 7) && (p->data.depart_port.length() < 12))
            {
                cout << "\t\t " << p->data.arriv_port;
            }
            else
            {
                if (p->data.depart_port.length() > 12)
                {
                    cout << "\t " << p->data.arriv_port;
                }
                else
                {
                    cout << "\t\t\t " << p->data.arriv_port;
                }
            }
            if ((p->data.arriv_port.length() >= 7) && (p->data.arriv_port.length() < 12))
            {
                cout << "\t " << p->data.date << "   " << p->data.time << "\t" << p->data.total_seats << "\t     " << p->data.avail_seats << endl;
            }
            else
            {
                if (p->data.arriv_port.length() > 12)
                {
                    cout << " " << p->data.date << "   " << p->data.time << "\t" << p->data.total_seats << "\t     " << p->data.avail_seats << endl;
                }
                else
                {
                    cout << "\t\t " << p->data.date << "   " << p->data.time << "\t" << p->data.total_seats << "\t     " << p->data.avail_seats << endl;

                }
            }
        }
    }

}

void clear(node** p) // очистка дерева
{
    if ((*p) != NULL) //  если дерево не пусто
    {
        clear(&(*p)->left);   //  удаляем левое поддерево     
        clear(&(*p)->right);  //  удаляем правое поддерево    
        delete* p; //Удаляем указатель на текущий узел
        *p = NULL;//Присваиваем ему пустой указатель
    }
}

void OutLinePassengers(Hash_Table* p)
{
    bool void_Tab = false;
    for (int i = 0; i < size_of_tab - 1; i++)
    {
        if ((hash_table[i].status == NULL) || (hash_table[i].status == false))
        {
            if (i == size_of_tab - 2)
            {
                cout << "Список данных о пассажирах пуст";
                void_Tab = true;
            }
            continue;
        }
        else
        {
            break;
        }
    }
    if (void_Tab == false)
    {
        cout << "№ паспорта" << "\t" << "Место и дата регистрации паспорта" << "\t" << "Фамилия Имя Отчество" << "\t\t" << "Дата рождения" << endl << endl;
        for (int i = 0; i < size_of_tab - 1; i++)
        {
            if ((hash_table[i].status == NULL) || (hash_table[i].status == false))
            {
                continue;
            }
            else
            {
                cout << p[i].key << "\t" << p[i].data.placedate << "\t\t" << p[i].data.fullname << "\t";
                if (p[i].data.fullname.length() > 23)

                {
                    cout << p[i].data.birthdate << endl;
                }
                else
                {
                    cout << "\t" << p[i].data.birthdate << endl;
                }
            }
        }
    }
}

void rand_pass()
{
    int a = 0;
    Passengers pass;
    string fio, passnum, placement, passday, birtday;
    string surname[10] = { "Иванов", "Сидоров", "Петров", "Петухов", "Байбурин", "Андреев", "Кузнецов", "Богатырев", "Печкин", "Саночкин" };
    string name[10] = { "Иван", "Петр", "Сидор", "Никола", "Илон", "Альберт", "Исаак", "Роберт", "Андрей", "Иосиф" };
    string patronymics[10] = { "Иванович","Петрович","Сидорович","Николаевич","Илонович","Альбертович","Исаакович","Робертович","Маркович","Викторович" };
    string place[10] = { "Архангельск", "Санкт-Петербург", "Москва", "Анадырь", "Иваново", "Сыктывкар", "Челябинск","Новосибирск", "Нижний Новгород", "Тагил" };
    for (int i = 0; i < 9; i++)
    {
        fio = "";
        passnum = "";
        passday = "";
        birtday = "";
        for (int i = 0; i < 11; i++)
        {
            if (i == 4)
            {
                passnum += "-";
            }
            else
            {
                passnum += (rand() % 10 + 48);
            }
        }
        a = rand() % 10; placement = place[a];
        passday += to_string(1 + rand() % 29);
        if (passday.length() < 2)
        {
            passday += " ";
            passday[1] = passday[0];
            passday[0] = 48;
        }
        passday += ".";
        passday += to_string(1 + rand() % 12);
        if (passday.length() < 5)
        {
            passday += " ";
            passday[4] = passday[3];
            passday[3] = 48;
        }
        passday += ".201";
        passday += (rand() % 10 + 48);
        birtday += to_string(1 + rand() % 29);
        if (birtday.length() < 2)
        {
            birtday += " ";
            birtday[1] = birtday[0];
            birtday[0] = 48;
        }
        birtday += ".";
        birtday += to_string(1 + rand() % 12);
        if (birtday.length() < 5)
        {
            birtday += " ";
            birtday[4] = birtday[3];
            birtday[3] = 48;
        }
        birtday += ".19";
        birtday += to_string(60 + rand() % 40);
        a = rand() % 10; fio += surname[a] + ' ';
        a = rand() % 10; fio += name[a] + ' ';
        a = rand() % 10; fio += patronymics[a];
        pass.passport = passnum;
        if (placement.length() < 8)
        {
            pass.placedate = placement + '\t' + '\t' + passday;
        }
        else
        {
            pass.placedate = placement + '\t' + passday;
        }
        pass.fullname = fio;
        pass.birthdate = birtday;
        add_to_hash(pass);
    }
}

void mergee(Flights merged[], Flights L[], int lenL, Flights R[], int lenR)
{
    int i = 0;
    int j = 0;
    while (i < lenL || j < lenR) {
        if (i < lenL & j < lenR) {
            if (into_int(L[i].fly_num) <= into_int(R[j].fly_num)) {
                merged[i + j] = L[i];
                i++;
            }
            else {
                merged[i + j] = R[j];
                j++;
            }
        }
        else if (i < lenL) {
            merged[i + j] = L[i];
            i++;
        }
        else if (j < lenR) {
            merged[i + j] = R[j];
            j++;
        }
    }
}

void mergeeSort(Flights data[], int lenD)
{
    if (lenD > 1) {
        int middle = lenD / 2;
        int rem = lenD - middle;
        Flights* L = new Flights[middle];
        Flights* R = new Flights[rem];
        for (int i = 0; i < lenD; i++) {
            if (i < middle) {
                L[i] = data[i];
            }
            else {
                R[i - middle] = data[i];
            }
        }
        mergeeSort(L, middle);
        mergeeSort(R, rem);
        mergee(data, lenD, L, middle, R, rem);
    }
}

void rand_flights()
{
    Flights flight;
    Flights massflights[10];
    string fly_nums[6] = { "RUS-", "SEV-", "AER-", "AZM-", "WIN-", "IRA-" };
    string airline_names[6] = { "Россия", "Северсталь", "Аэрофлот", "Азимут", "Победа", "Ираэро" };
    string depart_ports[6] = { "Санкт-Петербург", "Казань", "Москва", "Калининград", "Новосибирск", "Симферополь" };
    string arriv_ports[6] = { "Саратов", "Самара", "Краснодар", "Хельсинки", "Архангельск", "Сочи" };
    int a = 0;
    for (int i = 0; i < num_flights; i++)
    {
        flight.date = "";
        flight.time = "";
        a = rand() % 6; flight.fly_num = fly_nums[a]; flight.airline_name = airline_names[a];
        a = rand() % 999 + 1;
        flight.fly_num += to_string(a);
        if (flight.fly_num.length() < 7)
        {
            flight.fly_num += " ";
            flight.fly_num[6] = flight.fly_num[5];
            flight.fly_num[5] = flight.fly_num[4];
            flight.fly_num[4] = 48;
        }
        a = rand() % 6; flight.depart_port = depart_ports[a];
        a = rand() % 6; flight.arriv_port = arriv_ports[a];
        flight.date += to_string(1 + rand() % 28);
        if (flight.date.length() < 2)
        {
            flight.date += " ";
            flight.date[1] = flight.date[0];
            flight.date[0] = 48;
        }
        flight.date += ".";
        flight.date += to_string(1 + rand() % 12);
        if (flight.date.length() < 5)
        {
            flight.date += " ";
            flight.date[4] = flight.date[3];
            flight.date[3] = 48;
        }
        flight.date += ".2021";
        flight.time += to_string(rand() % 24);
        if (flight.time.length() < 2)
        {
            flight.time += " ";
            flight.time[1] = flight.time[0];
            flight.time[0] = 48;
        }
        flight.time += ":";
        flight.time += to_string(rand() % 60);
        if (flight.time.length() < 5)
        {
            flight.time += " ";
            flight.time[4] = flight.time[3];
            flight.time[3] = 48;
        }
        flight.total_seats = rand() % 200 + 50;
        flight.avail_seats = flight.total_seats;
        flight_for_tickets[i] = flight.fly_num;
        massflights[i] = flight;
    }
    mergeeSort(massflights, 10);
    for (int i = 0; i < 10; i++)
    {
        insert(&new_node, massflights[i]);
    }
}

void add_flight() //регистрация нового авиарейса 
{
    Flights flight;
    int a;
    cout << "Введите код авиакомпании в формате ААА: ";
    while (!(cin >> flight.fly_num) || ((check_air_Name(flight.fly_num) != true)))
    {
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
    }
    flight.fly_num += "-";
    a = rand() % 999 + 1;
    flight.fly_num += to_string(a);
    if (flight.fly_num.length() < 7)
    {
        flight.fly_num += " ";
        flight.fly_num[6] = flight.fly_num[5];
        flight.fly_num[5] = flight.fly_num[4];
        flight.fly_num[4] = 48;
    }
    cout << "Введите название авиакомпании: ";
    while (!(cin >> flight.airline_name) || (check_Name(flight.airline_name) != true))
    {
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
    }
    if ((int)flight.airline_name[0] <= 0)
    {
        flight.airline_name = decode_kirill(flight.airline_name);
    }
    cout << "Введите название аэропорта отправления: ";
    while (!(cin >> flight.depart_port) || (check_Name(flight.depart_port) != true))
    {
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
    }
    if ((int)flight.depart_port[0] <= 0)
    {
        flight.depart_port = decode_kirill(flight.depart_port);
    }
    cout << "Введите название аэропорта прибытия: ";
    while (!(cin >> flight.arriv_port) || (check_Name(flight.arriv_port) != true))
    {
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
    }
    if ((int)flight.arriv_port[0] <= 0)
    {
        flight.arriv_port = decode_kirill(flight.arriv_port);
    }
    cout << "Введите дату отправления: ";
    while (!(cin >> flight.date) || (valid_date(flight.date) != true))
    {
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
    }
    cout << "Введите время отправления: ";
    while (!(cin >> flight.time) || !(valid_time(flight.time)))
    {
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
    }
    cout << "Введите количество мест в самолете отправления: ";
    while (!(cin >> flight.total_seats) || (flight.total_seats <= 0) || (flight.total_seats > 500) || (cin.peek() != '\n'))
    {
        cin.clear();
        while (cin.get() != '\n');
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
    }
    flight.avail_seats = flight.total_seats;
    insert(&new_node, flight);
    cout << "Данные об авиарейсе добавлены." << endl;
}

node* find_air_num(node* airlines, string k)
{
    if (airlines != NULL)
    {
        if (into_int(airlines->data.fly_num) == into_int(k))
        {
            return airlines;
        }
        if (into_int(airlines->data.fly_num) > into_int(k))
        {
            return find_air_num(airlines->left, k);
        }
        else
        {
            return find_air_num(airlines->right, k);
        }
    }
    return NULL;
}

node* findmin(node* p) // поиск узла с минимальным ключом в дереве p 
{
    return p->left ? findmin(p->left) : p;
}

node* removemin(node* p)
{
    if (p->left == 0)
        return p->right;
    p->left = removemin(p->left);
    return p;
}

node* del_air(node* p, string k)
{
    if (!p) return 0;
    if (into_int(k) < into_int(p->data.fly_num))
        p->left = del_air(p->left, k);
    else if (into_int(k) > into_int(p->data.fly_num))
        p->right = del_air(p->right, k);
    else //  k == p->key 
    {
        node* q = p->left;
        node* r = p->right;
        delete p;
        if ((height((q)->left) - height((q)->right)) == 2)
        {
            if (into_int(k) < into_int((q)->left->data.fly_num))
            {
                q = srl(q);
            }
            else
            {
                q = drl(q);
            }
        }
        if ((height((q)->right) - height((q)->left)) == 2)
        {
            if (into_int(k) > into_int((q)->data.fly_num))
            {
                q = srr(q);
            }
            else
            {
                q = drr(q);
            }
        }
        if (!r) return q;
        node* min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        if ((height((min)->left) - height((min)->right)) == 2)
        {
            if (into_int(k) < into_int((min)->left->data.fly_num))
            {
                min = srl(min);
            }
            else
            {
                min = drl(min);
            }
        }
        if ((height((min)->right) - height((min)->left)) == 2)
        {
            if (into_int(k) > into_int((min)->data.fly_num))
            {
                min = srr(min);
            }
            else
            {
                min = drr(min);
            }
        }
        return min;
    }
    if ((height((p)->left) - height((p)->right)) == 2)
    {
        if (into_int(k) < into_int((p)->left->data.fly_num))
        {
            p = srl(p);
        }
        else
        {
            p = drl(p);
        }
    }
    if ((height((p)->right) - height((p)->left)) == 2)
    {
        if (into_int(k) > into_int((p)->data.fly_num))
        {
            p = srr(p);
        }
        else
        {
            p = drr(p);
        }
    }
    return p;
}

void make_delta1(int* delta1, char* pat, long int patlen)
{
    int i;
    for (i = 0; i < ALPHABET_LEN; i++) delta1[i] = NOT_FOUND;
    for (i = 0; i < patlen - 1; i++) delta1[pat[i]] = patlen - 1 - i;
}

int is_prefix(char* word, int wordlen, int pos)
{
    int suffixlen = wordlen - pos;
    for (int i = 0; i < suffixlen; i++)
    {
        if (word[i] != word[pos + i]) return 0;
    }
    return 1;
}

int suffix_length(char* word, int wordlen, int pos)
{
    int i;
    for (i = 0; (word[pos - i] == word[wordlen - 1 - i]) && (i < pos); i++);
    return i;
}

void make_delta2(int* delta2, char* pat, long int patlen)
{
    int last_prefix_index = patlen - 1;
    int p;
    for (p = patlen - 1; p >= 0; p--)
    {
        if (is_prefix(pat, patlen, p + 1)) last_prefix_index = p + 1;
        delta2[p] = last_prefix_index + (patlen - 1 - p);
    }
    for (p = 0; p < patlen - 1; p++)
    {
        int slen = suffix_length(pat, patlen, p);
        if (pat[p - slen] != pat[patlen - 1 - slen]) delta2[patlen - 1 - slen] = patlen - 1 - p + slen;
    }
}

int boyer_moore(char* string, char* pat)
{
    long int stringlen = strlen(string);
    long int patlen = strlen(pat);
    int delta1[ALPHABET_LEN];
    make_delta1(delta1, pat, patlen);
    int* delta2 = new int[patlen * sizeof(int)];
    make_delta2(delta2, pat, patlen);
    int i = patlen - 1;
    while (i < stringlen)
    {
        int j = patlen - 1;
        while (j >= 0 && (string[i] == pat[j]))
        {
            --i; --j;
        }
        if (j < 0)
        {
            delete delta2;
            return i + 2;
            int k = i + 2;
            for (int f = 0; f <= k + 1; f++)
                string[f] = 0;

        }
        i += maxxx(delta1[string[i]], delta2[j]);

    }
    return NULL;
}

node* find_arriv_port(node* airlines, string arriv_port)
{
    if (airlines != NULL)
    {
        char p1[100]; strcpy(p1, airlines->data.arriv_port.c_str());
        char p2[100]; strcpy(p2, arriv_port.c_str());
        if (boyer_moore(p1, p2) != 0)
        {
            insert(&find_node, airlines->data);
            find_arriv_port(airlines->left, arriv_port);
        }
        else if (airlines->left == NULL)
        {
            find_arriv_port(airlines->right, arriv_port);
        }
        else if (airlines->right == NULL)
        {
            find_arriv_port(airlines->left, arriv_port);
        }
        else
        {
            find_arriv_port(airlines->left, arriv_port) &&
                find_arriv_port(airlines->right, arriv_port);
        }
    }
    else
    {
        return NULL;
    }
}

list_tickets* init(list_tickets** begin, Tickets elem)
{
    list_tickets* p = new list_tickets;
    p->data = elem;
    p->next = NULL;
    p->prev = NULL;
    return p;
}

list_tickets* AddElem(list_tickets* lst, Tickets elem)
{
    list_tickets* temp = new list_tickets;
    list_tickets* p = new list_tickets;
    p = lst->next; // сохранение указателя на следующий узел
    lst->next = temp; // предыдущий узел указывает на создаваемый
    temp->data = elem; // сохранение поля данных добавляемого узла
    temp->next = p; // созданный узел указывает на следующий узел
    temp->prev = lst; // созданный узел указывает на предыдущий узел
    if (p != NULL)
        p->prev = temp;
    return(temp);
}

list_tickets* deletelem(list_tickets* lst)
{
    struct list_tickets* prev, * next;
    prev = lst->prev; // узел, предшествующий lst
    next = lst->next; // узел, следующий за lst
    if (prev != NULL)
        prev->next = lst->next; // переставляем указатель
    if (next != NULL)
        next->prev = lst->prev; // переставляем указатель
    free(lst); // освобождаем память удаляемого элемента
    return(prev);
}

list_tickets* deletehead(list_tickets* root)
{
    struct list_tickets* temp;
    if (root->next == NULL)
    {
        return NULL;
    }
    temp = root->next;
    temp->prev = NULL;
    free(root);   // освобождение памяти текущего корня
    return temp; // новый корень списка
}

void listprint(list_tickets* lst)
{
    if (lst != NULL)
    {
        cout << "№ паспорта " << '\t' << "№ авиарейса " << '\t' << "№ билета " << endl;
        struct list_tickets* p;
        p = lst;
        do {
            cout << p->data.passport << '\t' << p->data.fly_num << '\t' << '\t' << p->data.bilet_num << endl;
            p = p->next; // переход к следующему узлу
        } while (p != NULL); // условие окончания обхода
    }
    else
    {
        cout << "Купленных билетов нет." << endl;
    }
}

bool find_copies(list_tickets* head, string passport, string flight)
{
    list_tickets* ptr;
    ptr = head;
    do
    {
        if ((passport == ptr->data.passport) && (flight == ptr->data.fly_num))
            return true;
        else
        {
            if (ptr->next == NULL)
            {
                return false;
            }
            else
            {
                ptr = ptr->next;
            }
        }
    } while (ptr != head);
    return false;
}

list_tickets* find_bilet_num(list_tickets* head, string bilet_num)
{
    list_tickets* ptr;
    ptr = head;
    do
    {
        if (bilet_num == ptr->data.bilet_num)
            return ptr;
        else
        {
            if (ptr->next == NULL)
            {
                return NULL;
            }
            else
            {
                ptr = ptr->next;
            }
        }
    } while (ptr != head);
    return NULL;
}

string* find_pass_bilets(list_tickets* head, string passport)
{
    list_tickets* ptr;
    string* ret = new string[10];
    int i = 0;
    ptr = head;
    do
    {
        if (passport == ptr->data.passport)
        {
            ret[i] = ptr->data.fly_num;
            i++;
            if (ptr->next == NULL)
            {
                return ret;
            }
            else
            {
                ptr = ptr->next;
            }
        }
        else
        {
            if (ptr->next == NULL)
            {
                return ret;
            }
            else
            {
                ptr = ptr->next;
            }
        }
    } while (ptr != head);
    return ret;
}

string* find_flights_bilets(list_tickets* head, string num_flight)
{
    list_tickets* ptr;
    string* ret = new string[250];
    int i = 0;
    ptr = head;
    do
    {
        if (num_flight == ptr->data.fly_num)
        {
            char key[12] = { NULL };
            for (int iter = 0; iter < 12; iter++)
            {
                key[iter] = ptr->data.passport[iter];
            }
            adress = h(key, size_of_tab);
            if (hash_table[adress].status == true)
            {
                ret[i] += hash_table[adress].key;
                ret[i] += "\t";
                ret[i] += hash_table[adress].data.fullname;
            }
            else
            {
                find_pass(key);
                ret[i] += hash_table[adress].key;
                ret[i] += "\t";
                ret[i] += hash_table[adress].data.fullname;
            }
            if (ptr->next == NULL)
            {
                return ret;
            }
            else
            {
                i++;
                ptr = ptr->next;
            }
        }
        else
        {
            if (ptr->next == NULL)
            {
                return ret;
            }
            else
            {
                ptr = ptr->next;
            }
        }
    } while (ptr != head);
    return ret;
}

void del_gar_bilets(list_tickets* head, string num_flight)
{
    list_tickets* ptr;
    ptr = head;
    do
    {
        if (ptr->data.fly_num == num_flight)
        {
            list_tickets* del_elem = NULL;
            del_elem = find_bilet_num(head, ptr->data.bilet_num);
            if (del_elem != NULL)
            {
                if (del_elem == head)
                {
                    head = deletehead(del_elem);
                }
                else
                {
                    deletelem(del_elem);
                    ptr = head;
                }
            }
        }
        else
        {
            if (ptr->next == NULL)
            {
                break;
            }
            else
            {
                ptr = ptr->next;
            }
        }
    } while (true);
}

void del_gar_pass(list_tickets* head, string passport)
{
    list_tickets* ptr;
    ptr = head;
    do
    {
        if (ptr->data.passport == passport)
        {
            list_tickets* del_elem = NULL;
            find_air_num(new_node, ptr->data.fly_num)->data.avail_seats++;
            del_elem = find_bilet_num(head, ptr->data.bilet_num);
            if (del_elem != NULL)
            {
                if (del_elem == head)
                {
                    head = deletehead(del_elem);
                }
                else
                {
                    deletelem(del_elem);
                    ptr = head;
                }
            }
        }
        else
        {
            if (ptr->next == NULL)
            {
                break;
            }
            else
            {
                ptr = ptr->next;
            }
        }
    } while (true);
}

bool check_bilet_num(string bilet_num)
{
    if (bilet_num.length() == 9)
    {
        for (int i = 0; i < 9; i++)
        {
            if (((int)bilet_num[i] >= 48) && ((int)bilet_num[i] <= 57))
            {
                continue;
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

list_tickets* fast(list_tickets* first, list_tickets* second)
{
    // Если первый связанный список пуст
    if (!first)
        return second;
    // Если второй связанный список пуст
    if (!second)
        return first;
    // Выбрать меньшее значение
    if (atoi(first->data.bilet_num.c_str()) < atoi(second->data.bilet_num.c_str()))
    {
        first->next = fast(first->next, second);
        first->next->prev = first;
        first->prev = NULL;
        return first;
    }
    else
    {
        second->next = fast(first, second->next);
        second->next->prev = second;
        second->prev = NULL;
        return second;
    }
}

list_tickets* split(list_tickets* head)
{
    list_tickets* fast = head, * slow = head;
    while (fast->next && fast->next->next)
    {
        fast = fast->next->next;
        slow = slow->next;
    }
    list_tickets* temp = slow->next;
    slow->next = NULL;
    return temp;
}

list_tickets* fastSort(list_tickets* head)
{
    if (!head || !head->next)
        return head;
    list_tickets* second = split(head);
    // Повтор для левой и правой половин
    head = fastSort(head);
    second = fastSort(second);
    // Объединяем две отсортированные половинки
    return fast(head, second);
}

void rand_tickets()
{
    Tickets ticket;
    int a = 0;
    for (int i = 0; i < num_tickets; i++)
    {
        a = rand() % size_of_tab;
        while (hash_table[a].status == NULL)
        {
            a++;
            if (a == size_of_tab)
            {
                a = 0;
            }
        }
        ticket.passport = hash_table[a].key;
        a = rand() % num_flights;
        ticket.fly_num = flight_for_tickets[a];
        if (i > 0)
        {
            if (find_copies(head, ticket.passport, ticket.fly_num))
            {
                continue;
            }
        }
        find_air_num(new_node, ticket.fly_num)->data.avail_seats--;
        ticket.bilet_num = "";
        for (int i = 0; i < 9; i++)
        {
            a = rand() % 9 + 1;
            ticket.bilet_num += to_string(a);
        }
        if (i == 0)
        {
            head = (init(&head, ticket));
            cur = head;
        }
        else
        {
            AddElem(cur, ticket);
        }
    }
    head = fastSort(head);
}

void add_ticket()
{
    Tickets ticket;
    int a = 0;
    string AAA;
    string place, date, fio, surname, name, otchestvo, birthday;
    char key[12] = { NULL };
    cout << "Введите номер паспорта формата СССС-НННННН ";
    input(key);
    adress = h(key, size_of_tab);
    while (find_pass(key) == false)
    {
        cout << "Пассажир не найден" << endl;
        cout << "Введите номер паспорта формата СССС-НННННН ";
        input(key);
        adress = h(key, size_of_tab);
    }
    ticket.passport = hash_table[adress].key;
    cout << "Введите номер авиарейса: ";
    while (!(cin >> AAA) || (stoi(AAA) <= 0) || (stoi(AAA) > 999) || AAA.length() != 3 || (cin.peek() != '\n'))
    {
        cin.clear();
        while (cin.get() != '\n');
        cout << "Ошибка ввода. Попробуйте ещё раз: ";
    }
    if (AAA.length() == 2)
    {
        AAA.insert(0, "0");
    }
    if (AAA.length() == 1)
    {
        AAA.insert(0, "00");
    }
    AAA.insert(0, "AAA-");
    while (find_copies(head, ticket.passport, find_air_num(new_node, AAA)->data.fly_num) == true)
    {
        cout << "У этого пассажира уже есть билет на этот рейс." << endl;
        cout << "Введите номер авиарейса: ";
        while (!(cin >> AAA) || (stoi(AAA) <= 0) || (stoi(AAA) > 999) || AAA.length() != 3 || (cin.peek() != '\n'))
        {
            cin.clear();
            while (cin.get() != '\n');
            cout << "Ошибка ввода. Попробуйте ещё раз: ";
        }
        if (AAA.length() == 2)
        {
            AAA.insert(0, "0");
        }
        if (AAA.length() == 1)
        {
            AAA.insert(0, "00");
        }
        AAA.insert(0, "AAA-");
    }
    while (find_air_num(new_node, AAA)->data.avail_seats < 1)
    {
        cout << "На этот рейс все билеты распроданы." << endl;
        cout << "Введите номер авиарейса: ";
        while (!(cin >> AAA) || (stoi(AAA) <= 0) || (stoi(AAA) > 999) || AAA.length() != 3 || (cin.peek() != '\n'))
        {
            cin.clear();
            while (cin.get() != '\n');
            cout << "Ошибка ввода. Попробуйте ещё раз: ";
        }
        if (AAA.length() == 2)
        {
            AAA.insert(0, "0");
        }
        if (AAA.length() == 1)
        {
            AAA.insert(0, "00");
        }
        AAA.insert(0, "AAA-");
    }

    ticket.fly_num = find_air_num(new_node, AAA)->data.fly_num;
    ticket.bilet_num = "";
    for (int i = 0; i < 9; i++)
    {
        a = rand() % 9 + 1;
        ticket.bilet_num += to_string(a);
    }
    if (head != NULL)
    {
        AddElem(cur, ticket);
    }
    else
    {
        head = (init(&head, ticket));
        cur = head;
    }
    head = fastSort(head);
    cout << "Билет приобретён." << endl;
}

void del_ticket(string bilet_num)
{
    list_tickets* del_elem = NULL;
    del_elem = find_bilet_num(head, bilet_num);
    if (del_elem != NULL)
    {
        if (del_elem == head)
        {
            head = deletehead(del_elem);
        }
        else
        {
            deletelem(del_elem);
        }
        cout << "Билет удалён." << endl;
    }
    else
    {
        cout << "Билет не найден." << endl;
    }
}

int main()
{
    system("color F0");
    setlocale(LC_ALL, "");
    int menu = -1;
    rand_pass();
    rand_flights();
    rand_tickets();
    while (menu != 0) {
        cout << endl <<
            "1 - Регистрация нового пассажира" << endl <<
            "2 - Удаление данных о пассажире" << endl <<
            "3 - Просмотр всех зарегистрированных пассажиров" << endl <<
            "4 - Очистка данных о пассажирах" << endl <<
            "5 - Поиск пассажира по номеру паспорта" << endl <<
            "6 - Поиск пассажира по его ФИО" << endl <<
            "7 - Добавление нового авиарейса" << endl <<
            "8 - Удаление сведений об авиарейсе" << endl <<
            "9 - Просмотр всех авиарейсов" << endl <<
            "10 - Очистка данных об авиарейсах" << endl <<
            "11 - Поиск авиарейса по номеру авиарейса" << endl <<
            "12 - Поиск авиарейса по фрагментам названия аэропорта прибытия" << endl <<
            "13 - Регистрация продажи пассажиру авиабилета" << endl <<
            "14 - Регистрация возврата пассажиром авиабилета" << endl <<
            "0 - Выход\nВыберите действие: ";

        while (!(cin >> menu) || (menu < 0) || (menu > 16) || (cin.peek() != '\n'))
        {
            cin.clear();
            while (cin.get() != '\n');
            cout << "Ошибка ввода. Попробуйте ещё раз: ";
        }
        switch (menu)
        {
        case 1:
        {
            add_pass();
            break;
        }
        case 2:
        {
            del_pass();
            break;
        }
        case 3:
        {
            OutLinePassengers(hash_table);
            break;
        }
        case 4:
        {
            Hash_Table* del_Tab = new Hash_Table[4];
            swap(del_Tab, hash_table);
            delete[] del_Tab;
            size_of_tab = 4;
            cout << "Данные о пассажирах удалены" << endl;
            break;
        }
        case 5:
        {
            bool pass_void = true;
            for (int i = 0; i < size_of_tab - 1; i++)
            {
                if ((hash_table[i].status == NULL) || (hash_table[i].status == false))
                {
                    if (i == size_of_tab - 2)
                    {
                        break;
                    }
                    continue;
                }
                else
                {
                    pass_void = false;
                    break;
                }
            }
            if (pass_void == false)
            {
                char key[12] = { NULL };
                cout << "Введите номер паспорта формата СССС-НННННН ";
                input(key);
                adress = h(key, size_of_tab);
                if (find_pass(key) == NULL)
                    adress = 0;
                if (find_pass(key) == false)
                    cout << "Пассажир не найден" << endl;
                else
                {
                    cout << "Пассажир найден" << endl;
                    cout << "№ паспорта" << "\t" << "Место и дата регистрации паспорта" << "\t" << "Фамилия Имя Отчество" << "\t\t" << "Дата рождения" << endl;
                    cout << hash_table[adress].key << "\t" << hash_table[adress].data.placedate << "\t\t" << hash_table[adress].data.fullname << "\t";
                    if (hash_table[adress].data.fullname.length() > 23)

                    {
                        cout << hash_table[adress].data.birthdate << endl;
                    }
                    else
                    {
                        cout << "\t" << hash_table[adress].data.birthdate << endl;
                    }
                    string* ctr = find_pass_bilets(head, hash_table[adress].data.passport);
                    if (ctr[0] != "")
                    {
                        cout << "Номера авиарейсов, на которые он купил билет: ";
                        for (int i = 0; i < 10; i++)
                        {
                            if (ctr[i] != "")
                            {
                                cout << ctr[i] << " ";
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        cout << "Пассажир не купил ни одного билета. ";
                    }
                    cout << endl;
                }
            }
            else
            {
                cout << "Список данных о пассажирах пуст";
            }
            break;
        }
        case 6:
        {
            find_fio();
            break;
        }
        case 7:
        {
            add_flight();
            break;
        }
        case 8:
        {
            if (new_node != NULL) //  если дерево не пусто
            {
                string AAA;
                cout << "Введите номер авиарейса: ";
                while (!(cin >> AAA) || (stoi(AAA) <= 0) || (stoi(AAA) > 999) || (cin.peek() != '\n'))
                {
                    cin.clear();
                    while (cin.get() != '\n');
                    cout << "Ошибка ввода. Попробуйте ещё раз: ";
                }
                if (AAA.length() == 2)
                {
                    AAA.insert(0, "0");
                }
                if (AAA.length() == 1)
                {
                    AAA.insert(0, "00");
                }
                AAA.insert(0, "AAA-");
                if (find_air_num(new_node, AAA) == NULL)
                {
                    cout << "Данных об авиарейсе не обнаружено." << endl;
                }
                else
                {
                    node* finded_air = find_air_num(new_node, AAA);
                    del_gar_bilets(head, finded_air->data.fly_num);
                    head = fastSort(head);
                    del_air(new_node, AAA);
                    cout << "Данные об авиарейсе удалены." << endl;
                }
            }
            else
            {
                cout << "Данных об авиарейсах не обнаружено." << endl;
            }
            break;
        }
        case 9:
        {
            if (new_node != NULL)
            {
                cout << "№ рейса\t" << " Авиакомпания\t" << " Отправляется из\t" << " Прибудет в\t"
                    << " Дата" << "\t      " << "Время" << "  Всего мест" << "   Свободных" << endl << endl;
                printsim(new_node, true);
            }
            else
            {
                cout << "Данных об авиарейсах не обнаружено." << endl;
            }
            break;
        }
        case 10:
        {
            if (new_node != NULL) //  если дерево не пусто
            {
                clear(&new_node);
                cout << "Данные об авиарейсах удалены." << endl;
            }
            else
            {
                cout << "Данных об авиарейсах не обнаружено." << endl;
            }
            break;
        }
        case 11:
        {
            string AAA;
            cout << "Введите номер авиарейса: ";
            while (!(cin >> AAA) || (stoi(AAA) <= 0) || (stoi(AAA) > 999) || (cin.peek() != '\n'))
            {
                cin.clear();
                while (cin.get() != '\n');
                cout << "Ошибка ввода. Попробуйте ещё раз: ";
            }
            if (AAA.length() == 2)
            {
                AAA.insert(0, "0");
            }
            if (AAA.length() == 1)
            {
                AAA.insert(0, "00");
            }
            AAA.insert(0, "AAA-");
            node* finded_air = find_air_num(new_node, AAA);
            if (finded_air != NULL)
            {
                cout << "№ рейса\t" << " Авиакомпания\t" << " Отправляется из\t" << " Прибудет в\t"
                    << " Дата" << "\t      " << "Время" << "  Всего мест" << "   Свободных" << endl << endl;
                printsim(finded_air, false);
                string* ctr = find_flights_bilets(head, finded_air->data.fly_num);
                if (ctr[0] != "")
                {
                    cout << "Пассажиры, купившие билеты на этот рейс: " << endl;
                    for (int i = 0; i < finded_air->data.total_seats; i++)
                    {
                        if (ctr[i] != "")
                        {
                            cout << ctr[i] << endl;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else
                {
                    cout << "На этот рейс билетов не куплено. " << endl;
                }

            }
            else
            {
                cout << "Данных об авиарейсе не обнаружено." << endl;
            }
            break;
        }
        case 12:
        {
            string arriv_port;
            cout << "Введите название аэропорта прибытия: ";
            while (!(cin >> arriv_port) || (check_Name(arriv_port) != true))
            {
                cout << "Ошибка ввода. Попробуйте ещё раз: ";
            }
            if ((int)arriv_port[0] <= 0)
            {
                arriv_port = decode_kirill(arriv_port);
            }
            clear(&find_node);
            find_arriv_port(new_node, arriv_port);
            if (find_node != NULL)
            {
                cout << "№ рейса\t" << " Авиакомпания\t" << " Отправляется из\t" << " Прибудет в\t"
                    << " Дата" << "\t      " << "Время" << "  Всего мест" << "   Свободных" << endl << endl;
                printsim(find_node, true);
            }
            else
            {
                cout << "Данных об авиарейсах не обнаружено." << endl;
            }

            break;
        }
        case 13:
        {
            add_ticket();
            break;
        }
        case 14:
        {
            string bilet_num;
            cout << "Введите номер билета: ";
            while (!(cin >> bilet_num) || ((check_bilet_num(bilet_num) != true)))
            {
                cout << "Ошибка ввода. Попробуйте ещё раз: ";
            }
            del_ticket(bilet_num);
            break;
        }
        case 15:
        {
            listprint(head);
            break;
        }
        }
    }
    return EXIT_SUCCESS;
}

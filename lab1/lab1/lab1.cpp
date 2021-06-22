#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Player // клас гравця
{
public:

    Player(const std::string& name)
    {
        this->name = name;
    }
    string get_name() const
    {
        return this->name;
    }
private:
    string name;
};

class Team // клас команди
{
public:
    Team() = default;
    Team(const string& name)
    {
        this->name = name;
    }
    void add_player(const string& name)
    {
        players.push_back(Player(name));
    }
    string get_name() const
    {
        return this->name;
    }
    friend ostream& operator<< (std::ostream& out, const Team& section);
private:
    string name;
    vector<Player> players;
};

std::ostream& operator<<(std::ostream& out, const Team& team)
{
    if (team.name.length() != 0)
    {
        out << " " << team.name << " { ";
        for (const auto a : team.players)
        {
            out << a.get_name() + " ";
        }
        out << "}";
    }
    else
    {
        out << "[]";
    }
    return out;
}

class Hash // клас хеш-функції
{
public:
    int a, b, m;
    vector<int> calculate(const vector<Team>& sections);
    int calculate(const string& section);
};

vector<int> Hash::calculate(const vector<Team>& teams)  // порахувати хеш для набору команд
{
    vector<int> result;
    for (const auto& c : teams)
    {
        int temp = 0;
        for (int i = 0; i < c.get_name().size(); i++)
        {
            temp += (int)c.get_name()[i];
        }
        int hash = ((temp * a + b) % 101) % m;
        result.push_back(hash);
    }
    return result;
}

int Hash::calculate(const string& team)  // порахувати один хеш
{
    int temp = 0;
    for (int i = 0; i < team.size(); i++)
    {
        temp += (int)team[i];
    }
    int hash = ((temp * a + b) % 101) % m;
    return hash;
}

class HashTable
{
private:
    Hash first_level_hash; // хеш функція першого рівня
    vector<Hash> second_level_hash; // хеш функції другого рівня
    vector<Team> teams; // команди
    vector<vector<Team>> table; // хеш-таблиця 
public:
    HashTable();
    Team search(const string& team_name);
    void print();
};

HashTable::HashTable()
{
    ifstream file("data.txt");

    string s;

    getline(file, s);

    int count = stoi(s);

    for (int i = 0; i < count; i++) // зчитуємо інформацію з файла
    {
        string team_name;
        getline(file, team_name);
        Team new_team(team_name);
        getline(file, s);
        int players_count;
        players_count = stoi(s);
        for (int i = 0; i < players_count; i++)
        {
            getline(file, s);
            new_team.add_player(s);
        }
        teams.push_back(new_team);
    }


    first_level_hash.a = 3;
    first_level_hash.b = 42;
    first_level_hash.m = teams.size();
    vector<int> first_level = first_level_hash.calculate(teams); // рахуємо хеш першого рівня
    vector<int> counts(teams.size(), 0);
    for (auto& a : first_level) // рахуємо кількість однакових хешів
    {
        counts[a]++;
    }

    second_level_hash.resize(teams.size());
    table.resize(teams.size());

    for (int i = 0; i < teams.size(); i++) // генеруємо хеші 2-го рівня і задаємо розмір хеш таблиці
    {
        table[i].resize(pow(counts[i], 2));
        second_level_hash[i].a = rand() % 10;
        second_level_hash[i].b = rand() % 10;
        second_level_hash[i].m = pow(counts[i], 2);
    }

    for (int i = 0; i < teams.size(); i++) // заповнюємо комірки хеш таблиці
    {
        table[first_level[i]][second_level_hash[first_level[i]].calculate(teams[i].get_name())] = teams[i];
    }
}

Team HashTable::search(const string& team_name) // пошук
{
    int first_hash = first_level_hash.calculate(team_name);
    return table[first_hash][second_level_hash[first_hash].calculate(team_name)];
}

void HashTable::print() // виведення хеш карти в консоль
{
    for (auto& a : table)
    {
        for (auto& b : a)
        {
            cout << b;
        }
        cout << endl;
    }
    cout << endl;
}

int main()
{
    HashTable my_table;
    string operation;
    while (true)
    {
        cin >> operation;
        if (operation == "search")
        {
            string team_name;
            cin >> team_name;
            cout << my_table.search(team_name);
        }
        else if (operation == "print")
        {
            my_table.print();
        }
        else if (operation == "exit")
        {
            return 0;
        }
        else
        {
            cout << "Wrong operation!" << endl;
        }
    }
}


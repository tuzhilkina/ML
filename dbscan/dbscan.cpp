#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <stack>
#include <algorithm>

using namespace std;

struct Dot {
    Dot() = default;
    ~Dot() = default;
    Dot(const Dot&) = default;
    Dot& operator=(const Dot&) = default;
    Dot(const double& x_, const double& y_) : x(x_), y(y_) { };
    void print() const {
        cout << "(" << x << "," << y << "), ";
    }

    double x{ 0 };
    double y{ 0 };
    vector<Dot*> neighb;
    vector<Dot*> n_cores;
    int type{ 0 };
    int cluster{ 0 };
};

class DBSCAN {
public:
    DBSCAN() = default;
    DBSCAN(const double& r, const int& m) : R(r), M(m) { };
    ~DBSCAN() = default;
    DBSCAN(const DBSCAN&) = default;
    DBSCAN& operator=(const DBSCAN&) = default;

    // Считывает данные из файла датасета
    void read_data(ifstream& input);
    void print();
    // Определяет тип точки
    void define();
    void cluster();

private:
    // Данные для кластеризации
    vector<Dot> data;
    // Ядра
    vector<Dot*> core;
    double R{ 0.25 };
    double M{ 6 };
};

// Считывает данные из файла датасета
void DBSCAN::read_data(ifstream& input) {
    if (input.is_open()) {
        while (!input.eof()) {
            string first(""), second(""), waste("");
            getline(input, first, ',');
            getline(input, second, ',');
            getline(input, waste);
            data.push_back(Dot(atof(first.c_str()), atof(second.c_str())));
        }
    }
    data.pop_back();
    sort(data.begin(),data.end(),
        [](const Dot& a, const Dot& b) {
            return a.x < b.x; });
    define();
    cluster();
    print();
}

void DBSCAN::define() {
    for (auto& dot : data) {
        for (auto& it : data) {
            double dist = pow(pow(dot.x - it.x, 2) + pow(dot.y - it.y, 2), 0.5);
            if (dist <= R) dot.neighb.push_back(&it); // находим соседей
        }
        // если это ядро
        if (dot.neighb.size() >= M) {
            dot.type = 3;
            core.push_back(&dot);
            for (auto n : dot.neighb)
                n->n_cores.push_back(&dot);
        }  
    }
}

// Нахождение компонент связности неориентированного графа ядер
void DBSCAN::cluster() {
    int ID(0); // ID кластера
    stack<Dot*> s;
    // Цикл по вершинам
    for (auto dot : core ) {
        // Если вершина помечена, рассматриваем следующую
        if (dot->cluster != 0) continue;
        ++ID; // Номер текущей компоненты связности
        auto current(dot); // Текущая вершина
        // Просмотр ребер текущей вершины, начиная с первого непросмотренного
        while (true) {
            current->cluster = ID;
            Dot* first_unknown{ nullptr };
            for (auto connected : current->n_cores) 
                if (connected->cluster == 0) {
                    first_unknown = connected;
                    break;
                }
            // Если мы нашли непомеченную вершину
            if (first_unknown != nullptr) {
                s.push(current);
                current = first_unknown;
            }
            else {
                if (s.empty()) break; // Стэк пуст и мы обошли всю компоненту
                else {
                    current = s.top();
                    s.pop();
                }
            }
        }
    }
}


void DBSCAN::print() {
    sort(data.begin(), data.end(),
        [](const Dot& a, const Dot& b) {
            return a.cluster < b.cluster; });
    bool changed(false);
    data[0].print();
    for (int i(1); i < data.size(); ++i) {
        changed = (data[i].cluster != data[i - 1].cluster);
        if (changed) cout << "\n\n\n";
        data[i].print();
    }
}

int main() {
    ifstream input("iris.data");
    DBSCAN dbscan = DBSCAN();
    dbscan.read_data(input);
}
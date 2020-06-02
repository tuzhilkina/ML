#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <cmath>

class kNN {
public:
    kNN() = default;
    kNN(size_t k_) { k = k_; };
    ~kNN() = default;
    kNN(const kNN&) = default;
    kNN& operator=(const kNN&) = default;

    // Считывает данные из файла датасета
    void readData(std::ifstream& input);
    // Генерирует n строк данных для классификации
    void generateData(const size_t& n);
    // Классифицирует данные
    void classify();
    // Выводит отклассифицированные данные
    void printResult();
    // Вывести координаты для визуализации
    void printCoord();
    

private:
    // Генерирует значение признака случайным образом в диапазоне от a до b 
    double random(const double& a, const double& b);
    // Определяет принадлежность одного элемента к виду
    void define(std::array<double, 3>&);

    // Обучающая выборка
    std::vector<std::array<double, 3>> data;
    // data[][0] - метка класса (class label) [ 0: Iris-setosa, 1: Iris-versicolor ]
    // data[][1] - 1 признак
    // data[][2] - 2 признак
    // Данные для классификации
    std::vector<std::array<double, 3>> raw;
    // Количество соседей для сравнения
    size_t k{ 5 };
};


void kNN::readData(std::ifstream& input) {
    if (input.is_open()) {
        while (!input.eof()) {
            std::string first(""), second(""), waste(""), name("");
            std::getline(input, first, ',');
            std::getline(input, second, ',');
            std::getline(input, waste, ',');
            std::getline(input, waste, ',');
            std::getline(input, name);
            std::array<double, 3> item;
            if (name == "Iris-setosa") item[0] = 0.0; else item[0] = 1.0;
            item[1] = atof(first.c_str());
            item[2] = atof(second.c_str());
            data.push_back(item);
        }
    }
    data.pop_back();
}

void kNN::generateData(const size_t& n) {
    for (ptrdiff_t i(0); i < n; ++i) {
        std::array<double, 3> item;
        item[0] = -1;
        item[1] = random(4, 16);
        item[2] = random(24, 35);
        raw.push_back(item);
    }
}

void kNN::classify() {
    for (ptrdiff_t i(0); i < raw.size(); ++i)
        define(raw[i]);
}

void kNN::printResult() {
    for (ptrdiff_t i(0); i < raw.size(); ++i) {
        for (ptrdiff_t j(1); j < 3; ++j) {
            std::cout << raw[i][j] << " ";
        }
        if (raw[i][0] == 0) std::cout << "Iris-setosa ";
        else std::cout << "Iris-versicolor ";
        std::cout << "\n";
    }
}

void kNN::printCoord() {
    setlocale(LC_ALL, "Russian");
    std::cout << "\nИсходные данные: 1 порода ирисов:\n";
    for (ptrdiff_t i(0); i < 50; ++i)
        std::cout << "(" << data[i][1] << ";" << data[i][2] << ") ";
    std::cout << "\nИсходные данные: 2 порода ирисов:\n";
    for (ptrdiff_t i(50); i < 100; ++i)
        std::cout << "(" << data[i][1] << ";" << data[i][2] << ") ";
    std::cout << "\nОбработанные данные: 1 порода ирисов\n";
    for (ptrdiff_t i(0); i < raw.size(); ++i)
        if (raw[i][0] == 0)
            std::cout << "(" << raw[i][1] << ";" << raw[i][2] << ") ";
    std::cout << "\nОбработанные данные: 2 порода ирисов\n";
    for (ptrdiff_t i(0); i < raw.size(); ++i)
        if (raw[i][0] == 1)
            std::cout << "(" << raw[i][1] << ";" << raw[i][2] << ") ";
    std::cout << "\n\n";
    for (auto it : raw)
        std::cout << "(" << it[1] << "," << it[2] << "), ";
}

double kNN::random(const double& a, const double& b) {
    int a_ = 10 * a;
    int b_ = 10 * b;
    return ((rand() % (b_ - a_ + 1) + a_) / 10.0);
}

void kNN::define(std::array<double, 3>& item) {
    std::vector<std::array<double, 2>> distance;
    for (ptrdiff_t i(0); i < data.size(); ++i) {
        std::array<double, 2> temp;
        temp[0] = data[i][0];
        temp[1] = sqrt(pow(item[1] - data[i][1], 2) + pow(item[2] - data[i][2], 2));
        distance.push_back(temp);
    }
    std::sort(
        distance.begin(),
        distance.end(),
        [](const std::array<double, 2>& a, const std::array<double, 2>& b) {
            return a[1] < b[1];
        }
    );

    int count0(0);
    int count1(0);
    for (ptrdiff_t i(0); i < k; ++i) {
        if (distance[i][0] == 0) ++count0;
        else ++count1;
    }
    item[0] = (count0 > count1 ? 0 : 1);
}


int main() {
    std::ifstream input("iris.data");
    kNN knn = kNN();
    knn.readData(input);
    knn.generateData(50);
    knn.classify();
    knn.printResult();
    knn.printCoord();
}
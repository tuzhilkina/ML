#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <cmath>

using namespace std;

class kMeans {
public:
    kMeans() = default;
    ~kMeans() = default;
    kMeans(const kMeans&) = default;
    kMeans& operator=(const kMeans&) = default;

    // ��������� ������ �� ����� ��������
    void read_data(ifstream& input);
    // ��������� ����������� ���������� ��������� � ������������ ������
    void cluster();
    // ������� ������������������ ������
    void print_result();
    // ������������ ��� �������� k
    void cluster_k(const ptrdiff_t& k);

private:
    // ����������� ���������� �� ���������� � ����������� ����� ��������
    void define();
    // ��������� ����� ���������
    bool new_centr();

    // data[][0] - ����� ��������
    // data[][1] - 1 �������
    // data[][2] - 2 �������
    // data[][3] - ���������� �� ���������
    // ������ ��� �������������
    vector<array<double, 4>> data;
    // ���������
    vector<array<double, 4>> centroids;
    // ���������� ���������
    size_t k{ 2 };
    // ����� ��������� ���������� ��� [i] ���������
    vector<double> sum;
};

// ��������� ������ �� ����� ��������
void kMeans::read_data(ifstream& input) {
    if (input.is_open()) {
        while (!input.eof()) {
            string first(""), second(""), waste("");
            getline(input, first, ',');
            getline(input, second, ',');
            getline(input, waste);
            array<double, 4> item;
            item[0] = 0.0;
            item[1] = atof(first.c_str());
            item[2] = atof(second.c_str());
            item[3] = 0.0;
            data.push_back(item);
        }
    }
    data.pop_back();
    sum.push_back(0);
}

// ��������� ����������� ���������� ��������� � ������������ ������
void kMeans::cluster() {
    for (ptrdiff_t i(1); i <= 3; ++i)
        cluster_k(i);
    ptrdiff_t i(2);
    bool flag(true);
    while (flag) {
        if ((max(sum[i - 1], sum[i + 1]) / min(sum[i - 1], sum[i + 1])) < 2) flag = false;
        else {
            ++i;
            cluster_k(i+1);
        }
    }
}

// ������������ ��� �������� k
void kMeans::cluster_k(const ptrdiff_t& k_) {
    for (ptrdiff_t j(0); j < k_; ++j)
        centroids.push_back(data[j]);
    k = k_;
    bool flag = true;
    sum.push_back(0);
    while (flag) {
        define();
        flag = new_centr();
    }
}

// ����������� ���������� �� ���������� � ����������� ����� ��������
void kMeans::define() {
    for (ptrdiff_t i(0); i < data.size(); ++i) {
        // ���������� �� ����� �� ���������� 
        vector<double> dist;
        double mindist(100);
        for (ptrdiff_t j(0); j < k; ++j) {
            dist.push_back(sqrt(pow(data[i][1] - centroids[j][1], 2) + pow(data[i][2] - centroids[j][2], 2)));
            if (dist[j] < mindist) {
                mindist = dist[j];
                data[i][0] = j;
                data[i][3] = mindist;
            }
        }
    }
    sort(
        data.begin(),
        data.end(),
        [](const array<double, 4>& a, const array<double, 4>& b) {
            return a[0] < b[0]; });
}

// ��������� ����� ���������
bool kMeans::new_centr() {
    ptrdiff_t i(0); bool flag(true); double sum_sqr(0);
    for (ptrdiff_t j(0); j < k; ++j) {
        double x(0), y(0), n(0);
        while (i != data.size() && data[i][0] == j) {
            x += data[i][1];
            y += data[i][2];
            ++n;
            ++i;
        }
        centroids[j][0] = j;
        if ((centroids[j][1] == x / n) && (centroids[j][2] == y / n)) {
            flag = false;
        }
        centroids[j][1] = x / n;
        centroids[j][2] = y / n;
    }
    for (const auto& it : data) {
        sum_sqr += it[3] * it[3];
    }
    sum[k] = sum_sqr;
    return flag;
}

// ������� ������������������ ������
void kMeans::print_result() {
    ptrdiff_t i(0);
    double x(0), y(0), n(0);
    for (ptrdiff_t j(0); j < k; ++j) {
        while (i != data.size() && data[i][0] == j) {
            cout << "(" << data[i][1] << "," << data[i][2] << "), ";
            ++i;
        }
        cout << "\n\n";
    }
    cout << "Centroids:\n";
    for (ptrdiff_t j(0); j < k; ++j) 
        cout << "(" << centroids[j][1] << "," << centroids[j][2] << "), ";
    cout << "\n\n";
}


int main() {
    ifstream input("iris.data");
    kMeans kM = kMeans();
    kM.read_data(input);
    kM.cluster();
    kM.print_result();
}
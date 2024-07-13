#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int DateToNum(const string& date);

struct DateValue {
    string originalDate;
    string value;

    DateValue() = default;
    DateValue(string date, string val) : originalDate(move(date)), value(move(val)) {}

    int getDateNumeric() const {
        return DateToNum(originalDate);
    }
};

int DateToNum(const string& date) {
    int day, month, year;
    sscanf(date.c_str(), "%d.%d.%d", &day, &month, &year);
    return year * 10000 + month * 100 + day;
}


void radixSort(vector<DateValue>& dv) {
    int maxim = 0;
    for (const auto& item : dv) {
        int numericDate = item.getDateNumeric();
        if (numericDate > maxim) maxim = numericDate;
    }
    
    int value = 10000; 
    for (int exp = 1; maxim / exp > 0; exp *= value) {
        vector<int> count(value, 0);
        
        for (const auto& item : dv) {
            int index = (item.getDateNumeric() / exp) % value;
            count[index]++;
        }
        
        for (int i = 1; i < value; i++) {
            count[i] += count[i - 1];
        }
        
        vector<DateValue> tempData(dv.size());
        
        for (int i = dv.size() - 1; i >= 0; i--) {
            int index = (dv[i].getDateNumeric() / exp) % value;
            tempData[count[index] - 1] = move(dv[i]);
            count[index]--;
        }
        dv = move(tempData);
    }
}


int main() {
    vector<DateValue> data;
    string dateString;
    string value;
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    while (cin >> dateString >> value) {
        data.emplace_back(move(dateString), move(value));
            
    }

    radixSort(data);

    
    for (const auto& elem : data) {
        cout << elem.originalDate << "\t" << elem.value << '\n';
    }

    return 0;
}

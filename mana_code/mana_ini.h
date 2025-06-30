#ifndef INI_UTIL_H
#define INI_UTIL_H
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
using namespace std;
struct indexoutD { // 用于存储double类型数据的结构
    vector<string> first_idx; // 第一维的顺序向量
    unordered_map<string, vector<string>> second_idx; // 第二维的顺序向量
    unordered_map<string, unordered_map<string, double>> data; // 存储数据
};
struct indexoutS { // 用于存储string类型数据的结构
    vector<string> first_idx; // 第一维的顺序向量
    unordered_map<string, vector<string>> second_idx; // 第二维的顺序向量
    unordered_map<string, unordered_map<string, string>> data; // 存储数据
};
string substring(const string& str, int start, int end) {
    if (start < 0 || end > str.size() || start >= end) {
        return "";
    }
    return str.substr(start, end - start);

}
string getDayName(int day) {
        switch(day) {
            case 1: return "星期一";
            case 2: return "星期二";
            case 3: return "星期三";
            case 4: return "星期四";
            case 5: return "星期五";
            case 6: return "星期六";
            case 7: return "星期日";
            default: return "未知";
        }
}
    unordered_map<string, double> readCourseGrades(const string& courseId) { // 读取课程成绩表，文件名没有前缀，输入时需要前缀
        string filename=courseId+ ".ini"; // 假设文件名为课程ID加上.ini后缀
        unordered_map<string, double> grades;
        ifstream fin(filename);
        if (!fin.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return grades;
        }
        else {
            cout << "成功打开文件: " << filename << endl;
        }
        string line;
        while (getline(fin, line)) {
            if (line.empty() || line[0] == ';' || line[0] == '[') continue;
            size_t pos = line.find('=');
            if (pos == string::npos) continue;
            string studentId = line.substr(0, pos);
            string gradeStr = line.substr(pos + 1);
            try {
                double grade = stod(gradeStr);
                grades[studentId] = grade;
            } catch (...) {
                // 非法数据，跳过
            }
        }
        fin.close();
        return grades;
    }
inline void writeIniFromMapD(const string& filename, indexoutD outdata) { //写入double
    if (filename.empty()) {
        cerr << "文件名不能为空" << endl;
        return;
    }
    ofstream fout(filename);
    if (!fout.is_open()) {
        cerr << "无法写入文件: " << filename << endl;
        return;
    }
    for (const auto& section : outdata.first_idx) {
        fout << "[" << section << "]" << endl;
        const auto& secondMap = outdata.second_idx[section];
        for (const auto& key : secondMap) {
            fout << key << "=";
            double value = outdata.data.at(section).at(key);
            fout << fixed << setprecision(2) << value; // 保留两位小数
            fout << endl;
        }
        fout << endl;
    }
    fout.close();
}
inline void writeIniFromMapS(const string& filename, indexoutS outdata) { //写入string
    if (filename.empty()) {
        cerr << "文件名不能为空" << endl;
        return;
    }
    ofstream fout(filename);
    if (!fout.is_open()) {
        cerr << "无法写入文件: " << filename << endl;
        return;
    }
    for(const auto& section : outdata.first_idx) {
        fout << "[" << section << "]" << endl;
        const auto& secondMap = outdata.second_idx[section];
        for (const auto& key : secondMap) {
            fout << key << "=";
            string value = outdata.data.at(section).at(key);
            fout << value; // 直接写入字符串
            fout << endl;
        }
        fout << endl;
    }
    fout.close();
}
#endif // INI_UTIL_H
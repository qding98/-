#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

class INIWriter {
private:
    string filename;
    map<string, map<string, string>> config;
    vector<string> sectionOrder; // 保持节的原始顺序

    // 解析INI文件内容到config结构
    void parseContent(const vector<string>& lines) {
        string currentSection = "";
        
        for (const auto& line : lines) {
            string trimmed = trim(line);
            if (trimmed.empty() || trimmed[0] == ';' || trimmed[0] == '#') {
                continue; // 跳过注释和空行
            }
            
            if (trimmed[0] == '[' && trimmed.back() == ']') {
                currentSection = trimmed.substr(1, trimmed.size() - 2);
                if (config.find(currentSection) == config.end()) {
                    sectionOrder.push_back(currentSection);
                }
            } else {
                size_t pos = trimmed.find('=');
                if (pos != string::npos) {
                    string key = trim(trimmed.substr(0, pos));
                    string value = trim(trimmed.substr(pos + 1));
                    if (!currentSection.empty() && !key.empty()) {
                        config[currentSection][key] = value;
                    }
                }
            }
        }
    }

    // 辅助函数：去除字符串两端空白
    string trim(const string& str) {
        size_t first = str.find_first_not_of(" \t");
        if (first == string::npos) return "";
        size_t last = str.find_last_not_of(" \t");
        return str.substr(first, (last - first + 1));
    }

public:
    INIWriter() {}
    
    // 设置INI文件名（用于后续操作）
    void setFileName(const string& name) {
        filename = name;
    }
    
    // 读取INI文件到内存
    bool readFile() {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Unable to open file: " << filename << endl;
            return false;
        }

        vector<string> lines;
        string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }
        file.close();

        config.clear();
        sectionOrder.clear();
        parseContent(lines);
        return true;
    }

    // 写入INI文件
    bool writeFile() {
        if (filename.empty()) {
            cout << "Filename not set" << endl;
            return false;
        }
        
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Unable to open file: " << filename << endl;
            return false;
        }

        for (const auto& section : sectionOrder) {
            if (config.find(section) != config.end()) {
                file << "[" << section << "]" << endl;
                for (const auto& kv : config[section]) {
                    file << kv.first << "=" << kv.second << endl;
                }
                file << endl; // 节之间添加空行
            }
        }

        file.close();
        cout << "File written: " << filename << endl;
        return true;
    }

    // 设置值
    void setValue(const string& section, const string& key, const string& value) {
        if (config.find(section) == config.end()) {
            sectionOrder.push_back(section); // 添加新节到顺序列表
        }
        config[section][key] = value;
    }

    // 删除整个节
    bool removeSection(const string& section) {
        auto secIt = config.find(section);
        if (secIt != config.end()) {
            config.erase(secIt);
            
            // 从顺序列表中移除
            auto pos = find(sectionOrder.begin(), sectionOrder.end(), section);
            if (pos != sectionOrder.end()) {
                sectionOrder.erase(pos);
            }
            return true;
        }
        return false;
    }

    // 删除特定键
    bool removeKey(const string& section, const string& key) {
        auto secIt = config.find(section);
        if (secIt != config.end()) {
            auto& keyMap = secIt->second;
            auto keyIt = keyMap.find(key);
            if (keyIt != keyMap.end()) {
                keyMap.erase(keyIt);
                return true;
            }
        }
        return false;
    }
};

int main() {
    INIWriter writer;
    string filename;
    cout << "Enter filename (without extension): ";
    cin >> filename;
    writer.setFileName(filename + ".ini");
    
    // 1. 先创建文件（如果不存在）
    if (!writer.writeFile()) {
        cerr << "Failed to create file!" << endl;
        return 1;
    }
    
    // 2. 读取文件
    if(!writer.readFile()) {
        cerr << "Failed to read file!" << endl;
        return 1;
    }
    
    cout << "File read successfully." << endl;
    
    // 3. 设置新值
    cin.ignore(); // 清除输入缓冲区
    
    while (true) {
        string section, key, value;
        
        cout << "\nEnter section name (or 'exit' to quit): ";
        getline(cin, section);
        if (section == "exit") break;
        
        cout << "Enter key: ";
        getline(cin, key);
        
        cout << "Enter value: ";
        getline(cin, value);
        
        writer.setValue(section, key, value);
        cout << "Set " << section << "." << key << " = " << value << endl;
        
        // 4. 立即保存更改
        if (!writer.writeFile()) {
            cerr << "Failed to save changes!" << endl;
        } else {
            cout << "Changes saved!" << endl;
        }
    }

    return 0;
}
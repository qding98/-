//声明一个班级名字和班级的映射
#ifndef GLOBAL_DATA_HH
#define GLOBAL_DATA_HH
#include <unordered_map>
#include <string>
#include"mana_Class.h"
// 声明（注意是 extern）
extern std::unordered_map<std::string, Clas> classmap;
#endif
//声明一个课程号和课程的映射，学号和学生的映射
#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H
#include <unordered_map>
#include <string>
#include"mana_Course.h"
// 声明（注意是 extern）
extern std::unordered_map<std::string, Course> coursemap;
#endif
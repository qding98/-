#ifndef MANA_BASE_COURSE
#define MANA_BASE_COURSE
#include<unordered_map>
#include <string>
#include<unordered_set>
#include<iostream>
using namespace std;
class Course {
    private:
    string courseId;
    string courseName;
    bool isElective;
    int dday,st,ed,val;
    unordered_set<string> mandatoryForClasses; // 必修此课的班级
    unordered_set<string> selectstu; //修这个课的学生
    string teacher;
public:
    Course() : 
        courseId(""), courseName(""), isElective(0), dday(0) ,st(0),ed(0),val(0),mandatoryForClasses({""}),selectstu({""}),teacher(""){}
    Course(string id, string name, bool isElective,int dayy,int stt,int edd,int val)
        : courseId(id), courseName(name), isElective(isElective),dday(dayy),st(stt),ed(edd),val(val) {}
    // 设置课程为某个班级的必修课
    void setMandatoryForClass(const string& classnumber) {
        if (mandatoryForClasses.count(classnumber)) {
            cout << "This class is already mandatory for this course." << endl;
            return;
        }
        mandatoryForClasses.insert(classnumber);
    }
    void eraseMandatoryForClass(const string& classnumber) {
        if (!mandatoryForClasses.count(classnumber)) {
            cout << "This class is not mandatory for this course." << endl;
            return;
        }
        mandatoryForClasses.erase(classnumber);
    }
    // 设置课程时间表
    void setSchedule(int day, int start, int end) {
        dday = day;
        st = start;
        ed = end;
    }
    //改变课程的选修状态
    void setElective(bool elective) {
        isElective = elective;
    }
    void stu_select(const string& stu_number){
        selectstu.insert(stu_number);
    }
    // 检查是否为某班级的必修课
    bool isMandatoryForClass(const string& classnumber) const {
        return mandatoryForClasses.count(classnumber);
    }
    // 分配授课教师
    void assign_teach(const string &teach){
        teacher=teach;
    }
    string getCourseId() const { return courseId; }
    string getCourseName() const { return courseName; }
    bool getIsElective() const { return isElective; }
    string getAssignedTeacher() const { return teacher; }
    int getst() const{ return st; }
    int geted() const { return ed; }
    int getval() const { return val; }
    int getday() const { return dday; }
    unordered_set<string> outstu() const {
        return selectstu;
    }
    unordered_set<string> outmanclass() const{
        return mandatoryForClasses;
    }
}; 

#endif // MANA_BASE_COURSE

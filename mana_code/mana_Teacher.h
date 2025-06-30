#ifndef MANA_TEACHER
#define MANA_TEACHER
#include<iostream>
#include<unordered_set>
#include<utility>
#include<map>
#include"global_data.h"
#include"global_data2.h"
#include"mana_Student.h"
#include"mana_ini.h"
using namespace std;

class Teacher : public Person {
private:
    string teacherId;
    unordered_set<string> assignedCourses;        // 负责的课程
    bool chongtu;
public:
    Teacher() : Person("", ""), teacherId(""), chongtu(false) {}
    // 构造函数
    Teacher(string id, string name, 
            string gender)
        : Person(name, gender), teacherId(id) {}
    // 获取教师ID
    string getTeacherId() const { return teacherId; }
    // 课程管理
    void assignCourse(const string& course){
        if(!assignedCourses.count(course)){
            assignedCourses.insert(course);
            coursemap[course].assign_teach(teacherId);
        }
        else {
            cout << "This course is already assigned to this teacher." << endl;
        }
    }
    void removeCourse(const string& course){
        if(assignedCourses.count(course)) {
            assignedCourses.erase(course);
            coursemap[course].assign_teach("");
        }
        else {
            cout << "This course is not assigned to this teacher." << endl;
        }
    }
    //在读入班级的成绩表时采用文件读入的方法
    void teacherreadCourseGrades(const string& courseId) {
        unordered_map<string, double> grades = readCourseGrades(courseId);
        if (grades.empty()) {
            cout << "没有找到课程 " << courseId << " 的成绩数据。" << endl;
            return;
        }
        setGradeForCourses(courseId, grades);
    }
    // 获取已分配的课程
    unordered_set<string> getAssignedCourses() const {
        return assignedCourses;
    }
    // 设置单个学生的成绩
    void setGradeForStudent(const string& course, const string& studentId, double grade) {
        if (assignedCourses.count(course) == 0) {
            cout << "该课程未分配给此教师，无法设置成绩。" << endl;
            return;
        }
        if (studentmap.find(studentId) == studentmap.end()) {
            cout << "学生不存在，无法设置成绩。" << endl;
            return;
        }
        Course coursenow = coursemap[course];
        Student& student = studentmap[studentId];
        student.setGrade(course, grade);
        studentmap[studentId] = student; // 更新学生信息
    }
    bool checkScheduleConflict() { //判断课表是否冲突
        bool busy[8][12];
        for(int i=1;i<=7;i++)
            for(int j=1;j<=11;j++)
                busy[i][j]=0;
        for(const string& item : assignedCourses){
            Course coursenow=coursemap[item];
            int st=coursenow.getst(),ed=coursenow.geted(),daynow=coursenow.getday();
            if(st<=11&&st>=1&&ed<=11&&ed>=1&&daynow>=1&&daynow<=11)
                for(int i=st;i<=ed;i++){
                    if(busy[daynow][i]) return 1;
                    else busy[daynow][i]=1;
                }
        }
        return 0;
    }
    // 数字与每周的对应关系
    // 设置成绩
    void setGradeForCourses(const string& course, const unordered_map<string, double>& grades){
        Course coursenow=coursemap[course];
        unordered_set<string> stu2=coursenow.outstu(); // 获取选修该课程的学生
        unordered_set<string> man2=coursenow.outmanclass(); // 获取必修该课程的班级
        // 更新选修课程的学生成绩
        for(const string & item : stu2){
            if(grades.count(item)) {
                studentmap[item].setGrade(course, grades.at(item));
                cout<<item<<" "<<coursenow.getCourseName()<<" 成绩已更新为 "<<grades.at(item)<<endl;
            }
        }
        // 更新必修课程的班级成绩
        for(const string & item : man2){
            unordered_set<string> stunow=classmap[item].getStudents(); // 获取该班级的学生
            for(const string & stu : stunow){
                if(grades.count(stu)) {
                    studentmap[stu].setGrade(course, grades.at(stu));
                    cout<<stu<<" "<<coursenow.getCourseName()<<" 成绩已更新为 "<<grades.at(stu)<<endl;
                }
            }
        }
        coursemap[course]= coursenow; // 更新课程信息
    }
    // 按照班级输出课程成绩
    void printCourseGradesByClass(const string& classId) {
        if (classmap.find(classId) == classmap.end()) {
            cout << "班级不存在，无法输出成绩。" << endl;
            return;
        }
        Clas& clas = classmap[classId];
        indexoutD grade_data = clas.get_all_cou_grade(teacherId, false); // 获取班级的课程成绩
        if (grade_data.data.empty()) {
            cout << "该班级没有课程成绩数据。" << endl;
            return;
        }
        writeIniFromMapD(classId+"teached_by_" + teacherId + "_grades.ini", grade_data); // 将成绩写入文件
        cout << "班级 " << classId << " 的课程成绩已写入文件" << classId+"teached_by_" + teacherId + "_grades.ini" << endl;
    }
    void getidnamegender(){
        cout<<"工号: " << teacherId << ", 姓名: " << getName() << ", 性别: " << getGender() << endl;
    }
    indexoutS getTimetable(){
        unordered_map<int,unordered_map<int,string>> check_time;
        unordered_map<string,unordered_map<string,string>> res1; //存储课表
        unordered_map<string,vector<string>> res2; //第二维的顺序向量
        vector<string> res; //存储星期的顺序(第一维顺序向量)
        indexoutS ans; // 返回的课表结构
        for(int i=1;i<=7;i++)
            for(int j=1;j<=11;j++)
                check_time[i][j]="NULL";
        for(const string& item : assignedCourses){
            Course coursenow=coursemap[item];
            int st=coursenow.getst(),ed=coursenow.geted(),daynow=coursenow.getday();
            if(st>11 || st<1 || ed>11 || ed<1 || daynow<1 || daynow>7) {
                cout << "课程时间设置错误，请检查课程 " << coursenow.getCourseName() << " 的时间设置。" << endl;
                continue;
            }
            for(int i=st;i<=ed;i++){
                if(check_time[daynow][i] != "NULL") {
                    check_time[daynow][i] == " 冲突 ";
                    chongtu = true;
                }
                else check_time[daynow][i]= coursenow.getCourseName();
            }
        }
        for(int i=1;i<=7;i++){
            string daynow= getDayName(i);
            vector<string> temp;
            for(int j=1;j<=11;j++){
                string timenow="第"+to_string(j)+"节";
                temp.push_back(timenow);
                if(check_time[i][j] == "NULL") {
                    res1[daynow][timenow]=" 无课 ";
                } else if(check_time[i][j] == " 冲突 ") {
                    res1[daynow][timenow]=" 冲突 ";
                } else {
                    res1[daynow][timenow]=check_time[i][j];
                }
            }
            res2[daynow]=temp;
            res.push_back(daynow);
        }
        ans.first_idx=res;
        ans.second_idx=res2;
        ans.data=res1;
        return ans;
    }
    //输出姓名
    string getName() const {
        return Person::getName();
    }
    //打印课表
    void printTimetable() {
        indexoutS timetable = getTimetable();
        string filename;
        filename= substring(teacherId, 1, teacherId.size()) +getName()+ ".ini"; // 假设文件名为教师ID加上.ini后缀
        writeIniFromMapS(filename, timetable);
        cout << "教师 " << getName() << " 的课表已写入 " << filename << " 文件。" << endl;
        chongtu=checkScheduleConflict();
        if(chongtu) cout<<"有冲突的课程，请检查！"<<endl;
    }
    // 按照班级获得课程成绩
};
#endif
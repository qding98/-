#ifndef MANA_CLASS
#define MANA_CLASS
#include"global_data1.h"
#include"mana_ini.h"
#include<string>
#include<utility>
#include<vector>
#include<unordered_set>
#include<algorithm>
class Clas {
private:
    string className;            // 班级名称
    unordered_set<string> students;   // 班级学生列表
    unordered_set<string> mancou; // 必修课程列表
public:
    Clas() : className(""),students({""}),mancou({""}) {}
    Clas(string name) : className(name) {}
    //输出学生
    unordered_set<string> getStudents() const {
        return students;
    }
    // 添加/删除学生
    void addStudent(const string& student){
        //边界条件
        if(studentmap.find(student) == studentmap.end()) {
            cout << "Student not found." << endl;
            return;
        }
        if(!students.count(student)){
            students.insert(student);
            //更新学生信息
            Student stu=studentmap[student];
            stu.setClassName(className);
            //更新学生必修课
            for(const string& item : mancou){
                stu.setmancourse(item); // 设置必修课
            }
            studentmap[student]=stu; // 更新学生信息
        }
    }
    void removeStudent(const string& student){
        //边界条件
        if(studentmap.find(student) == studentmap.end()) {
            cout << "Student not found." << endl;
            return;
        }
        if(students.count(student))
            students.erase(student);
        //更新学生信息
        Student stu=studentmap[student];
        stu.setClassName(""); // 清空班级信息
        //更新学生必修课
        for(const string& item : mancou){
            stu.dropmancourse(item); // 删除必修课
        }
        studentmap[student]=stu; // 更新学生信息
    }
    void addMandatoryCourse(const string& course){ // 添加必修课程
        if(coursemap.find(course) == coursemap.end()) {
            cout << "Course not found." << endl;
            return;
        }
        if(!mancou.count(course)){
            mancou.insert(course);
            for(const string& item : students){
                Student stu=studentmap[item];
                stu.setmancourse(course);
                studentmap[item]=stu; // 更新学生信息
            }
            //更新课程信息
            coursemap[course].setMandatoryForClass(className);
        }
        else {
            cout << "This course is already mandatory for this class." << endl;
        }
    }
    void removeMandatoryCourse(const string& course){ // 删除必修课程
        if(coursemap.find(course) == coursemap.end()) {
            cout << "Course not found." << endl;
            return;
        }
        if(mancou.count(course))
            mancou.erase(course);
        for(const string& item : students){
            Student stu=studentmap[item];
            stu.dropmancourse(course);
            studentmap[item]=stu; // 更新学生信息
        }
        //更新课程信息
        coursemap[course].outmanclass().erase(className);
    }
    // 获取信息
    string getClasName() const { return className; }
    // 计算班级平均分
    double calAveragegpa() const{
        double tot=0,num=0;
        for(const string& item : students){
            Student stu=studentmap[item];
            num+=1;
            tot+=stu.outGPA();
        }
        return tot/num;
    }
    //按照GPA对学生进行排序
    vector<pair<string,double>> sortstu() const{
        vector<pair<string,double>> stulist;
        for(const string& item : students){
            pair<string,double> stunow;
            Student stu=studentmap[item];
            stunow.first=stu.getStudentId();
            stunow.second=stu.outGPA();
            stulist.push_back(stunow);
        }
        sort(stulist.begin(), stulist.end(), [](const pair<string, double>& a, const pair<string, int>& b) {
            return a.second < b.second;
        });
        return stulist;
    }
    //输出学和班级的端口
    unordered_set<string> getstu() const {
        return students;
    }
    void outstu(){
        for(const string& item : students){
            cout << item << " ";
        }
        cout << endl;
    }
    unordered_set<string> getcou() const {
        return mancou; 
    }
    //得到所有老师教的课程成绩
    indexoutD get_all_cou_grade(const string &teacherId,bool ifall) const {
        unordered_map<string, unordered_map<string, double>> grade_data;
        unordered_map<string,vector<string>> second_idx;
        vector<string> first_idx;
        indexoutD allGrades;
        for(const string& coursename : mancou) {
            Course course = coursemap[coursename];
            if (ifall || course.getAssignedTeacher() == teacherId) {
                for (const string& studentId : students) {
                    double grade = studentmap[studentId].courseGrades[coursename];
                        grade_data[coursename][studentId] = grade;
                        second_idx[coursename].push_back(studentId);
                }
                first_idx.push_back(coursename);
            }
        }
        allGrades.first_idx = first_idx;
        allGrades.second_idx = second_idx;
        allGrades.data = grade_data;
        return allGrades;
    }
    // 得到所有课程成绩(包含选修课程)
    indexoutD inquiremanselcougrade(){
        unordered_map<string, unordered_map<string, double>> grade_data;
        unordered_map<string,vector<string>> second_idx;
        vector<string> first_idx;
        indexoutD allGrades;
        for(const string & studentid : students) {
            Student stu = studentmap[studentid];
            for(const string& coursename : stu.mancourses) {
                double grade = stu.courseGrades[coursename];
                grade_data[coursename][studentid] = grade;
                second_idx[coursename].push_back(studentid);
                if (find(first_idx.begin(), first_idx.end(), coursename) == first_idx.end()) {
                    first_idx.push_back(coursename);
                }
            }
        }
        allGrades.first_idx = first_idx;
        allGrades.second_idx = second_idx;
        allGrades.data = grade_data;
        return allGrades;
    }
};
#endif
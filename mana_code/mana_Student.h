#ifndef mana_Student
#define mana_Student
#include<unordered_map>
#include<string>
#include<map>
#include<iostream>
#include<iomanip>
#include"mana_Course.h"
#include"mana_Person.h"
#include"global_data.h"
#include"mana_ini.h"
#include<unordered_set>
using namespace std;
class Student : public Person {
protected:
    string studentId;
    string className;        // 已选选修课课程 
    double gpa;
public:
    Student() : Person("", ""), studentId(""), className(""), gpa(0) {}
    Student(string id, string name, 
            string gender, string className)
        : Person(name, gender), studentId(id), className(className) {}
    unordered_set<string> selectedCourses;
    unordered_set<string> mancourses;
    map<string, double> courseGrades;   
    //设置班级
    void setClassName(const string& className) {
        this->className = className;
    }
    void selectCourse(const string& coursenumber){ //选课只能选选修课
        if(coursemap.find(coursenumber) == coursemap.end()) {
            cout << "课程不存在，无法选课。" << endl;
            return;
        }
        if(!coursemap[coursenumber].getIsElective()) {
            cout << "只能选修选修课，无法选必修课。" << endl;
            return;
        }
        if(!selectedCourses.count(coursenumber)){
            selectedCourses.insert(coursenumber);
            coursemap[coursenumber].stu_select(studentId); // 将学生添加到课程的选修学生列表中
            cout<< "选课成功！" << endl;
            if(courseGrades.find(coursenumber) == courseGrades.end()) {
                courseGrades[coursenumber] = 0; // 初始化成绩为0
            }
        } 
    }
    void dropCourse(const string& course){ //退课
        if(coursemap.find(course) == coursemap.end()) {
            cout << "课程不存在，无法退课。" << endl;
            return;
        }
        if(!coursemap[course].getIsElective()) {
            cout << "只能退选修课，无法退必修课。" << endl;
            return;
        }
       if(selectedCourses.count(course)){
            selectedCourses.erase(course);
            coursemap[course].outstu().erase(studentId); // 从课程的选修学生列表中移除
            courseGrades.erase(course); // 删除成绩记录
            cout<< "退课成功！" << endl;
       } 
    }
    void setmancourse(const string& course){ //设置必修课
        if(coursemap.find(course) == coursemap.end()) {
            cout << "课程不存在，无法设置为必修课。" << endl;
            return;
        }
        if(!mancourses.count(course)){
            mancourses.insert(course);
            if(courseGrades.find(course) == courseGrades.end()) {
                courseGrades[course] = 0; // 初始化成绩为0
            }
        }
    }
    void dropmancourse(const string& course){ //删除必修课
        if(coursemap.find(course) == coursemap.end()) {
            cout << "课程不存在，无法删除必修课。" << endl;
            return;
        }
        if(mancourses.count(course)) {
            mancourses.erase(course);
            courseGrades.erase(course); // 删除成绩记录
        }
    }
    //设置成绩
    void setGrade(const string& course, double grade){
        if(mancourses.find(course) == mancourses.end() && selectedCourses.find(course) == selectedCourses.end()) {
            cout << "课程不存在于该学生的课程表，无法设置成绩。" << endl;
            return;
        }
        courseGrades[course]=grade;
        cout<< "成绩已设置为 " << grade << endl;
    }
    // 计算GPA
    void getGPA(){
        double totnum=0,totval=0;
        for(const string& item : mancourses){
            totnum+=courseGrades[item]*coursemap[item].getval();
            totval+=coursemap[item].getval();
        }
        if(totval==0) {
            cout << "没有成绩记录，无法计算 GPA" << endl;
            return;
        }
        gpa=totnum/(totval*25);
    }
    double outGPA(){
        getGPA(); // 确保 GPA 已经计算
        return gpa;
    }
    string getStudentId() const { return studentId; }
    string getClassName() const { return className; }
    void print_grade(){
        double totnum=0,totval=0;
        for(const string& item : mancourses){
            Course coursenow = coursemap[item];
            cout<<item<<"(必修课)分数："<<courseGrades[item]<<endl;
            totnum+=courseGrades[item]*coursenow.getval();
            totval+=coursenow.getval();
        }
        for(const string& item : selectedCourses){
            Course coursenow = coursemap[item];
            cout<<item<<"(选修课)分数："<<courseGrades[item]<<endl;
            totnum+=courseGrades[item]*coursenow.getval();
            totval+=coursenow.getval();
        }
        if(totval==0) {
            cout << "没有成绩记录，无法计算 GPA" << endl;
            return;
        }
        gpa=totnum/(totval*25);
        cout<<"你的GPA为"<<fixed << setprecision(2)<< gpa<<"/4.00"<<endl;
    }
    //打印课程表
    indexoutS getTimetable() {
        indexoutS ans;
        unordered_map<int, unordered_map<int, string>> check_time;
        unordered_map<string, unordered_map<string, string>> res1; //存储课表
        unordered_map<string, vector<string>> res2; //第二维的顺序向量
        vector<string> res; //存储星期的顺序(第一维顺序向量)
        for (int i = 1; i <= 7; i++)
            for (int j = 1; j <= 11; j++)
                check_time[i][j] = "NULL";
        for (const string& item : selectedCourses) {
            Course coursenow = coursemap[item];
            int daynow = coursenow.getday();
            int stnow = coursenow.getst();
            int ednow = coursenow.geted();
            for (int i = stnow; i <= ednow; i++) {
                if (daynow < 1 || daynow > 7 || i < 1 || i > 11) {
                    cout << "课程时间设置错误，无法添加到课表。" << endl;
                    return ans;
                }
                if (check_time[daynow][i] != "NULL") {
                    check_time[daynow][i]= "冲突";
                }
                else check_time[daynow][i] = item+"(选修课)",cout<<daynow<<" "<<i<<" "<<item+"(选修课)"<<endl;
            }
        }
        for(const string& item : mancourses) {
            Course coursenow = coursemap[item];
            int daynow = coursenow.getday();
            int stnow = coursenow.getst();
            int ednow = coursenow.geted();
            for (int i = stnow; i <= ednow; i++) {
                if (daynow < 1 || daynow > 7 || i < 1 || i > 11) {
                    cout << "课程时间设置错误，无法添加到课表。" << endl;
                    return ans;
                }
                if (check_time[daynow][i] != "NULL") {
                    check_time[daynow][i]= "冲突";
                }
                else check_time[daynow][i] = item+"(必修课)",cout<<daynow<<" "<<i<<" "<<item+"(必修课)"<<endl;
            }
        }
        for (int i = 1; i <= 7; i++) {
            string daynow = getDayName(i);
            vector<string> temp;
            for (int j = 1; j <= 11; j++) {
                string timenow = "第" + to_string(j) + "节";
                temp.push_back(timenow);
                if (check_time[i][j] == "NULL") {
                    res1[daynow][timenow] = "无课";
                } else if (check_time[i][j] == "冲突") {
                    res1[daynow][timenow] = "冲突";
                } else {
                    res1[daynow][timenow] = check_time[i][j];
                }
            }
            res2[daynow] = temp;
            res.push_back(daynow);
        }
        ans.first_idx = res;
        ans.second_idx = res2;
        ans.data = res1;
        return ans;
    }
    // 打印课表
    void printTimetable() {
        indexoutS timetable = getTimetable();
        string filename= substring(studentId, 1, studentId.size()) +Person::getName()+ ".ini"; // 假设文件名为学号加上.ini后缀
        writeIniFromMapS(filename, timetable);
        cout << "学生 " << getName() << " 的课表已写入 " << filename << " 文件。" << endl;
    }
    // 输出必修课
    void printMandatoryCourses() const {
        cout << "必修课列表：" << endl;
        for (const string& course : mancourses) {
            cout << course << endl;
        }
    }
    // 修改信息
    void modifyInfo(const string &newid, const string &newname, const string &newgender, const string &newclass) {
        if (newid.empty() || newname.empty() || newgender.empty() || newclass.empty()) {
            cout << "信息不能为空，请重新输入。" << endl;
            return;
        }
        for(const string& item : selectedCourses) { // 更新选修课程的学号
            coursemap[item].outstu().erase(studentId); // 从课程的选修学生列表中移除
            coursemap[item].stu_select(newid); // 将新学号添加到课程的选修学生列表中
        }
        studentId = newid;
        setName(newname);
        setGender(newgender);
        setClassName(newclass);
    }
};
#endif
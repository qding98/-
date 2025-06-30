#ifndef MANA_ADMINISTRATION
#define MANA_ADMINISTRATION
#include"global_data3.h"
#include"mana_ini.h"
class Administrator : public Person {
private:
public:
    Administrator() : Person("", "") {}
    // 构造函数
    Administrator(string name, string gender)
        : Person(name, gender) {}
    // 人员管理
    void addStudent(Student* student){
        string stuname=student->getStudentId();
        if (studentmap.find(stuname) != studentmap.end()) {
            cout << "Student already exists." << endl;
            return;
        }
        if (classmap.find(student->getClassName()) == classmap.end()) {
            cout << "Class not found." << endl;
            return;
        }
        studentmap[stuname] = *student; // 添加学生到学生映射
        classmap[student->getClassName()].addStudent(stuname); // 将学生添加到班级
        // 初始化学生必修课成绩为0
        for (const auto& course : classmap[student->getClassName()].getcou()) {
            student->setmancourse(course); // 设置必修课
            student->setGrade(course, 0); // 设置必修课成绩为0
        }
        studentmap[stuname] = *student; // 添加学生到学生映射
    }
    void removeStudent(const string& student){
        if (studentmap.find(student) == studentmap.end()) {
            cout << "Student not found." << endl;
            return;
        }
        classmap[studentmap[student].getClassName()].removeStudent(student); // 从班级中移除学生
        studentmap.erase(student); // 从学生映射中删除学生
        cout << "Student " << student << " has been removed." << endl;
    }
    // 修改学生信息
    void modifyStudent(const string& studentId, const string& newName, const string& newGender, const string& newClassName) {
        if (studentmap.find(studentId) == studentmap.end()) {
            cout << "Student not found." << endl;
            return;
        }
        Student& student = studentmap[studentId];
        student.setName(newName);
        student.setGender(newGender);
        studentmap[studentId] = student; // 更新学生信息
        if (classmap.find(newClassName) == classmap.end()) {
            cout << "Class not found." << endl;
            return;
        }
        // 如果班级发生变化，先从原班级中移除学生
        if (student.getClassName() != newClassName) {
            classmap[student.getClassName()].removeStudent(studentId);
            classmap[newClassName].addStudent(studentId); // 将学生添加到新班级
        }
        cout<< "Student " << studentId << " has been modified." << endl;
    }
    // 修改教师信息
    void modifyTeacher(const string& teacherId, const string& newName, const string& newGender) {
        if (teachermap.find(teacherId) == teachermap.end()) {
            cout << "Teacher not found." << endl;
            return;
        }
        Teacher& teacher = teachermap[teacherId];
        teacher.setName(newName);
        teacher.setGender(newGender);
        teachermap[teacherId] = teacher; // 更新教师信息
        cout << "Teacher " << teacherId << " has been modified." << endl;
    }
    void addTeacher(Teacher* teacher){
        if (teachermap.find(teacher->getTeacherId()) != teachermap.end()) {
            cout << "Teacher already exists." << endl;
            return;
        }
        string teachername=teacher->getTeacherId();
        teachermap[teachername]=*teacher;
    }
    void removeTeacher(const string& teacher){
        if (teachermap.find(teacher) == teachermap.end()) {
            cout << "Teacher not found." << endl;
            return;
        }
        for (const auto& course : teachermap[teacher].getAssignedCourses()) {
            coursemap[course].assign_teach(""); // 清除课程的授课教师
        }
        teachermap.erase(teacher);
        cout<< "Teacher " << teacher << " has been removed." << endl;
    }
    void addCourse(Course* course){
        if(coursemap.find(course->getCourseId()) != coursemap.end()) {
            cout << "Course already exists." << endl;
            return;
        }
        string coursename=course->getCourseId();
        coursemap[coursename]=*course;
    }
    void setCourseMandatory(const string &course, const string& className){
        if (coursemap.find(course) != coursemap.end()&& classmap.find(className) != classmap.end()) {
            classmap[className].addMandatoryCourse(course);

        } else {
            cout << "Course or Class not found." << endl;
        }
    }
    void assignCourseToTeacher(const string& course,const string& teacher){
        if (coursemap.find(course) != coursemap.end() && teachermap.find(teacher) != teachermap.end()) {
            teachermap[teacher].assignCourse(course);
        } else {
            cout << "Course or Teacher not found." << endl;
        }
    }
    
    // 排课功能
    void scheduleCourseTime(const string &course, int date,int st,int ed){
        if (coursemap.find(course) != coursemap.end()) {
            coursemap[course].setSchedule(date, st, ed);
        } else {
            cout << "Course not found." << endl;
        }
    }
    // 成绩管理
    void printClassGrades(const string& className){
        if (classmap.find(className) != classmap.end()) {
            string filename=substring(className,1, className.size());
            filename += ".ini";
            writeIniFromMapD(filename, classmap[className].get_all_cou_grade("all",1));
            cout << "Class grades for " << className << " have been written to " << filename << "." << endl;
        } else {
            cout << "Class not found." << endl;
        }
    }
    void sortClassesByAverage() {
        vector<pair<string, double>> classAverages;
        for (const auto& entry : classmap) {
            const string& className = entry.first;
            const Clas& clas = entry.second;
            double averageGPA = clas.calAveragegpa();
            classAverages.emplace_back(className, averageGPA);
        }
        sort(classAverages.begin(), classAverages.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
            return a.second > b.second;
        });
        cout << "Classes sorted by average GPA:" << endl;
        for (const auto& item : classAverages) {
            cout << item.first << ": " << fixed << setprecision(2) << item.second <<"/4.00"<< endl;
        }
    }
    //增加班级
    void addClass(const string& className) {
        if (classmap.find(className) == classmap.end()) {
            classmap[className] = Clas(className);
        } else {
            cout << "Class " << className << " already exists." << endl;
        }
    }
    //查询班级所有成绩，包含选修课
    void inquireClassAllGrades(const string& className) {
        if (classmap.find(className) != classmap.end()) {
            indexoutD allGrades = classmap[className].inquiremanselcougrade();
            writeIniFromMapD(className + "_all_grades.ini", allGrades);
            cout << "班级 " << className << " 的所有成绩已写入文件 " << className + "_all_grades.ini" << endl;
        } else {
            cout << "班级不存在，请重新输入。" << endl;
        }
    }
};
#endif
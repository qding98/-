#ifndef DB_STORAGE_H
#define DB_STORAGE_H

#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <iostream>
#include "global_data3.h"
using namespace std;

// --- 学生 ---
inline void saveStudents(const string& filename, const unordered_map<string, Student>& Studentmap) {
    ofstream fout(filename);
    for (auto it = Studentmap.begin(); it != Studentmap.end(); ++it) {
        const string& id = it->first;
        const Student& stu = it->second;
        fout << "[Student]" << endl;
        fout << "id=" << id << endl;
        fout << "name=" << stu.getName() << endl;
        fout << "gender=" << stu.getGender() << endl;
        fout << "class=" << stu.getClassName() << endl;
        // 课程成绩
        for (auto cg_it = stu.courseGrades.begin(); cg_it != stu.courseGrades.end(); ++cg_it) {
            const string& cid = cg_it->first;
            double grade = cg_it->second;
            fout << "grade_" << cid << "=" << fixed << setprecision(2) << grade << endl;
        }
        // 必修课
        for (const auto& c : stu.mancourses) {
            fout << "mancourse=" << c << endl;
        }
        // 选修课
        for (const auto& c : stu.selectedCourses) {
            fout << "selectedcourse=" << c << endl;
        }
        fout << endl;
    }
    fout.close();
}

inline void loadStudents(const string& filename, unordered_map<string, Student>& Studentmap) {
    ifstream fin(filename);
    if (!fin.is_open()) return;
    string line, id, name, gender, classname;
    Student stu;
    while (getline(fin, line)) {
        if (line == "[Student]") {
            if (!id.empty()) Studentmap[id] = stu;
            stu = Student();
            id = name = gender = classname = "";
            stu.selectedCourses.clear();
            stu.mancourses.clear();
            stu.courseGrades.clear();
        } else if (line.find("id=") == 0) {
            id = line.substr(3);
            stu = Student(id, "", "", "");
        } else if (line.find("name=") == 0) {
            stu.setName(line.substr(5));
        } else if (line.find("gender=") == 0) {
            stu.setGender(line.substr(7));
        } else if (line.find("class=") == 0) {
            stu.setClassName(line.substr(6));
        } else if (line.find("grade_") == 0) {
            size_t eq = line.find('=');
            string cid = line.substr(6, eq-6);
            double grade = stod(line.substr(eq+1));
            stu.courseGrades[cid] = grade;
        } else if (line.find("mancourse=") == 0) {
            stu.mancourses.insert(line.substr(10));
        } else if (line.find("selectedcourse=") == 0) {
            stu.selectedCourses.insert(line.substr(15));
        }
    }
    if (!id.empty()) Studentmap[id] = stu;
    fin.close();
}

// --- 教师 ---
inline void saveTeachers(const string& filename, const unordered_map<string, Teacher>& Teachermap) {
    ofstream fout(filename);
    for (auto it = Teachermap.begin(); it != Teachermap.end(); ++it) {
        const string& id = it->first;
        const Teacher& t = it->second;
        fout << "[Teacher]" << endl;
        fout << "id=" << id << endl;
        fout << "name=" << t.getName() << endl;
        fout << "gender=" << t.getGender() << endl;
        for (const auto& c : t.getAssignedCourses()) {
            fout << "course=" << c << endl;
        }
        fout << endl;
    }
    fout.close();
}

inline void loadTeachers(const string& filename, unordered_map<string, Teacher>& Teachermap) {
    ifstream fin(filename);
    if (!fin.is_open()) return;
    string line, id, name, gender;
    unordered_set<string> courses;
    while (getline(fin, line)) {
        if (line == "[Teacher]") {
            if (!id.empty()) {
                Teacher t(id, name, gender);
                for (const auto& c : courses) t.assignCourse(c);
                Teachermap[id] = t;
            }
            id = name = gender = "";
            courses.clear();
        } else if (line.find("id=") == 0) {
            id = line.substr(3);
        } else if (line.find("name=") == 0) {
            name = line.substr(5);
        } else if (line.find("gender=") == 0) {
            gender = line.substr(7);
        } else if (line.find("course=") == 0) {
            courses.insert(line.substr(7));
        }
    }
    if (!id.empty()) {
        Teacher t(id, name, gender);
        for (const auto& c : courses) t.assignCourse(c);
        Teachermap[id] = t;
    }
    fin.close();
}

// --- 课程 ---
inline void saveCourses(const string& filename, const unordered_map<string, Course>& Coursemap) {
    ofstream fout(filename);
    for (unordered_map<string, Course>::const_iterator it = Coursemap.begin(); it != Coursemap.end(); ++it) {
        const string& id = it->first;
        const Course& c = it->second;
        fout << "[Course]" << endl;
        fout << "id=" << id << endl;
        fout << "name=" << c.getCourseName() << endl;
        fout << "isElective=" << c.getIsElective() << endl;
        fout << "day=" << c.getday() << endl;
        fout << "st=" << c.getst() << endl;
        fout << "ed=" << c.geted() << endl;
        fout << "val=" << c.getval() << endl;
        fout << "teacher=" << c.getAssignedTeacher() << endl;
        for (const auto& cls : c.outmanclass()) {
            fout << "mandatoryForClass=" << cls << endl;
        }
        for (const auto& stu : c.outstu()) {
            fout << "selectstu=" << stu << endl;
        }
        fout << endl;
    }
    fout.close();
}

inline void loadCourses(const string& filename, unordered_map<string, Course>& Coursemap) {
    ifstream fin(filename);
    if (!fin.is_open()) return;
    string line, id, name, teacher;
    bool isElective = false;
    int day = 0, st = 0, ed = 0, val = 0;
    unordered_set<string> manclass, selectstu;
    while (getline(fin, line)) {
        if (line == "[Course]") {
            if (!id.empty()) {
                Course c(id, name, isElective, day, st, ed, val);
                for (const auto& cls : manclass) c.setMandatoryForClass(cls);
                for (const auto& stu : selectstu) c.stu_select(stu);
                c.assign_teach(teacher);
                Coursemap[id] = c;
            }
            id = name = teacher = "";
            isElective = false; day = st = ed = val = 0;
            manclass.clear(); selectstu.clear();
        } else if (line.find("id=") == 0) {
            id = line.substr(3);
        } else if (line.find("name=") == 0) {
            name = line.substr(5);
        } else if (line.find("isElective=") == 0) {
            isElective = (line.substr(11) == "1" || line.substr(11) == "true");
        } else if (line.find("day=") == 0) {
            day = stoi(line.substr(4));
        } else if (line.find("st=") == 0) {
            st = stoi(line.substr(3));
        } else if (line.find("ed=") == 0) {
            ed = stoi(line.substr(3));
        } else if (line.find("val=") == 0) {
            val = stoi(line.substr(4));
        } else if (line.find("teacher=") == 0) {
            teacher = line.substr(8);
        } else if (line.find("mandatoryForClass=") == 0) {
            manclass.insert(line.substr(17));
        } else if (line.find("selectstu=") == 0) {
            selectstu.insert(line.substr(10));
        }
    }
    if (!id.empty()) {
        Course c(id, name, isElective, day, st, ed, val);
        for (const auto& cls : manclass) c.setMandatoryForClass(cls);
        for (const auto& stu : selectstu) c.stu_select(stu);
        c.assign_teach(teacher);
        Coursemap[id] = c;
    }
    fin.close();
}

// --- 班级 ---
inline void saveClasses(const string& filename, const unordered_map<string, Clas>& Classmap) {
    ofstream fout(filename);
    for (unordered_map<string, Clas>::const_iterator it = Classmap.begin(); it != Classmap.end(); ++it) {
        const string& id = it->first;
        const Clas& c = it->second;
        fout << "[Class]" << endl;
        fout << "id=" << id << endl;
        for (const auto& stu : c.getStudents()) {
            fout << "student=" << stu << endl;
        }
        for (const auto& course : c.getcou()) {
            fout << "mancourse=" << course << endl;
        }
        fout << endl;
    }
    fout.close();
}

inline void loadClasses(const string& filename, unordered_map<string, Clas>& Classmap) {
    ifstream fin(filename);
    if (!fin.is_open()) return;
    string line, id;
    unordered_set<string> students, mancourses;
    while (getline(fin, line)) {
        if (line == "[Class]") {
            if (!id.empty()) {
                Clas c(id);
                for (const auto& stu : students) c.addStudent(stu);
                for (const auto& course : mancourses) c.addMandatoryCourse(course);
                Classmap[id] = c;
            }
            id = "";
            students.clear(); mancourses.clear();
        } else if (line.find("id=") == 0) {
            id = line.substr(3);
        } else if (line.find("student=") == 0) {
            students.insert(line.substr(8));
        } else if (line.find("mancourse=") == 0) {
            mancourses.insert(line.substr(10));
        }
    }
    if (!id.empty()) {
        Clas c(id);
        for (const auto& stu : students) c.addStudent(stu);
        for (const auto& course : mancourses) c.addMandatoryCourse(course);
        Classmap[id] = c;
    }
    fin.close();
}

#endif // DB_STORAGE_H
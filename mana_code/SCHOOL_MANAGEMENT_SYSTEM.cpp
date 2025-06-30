#include "mana_adm.h"
#include"db_storage.h"
#include <bits/stdc++.h>
using namespace std;
unordered_map<string, Administrator> adminmap;
unordered_map<string, Student> studentmap;
unordered_map<string, Teacher> teachermap;
unordered_map<string, Course> coursemap;
unordered_map<string, Clas> classmap;
// 系统反馈的错误输出要在主程序中，每一次高一层的对象在改变数据时，都要对低一层的类进行维护
// 类的结构course->student->class->teacher->administrator
// 全局数据结构
// 学号开头为@+数字
// 教师工号开头为#+数字
// 课程编号开头为$+英语
// 班级编号开头为%+数字
// 管理员账号开头为&
void initGlobalData() {
    // 初始化管理员
    // 初始化班级
    adminmap["&admin"] = Administrator("唐洪武", "男");
    Administrator admin = adminmap["&admin"];
    admin.addClass("%数学自动化");
    admin.addClass("%计算机科学");
    admin.addClass("%软件工程");
    // 初始化学生
    admin.addStudent(new Student("@20240001", "张三", "男", "%数学自动化"));
    admin.addStudent(new Student("@20240002", "李四", "女", "%计算机科学"));    
    admin.addStudent(new Student("@20240003", "王五", "男", "%软件工程"));
    admin.addStudent(new Student("@20240004", "赵六", "女", "%数学自动化"));
    admin.addStudent(new Student("@20240005", "钱七", "男", "%计算机科学"));
    admin.addStudent(new Student("@20240006", "孙八", "女", "%软件工程"));
    // 初始化课程
    admin.addCourse(new Course("$math_analysis", "数学分析", false, 1, 1, 2, 4));
    admin.addCourse(new Course("$data_structure", "数据结构", true, 2, 1, 2, 3));
    admin.addCourse(new Course("$computer_network", "计算机网络", true, 3, 1, 2, 3));
    admin.addCourse(new Course("$operating_system", "操作系统", true, 4, 1, 2, 3));
    admin.addCourse(new Course("$machine_learning", "机器学习", true, 5, 1, 2, 3));
    //初始化老师
    admin.addTeacher(new Teacher("#001", "张东升", "男"));
    admin.addTeacher(new Teacher("#002", "李小红", "女"));
    admin.addTeacher(new Teacher("#003", "王伟", "男"));
    // 初始化必修课
    admin.setCourseMandatory("$math_analysis", "%数学自动化");
    admin.setCourseMandatory("$data_structure", "%计算机科学");
    admin.setCourseMandatory("$computer_network", "%软件工程");
    //初始化教师教学课程
    admin.assignCourseToTeacher("$math_analysis", "#001");
    admin.assignCourseToTeacher("$data_structure", "#002");
    admin.assignCourseToTeacher("$computer_network", "#003");
}
int main(){
    // --- 新增：尝试读取本地数据库 ---
    loadStudents("students.ini", studentmap);
    loadTeachers("teachers.ini", teachermap);
    loadCourses("courses.ini", coursemap);
    loadClasses("classes.ini", classmap);

    // 如果数据库为空，才初始化默认数据
    if (studentmap.empty() || teachermap.empty() || coursemap.empty() || classmap.empty()) {
        initGlobalData();
    }
    else {
        adminmap["&admin"] = Administrator("唐洪武", "男");
        cout<<"文件读取成功!"<<endl;
    }
    int state;// 输入状态，1为学生端，2为老师端，3为管理员端
    cout << "欢迎使用管理系统，请选择登录端口：" << endl;
    cout << "1. 学生端" << endl;   
    cout << "2. 教师端" << endl;
    cout << "3. 管理员端" << endl;
    cout << "0. 退出系统" << endl;
    int alltimes=0; // 记录循环次数，防止死循环
    // 主循环
    while(true){
        cout << "请输入选择：";
        cin >> state;
        
        // 处理退出
        if(state == 0){
            cout << "感谢使用，再见！" << endl;
            break; // 退出整个程序
        }
        int times=0; // 记录循环次数，防止死循环
        // 处理学生端
        if(state == 1) {
            string id, name, className;
            // 学生登录循环
            while(true){
                cout << "请输入您的学号，姓名和班级（输入0返回主菜单）" << endl;
                cin >> id;
                if(id == "0") break; // 返回主菜单
                cin >> name >> className;
                
                if(studentmap.find(id) != studentmap.end()) {
                    Student& student = studentmap[id];
                    if(student.getName() == name && student.getClassName() == className) {
                        cout << "欢迎，" << name << "！您已成功登录学生端。" << endl;
                        
                        // 学生功能循环
                        while(true){
                            cout << "选修课程请按1，退选课程请按2，查看成绩请按3，打印课表请按4，退出请按0" << endl;
                            int choice;
                            cin >> choice;
                            
                            if(choice == 0) {
                                cout << "退出学生端。" << endl;
                                break; // 退出学生功能循环
                            }
                            else if(choice == 1) {
                                cout << "请输入要选修的课程编号：" << endl;
                                string courseId;
                                cin >> courseId;
                                student.selectCourse(courseId);
                                cout << "课程 " << courseId << " 已选修。" << endl;
                            } 
                            else if(choice == 2) {
                                cout << "请输入要退选的课程编号：" << endl;
                                string courseId;
                                cin >> courseId;
                                student.dropCourse(courseId);
                                cout << "课程 " << courseId << " 已退选。" << endl;
                            } 
                            else if(choice == 3) {
                                student.print_grade();
                                cout << "您的成绩已打印。" << endl;
                            } 
                            else if(choice == 4) {
                                student.printTimetable();
                            } 
                            else {
                                cout << "无效的选择，请重新输入。" << endl;
                            }
                        }
                        break; // 成功登录并退出后跳出登录循环
                    }
                    else {
                        cout << "学号、姓名或班级信息不匹配，请重新输入。" << endl;
                    }
                } 
                else {
                    cout << "学号不存在，请重新输入。" << endl;
                    times++;
                    if(times >= 3) {
                        alltimes++;
                        cout << "连续三次输入错误，返回主菜单。" << endl;
                        break; // 返回主菜单
                    }
                }
            }
        }
        // 处理教师端
        else if(state == 2) {
            string id, name;
            // 教师登录循环
            while(true){
                cout << "请输入您的工号，姓名（输入0返回主菜单）" << endl;
                cin >> id;
                if(id == "0") break; // 返回主菜单
                cin >> name;
                
                if(teachermap.find(id) != teachermap.end()) {
                    Teacher& teacher = teachermap[id];
                    if(teacher.getName() == name) {
                        cout << "欢迎，" << name << "！您已成功登录教师端。" << endl;
                        
                        // 教师功能循环
                        while(true){
                            cout << "查看课表请按1，通过文件读取设置成绩请按2，查询基本信息请按3，设置单个学生成绩请按4，按照班级查询课程成绩请按5，退出请按0" << endl;
                            int choice;
                            cin >> choice;
                            
                            if(choice == 0) {
                                cout << "退出教师端。" << endl;
                                break; // 退出教师功能循环
                            }
                            else if(choice == 1) {
                                teacher.printTimetable();
                            } 
                            else if(choice == 2) {
                                cout << "请输入课程编号系统将自动读取以课程编号命名的ini文件（格式：课程编号 ）：" << endl;
                                string courseId;
                                cin >> courseId;
                                //采用文件读取的方式读取成绩
                                teacher.teacherreadCourseGrades(courseId);
                                teachermap[id]=teacher; // 更新教师信息
                            } 
                            else if(choice == 3) {
                                teacher.getidnamegender();
                                cout << "负责的课程：" << endl;
                                for(const string& course : teacher.getAssignedCourses()) {
                                    cout << course << endl;
                                }
                            }
                            else if(choice == 4) {
                                cout << "请输入课程编号、学生学号和成绩（格式：课程编号 学生学号 成绩）：" << endl;
                                string courseId, studentId;
                                double grade;
                                cin >> courseId >> studentId >> grade;
                                teacher.setGradeForStudent(courseId, studentId, grade);
                            }
                            else if(choice == 5) {
                                cout << "请输入班级编号（格式：班级编号）：" << endl;
                                string className;
                                cin >> className;
                                if(classmap.find(className) != classmap.end()) {
                                    teacher.printCourseGradesByClass(className);  // 打印班级成绩
                                } else {
                                    cout << "班级不存在，请重新输入。" << endl;
                                }
                            }
                            else {
                                cout << "无效的选择，请重新输入。" << endl;
                            }
                        }
                        break; // 成功登录并退出后跳出登录循环
                    }
                    else {
                        cout << "工号或姓名信息不匹配，请重新输入。" << endl;
                    }
                } 
                else {
                    cout << "工号不存在，请重新输入。" << endl;
                    times++;
                    if(times >= 3) {
                        alltimes++;
                        cout << "连续三次输入错误，返回主菜单。" << endl;
                        break; // 返回主菜单
                    }
                }
            }
        }
        // 处理管理员端
        else if(state == 3) {
            // 管理员登录循环
            while(true){
                cout << "请输入管理员账号和姓名（输入0返回主菜单）" << endl;
                string adminId, adminName;
                cin >> adminId;
                if(adminId == "0") break; // 返回主菜单
                cin >> adminName;
                
                if(adminmap.find(adminId) != adminmap.end()) {
                    Administrator& admin = adminmap[adminId];
                    if(admin.getName() == adminName) {
                        cout << "欢迎，" << adminName << "！您已成功登录管理员端。" << endl;
                        
                        // 管理员功能循环
                        while(true){
                            cout << "请输入你的选择" << endl;
                            cout << "1. 添加学生" << endl;
                            cout << "2. 添加教师" << endl;
                            cout << "3. 添加课程" << endl;
                            cout << "4. 设置课程为某班级必修课" << endl;
                            cout << "5. 分配课程给教师" << endl;
                            cout << "6. 排课" << endl;
                            cout << "7. 打印班级成绩" << endl;
                            cout << "8. 将班级按照学生平均分排序" << endl;
                            cout << "9. 添加班级" << endl;
                            cout<<"10. 查看某班级所有学生的课程成绩（包含选修课）"<<endl;
                            cout<<"11. 删除学生"<<endl;
                            cout<<"12. 删除教师"<<endl;
                            cout<<"13. 修改学生信息"<<endl;
                            cout<<"14. 修改教师信息"<<endl;
                            cout << "0. 退出" << endl;
                            
                            int choice;
                            cin >> choice;
                            
                            if(choice == 0) {
                                cout << "退出管理员端。" << endl;
                                break; // 退出管理员功能循环
                            }
                            else if(choice == 1) {
                                cout << "请输入学生信息（学号 姓名 性别 班级）：" << endl;
                                string id, name,gender, className;
                                cin >> id >> name >>gender >> className;
                                if(studentmap.find(id) == studentmap.end()) {
                                    Student newStudent(id,name,gender, className);
                                    admin.addStudent(&newStudent);
                                    cout << "学生 " << name << " 已添加成功。" << endl;
                                } else {
                                    cout << "学号已存在，请重新输入。" << endl;
                                }
                            } 
                            else if(choice == 2) {
                                cout << "请输入教师信息（工号 姓名 性别）：" << endl;
                                string id, name,gender;
                                cin >> id >> name >>gender;
                                if(teachermap.find(id) == teachermap.end()) {
                                    Teacher newTeacher(id,name,gender);
                                    if(teachermap.find(id) == teachermap.end()) {
                                        admin.addTeacher(&newTeacher);
                                        cout << "教师 " << name << " 已添加成功。" << endl;
                                    } else {
                                        cout << "工号已存在，请重新输入。" << endl;
                                    }
                                }
                            } 
                            else if(choice == 3) {
                                cout << "请输入课程信息（课程编号 课程名称 是否选修 课程日期 开始时间 结束时间 学分）：" << endl;
                                string id, name;
                                bool isElective;
                                int day, st, ed, val;
                                cin >> id >> name >> isElective >> day >> st >> ed >> val;
                                if(coursemap.find(id) == coursemap.end()) {
                                    Course newCourse(id, name, isElective, day, st, ed, val);
                                    admin.addCourse(&newCourse);
                                    cout << "课程 " << name << " 已添加成功。" << endl;
                                } else {
                                    cout << "课程编号已存在，请重新输入。" << endl;
                                }
                            } 
                            else if(choice == 4) {
                                cout << "请输入课程编号和班级编号（格式：课程编号 班级编号）：" << endl;
                                string courseId, className;
                                cin >> courseId >> className;
                                admin.setCourseMandatory(courseId, className);
                                cout << "课程 " << courseId << " 已设置为班级 " << className << " 的必修课。" << endl;
                            } 
                            else if(choice == 5) {
                                cout << "请输入课程编号和教师工号（格式：课程编号 教师工号）：" << endl;
                                string courseId, teacherId;
                                cin >> courseId >> teacherId;
                                admin.assignCourseToTeacher(courseId, teacherId);
                                cout << "课程 " << courseId << " 已分配给教师 " << teacherId << "。" << endl;
                            } 
                            else if(choice == 6) {
                                cout << "请输入课程编号、日期、开始时间和结束时间（格式：课程编号 日期 开始时间 结束时间）：" << endl;
                                string courseId;
                                int date, st, ed;
                                cin >> courseId >> date >> st >> ed;
                                admin.scheduleCourseTime(courseId, date, st, ed);
                                cout << "课程 " << courseId << " 已排定在日期 " << date << "，时间段 " << st << "-" << ed << "." << endl;
                            } 
                            else if(choice == 7) {
                                cout << "请输入班级编号：" << endl;
                                string className;
                                cin >> className;
                                admin.printClassGrades(className);
                            } 
                            else if(choice == 8) {
                                admin.sortClassesByAverage();
                            } 
                            else if(choice == 9) {
                                cout << "请输入班级名称：" << endl;
                                string className;
                                cin >> className;
                                if(classmap.find(className) == classmap.end()) {
                                    admin.addClass(className);
                                    cout << "班级 " << className << " 已添加成功。" << endl;
                                } else {
                                    cout << "班级已存在，请重新输入。" << endl;
                                }
                            }
                            else if(choice == 10) {
                                cout << "请输入班级编号：" << endl;
                                string className;
                                cin >> className;
                                if(classmap.find(className) != classmap.end()) {
                                    admin.inquireClassAllGrades(className);
                                } else {
                                    cout << "班级不存在，请重新输入。" << endl;
                                }
                            }
                            else if(choice == 11) {
                                cout << "请输入要删除的学生学号：" << endl;
                                string studentId;
                                cin >> studentId;
                                admin.removeStudent(studentId);
                            }
                            else if(choice == 12) {
                                cout << "请输入要删除的教师工号：" << endl;
                                string teacherId;
                                cin >> teacherId;
                                admin.removeTeacher(teacherId);
                            }
                            else if(choice == 13) {
                                cout << "请输入学生学号和新的姓名、性别、班级（格式：学号 姓名 性别 班级）：" << endl;
                                string studentId, newName, newGender, newClassName;
                                cin >> studentId >> newName >> newGender >> newClassName;
                                admin.modifyStudent(studentId, newName, newGender, newClassName);
                            }
                            else if(choice == 14) {
                                cout << "请输入教师工号和新的姓名、性别（格式：工号 姓名 性别）：" << endl;
                                string teacherId, newName, newGender;
                                cin >> teacherId >> newName >> newGender;
                                admin.modifyTeacher(teacherId, newName, newGender);
                            }
                            else {
                                cout << "无效的选择，请重新输入。" << endl;
                            }
                        }
                        break; // 成功登录并退出后跳出登录循环
                    }
                    else {
                        cout << "管理员姓名不匹配，请重新输入。" << endl;
                    }
                } 
                else {
                    cout << "管理员账号不存在，请重新输入。" << endl;
                    times++;
                    if(times >= 3) {
                        alltimes++;
                        cout << "连续三次输入错误，返回主菜单。" << endl;
                        break; // 返回主菜单
                    }
                }
            }
        }
        // 处理无效输入
        else {
            cout << "无效的输入，请重新选择端口（1-学生端, 2-教师端, 3-管理员端, 0-退出）" << endl;
        }
        // 检查是否连续十次输入错误
        if(alltimes >= 10) {
            cout << "连续十次输入错误，程序将退出。" << endl;
            break; // 退出整个程序
        }
    }
    // --- 新增：保存数据到本地数据库 ---
    saveStudents("students.ini", studentmap);
    saveTeachers("teachers.ini", teachermap);
    saveCourses("courses.ini", coursemap);
    saveClasses("classes.ini", classmap);
    return 0;
}

-- 
-- SQL Server 2008 R2
-- http://blog.csdn.net/shuxiao9058/article/details/7525447/
--
------------------------------------------------------------------------------------------------------



-- 5.1 创建表
-- 考虑到各个表之间的约束条件以及外键索引等要求，在创建表的时候应当按照一定的次序进行创建，否则会出现错误，
-- 还有一种方法是先创建各个基本表，然后在对特定的表添加列和外码约束，在本报告册中将采取第一种方法。
-- （1）tb_dept院系信息表
CREATE TABLE tb_dept(  
    DeptNum char(10) NOT NULL PRIMARY KEY,  -- 
    DeptName varchar(20) NOT NULL,  -- 
    DeptChairman varchar(10) NOT NULL,  -- 
    DeptTel varchar(15) NOT NULL,  -- 
    DeptDesc text NOT NULL,  -- 
)  ;

-- （2）tb_major专业信息表
CREATE TABLE tb_major(  
    MajorNum char(10) NOT NULL PRIMARY KEY,  -- 
    DeptNum char(10) NOT NULL,  -- 
    MajorName varchar(20) NOT NULL,  -- 
    MajorAssistant varchar(10) NOT NULL,  -- 
    MajorTel varchar(15) NOT NULL,  -- 
    FOREIGN KEY (DeptNum) REFERENCES tb_dept(DeptNum)  -- 
)  ;

-- （3）tb_student学生信息表
CREATE TABLE tb_student(  
    StudentNum char(10) NOT NULL PRIMARY KEY,  -- 
    MajorNum char(10) NOT NULL,  -- 
    StudentName varchar(10) NULL,  -- 
    StudentSex char(2) NOT NULL,  -- 
    StudentBirthday datetime NOT NULL,  -- 
    StudentPassword varchar(20) NOT NULL,  -- 
    FOREIGN KEY (MajorNum) REFERENCES tb_major(MajorNum)  -- 
)  ;

-- （4）tb_teacher教师信息表
CREATE TABLE tb_teacher(  
    TeacherNum char(10) NOT NULL PRIMARY KEY,  -- 
    DeptNum char(10) NOT NULL,  -- 
    TeacherName varchar(10) NOT NULL,  -- 
    TeacherSex char(2) NOT NULL,  -- 
    TeacherBirthday datetime NOT NULL,  -- 
    TeacherTitle varchar(20) NULL,  -- 
    FOREIGN KEY (DeptNum) REFERENCES tb_dept(DeptNum)  -- 
)  ;

-- （5）tb_manager管理员信息表
CREATE TABLE tb_manager(  
ManagerNum char(10) NOT NULL PRIMARY KEY,  -- 
ManagerName varchar(10) NOT NULL,  -- 
ManagerSex char(2) NOT NULL,  -- 
ManagerBirthdate datetime NOT NULL,  -- 
ManagerRights int NOT NULL  -- 
 )  ;
 
 -- （6）tb_course课程信息表
 CREATE TABLE tb_course(  
    CourseNum varchar(10) NOT NULL PRIMARY KEY,  -- 
    CourseName varchar(20) NOT NULL,  -- 
    CourseCredit float NOT NULL,  -- 
    CourseClass smallint NOT NULL,  -- 
    CourseDesc text NOT NULL,  -- 
)  ;

-- （7）tb_stucourse学生选课信息表
CREATE TABLE tb_stucourse(  
    StudentNum char(10) NOT NULL,  -- 
    CourseNum char(10) NOT NULL,  -- 
    TeacherNum char(10) NOT NULL,  -- 
    Grade smallint NULL,  -- 
    FOREIGN KEY (StudentNum) REFERENCES tb_student(StudentNum),  -- 
    FOREIGN KEY (CourseNum) REFERENCES tb_Course(CourseNum),  -- 
    FOREIGN KEY (TeacherNum) REFERENCES tb_teacher(TeacherNum),   -- 
)  ;

-- （8）tb_control控制设置表
CREATE TABLE tb_control(  
    IfTakeCourse char(1) NOT NULL check(IfTakeCourse in ( '0', '1')),  -- 
    IfInputGrade char(1) NOT NULL check(IfInputGrade in ( '0', '1')),  -- 
)  ;

-- 5.2 创建必要视图
-- （1）建立学生成绩视图vi_grade，从学生、老师、选课表中选择Grade不为空的记录，其关键代码如下所示：
CREATE VIEW vi_grade  
AS  
SELECT tb_stucourse.StudentNum,StudentName,CourseName  
        ,CourseCredit,TeacherName,Grade  
FROM tb_stucourse,tb_student,tb_course,tb_teacher  
where tb_stucourse.StudentNum=tb_student.StudentNum and  
      tb_stucourse.TeacherNum=tb_teacher.TeacherNum and  
      tb_stucourse.CourseNum=tb_course.CourseNum and  
      Grade is not null;

	  
-- （2）建立专业学生信息视图vi_major，从学生、专业表中选择学生中的专业号码与专业表中专业号码相等的记录，其关键代码如下所示：
CREATE VIEW vi_major  
AS  
SELECT tb_major.MajorName,StudentNum,StudentName,StudentSex,StudentBirthday  
FROM tb_major,tb_student  
WHERE tb_major.MajorNum=tb_student.MajorNum  ;

-- 5.3 创建必要触发器
-- （1）建立学生添加院系触发器tri_adddept，当该表中已存在所对应院系号码的院系时，系统给与错误提示并回滚，其关键代码如下所示：
CREATE TRIGGER tri_adddept ON tb_dept  
FOR INSERT,UPDATE  
AS  
IF  
    (SELECT COUNT(*) FROM tb_dept,inserted   
    WHERE tb_dept.DeptNum=inserted.DeptNum)>0  
BEGIN  
    PRINT '院系号码产生冲突，请核对后重试！'  
    ROLLBACK  
END  

--- （2）建立学生添加专业触发器tb_major，当专业信息中的院系号不对或者该表中已存在所对应专业号码的专业时，系统给与错误提示并回滚，其关键代码如下所示：
CREATE TRIGGER tri_addmajor ON tb_major  
FOR INSERT,UPDATE  
AS  
    IF(SELECT COUNT(*) FROM tb_dept,inserted  
    WHERE tb_dept.DeptNum=inserted.DeptNum)=0  
BEGIN  
    PRINT '未找到该专业的院系信息，请添加相应院系后重试！'  
    ROLLBACK  
END  
ELSE IF  
    (SELECT COUNT(*) FROM tb_major,inserted   
    WHERE tb_major.MajorNum=inserted.MajorNum)>0  
BEGIN  
    PRINT '院系号码产生冲突，请核对后重试！'  
    ROLLBACK  
END  

-- （3）建立添加学生触发器tri_addstudent，当学生信息中的专业号不对或者系统中已存在所对应学号的学生时，系统给与错误提示并回滚，其关键代码如下所示：
CREATE TRIGGER tri_addstudent ON tb_student  
FOR INSERT,UPDATE  
AS  
    IF(SELECT COUNT(*) FROM tb_major,inserted  
    WHERE tb_major.MajorNum=inserted.MajorNum)=0  
BEGIN  
    PRINT '未找到该学生的专业信息，请添加相应专业后重试！'  
    ROLLBACK  
END  
ELSE IF  
    (SELECT COUNT(*) FROM tb_student,inserted   
    WHERE tb_student.StudentNum=inserted.StudentNum)>0  
BEGIN  
    PRINT '学号产生冲突，请核对后重试！'  
    ROLLBACK  
END  

-- （4）建立学生选课触发器tri_takecourse，课程选课人数超过40个或者对应学生选课门数超过5门或者当前时间不是选课时间段时，系统给与错误提示并回滚，其关键代码如下所示：
CREATE TRIGGER tri_takecourse ON tb_stucourse  
FOR INSERT,UPDATE  
AS  
    IF(SELECT COUNT(*) FROM tb_stucourse,inserted  
    WHERE tb_stucourse.CourseNum=inserted.CourseNum)>40  
BEGIN  
    PRINT '所对应课程选课人数不能超过40个！'  
    ROLLBACK  
END  
ELSE IF  
    (SELECT COUNT(*) FROM tb_student,inserted   
    WHERE tb_student.StudentNum=inserted.StudentNum)>5  
BEGIN  
    PRINT '对应学生的选课不能超过5门！'  
    ROLLBACK  
END  
ELSE IF  
    (SELECT IfTakeCourse FROM tb_control)='0'  
BEGIN  
    PRINT '当前不是选课时间段！'  
    ROLLBACK  
END  

-- 5.4 创建必要存储过程
-- （1）建立学生选课存储过程tri_takecourse，其作用为查询所有学生的选课记录，其关键代码如下所示：
CREATE PROC proc_getcourse  
@StudentNum char(10) OUT,  
@StudentName char(10) OUT,  
@CourseName varchar(20) OUT,  
@CourseCredit float OUT,  
@TeacherName char(10) OUT  
AS  
    SELECT @StudentNum=tb_student.StudentNum  
            ,@StudentName=tb_student.StudentName  
            , @CourseName=tb_course.CourseName  
            ,@CourseCredit=tb_course.CourseCredit  
            , @TeacherName=tb_teacher.TeacherName  
    FROM tb_stucourse,tb_course,tb_student,tb_teacher  
    WHERE tb_stucourse.StudentNum=tb_student.StudentNum AND  
            tb_stucourse.CourseNum=tb_course.CourseNum AND  
            tb_stucourse.TeacherNum=tb_teacher.TeacherNum  ;

-- （2）建立查询教师课程存储过程proc_teachercourse，其作用为查询指定教师所有课程的选课记录，其关键代码如下所示：
CREATE PROC proc_teachercourse  
@TeacherNum char(10),  
@StudentNum char(10) OUT,  
@StudentName char(10) OUT,  
@CourseName varchar(20) OUT,  
@CourseCredit float OUT,  
@TeacherName char(10) OUT  
AS  
    SELECT @StudentNum=tb_student.StudentNum  
            ,@StudentName=tb_student.StudentName  
            , @CourseName=tb_course.CourseName  
            ,@CourseCredit=tb_course.CourseCredit  
            , @TeacherName=tb_teacher.TeacherName  
    FROM tb_stucourse,tb_course,tb_student,tb_teacher  
    WHERE tb_stucourse.StudentNum=tb_student.StudentNum AND  
            tb_stucourse.CourseNum=tb_course.CourseNum AND  
            tb_stucourse.TeacherNum=tb_teacher.TeacherNum and  
            tb_teacher.TeacherNum=@TeacherNum  ;
			
-- （3）建立查询学生课程存储过程proc_studentcourse，其作用为查询指定学生所有课程的选课记录，其关键代码如下所示：
CREATE PROC proc_studentcourse  
@StudentNum char(10),  
@StudentName char(10) OUT,  
@CourseName varchar(20) OUT,  
@CourseCredit float OUT,  
@TeacherName char(10) OUT  
AS  
    SELECT @StudentNum=tb_student.StudentNum  
            ,@StudentName=tb_student.StudentName  
            , @CourseName=tb_course.CourseName  
            ,@CourseCredit=tb_course.CourseCredit  
            , @TeacherName=tb_teacher.TeacherName  
    FROM tb_stucourse,tb_course,tb_student,tb_teacher  
    WHERE tb_stucourse.StudentNum=tb_student.StudentNum AND  
            tb_stucourse.CourseNum=tb_course.CourseNum AND  
            tb_stucourse.TeacherNum=tb_teacher.TeacherNum and  
            tb_student.StudentNum=@StudentNum ;
			

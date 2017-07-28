PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;


-- 
-- http://blog.csdn.net/shuxiao9058/article/details/7525447/
---------------------------------------------------------------------------------
CREATE TABLE [School_StudentInfo] (
	StudentNum INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, -- 学号
	MajorNum VARCHAR DEFAULT '' NOT NULL, -- 专业号码
	StudentName VARCHAR DEFAULT '' NOT NULL, -- 姓名
	StudentSex VARCHAR DEFAULT '' NOT NULL, -- 性别
  -- http://www.runoob.com/sqlite/sqlite-data-types.html
	StudentBirthday TEXT DEFAULT '' NOT NULL, -- 生日
	StudentPassword VARCHAR DEFAULT '' NOT NULL -- 密码
);

CREATE TABLE [School_TeacherInfo] (
	MajorNum VARCHAR DEFAULT '' NOT NULL, -- 专业号码
	DeptNum VARCHAR DEFAULT '' NOT NULL, -- 系号码
	MajorName VARCHAR DEFAULT '' NOT NULL, -- 专业名
	MajorAssistant VARCHAR DEFAULT '' NOT NULL, -- 辅导员
	MajorTel VARCHAR DEFAULT '' NOT NULL -- 联系方式
);

CREATE TABLE [School_AdministratorInfo] (
	ManagerNum VARCHAR DEFAULT '' NOT NULL, -- 
	ManagerName VARCHAR DEFAULT '' NOT NULL, -- 
	ManagerSex VARCHAR DEFAULT '' NOT NULL, -- 
	ManagerBirthday TEXT DEFAULT '' NOT NULL -- 
);

CREATE TABLE [School_ProfessionInfo] (
MajorNum VARCHAR DEFAULT '' NOT NULL, -- 
DeptNum VARCHAR DEFAULT '' NOT NULL, -- 
MajorName VARCHAR DEFAULT '' NOT NULL, -- 
MajorAssistant VARCHAR DEFAULT '' NOT NULL, -- 
MajorTel VARCHAR DEFAULT '' NOT NULL -- 
);

CREATE TABLE [School_DepartmentInfo] (
DeptNum VARCHAR DEFAULT '' NOT NULL, -- 
DeptName VARCHAR DEFAULT '' NOT NULL, -- 
DeptChairman VARCHAR DEFAULT '' NOT NULL, -- 
DeptTel VARCHAR DEFAULT '' NOT NULL, -- 
DeptDesc VARCHAR DEFAULT '' NOT NULL -- 
);

CREATE TABLE [School_ CourseInfo] (
CourseNum VARCHAR , --
CourseName VARCHAR DEFAULT '' NOT NULL, -- 
CourseCredit VARCHAR DEFAULT '' NOT NULL, -- 
CourseClass VARCHAR DEFAULT '' NOT NULL, -- 
CourseDesc VARCHAR  --
);

CREATE TABLE [School_SelCourseInfo] (
StuCourseID VARCHAR , --
StudentNum VARCHAR , --
CourseNum VARCHAR , --
TeacherNum VARCHAR , --
Grade VARCHAR   --
);

CREATE TABLE [School_ControlsettingsInfo] (
IfTakeCourse VARCHAR , --
IfInputGrade VARCHAR   --
);



COMMIT;

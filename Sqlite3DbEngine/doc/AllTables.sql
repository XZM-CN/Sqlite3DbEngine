PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;


-- 
-- http://blog.csdn.net/shuxiao9058/article/details/7525447/
---------------------------------------------------------------------------------
CREATE TABLE [School_StudentInfo] (
	StudentNum INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, -- ѧ��
	MajorNum VARCHAR DEFAULT '' NOT NULL, -- רҵ����
	StudentName VARCHAR DEFAULT '' NOT NULL, -- ����
	StudentSex VARCHAR DEFAULT '' NOT NULL, -- �Ա�
  -- http://www.runoob.com/sqlite/sqlite-data-types.html
	StudentBirthday TEXT DEFAULT '' NOT NULL, -- ����
	StudentPassword VARCHAR DEFAULT '' NOT NULL -- ����
);

CREATE TABLE [School_TeacherInfo] (
	MajorNum VARCHAR DEFAULT '' NOT NULL, -- רҵ����
	DeptNum VARCHAR DEFAULT '' NOT NULL, -- ϵ����
	MajorName VARCHAR DEFAULT '' NOT NULL, -- רҵ��
	MajorAssistant VARCHAR DEFAULT '' NOT NULL, -- ����Ա
	MajorTel VARCHAR DEFAULT '' NOT NULL -- ��ϵ��ʽ
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

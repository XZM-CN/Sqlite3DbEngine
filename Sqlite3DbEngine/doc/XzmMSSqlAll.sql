USE [master]
GO

-- ������ݿ�[XzmMSSqlAll]���ڣ���ôɾ��[XzmMSSqlAll]���ݿ�
IF DB_ID('[XzmMSSqlAll]') IS NOT NULL DROP DATABASE [XzmMSSqlAll];

-- If database could not be created due to open connections, abort
IF @@ERROR = 3702 
   RAISERROR('Database cannot be dropped because there are still open connections.', 127, 127) WITH NOWAIT, LOG;

-- �������ݿ�
---------------------------------------------------------------------------------------------------------------------------------------------
CREATE DATABASE [XzmMSSqlAll]
 CONTAINMENT = NONE
 ON  PRIMARY 
( NAME = N'XzmMSSqlAll', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL11.MSSQLSERVER\MSSQL\DATA\XzmMSSqlAll.mdf' , SIZE = 5120KB , MAXSIZE = UNLIMITED, FILEGROWTH = 1024KB )
 LOG ON 
( NAME = N'XzmSQL2012Date20170727_log', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL11.MSSQLSERVER\MSSQL\DATA\XzmSQL2012Date20170727_log.ldf' , SIZE = 2048KB , MAXSIZE = 2048GB , FILEGROWTH = 10%)
GO

ALTER DATABASE [XzmMSSqlAll] SET COMPATIBILITY_LEVEL = 110
GO

IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [XzmMSSqlAll].[dbo].[sp_fulltext_database] @action = 'enable'
end
GO

ALTER DATABASE [XzmMSSqlAll] SET ANSI_NULL_DEFAULT OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET ANSI_NULLS OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET ANSI_PADDING OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET ANSI_WARNINGS OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET ARITHABORT OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET AUTO_CLOSE OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET AUTO_CREATE_STATISTICS ON 
GO

ALTER DATABASE [XzmMSSqlAll] SET AUTO_SHRINK OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET AUTO_UPDATE_STATISTICS ON 
GO

ALTER DATABASE [XzmMSSqlAll] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET CURSOR_DEFAULT  GLOBAL 
GO

ALTER DATABASE [XzmMSSqlAll] SET CONCAT_NULL_YIELDS_NULL OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET NUMERIC_ROUNDABORT OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET QUOTED_IDENTIFIER OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET RECURSIVE_TRIGGERS OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET  DISABLE_BROKER 
GO

ALTER DATABASE [XzmMSSqlAll] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET TRUSTWORTHY OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET PARAMETERIZATION SIMPLE 
GO

ALTER DATABASE [XzmMSSqlAll] SET READ_COMMITTED_SNAPSHOT OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET HONOR_BROKER_PRIORITY OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET RECOVERY FULL 
GO

ALTER DATABASE [XzmMSSqlAll] SET  MULTI_USER 
GO

ALTER DATABASE [XzmMSSqlAll] SET PAGE_VERIFY CHECKSUM  
GO

ALTER DATABASE [XzmMSSqlAll] SET DB_CHAINING OFF 
GO

ALTER DATABASE [XzmMSSqlAll] SET FILESTREAM( NON_TRANSACTED_ACCESS = OFF ) 
GO

ALTER DATABASE [XzmMSSqlAll] SET TARGET_RECOVERY_TIME = 0 SECONDS 
GO

ALTER DATABASE [XzmMSSqlAll] SET  READ_WRITE 
GO



--ʹ��[XzmMSSqlAll]���ݿ�
USE [XzmMSSqlAll];
GO

CREATE SCHEMA [Study] AUTHORIZATION [dbo];
GO
-- 5.1 ������
-- ���ǵ�������֮���Լ�������Լ����������Ҫ���ڴ������ʱ��Ӧ������һ���Ĵ�����д������������ִ���
-- ����һ�ַ������ȴ�������������Ȼ���ڶ��ض��ı�����к�����Լ�����ڱ�������н���ȡ��һ�ַ�����
-- ��1��tb_deptԺϵ��Ϣ��
CREATE TABLE [Study].[tb_dept](  
    DeptNum char(10) NOT NULL PRIMARY KEY,  -- 
    DeptName varchar(20) NOT NULL,  -- 
    DeptChairman varchar(10) NOT NULL,  -- 
    DeptTel varchar(15) NOT NULL,  -- 
    DeptDesc text NOT NULL,  -- 
)  ;
GO

-- ��2��tb_majorרҵ��Ϣ��
CREATE TABLE [Study].[tb_major](  
    MajorNum char(10) NOT NULL PRIMARY KEY,  -- 
    DeptNum char(10) NOT NULL,  -- 
    MajorName varchar(20) NOT NULL,  -- 
    MajorAssistant varchar(10) NOT NULL,  -- 
    MajorTel varchar(15) NOT NULL,  -- 
    FOREIGN KEY (DeptNum) REFERENCES [Study].[tb_dept](DeptNum)  -- 
)  ;
GO

-- ��3��tb_studentѧ����Ϣ��
CREATE TABLE [Study].[tb_student](  
    StudentNum char(10) NOT NULL PRIMARY KEY,  -- 
    MajorNum char(10) NOT NULL,  -- 
    StudentName varchar(10) NULL,  -- 
    StudentSex char(2) NOT NULL,  -- 
    StudentBirthday datetime NOT NULL,  -- 
    StudentPassword varchar(20) NOT NULL,  -- 
    FOREIGN KEY (MajorNum) REFERENCES [Study].[tb_major](MajorNum)  -- 
)  ;
GO

-- ��4��tb_teacher��ʦ��Ϣ��
CREATE TABLE [Study].[tb_teacher](  
    TeacherNum char(10) NOT NULL PRIMARY KEY,  -- 
    DeptNum char(10) NOT NULL,  -- 
    TeacherName varchar(10) NOT NULL,  -- 
    TeacherSex char(2) NOT NULL,  -- 
    TeacherBirthday datetime NOT NULL,  -- 
    TeacherTitle varchar(20) NULL,  -- 
    FOREIGN KEY (DeptNum) REFERENCES [Study].[tb_dept](DeptNum)  -- 
)  ;
GO

-- ��5��tb_manager����Ա��Ϣ��
CREATE TABLE [Study].[tb_manager](  
ManagerNum char(10) NOT NULL PRIMARY KEY,  -- 
ManagerName varchar(10) NOT NULL,  -- 
ManagerSex char(2) NOT NULL,  -- 
ManagerBirthdate datetime NOT NULL,  -- 
ManagerRights int NOT NULL  -- 
 )  ;
GO
 
 -- ��6��tb_course�γ���Ϣ��
 CREATE TABLE [Study].[tb_course](  
    CourseNum char(10) NOT NULL PRIMARY KEY,  -- 
    CourseName varchar(20) NOT NULL,  -- 
    CourseCredit float NOT NULL,  -- 
    CourseClass smallint NOT NULL,  -- 
    CourseDesc text NOT NULL,  -- 
)  ;
GO

-- ��7��tb_stucourseѧ��ѡ����Ϣ��
CREATE TABLE [Study].[tb_stucourse](  
    StudentNum char(10) NOT NULL,  -- 
    CourseNum char(10) NOT NULL,  -- 
    TeacherNum char(10) NOT NULL,  -- 
    Grade smallint NULL,  -- 
    FOREIGN KEY (StudentNum) REFERENCES [Study].[tb_student](StudentNum),  -- 
    FOREIGN KEY (CourseNum) REFERENCES [Study].[tb_Course](CourseNum),  -- 
    FOREIGN KEY (TeacherNum) REFERENCES [Study].[tb_teacher](TeacherNum),   -- 
)  ;
GO

-- ��8��tb_control�������ñ�
CREATE TABLE [Study].[tb_control](  
    IfTakeCourse char(1) NOT NULL check(IfTakeCourse in ( '0', '1')),  -- 
    IfInputGrade char(1) NOT NULL check(IfInputGrade in ( '0', '1')),  -- 
)  ;
GO

-- 5.2 ������Ҫ��ͼ
-- ��1������ѧ���ɼ���ͼvi_grade����ѧ������ʦ��ѡ�α���ѡ��Grade��Ϊ�յļ�¼����ؼ�����������ʾ��
CREATE VIEW [Study].[vi_grade]   
AS  
SELECT [Study].[tb_stucourse].StudentNum,StudentName,CourseName,CourseCredit,TeacherName,Grade  
FROM [Study].[tb_stucourse],tb_student,tb_course,tb_teacher  
where [Study].[tb_stucourse].StudentNum=tb_student.StudentNum and  
      [Study].[tb_stucourse].TeacherNum=tb_teacher.TeacherNum and  
      [Study].[tb_stucourse].CourseNum=tb_course.CourseNum and  
      Grade is not null;
GO
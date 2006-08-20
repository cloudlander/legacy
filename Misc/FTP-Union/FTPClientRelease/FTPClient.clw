; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CFTPClientDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "FTPClient.h"

ClassCount=3
Class1=CFTPClientApp
Class2=CFTPClientDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_FTPCLIENT_DIALOG
Resource4=IDR_SYSTRAY_MENU

[CLS:CFTPClientApp]
Type=0
HeaderFile=FTPClient.h
ImplementationFile=FTPClient.cpp
Filter=N

[CLS:CFTPClientDlg]
Type=0
HeaderFile=FTPClientDlg.h
ImplementationFile=FTPClientDlg.cpp
Filter=D
LastObject=CFTPClientDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=FTPClientDlg.h
ImplementationFile=FTPClientDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_FTPCLIENT_DIALOG]
Type=1
Class=CFTPClientDlg
ControlCount=30
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_FTP,SysIPAddress32,1342242816
Control4=IDC_NEEDUSER,button,1342242819
Control5=IDC_STATIC,static,1342308352
Control6=IDC_FTPUSR,edit,1484849280
Control7=IDC_STATIC,static,1342308352
Control8=IDC_FTPPWD,edit,1484849312
Control9=IDC_STATIC,button,1342177287
Control10=IDC_NEEDLOG,button,1342242819
Control11=IDC_STATIC,static,1342308352
Control12=IDC_PATH,edit,1350633600
Control13=IDC_BROWSE,button,1342242816
Control14=IDC_STATIC,static,1342308352
Control15=IDC_AUTHUSR,edit,1350631552
Control16=IDC_STATIC,static,1342308352
Control17=IDC_AUTHPWD,edit,1350631584
Control18=IDC_STATIC,static,1342308352
Control19=IDC_AUTHSVR,SysIPAddress32,1342242816
Control20=IDC_AUTHENTICATE,button,1342242816
Control21=IDC_STATIC,button,1342177287
Control22=IDC_NEEDLOOP,button,1342242819
Control23=IDC_STATIC,static,1342308352
Control24=IDC_INTERVAL,edit,1350631552
Control25=IDC_STATIC,static,1342308352
Control26=IDC_START,button,1342242816
Control27=IDC_STATIC,button,1342177287
Control28=IDC_STATUS,static,1342308352
Control29=IDC_PORT,edit,1350631552
Control30=IDC_STATIC,static,1342308352

[MNU:IDR_SYSTRAY_MENU]
Type=1
Class=?
Command1=IDR_RESTORE
Command2=IDR_EXIT
CommandCount=2


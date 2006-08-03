; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWmd5Dlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "wmd5.h"

ClassCount=6
Class1=CWmd5App
Class2=CWmd5Dlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_WMD5_DIALOG
Resource4=IDD_WMD5_DIALOG (English (U.S.))
Class5=CQueueProgresser
Class4=CJobProgresser
Resource5=IDD_TIPBOX (English (U.S.))
Class6=CTipDlg
Resource6=IDD_ABOUTBOX (English (U.S.))

[CLS:CWmd5App]
Type=0
HeaderFile=wmd5.h
ImplementationFile=wmd5.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CWmd5Dlg]
Type=0
HeaderFile=wmd5Dlg.h
ImplementationFile=wmd5Dlg.cpp
Filter=W
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_JOBPATH

[CLS:CAboutDlg]
Type=0
HeaderFile=wmd5Dlg.h
ImplementationFile=wmd5Dlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_WMD5_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CWmd5Dlg

[DLG:IDD_WMD5_DIALOG (English (U.S.))]
Type=1
Class=CWmd5Dlg
ControlCount=11
Control1=IDC_QueueProgress,msctls_progress32,1350565889
Control2=IDC_JOBLIST,SysListView32,1350631693
Control3=IDC_SSUCCESS,static,1073872896
Control4=IDC_SUCCESS,static,1073872896
Control5=IDC_JOBPATH,static,1342308352
Control6=IDC_SERROR,static,1073872896
Control7=IDC_SMATCH,static,1073872896
Control8=IDC_SNOTMATCH,static,1073872896
Control9=IDC_MATCH,static,1073872896
Control10=IDC_ERROR,static,1073872896
Control11=IDC_NOTMATCH,static,1073872896

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=3
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1476526080
Control3=IDC_STATIC_MAILME,static,1342308352

[CLS:CJobProgresser]
Type=0
HeaderFile=JobProgresser.h
ImplementationFile=JobProgresser.cpp
BaseClass=CProgressCtrl
Filter=W
VirtualFilter=NWC

[CLS:CQueueProgresser]
Type=0
HeaderFile=jobprogresser.h
ImplementationFile=jobprogresser.cpp
BaseClass=CProgressCtrl
Filter=W
LastObject=CQueueProgresser

[DLG:IDD_TIPBOX (English (U.S.))]
Type=1
Class=CTipDlg
ControlCount=2
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342308352

[CLS:CTipDlg]
Type=0
HeaderFile=TipDlg.h
ImplementationFile=TipDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CTipDlg
VirtualFilter=dWC


; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSetView
LastTemplate=CFormView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Graph3D.h"
LastPage=0

ClassCount=5
Class1=CGraph3DApp
Class3=CMainFrame
Class4=CAboutDlg

ResourceCount=3
Resource1=IDR_MAINFRAME
Class2=CChildView
Class5=CSetView
Resource2=IDD_ABOUTBOX
Resource3=IDD_DIALOG1

[CLS:CGraph3DApp]
Type=0
HeaderFile=Graph3D.h
ImplementationFile=Graph3D.cpp
Filter=N

[CLS:CChildView]
Type=0
HeaderFile=ChildView.h
ImplementationFile=ChildView.cpp
Filter=C
BaseClass=CWnd 
VirtualFilter=WC
LastObject=CChildView

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=Graph3D.cpp
ImplementationFile=Graph3D.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_APP_EXIT
Command2=ID_EDIT_UNDO
Command3=ID_EDIT_CUT
Command4=ID_EDIT_COPY
Command5=ID_EDIT_PASTE
Command6=ID_VIEW_TOOLBAR
Command7=ID_APP_ABOUT
CommandCount=7

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_EDIT_PASTE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_CUT
Command10=ID_EDIT_UNDO
CommandCount=10

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_CUT
Command2=ID_EDIT_COPY
Command3=ID_EDIT_PASTE
Command4=ID_RESET
Command5=ID_APP_ABOUT
CommandCount=5

[CLS:CSetView]
Type=0
HeaderFile=SetView.h
ImplementationFile=SetView.cpp
BaseClass=CFormView
Filter=W
VirtualFilter=VWC
LastObject=ID_RESET

[DLG:IDD_DIALOG1]
Type=1
Class=CSetView
ControlCount=30
Control1=IDC_STATIC,static,1342308352
Control2=IDC_SLIDER1,msctls_trackbar32,1342242873
Control3=IDC_SLIDER2,msctls_trackbar32,1342242872
Control4=IDC_SLIDER3,msctls_trackbar32,1342242872
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATICX,static,1342308352
Control9=IDC_STATICY,static,1342308352
Control10=IDC_STATICZ,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_SLIDER4,msctls_trackbar32,1342242873
Control13=IDC_SLIDER5,msctls_trackbar32,1342242872
Control14=IDC_SLIDER6,msctls_trackbar32,1342242872
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATICX2,static,1342308352
Control19=IDC_STATICY2,static,1342308352
Control20=IDC_STATICZ2,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_SLIDER7,msctls_trackbar32,1342242873
Control23=IDC_SLIDER8,msctls_trackbar32,1342242872
Control24=IDC_SLIDER9,msctls_trackbar32,1342242872
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_STATIC,static,1342308352
Control28=IDC_STATICX3,static,1342308352
Control29=IDC_STATICY3,static,1342308352
Control30=IDC_STATICZ3,static,1342308352


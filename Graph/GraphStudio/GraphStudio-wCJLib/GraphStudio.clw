; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CVasView
LastTemplate=CScrollView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "GraphStudio.h"
LastPage=0

ClassCount=11
Class1=CGraphStudioApp
Class3=CMainFrame
Class4=CAboutDlg

ResourceCount=8
Resource1=IDR_VAS_TOOLBAR
Class2=CChildView
Class5=CPrjListView
Class6=CTeapotView
Resource2=IDR_3D_TOOLBAR
Class7=CVasView
Resource3=IDR_MAINFRAME
Class8=CScanLineView
Resource4=IDD_ABOUTBOX
Class9=C3DView
Resource5=IDR_TEAPOT_TOOLBAR
Resource6=IDR_SCAN_TOOLBAR
Class10=C3DSetup
Class11=CFloatView
Resource7=IDR_FLOAT_TOOLBAR
Resource8=IDD_3DSETUP

[CLS:CGraphStudioApp]
Type=0
HeaderFile=GraphStudio.h
ImplementationFile=GraphStudio.cpp
Filter=N
LastObject=CGraphStudioApp

[CLS:CChildView]
Type=0
HeaderFile=ChildView.h
ImplementationFile=ChildView.cpp
Filter=C
BaseClass=CHtmlView 
VirtualFilter=WC

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=C
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=CMainFrame




[CLS:CAboutDlg]
Type=0
HeaderFile=GraphStudio.cpp
ImplementationFile=GraphStudio.cpp
Filter=D

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
Command1=ID_SAVE_BMP
Command2=ID_APP_EXIT
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_VIEW_TOOLBAR
Command8=ID_VIEW_STATUS_BAR
Command9=ID_VIEW_PRJ
Command10=ID_APP_ABOUT
CommandCount=10

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
Command1=ID_SAVE_BMP
Command2=ID_EDIT_COPY
Command3=ID_EDIT_PASTE
Command4=ID_APP_ABOUT
CommandCount=4

[CLS:CPrjListView]
Type=0
HeaderFile=PrjListView.h
ImplementationFile=PrjListView.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC
LastObject=CPrjListView

[CLS:CTeapotView]
Type=0
HeaderFile=TeapotView.h
ImplementationFile=TeapotView.cpp
BaseClass=CScrollView
Filter=W
VirtualFilter=WC
LastObject=ID_START

[TB:IDR_TEAPOT_TOOLBAR]
Type=1
Class=?
Command1=ID_TEAPOT_X
Command2=ID_TEAPOT_Y
CommandCount=2

[CLS:CVasView]
Type=0
HeaderFile=VasView.h
ImplementationFile=VasView.cpp
BaseClass=CScrollView
Filter=C
VirtualFilter=VWC
LastObject=CVasView

[TB:IDR_VAS_TOOLBAR]
Type=1
Class=?
Command1=ID_OPEN_BMP
Command2=ID_ANTI_BMP
CommandCount=2

[CLS:CScanLineView]
Type=0
HeaderFile=ScanLineView.h
ImplementationFile=ScanLineView.cpp
BaseClass=CScrollView
Filter=C
VirtualFilter=VWC
LastObject=CScanLineView

[TB:IDR_SCAN_TOOLBAR]
Type=1
Class=?
Command1=ID_START
Command2=ID_STOP
Command3=ID_FRONTCLR
Command4=ID_BACKCLR
CommandCount=4

[CLS:C3DView]
Type=0
HeaderFile=3DView.h
ImplementationFile=3DView.cpp
BaseClass=CScrollView
Filter=C
VirtualFilter=VWC
LastObject=ID_SHOW_SETUP

[TB:IDR_3D_TOOLBAR]
Type=1
Class=?
Command1=ID_SHOW_SETUP
CommandCount=1

[DLG:IDD_3DSETUP]
Type=1
Class=C3DSetup
ControlCount=32
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_SLIDER1,msctls_trackbar32,1342242841
Control6=IDC_SLIDER2,msctls_trackbar32,1342242841
Control7=IDC_SLIDER3,msctls_trackbar32,1342242841
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_SLIDER4,msctls_trackbar32,1342242841
Control13=IDC_SLIDER5,msctls_trackbar32,1342242841
Control14=IDC_SLIDER6,msctls_trackbar32,1342242841
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_SLIDER7,msctls_trackbar32,1342242841
Control20=IDC_SLIDER8,msctls_trackbar32,1342242841
Control21=IDC_SLIDER9,msctls_trackbar32,1342242841
Control22=IDC_STATICX,static,1342308352
Control23=IDC_STATICY,static,1342308352
Control24=IDC_STATICZ,static,1342308352
Control25=IDC_STATICX2,static,1342308352
Control26=IDC_STATICY2,static,1342308352
Control27=IDC_STATICZ2,static,1342308352
Control28=IDC_STATICX3,static,1342308352
Control29=IDC_STATICY3,static,1342308352
Control30=IDC_STATICZ3,static,1342308352
Control31=IDC_RESET,button,1342242816
Control32=IDC_STATIC,static,1342308352

[CLS:C3DSetup]
Type=0
HeaderFile=3DSetup.h
ImplementationFile=3DSetup.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_RESET
VirtualFilter=dWC

[CLS:CFloatView]
Type=0
HeaderFile=FloatView.h
ImplementationFile=FloatView.cpp
BaseClass=CScrollView
Filter=C
VirtualFilter=VWC
LastObject=CFloatView

[TB:IDR_FLOAT_TOOLBAR]
Type=1
Class=?
Command1=ID_PROPERTY
CommandCount=1


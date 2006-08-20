; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CChildView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "graph.h"
LastPage=0

ClassCount=5
Class1=CChildFrame
Class2=CChildView
Class3=CGraphApp
Class4=CAboutDlg
Class5=CMainFrame

ResourceCount=3
Resource1=IDR_GRAPHTYPE
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
VirtualFilter=mfWC

[CLS:CChildView]
Type=0
BaseClass=CWnd 
HeaderFile=ChildView.h
ImplementationFile=ChildView.cpp
Filter=W
VirtualFilter=WC
LastObject=ID_BACKCLR

[CLS:CGraphApp]
Type=0
BaseClass=CWinApp
HeaderFile=Graph.h
ImplementationFile=Graph.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=Graph.cpp
ImplementationFile=Graph.cpp
LastObject=CAboutDlg

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_START
Command3=ID_STOP
Command4=ID_FRONTCLR
Command5=ID_BACKCLR
Command6=ID_APP_ABOUT
CommandCount=6

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_APP_EXIT
Command3=ID_VIEW_TOOLBAR
Command4=ID_APP_ABOUT
CommandCount=4

[MNU:IDR_GRAPHTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_CLOSE
Command3=ID_APP_EXIT
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_VIEW_TOOLBAR
Command9=ID_WINDOW_CASCADE
Command10=ID_WINDOW_TILE_HORZ
Command11=ID_WINDOW_ARRANGE
Command12=ID_APP_ABOUT
CommandCount=12

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_EDIT_PASTE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_NEXT_PANE
Command7=ID_PREV_PANE
Command8=ID_EDIT_COPY
Command9=ID_EDIT_PASTE
Command10=ID_EDIT_CUT
Command11=ID_EDIT_UNDO
CommandCount=11


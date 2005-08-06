/*
*             iEnhance 扩展头文件           
*
* 本头文件规定了扩展iEnhance的程序必须实现和可选实现的函数接口声明
* 
* iEnhance扩展程序在iEnhance主界面菜单中显示字符串来自于资源中FileDescription段落
*
* EventSpy源程序给出了一个iEnhance扩展程序的实现.
*/

#ifdef IENH_EXT_EXPORTS
#define IENH_EXT_API __declspec(dllexport)
#else
#define IENH_EXT_API __declspec(dllimport)
#endif
#define PREFIX	extern "C" IENH_EXT_API

// 当程序不需要使用IE事件(绝大多数情况下是要退出时), 向iEnhance主窗口发送本消息
// WPARAM 为iEnhance分配给程序的会话ID
// LPARAM 为0
#define WM_IENHEXT_CLOSESESSION		WM_USER+111

// 必须实现的函数, 否则将不被认为是合法的iEnhance扩展
// 当用户选择运行扩展程序时,iEnhance将首先调用本函数
// 参数说明如下:
// HWND hwndHost  --  iEnhance的主窗口句柄, 通常在发送WM_IENHEXT_CLOSESESSION消息时使用
// HWND hwndFrame --  Internet Explorer的主窗口句柄, 通常作为程序显示的窗口的父窗口
// void* pFrameWB --  IWebBrowser2接口的指针, 可以将其强制转换成IWebBrowser2*,以使用其中方法
// int sess       --  iEnhance分配给程序的会话ID,在发送WM_IENHEXT_CLOSESESSION时使用
// 返回值说明:
// 需要接收和处理Internet Explorer消息则返回TRUE,否则返回FALSE
PREFIX BOOL Create(HWND hwndHost,HWND hwndFrame,void* pFrameWB,int sess);

// 以下函数为可选的实现, 未来版本将加入Internet Explorer 6.0新加入的事件类型
// 如果程序需要处理IE事件,则iEnhance将试图获取对应的事件处理函数,并调用. 
// 函数原型和通常的开发文档给出的相同, 具体说明可以参照开发文档
PREFIX void OnBeforeNavigate2(IDispatch* pDisp,VARIANT* Url,VARIANT* Flags,
									VARIANT* TargetFrameName,VARIANT* PostData,
									VARIANT* Headers,VARIANT_BOOL* Cancel);

PREFIX void OnCommandStateChange(long Command,VARIANT_BOOL Enable);

PREFIX void OnDocumentComplete(IDispatch* pDisp,VARIANT* URL);

PREFIX void OnDownloadBegin();

PREFIX void OnDownloadComplete();

PREFIX void OnNavigateComplete2(IDispatch* pDisp,VARIANT* URL);

PREFIX void OnNewWindow2(IDispatch** ppDisp,VARIANT_BOOL* Cancel);

PREFIX void OnFullScreen(VARIANT_BOOL FullScreen);

PREFIX void OnMenuBar(VARIANT_BOOL MenuBar);

PREFIX void OnQuit();

PREFIX void OnStatusBar(VARIANT_BOOL StatusBar);

PREFIX void OnTheaterMode(VARIANT_BOOL TheaterMode);

PREFIX void OnToolBar(VARIANT_BOOL ToolBar);

PREFIX void OnVisible(VARIANT_BOOL Visible);

PREFIX void OnProgressChange(long Progress,long ProgressMax);

PREFIX void OnPropertyChange(BSTR szProperty);

PREFIX void OnStatusTextChange(BSTR Text);

PREFIX void OnTitleChange(BSTR Text);				 
						   

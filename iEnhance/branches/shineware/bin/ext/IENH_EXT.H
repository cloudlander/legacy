/*
*             iEnhance ��չͷ�ļ�           
*
* ��ͷ�ļ��涨����չiEnhance�ĳ������ʵ�ֺͿ�ѡʵ�ֵĺ����ӿ�����
* 
* iEnhance��չ������iEnhance������˵�����ʾ�ַ�����������Դ��FileDescription����
*
* EventSpyԴ���������һ��iEnhance��չ�����ʵ��.
*/

#ifdef IENH_EXT_EXPORTS
#define IENH_EXT_API __declspec(dllexport)
#else
#define IENH_EXT_API __declspec(dllimport)
#endif
#define PREFIX	extern "C" IENH_EXT_API

// ��������Ҫʹ��IE�¼�(��������������Ҫ�˳�ʱ), ��iEnhance�����ڷ��ͱ���Ϣ
// WPARAM ΪiEnhance���������ĻỰID
// LPARAM Ϊ0
#define WM_IENHEXT_CLOSESESSION		WM_USER+111

// ����ʵ�ֵĺ���, ���򽫲�����Ϊ�ǺϷ���iEnhance��չ
// ���û�ѡ��������չ����ʱ,iEnhance�����ȵ��ñ�����
// ����˵������:
// HWND hwndHost  --  iEnhance�������ھ��, ͨ���ڷ���WM_IENHEXT_CLOSESESSION��Ϣʱʹ��
// HWND hwndFrame --  Internet Explorer�������ھ��, ͨ����Ϊ������ʾ�Ĵ��ڵĸ�����
// void* pFrameWB --  IWebBrowser2�ӿڵ�ָ��, ���Խ���ǿ��ת����IWebBrowser2*,��ʹ�����з���
// int sess       --  iEnhance���������ĻỰID,�ڷ���WM_IENHEXT_CLOSESESSIONʱʹ��
// ����ֵ˵��:
// ��Ҫ���պʹ���Internet Explorer��Ϣ�򷵻�TRUE,���򷵻�FALSE
PREFIX BOOL Create(HWND hwndHost,HWND hwndFrame,void* pFrameWB,int sess);

// ���º���Ϊ��ѡ��ʵ��, δ���汾������Internet Explorer 6.0�¼�����¼�����
// ���������Ҫ����IE�¼�,��iEnhance����ͼ��ȡ��Ӧ���¼���������,������. 
// ����ԭ�ͺ�ͨ���Ŀ����ĵ���������ͬ, ����˵�����Բ��տ����ĵ�
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
						   
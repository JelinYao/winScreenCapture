# winScreenCapture
Windows上截屏小工具，DLL形式提供接口调用，带使用例子。


使用方法

  // TODO: 在此添加控件通知处理程序代码
  
	HINSTANCE hInst=LoadLibrary(_T("ScreenCaptureDll.dll"));
  
	if(hInst)
  
	{
  
		typedef void (*MYFUNC)();
    
		MYFUNC fun=NULL;
    
		fun=(MYFUNC)GetProcAddress(hInst,"InitCaptureDlg");
    
		if(fun) fun();
    
		FreeLibrary(hInst);
    
	}

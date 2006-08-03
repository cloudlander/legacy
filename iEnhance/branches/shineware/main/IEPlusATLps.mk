
IEPlusATLps.dll: dlldata.obj IEPlusATL_p.obj IEPlusATL_i.obj
	link /dll /out:IEPlusATLps.dll /def:IEPlusATLps.def /entry:DllMain dlldata.obj IEPlusATL_p.obj IEPlusATL_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del IEPlusATLps.dll
	@del IEPlusATLps.lib
	@del IEPlusATLps.exp
	@del dlldata.obj
	@del IEPlusATL_p.obj
	@del IEPlusATL_i.obj

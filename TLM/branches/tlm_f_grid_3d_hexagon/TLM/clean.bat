echo off
for /D %%v in (EX EY EZ HX HY HZ) do call :clean %%v
del GAUSS\* 
del GAUSS_SIN\* /Q
del ONLYSIN\*
del ONLYSIN_3D\* /Q
del HYBRID\*
goto :end

:clean
del %1_MAP\* /Q
del %1_3D\* /Q
del %1\*
:end

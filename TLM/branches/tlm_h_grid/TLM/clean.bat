echo off
for /D %%v in (HY) do call :clean %%v
del GAUSS_SIN\*
del GAUSS\* /Q
del ONLYSIN\*
del ONLYSIN_3D\* /Q
goto :end

:clean
del %1_MAP\* /Q
del %1_3D\* /Q
del %1\*
:end

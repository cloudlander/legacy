echo off
for /D %%v in (EY) do call :clean %%v
REM del GAUSS_SIN\*
REM del GAUSS\* /Q
goto :end

:clean
del %1_MAP\* /Q
del %1_3D\* /Q
del %1\*
:end

for /D %%v in (HX HY HZ) do call :gen %%v
if exist "GAUSS_SIN\IMG    1.png" (
.\ls.exe GAUSS_SIN\*.png > list
start "GAUSS_SIN" /WAIT mencoder\mencoder mf://@list -mf w=1024:h=768:fps=25:type=png -vf scale -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -o gauss_sin.avi
)
if exist "ONLYSIN_3D\IMG    1.png" (
.\ls.exe ONLYSIN_3D\*.png > list
start "ONLY_SIN" /WAIT mencoder\mencoder mf://@list -mf w=1024:h=768:fps=25:type=png -vf scale -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -o only_sin.avi
)
echo set terminal png size 1024,768 >> set.gnu
echo set output "gausA.png" >> set.gnu
echo load 'set.gnu' > gausA.cmd
echo splot 'gausA.out' >> gausA.cmd
wgnuplot gausA.cmd
del gausA.cmd
del list

REM transfrom grid ppm to jpg:
.\convert.exe grid.ppm -quality 100 grid.jpg
del grid.ppm
goto :end

:gen
if exist "%1_MAP\IMG    1.png" (
.\ls.exe %1_MAP\*.png > list
start "%1_MAP" /WAIT mencoder\mencoder mf://@list -mf w=1024:h=768:fps=25:type=png -vf scale -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -o %1_map_color.avi
)
if exist "%1_3D\IMG    1.png" (
.\ls.exe %1_3D\*.png > list
start "%1_3D" /WAIT mencoder\mencoder mf://@list -mf w=1024:h=768:fps=25:type=png -vf scale -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -o %1_3d_color.avi
)
:end

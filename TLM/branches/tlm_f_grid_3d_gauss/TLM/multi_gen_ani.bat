for /D %%v in (EX EY EZ HX HY HZ) do call :gen %%v
cd GAUSS_SIN
start "GAUSS_SIN" ..\mencoder\mencoder mf://*.png -mf w=1024:h=768:fps=25:type=png -vf scale -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -o ..\gauss_sin.avi
goto :end

:gen
cd %1_MAP
start "%1_MAP" ..\mencoder\mencoder mf://*.png -mf w=1024:h=768:fps=25:type=png -vf scale -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -o ..\%1_map_color.avi
cd ..\%1_3D
start "%1_3D" ..\mencoder\mencoder mf://*.png -mf w=1024:h=768:fps=25:type=png -vf scale -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -o ..\%1_3d_color.avi
cd ..
:end

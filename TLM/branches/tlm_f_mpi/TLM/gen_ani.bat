for /D %%v in (EY) do call :gen %%v
REM cd GAUSS_SIN
REM start "GAUSS_SIN" /WAIT ..\mencoder\mencoder mf://*.png -mf w=1024:h=768:fps=25:type=png -vf scale -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -o ..\gauss_sin.avi
goto :end

:gen
cd %1_MAP
start "%1_MAP" /WAIT ..\mencoder\mencoder mf://*.png -mf w=1024:h=768:fps=25:type=png -vf scale -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -o ..\%1_map_color.avi
cd ..\%1_3D
start "%1_3D" /WAIT ..\mencoder\mencoder mf://*.png -mf w=1024:h=768:fps=25:type=png -vf scale -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -o ..\%1_3d_color.avi
cd ..
:end

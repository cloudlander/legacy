cd EY_MAP
..\mencoder\mencoder mf://*.png -mf w=1024:h=768:fps=25:type=png -vf palette,scale -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -o ..\map_truecolor.avi
..\mencoder\mencoder mf://*.png -mf w=1024:h=768:fps=25:type=png -vf scale -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -o ..\map_color.avi

cd ..\EY_3D
..\mencoder\mencoder mf://*.png -mf w=1024:h=768:fps=25:type=png -vf palette,scale -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -o ..\3d_truecolor.avi
..\mencoder\mencoder mf://*.png -mf w=1024:h=768:fps=25:type=png -vf scale -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -o ..\3d_color.avi
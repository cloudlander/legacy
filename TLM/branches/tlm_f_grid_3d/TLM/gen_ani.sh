#!/bin/sh

for dir in EX EY EZ HX HY HZ
do
  cd ${dir}_MAP
  mencoder mf://*.png -mf w=1024:h=768:fps=25:type=png -vf scale -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -o ../${dir}_map_color.avi
  cd ../${dir}_3D
  mencoder mf://*.png -mf w=1024:h=768:fps=25:type=png -vf scale -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -o ../${dir}_3d_color.avi
  cd ..
done

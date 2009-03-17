                                    TLM使用说明

0.  系统需求:
    Windows XP(含) 以上操作系统  (需安装Python 2.4)
    或者
    Linux (含Python2.4,  gnuplot,  mencoder,  mplayer)
 
1.  文件目录结构:
    解压缩后的目录中含有:
    \                                                                                
        TLM\                                                    TLM主程序目录                                       
                Release\TLM.exe                                      TLM可执行程序
                EY\                                                          存放EY的数据文件
                EY_3D\                                                     根据EY数据绘制的3D图像
                EY_MAP\                                                  根据EY数据绘制的2D图像
                FEY\                                                        存放FEY的数据文件
                TLM.f90                                                    TLM的Fortran90源程序
                twotlme.in                                                TLM的输入文件
                gen_ani.bat                                               根据3D和2D图像生成动画的程序(不要修改,用于Windows)
                gen_ani.sh                                                 根据3D和2D图像生成动画的程序(不要修改,用于Linux)
                README.txt                                              本说明文件
                TLM.py                                                      TLM的批处理执行程序
                TLM.dsp                                                    Compac Fortran 6.5的项目文件(不要直接打开)
                TLM.vfproj                                                 Visual Studio 2005的项目文件(需要安装Intel Fortran Compiler,不要直接打开)
                mencoder\                                                 mencoder程序(不要删除和改动)
                gnuplot.exe                                                gnuplot程序(不要删除和改动)
                wgnuplot.exe                                             gnuplot程序(不要删除和改动)
        TLM_F.dsw                                           Compac Fortran 6.5的项目文件(可以直接打开)
        TLM_F.opt                                            项目文件(不要直接打开)
        TLM_F.sln                                             Visual Studio 2005的项目文件(可以直接打开)
        TLM_F.suo                                            项目文件(不要直接打开)


2. 使用步骤:
    0.  如何查看X-Z空间介质分布
         用Compaq Fortran打开dsw,点Run运行TLM.exe，等待出现10之后,按CTRL-C中止TLM
         双击wgnuplot.exe,键入以下命令:
            load 'set.gnu'
            set pm3d map
            set cbrange [-1:2]
            set zrange [-1:2]
            splot 'grid.dat'
    1.  修改源程序:
         如果需要编辑源程序,请双击TLM_F.dsw(如果已安装Compac Fortran6.5)或者TLM_F.sln(如果已安装Visual Studio 2005以及Intel Fortran Compiler)
         编辑完TLM.f90源程序后,根据实际需要编译成Release版本或者Debug版本
    2.  根据实际需要修改twotlme.in    (具体的修改参数参考后面叙述)
    3.  有两种运行方式:
         A.      仅运行TLM程序,不生成图像以及动画
                  在开发环境中单击Run按钮进行计算
                  
         B.      运行TLM程序的同时生成图像和动画
                  双击tlm.py即可
         

3.  twotlme.in配置文件修改说明:
     待续
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
    3.  运行方式:
         可以配置TLM.py程序控制整个TLM计算和绘图的过程已经绘图参数,下面是详细的说明:
         以下列出的是TLM.py中可以配置的程序段(可以用Notepad打开TLM.py进行修改
          self._config['threads']=2                                      控制每种绘图的线程数目(如果线程数为2,则如果同时要得到三维和二维的图像,系统总共会启动4个线程绘图.一般来说,系统总共启动的线程数目不能大于机器的CPU核心数量,否则性能反而会下降)
          self._config['PNG_SIZE']='1024,768'                     绘图输出文件的分辨率
          self._config['+RANGE']=0.3                                 EY的上限
          self._config['-RANGE']=-0.3                                 EY的下限
          self._config['+G_RANGE']=1.0                             Gauss分布的上限(仅对TLM Grid程序有效)
          self._config['-G_RANGE']=-1.0                             Gauss分布的下限(仅对TLM Grid程序有效)
          self._config['TLM']=True                                     是否运行TLM程序生成数据文件(在已经有数据文件的情况下,可以设置成False,这样不用重复计算,也可以绘图)
          self._config['MAP']=True                                    是否绘制二维图像
          self._config['SURFACE']=True                             是否绘制三维图像
          self._config['GAUSS']=False                                是否绘制Gauss分布图像(仅对TLM Grid程序有效,如果设置成True,TLM NRI程序会报错!)
          self._config['ANI']=True                                      是否生成二维和三维的动画
 
     4.  频域计算工具:
          在得到所有的输出数据后,可以双击tlm_dft.py,输入需要计算的X,Z坐标,即可得到相应的频域输出

     5.  其他有用的工具:
          如果我们需要看看当TLM运行一段时间之后输出数据的效果,就可以双击gen_ani.bat或者multi_gen_ani.bat得到相应的二维和三维动画,gen_ani.bat适用于较慢的机器,multi_gen_ani.bat并行的生成动画,所以适合于较快的多核机器
          clean.bat可以用于清除所有的输出数据文件和图像文件

3.  twotlme.in配置文件修改说明:
     待续
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
                wgnuplot.exe                                              gnuplot程序(不要删除和改动)
                其他.dll,.exe和.bat文件                                   批处理程序需要使用(不要删除和改动)
        TLM_F.dsw                                           Compac Fortran 6.5的项目文件(可以直接打开)
        TLM_F.opt                                            项目文件(不要直接打开)
        TLM_F.sln                                             Visual Studio 2005的项目文件(可以直接打开)
        TLM_F.suo                                            项目文件(不要直接打开)


2. 使用步骤:
    0.  如何查看X-Z空间介质分布: 
         用Compaq Fortran打开dsw,点Run运行TLM.exe，等待出现10之后,按CTRL-C中止TLM
         用ACDSee或者Iranview打开生成的grid.ppm图像文件即可
        
    1.  修改源程序:
         如果需要编辑源程序,请双击TLM_F.dsw(如果已安装Compac Fortran6.5)或者TLM_F.sln(如果已安装Visual Studio 2005以及Intel Fortran Compiler)
         编辑完TLM.f90源程序后,根据实际需要编译成Release版本或者Debug版本
         其中Release版本适合在批处理时使用,速度较快.Debug版本仅适合调试时在Visual Fortran环境中使用

    2.  根据实际需要修改twotlme.in    (具体的修改参数参考后面叙述)

    3.  运行方式:
         可以配置TLM.py程序控制整个TLM计算和绘图的过程已经绘图参数,下面是详细的说明:
         以下列出的是TLM.py中可以配置的程序段(可以用Notepad打开TLM.py进行修改
          self._config['threads']=2                   控制每种绘图的线程数目
                                                      (如果线程数为2,则如果同时要得到三维和二维的图像,
                                                       系统总共会启动4个线程绘图.一般来说,系统总共启动
                                                       的线程数目不能大于机器的CPU核心数量,否则性能反而
                                                       会下降.对一般的双核计算机来说,设置成2即可)
          self._config['PNG_SIZE']='1024,768'         绘图输出文件的分辨率
          self._config['+RANGE']=0.3                  EY的上限
          self._config['-RANGE']=-0.3                 EY的下限
          self._config['+G_RANGE']=1.0                Gauss分布的上限(仅对TLM Grid程序有效)
          self._config['-G_RANGE']=-1.0               Gauss分布的下限(仅对TLM Grid程序有效)
          self._config['TLM']=True                    是否运行TLM程序生成数据文件
                                                      (在已经有数据文件的情况下,可以设置成False,
                                                       这样不用重复计算,也可以绘图)
          self._config['MAP']=True                    是否绘制二维图像
          self._config['SURFACE']=True                是否绘制三维图像
          self._config['GAUSS']=False                 是否绘制Gauss分布图像
                                                      (仅对TLM Grid程序有效,否则,如果设置成True,TLM NRI程序会报错!)
          self._config['ONLY_SIN']=False               是否绘制SIN激励分布图像
                                                      (仅对TLM Grid程序有效,否则,如果设置成True,TLM NRI程序会报错!)
          self._config['SHOW_MEDIUM']=True            是否在绘制的图像上显示介质分布
          self._config['ANI']=True                    是否生成二维和三维的动画
 
     4.  频域计算工具:
          在得到所有的输出数据后,可以双击tlm_dft.py,输入需要计算的X,Z坐标,即可得到相应的频域输出

     5.  其他有用的工具:
          如果我们需要看看当TLM运行一段时间之后输出数据的效果,就可以双击gen_ani.bat得到相应的二维和三维动画,          
          clean.bat可以用于清除所有的输出数据文件和图像文件(清除前会要求确认),最终的动画文件不会被删除

3.  twotlme.in配置文件修改说明:
    TLM程序以如下顺序读入配置文件中的数据:
    SX,SY,SZ,                 模拟三维空间的起始坐标
    ENDX,ENDY,ENDZ,           模拟三维空间的终止坐标
    X,Y,Z,                    X和Z无意义，Y指定用于模拟的Y平面
    U,V,W,H,ER,UR,SGM,        物理参数，其中ER为三维空间的默认ER值
    NT,BL,                    NT为模拟次数 BL为边界参数
    SPLIT,                    SPLIT=1 按T分割输出文件, SPLIT=0 不分割,默认为1,为使程序正确工作,请确保SPLIT=1
    SIDE1,SIDE2,              介质网格的高和宽
    ER1,ER2,                  介质ER设置

    NANGLE,                   介质网格偏移角度(与Z轴张角,取值范围为(0,180) 默认值90为不倾斜
                              (本参数已不再使用,可填任意值对程序不会有任何影响,建议保留为默认值90)

    GTYPE,                    介质网格的形状:
                              0为矩形, 1为内切椭圆(正圆), 2为等腰三角形(网格为棋盘形式,ER交错)
                              3为蜂窝状六边形(程序将4x4共16个网格绘制成六边形,如需要正六边形,则请
                              确保SIDE1/SIDE2=SQRT(3)/2,ER1填充蜂窝空隙,ER2填充所有的六边形)

    COL,                      当GTYPE为3(要求蜂窝状六边形时),该值为4*SIDE2的小数倍 
                              其他情况下,该值为棋盘网格的列数

    FRE,NORMAL_FRE,           FRE不再使用,NORMAL_FRE为激励源的归一化频率

    W0,                       高斯分布参数(高斯分布功能默认关闭,如需打开请使用Visual Fortran打开源程序,修改第400行
                              !              IVB(III,I,J,K)=GAUSS(I,J,K)*IVB(III,I,J,K)
                              把注释符!去掉,即可打开高斯分布

    PN,                       PN=0时: 使用PSX,PSZ,PENDZ自动生成脉冲直线,直线旋转PANGLE角度
                              PN>0时: 使用输入文件的X,Z坐标作为脉冲点
                              (X,Z坐标从twotlme.in文件的第二行开始,每一行设置一个激励点位置)

    PSX,PSZ,PENDZ,PANGLE,     PSX,PSZ为输入脉冲开始位置,PENDZ为输入脉冲结束的Z坐标
                              PANGLE为输入脉冲直线与Z轴张角(角度参数)
                              (以上参数仅当PN=0时有效)

    LN,                       twotlme.in文件中指定的频域点数量,所有频域点坐标在脉冲坐标行之下

    BP,                       设置Z坐标为吸收边界,该参数默认关闭,如需打开请使用Visual Fortran打开源程序,修改第450行
                              !IVB(2,I,J,K)=0
                              把注释符!去掉,即可打开该功能

    SCALE                     如果为0,则根据PN的取值确定激励点,否则设置一个激励点,其位置有SCALE确定
                              (激励点距离介质=介质宽度/SCALE)

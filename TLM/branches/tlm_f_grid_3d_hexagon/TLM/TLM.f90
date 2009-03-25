      PROGRAM TWOTLME
      INTEGER SX,SY,SZ,ENDX,ENDY,ENDZ,NX,NY,NZ,X,Y,Z,T,BL,SIDE1,SIDE2   
!网格单位长度，起始网格位置结束网格位置，边界条件标志参数

      REAL, ALLOCATABLE:: RVA(:,:,:,:),RVB(:,:,:,:),RVC(:,:,:,:)
      REAL, ALLOCATABLE:: RVD(:,:,:,:),RVE(:,:,:,:),RVF(:,:,:,:)
      REAL, ALLOCATABLE:: IVA(:,:,:,:),IVB(:,:,:,:),IVC(:,:,:,:)
      REAL, ALLOCATABLE:: IVD(:,:,:,:),IVE(:,:,:,:),IVF(:,:,:,:)

      REAL, ALLOCATABLE:: REEX(:,:,:,:),IMEX(:,:,:,:)
      REAL, ALLOCATABLE:: REEY(:,:,:,:),IMEY(:,:,:,:)
      REAL, ALLOCATABLE:: REEZ(:,:,:,:),IMEZ(:,:,:,:)
      REAL, ALLOCATABLE:: REHX(:,:,:,:),IMHX(:,:,:,:)
      REAL, ALLOCATABLE:: REHY(:,:,:,:),IMHY(:,:,:,:)
      REAL, ALLOCATABLE:: REHZ(:,:,:,:),IMHZ(:,:,:,:)
	  REAL, ALLOcATABLE:: EH(:,:,:)
      INTEGER, ALLOCATABLE:: L(:)
      REAL, ALLOCATABLE:: YY(:,:)
	  REAL YX,YZ
      INTEGER, ALLOCATABLE:: PULSE(:,:)   !脉冲点
      INTEGER, ALLOCATABLE:: YYY(:,:)      
      REAL, ALLOCATABLE:: GAUSS(:,:,:),GAUSS2(:,:,:)
      
      REAL SA(5,5),SB(5,5),SC(5,5),SD(5,5),SE(5,5),SF(5,5)
      
	  INTEGER P_COLOR,ER_COLOR,ER1_COLOR,ER2_COLOR
      CHARACTER(LEN=12) COLORS(0:3)  !介质颜色表

      REAL FF      
	  REAL time_begin, time_end
      INTEGER UNIT_BASE,UNIT_BASE2
      INTEGER FILE_UNIT(6)
      INTEGER FUI

	  INTEGER LN,LI,LL
      REAL ZX,ZY,ZZ
      REAL GX,GY,GZ
      REAL ER,ER1,ER2
	  INTEGER U,V,W
	  INTEGER SPLIT !SPLIT=1 按T分割输出文件, SPLIT=0 不分割
	  REAL FRE !激发频率
	  REAL NORMAL_FRE  !归一化频率

      INTEGER START_ROW,START_COL,END_COL,GRID_ROW,GRID_COL   !网格开始位置
      REAL COL  !网格总列数
      REAL PART  !网格所占百分比

      REAL NANGLE !网格偏移角度(与Z轴张角,取值范围为(0,180) 90为不倾斜
	  INTEGER DUMMY1,DUMMY2
      INTEGER GTYPE !网格类型 0为矩形, 1为内切椭圆(正圆), 2为等腰三角形
      INTEGER PSX,PSZ !输入脉冲开始位置
      INTEGER PENDZ !输入脉冲结束的Z坐标
      REAL PANGLE  !输入脉冲直线与Z轴的张角(角度)
      REAL PLENGTH !输入脉冲直线的长度
      REAL PP !脉冲直线计算参数
	  INTEGER CC
	  REAL PI
      !输入脉冲数组,以下为取值说明:
      !=0: 使用PSX,PSZ,PENDZ和PLENGTH自动生成脉冲直线,直线旋转PANGLE角度,并且生成2个效果文件
      !>0: 使用输入文件的X,Z坐标作为脉冲点,介质旋转PANGLE角度,生成一个效果文件
      INTEGER PN 
	  INTEGER PIND, PLEN !脉冲点序号和总数
    
	  !吸收边界的Z值 
      INTEGER BP
      !脉冲点距介质边界占介质宽度比例
      REAL SCALE
	  INTEGER EXPAND

	  REAL GMAX
	  REAL EMAX(1:6)
      REAL W0 !高斯函数光斑半径

      !是否输出对应的数据文件: 1为输出, 0为不输出
      INTEGER DO_EX,DO_EY,DO_EZ,DO_HX,DO_HY,DO_HZ,DO_FEX,DO_FEY,DO_FEZ,DO_FHX,DO_FHY,DO_FHZ
      
      CHARACTER(LEN=5) NAME_COUNT

      UNIT_BASE=100
      UNIT_BASE2=16384

      !介质颜色, 0:激励, 1:ER, 2:ER1, 3:ER2
      P_COLOR=0    
	  ER_COLOR=1
	  ER1_COLOR=2
	  ER2_COLOR=3

      COLORS(P_COLOR)="255 255 255     "   !激发为白色
      COLORS(ER_COLOR)="35 35 35 "          !默认为黑色(ER)
      COLORS(ER1_COLOR)="12 175 159     "    !ER1
	  COLORS(ER2_COLOR)="35 35 35     "       !ER2

      OPEN(11,FILE='twotlme.in',FORM='FORMATTED')
      
	  !读入数据固定参数计算结构参数时使用
	  READ(11,*)SX,SY,SZ,ENDX,ENDY,ENDZ,X,Y,Z,U,V,W,H,ER,UR,SGM,NT,BL,SPLIT,SIDE1,SIDE2,ER1,ER2,NANGLE,GTYPE,COL,FRE,NORMAL_FRE,W0,PN,PSX,PSZ,PENDZ,PANGLE,LN,BP,SCALE,DO_EX,DO_EY,DO_EZ,DO_HX,DO_HY,DO_HZ,DO_FEX,DO_FEY,DO_FEZ,DO_FHX,DO_FHY,DO_FHZ
      
      ! 初始化文件句柄
      DO 17950 FUI=1,6
        FILE_UNIT(FUI)=-1
17950 CONTINUE
      
      IF(SPLIT .EQ. 0) THEN
        IF(DO_EX .EQ. 1) THEN
          FILE_UNIT(1)=12        
          OPEN(12,FILE='EX.out',FORM='FORMATTED')
        ENDIF
        IF(DO_EY .EQ. 1) THEN
          FILE_UNIT(2)=13        
          OPEN(13,FILE='EY.out',FORM='FORMATTED')
        ENDIF
        IF(DO_EZ .EQ. 1) THEN        
          FILE_UNIT(3)=14        
          OPEN(14,FILE='EZ.out',FORM='FORMATTED')
        ENDIF
        IF(DO_HX .EQ. 1) THEN        
          FILE_UNIT(4)=15        
          OPEN(15,FILE='HX.out',FORM='FORMATTED')
        ENDIF
        IF(DO_HY .EQ. 1) THEN        
          FILE_UNIT(5)=16        
          OPEN(16,FILE='HY.out',FORM='FORMATTED')        
        ENDIF
        IF(DO_HZ .EQ. 1) THEN
          FILE_UNIT(6)=17                
          OPEN(17,FILE='HZ.out',FORM='FORMATTED')
        ENDIF
      ENDIF
      
      !set.gnu已无实际应用效果,具体调整在tlm.py中
      OPEN(15,FILE='set.gnu',FORM='FORMATTED')
      WRITE(15,*) "set samples 100, 100"
      WRITE(15,*) "set isosamples 10, 10"
      WRITE(15,*) "set autoscale"
      WRITE(15,*) "set size square"
      WRITE(15,*) "set surface"
      WRITE(15,*) 'set title "TLM 2D"'
      WRITE(15,*) 'set xlabel "Z"'
      WRITE(15,*) 'set ylabel "X"'
      WRITE(15,*) "set xrange [",SZ,":",ENDZ,"] noreverse nowriteback"
      WRITE(15,*) "set yrange [",SX,":",ENDX,"] noreverse nowriteback"
      WRITE(15,*) "set zrange [ -0.300000 : 0.300000 ] noreverse nowriteback"
      WRITE(15,*) "set cbrange [ -0.300000 : 0.300000 ] noreverse nowriteback"
      WRITE(15,*) "set zero 1e-0020"
      WRITE(15,*) "set pm3d map"
      !WRITE(15,*) "set style data dots"
	  !"set xrange" 对应（HY.out第一列），为介质空间Z轴； "set yrange"对应（HY.out第二列），为介质空间X轴
	  !"set zrange" 0.03变为1.03为了画图不出现白色块
	  !"set pm3d map"为2D画图，"set pm3d at s"为3D画图
	  CLOSE(15)
            
      U0=12566.3706144		!真空中的磁导率
      E0=8.854187818E-2		!真空中的介电常数
      Z0=SQRT(U0/E0)
      PI=3.1415926
      
      COL=COL*4
      PART=REAL(COL*SIDE2)/(ENDZ-SZ)
      START_ROW=SX
      START_COL=SZ+(ENDZ-SZ)*(1-PART)/2
	  IF(START_COL .LT. SZ) THEN  !检查介质范围是否有效
	    WRITE(*,*) "ERROR: MEDIUM OVERLAPPED THE SIMULATION AREA!"
		STOP
	  ENDIF
      GRID_ROW=(ENDX-SX+1)/SIDE1
      GRID_COL=(ENDZ-SZ)*PART/SIDE2
      ENDX=SX+GRID_ROW*SIDE1-1
	  END_COL=START_COL+GRID_COL*SIDE2-1

      NX=ENDX-SX+3
      NY=ENDY-SY+3
      NZ=ENDZ-SZ+3

      ALLOCATE (RVA(5,NX,NY,NZ),RVB(5,NX,NY,NZ),RVC(5,NX,NY,NZ))      
      ALLOCATE (RVD(5,NX,NY,NZ),RVE(5,NX,NY,NZ),RVF(5,NX,NY,NZ))
      ALLOCATE (IVA(5,NX,NY,NZ),IVB(5,NX,NY,NZ),IVC(5,NX,NY,NZ))
      ALLOCATE (IVD(5,NX,NY,NZ),IVE(5,NX,NY,NZ),IVF(5,NX,NY,NZ))
     
      ALLOCATE (REEX(10,NX,NY,NZ),IMEX(10,NX,NY,NZ))
      ALLOCATE (REEY(10,NX,NY,NZ),IMEY(10,NX,NY,NZ))
      ALLOCATE (REEZ(10,NX,NY,NZ),IMEZ(10,NX,NY,NZ))
      ALLOCATE (REHX(10,NX,NY,NZ),IMHX(10,NX,NY,NZ))
      ALLOCATE (REHY(10,NX,NY,NZ),IMHY(10,NX,NY,NZ))
      ALLOCATE (REHZ(10,NX,NY,NZ),IMHZ(10,NX,NY,NZ))
      ALLOCATE (GAUSS(NX,NY,NZ),GAUSS2(NX,NY,NZ))
	  ALLOCATE (EH(NX,NY,NZ))
      ALLOCATE (YY(NX,NZ))
      ALLOCATE (YYY(NX,NZ))
      ALLOCATE (PULSE(NX,3))

      !  初始化X-Z平面ER
	  YX=2*(V*W*SQRT(H)*ER/U-2)
	  YZ=2*(V*U*SQRT(H)*ER/W-2)
      DO 2221 II=SX,ENDX
         DO 2222 KK=SZ,ENDZ
            YY(II,KK)=2*(U*W*SQRT(H)*ER/V-2)  !介电常数支线结构参数的计算(方向5即为介电常数支线)
            YYY(II,KK)=ER_COLOR
 2222    CONTINUE
 2221 CONTINUE
 
      !开始绘制网格            
	  IF(GTYPE .NE. 3) THEN   !绘制矩形,椭圆和三角形
		  DO 3331 II=0,GRID_ROW-1
			 DO 3332 KK=0,GRID_COL-1
				IF ( MOD((II+KK),2) .EQ. 1 ) THEN
				   CALL COLOR(GTYPE,SIDE1,SIDE2,START_ROW+II*SIDE1,START_COL+KK*SIDE2,YY,YYY,NX,NZ,2*(U*W*SQRT(H)*ER1/V-2),ER1_COLOR, 0.8)
				ELSE
				   CALL COLOR(GTYPE,SIDE1,SIDE2,START_ROW+II*SIDE1,START_COL+KK*SIDE2,YY,YYY,NX,NZ,2*(U*W*SQRT(H)*ER2/V-2),ER2_COLOR, 1.0)
				ENDIF
	 3332    CONTINUE
	 3331 CONTINUE
	  ELSE
	                          !绘制蜂窝状六角形
		  DO 2223 II=SX,ENDX
			 DO 2224 KK=START_COL,END_COL
				YY(II,KK)=2*(U*W*SQRT(H)*ER1/V-2)  !介电常数支线结构参数的计算(方向5即为介电常数支线)
				YYY(II,KK)=ER1_COLOR
	 2224    CONTINUE
	 2223 CONTINUE

		  II=SX              
		  CC=0
		  IF(MOD(GRID_COL,4) .EQ. 0) THEN        !调整蜂窝起始位置,尽量模拟从中心向两边扩展的分布
             KKK=-SIDE2
	      ELSE IF(MOD(GRID_COL,4) .EQ. 1) THEN
		     KKK=-2*SIDE2
	      ELSE IF(MOD(GRID_COL,4) .EQ. 2) THEN
		     KKK=-3*SIDE2
	      ELSE IF(MOD(GRID_COL,4) .EQ. 3) THEN
		     KKK=-2*SIDE2
          ENDIF
		  KK=START_COL+KKK
		  DO WHILE(II .LE. ENDX)
			DO WHILE(KK .LE. END_COL)
			   CALL HEXAGON(GTYPE,SIDE1*4,SIDE2*4,II,KK,YY,YYY,NX,NZ,SX,ENDX,START_COL,END_COL,2*(U*W*SQRT(H)*ER2/V-2),ER2_COLOR, 0.8)
			   KK=KK+6*SIDE2
			ENDDO
			II=II+2*SIDE1
			CC=CC+2
			IF(MOD(CC,4) .EQ. 0) THEN
			   KK=START_COL+KKK 
			ELSE
			   KK=START_COL+KKK+3*SIDE2
			ENDIF
		  ENDDO
      ENDIF

      !读入脉冲点位置数组X,Z坐标
	  IF(SCALE .EQ. 0) THEN
		  IF(PN .GT. 0) THEN

			DO 230 PIND=1,PN
			  READ(11,*)PULSE(PIND,1),PULSE(PIND,3)
			  PULSE(PIND,2)=Y
	  230   CONTINUE
			PLEN=PN
        
		  ELSE IF(PN .EQ. 0) THEN
      
		  !  开始绘制脉冲空间位置(直线),仅当不使用输入文件中指点的脉冲点时自动绘制直线
			  CALL LINE(X,Y,Z,PSX,PSZ,PENDZ,SX,ENDX,PANGLE,YYY,NX,NZ,PULSE,PIND,PLEN,P_COLOR)      
          
		  ENDIF
      ELSE
	      DO 232 II=1,PN       !过滤掉所有冗余的脉冲点设置
		    READ(11,*) DUMMY1,DUMMY2
  232     CONTINUE
	      PLEN=1
		  PIND=1
		  PULSE(PIND,1)=ENDX/2
		  PULSE(PIND,2)=Y
		  PULSE(PIND,3)=START_COL-(GRID_COL*SIDE2/SCALE)
	  ENDIF

      !检验激励点的合法性
	  DO 231 PIND=1,PN
	    IF(PULSE(PIND,1) .GE. SX .AND. PULSE(PIND,1) .LE. ENDX .AND. PULSE(PIND,3) .GE. SZ .AND. PULSE(PIND,3) .LE. ENDZ) THEN
	  	  YYY(PULSE(PIND,1),PULSE(PIND,3))=P_COLOR
	    ELSE
		  WRITE(*,*) "ERROR: PULSE POINT NOT VALID!",PULSE(PIND,1),PULSE(PIND,3)
		  STOP
		ENDIF
  231 CONTINUE

      OPEN(132,FILE='ER.out',FORM='FORMATTED')
      DO 11113 I=SX,ENDX
        DO 11114 K=SZ,ENDZ        
           WRITE(132,*) K,I,YY(I,K)
11114   CONTINUE
        WRITE(132,*)
11113 CONTINUE
      CLOSE(132) 

      OPEN(133,FILE='screen.txt',FORM='FORMATTED')
	  WRITE(133,*) "START_COL:", START_COL
	  WRITE(133,*) "END_COL:", END_COL
	  WRITE(133,*) "GRID:", GRID_ROW,GRID_COL
	  WRITE(133,*) "MESH SIZE:", ENDX-SX+1, ENDZ-SZ+1
      WRITE(133,*) "PULSE POINTS:"
      DO 76 PIND=1,PLEN
        WRITE(133,*) PULSE(PIND,1),PULSE(PIND,3)
   76 CONTINUE
      CLOSE(133)

      EXPAND=1
!  输出全X-Z平面的实际计算效果图
      OPEN(111,FILE='grid.ppm',FORM='FORMATTED')
      WRITE (111,'(A2)')"P3"
      WRITE (111,'(5I )',advance='no') (ENDZ-SZ+1)*EXPAND
      WRITE (111,'(5I)') (ENDX-SX+1)*EXPAND
      WRITE (111,'(A3)')"255"
      DO 56 I=ENDX,SX,-1
         DO 67 K=SZ,ENDZ
            DO 156 II=1,EXPAND
               WRITE (111,'(A12)',advance='no') COLORS(YYY(I,K))
   156      CONTINUE
            
   67    CONTINUE
   56 CONTINUE
      WRITE (111,*)
      CLOSE(111)
      WRITE(*,*) "grid.ppm generated"
	  WRITE(*,*) "START_COL:", START_COL
	  WRITE(*,*) "END_COL:", END_COL
	  WRITE(*,*) "GRID:", GRID_ROW,GRID_COL
	  WRITE(*,*) "MESH SIZE:", ENDX-SX+1, ENDZ-SZ+1
      WRITE(*,*) "PULSE POINTS:"
      DO 77 PIND=1,PLEN
        WRITE(*,*) PULSE(PIND,1),PULSE(PIND,3)
   77 CONTINUE

      !读入频域点数组
	  ALLOCATE (L(LN))
      IF (LN .NE. 0) THEN
        DO 220 III=1,LN
          READ(11,*)L(III)
  220   CONTINUE
      ELSE
        LN=16384
      ENDIF
      CLOSE(11)
     
      ! 应用用高斯函数
      DO 1003 I=SX,ENDX
            DO 3003 K=SZ,ENDZ
			     GAUSS(I,Y,K)=0
			     GAUSS2(I,Y,K)=0
   3003     CONTINUE
   1003 CONTINUE
	  DO 87 PIND=1,PLEN
        CALL GENGAUSS(GAUSS2,NX,NY,NZ,PULSE(PIND,1),PULSE(PIND,2),PULSE(PIND,3),W0)
		  DO 1005 I=SX,ENDX
            DO 3005 K=SZ,ENDZ
			     GAUSS(I,Y,K)=GAUSS(I,Y,K)+GAUSS2(I,Y,K)
   3005     CONTINUE
   1005 CONTINUE
  87  CONTINUE

      GMAX=0

        DO 1007 I=SX,ENDX
            DO 3007 K=SZ,ENDZ
              IF(GAUSS(I,Y,K) .GT. GMAX) THEN
			     GMAX=GAUSS(I,Y,K)
			  ENDIF
   3007     CONTINUE
   1007 CONTINUE
      
	  WRITE(*,*) "GAUSS MAX IS:",GMAX

        DO 1006 I=SX,ENDX
            DO 3006 K=SZ,ENDZ
			     GAUSS(I,Y,K)=GAUSS(I,Y,K)/GMAX
   3006     CONTINUE
   1006 CONTINUE


      OPEN(131,FILE='gausA.out',FORM='FORMATTED')
      DO 11111 I=SX,ENDX
        DO 11112 K=SZ,ENDZ        
           WRITE(131,*) K,I,GAUSS(I,Y,K)
11112   CONTINUE
11111 CONTINUE
      CLOSE(131)

      ZX=2*(V*W*SQRT(H)*UR/U-2)
      ZY=2*(U*W*SQRT(H)*UR/V-2)
      ZZ=2*(V*U*SQRT(H)*UR/W-2)
      GX=SGM*V*W*Z0/U
      GY=SGM*U*W*Z0/V              !损耗支线结构参数计算[SGM取0记不计该Gjj参数（程序中由GY表示）]
      GZ=SGM*V*U*Z0/W
      
      CALL SCL(SD,ZX)              !生成并联结点散射矩阵
      CALL SCL(SE,ZY)	
      CALL SCL(SF,ZZ)	    

      DO 1 I=SX,ENDX           !初始化五个支线的输入向量，理解为五个不同的向量，分别代表物理上不同方向的支线
       DO 2 J=SY,ENDY
	    DO 3 K=SZ,ENDZ
	       DO 4 T=1,5   
	       		IVA(T,I,J,K)=0
		        IVB(T,I,J,K)=0
	       		IVC(T,I,J,K)=0
		        IVD(T,I,J,K)=0
	       		IVE(T,I,J,K)=0
		        IVF(T,I,J,K)=0
   4       CONTINUE
   3    CONTINUE
   2   CONTINUE
   1  CONTINUE
     
      DO 11 T=1,6
	    EMAX(T)=0
 11   CONTINUE
       
      CALL CPU_TIME ( time_begin )
     
      DO 10 T=1,NT		!开始迭代
       
	   WRITE(NAME_COUNT,'(I5)') T      

	   OPEN(8134+T,FILE="GAUSS/GAUSS"//NAME_COUNT//".out",FORM='FORMATTED')
	   OPEN(8135+T,FILE="ONLYSIN/ONLYSIN"//NAME_COUNT//".out",FORM='FORMATTED')

         DO 6 III=2,2  !1上2右3下4左
             DO 992 PIND=1,PLEN
    !		      IVB(III,X,Y,Z)=1  !为点光源持续一个幅度激励
    !             IVB(III,X,Y,Z)=sin(2*PI*FRE*t)  !为sin激发 单色波形式
    !             其中t=(deltaL/C)*T   ,T为NT循环的迭代变量,deltaL/C(一个格点宽度/光速=单位距离所用的时间),deltaL为单位距离，C为光速 
	!             所以，IVB(III,X,Y,Z)=sin(2*PI*FRE*t)=sin(2*PI*FRE*(deltaL/C)*T)=sin(2*PI*(NORMAL_FRE/10)*T) 
	!             其中deltaL/a=1/10,a为lattic constant,FRE*(deltaL/C)=FRE*a/(10C)为归一化频率 NORMAL_FRE/10
	!             

	!             例如FRE=10GHz时，由于TLM色散限制 deltaL/lamd<0.1  ,此处取deltaL/lamd=(1/3)*0.1=3.33*E-2
	!             !IVB(III,X,Y,Z)=sin(6.283E+10*(0.33333E-11)*T)  !为sin激发单色波形式 FRE=10GHz
    !             !其中0.3333E-11=1/3E-11=dl/CC=deltaL/C(一个格点宽度/光速)  !dl取(3*E-2)*lamd=(3*E-2)*C/FRE 且取FRE=10GHz, C为光速
                 IVB(III,PULSE(PIND,1),PULSE(PIND,2),PULSE(PIND,3))=IVB(III,PULSE(PIND,1),PULSE(PIND,2),PULSE(PIND,3))+sin(2*PI*(NORMAL_FRE/(SIDE2*4))*T)  !为sin激发单色波形式
	!            IVB(III,PULSE(PIND,1),PULSE(PIND,2),PULSE(PIND,3))=IVB(III,PULSE(PIND,1),PULSE(PIND,2),PULSE(PIND,3))+sin(PI*T/15)  !为sin激发单色波形式
 992         CONTINUE 
  6      CONTINUE

         DO 374 III=2,2
             DO 993 I=SX,ENDX
			   DO 994 K=SZ,START_COL
			   	  write(8134+T,117) K,I,GAUSS(I,Y,K)*IVB(III,I,Y,K)
				  write(8135+T,117) K,I,IVB(III,I,Y,K)
    !              IVB(III,I,J,K)=GAUSS(I,J,K)*IVB(III,I,J,K)    
 994           CONTINUE  
               WRITE(8134+T,*)
			   WRITE(8135+T,*)
 993         CONTINUE
 374     CONTINUE

       CLOSE(8134+T)			 
	   CLOSE(8135+T)

      
	     DO 120 I=SX,ENDX		!结点散射的实施
	      DO 130 J=SY,ENDY
	       DO 140 K=SZ,ENDZ
	       
             CALL SBL(SA,YX,GX)	!生成并联结点散射矩阵
             CALL SBL(SB,YY(I,K),GY)	
             CALL SBL(SC,YZ,GZ)	
  
		     DO 501 M=1,5
		        RVA(M,I,J,K)=0
		        DO 601 N=1,5 
			       RVA(M,I,J,K)=SA(M,N)*IVA(N,I,J,K)+RVA(M,I,J,K)
  601           CONTINUE
  501        CONTINUE
  
		     DO 502 M=1,5
		        RVB(M,I,J,K)=0
		        DO 602 N=1,5 
			       RVB(M,I,J,K)=SB(M,N)*IVB(N,I,J,K)+RVB(M,I,J,K)
  602           CONTINUE
  502        CONTINUE
  
		     DO 503 M=1,5
		        RVC(M,I,J,K)=0
		        DO 603 N=1,5 
			       RVC(M,I,J,K)=SC(M,N)*IVC(N,I,J,K)+RVC(M,I,J,K)
  603           CONTINUE
  503        CONTINUE
      
		     DO 504 M=1,5
		        RVD(M,I,J,K)=0
		        DO 604 N=1,5 
			       RVD(M,I,J,K)=SD(M,N)*IVD(N,I,J,K)+RVD(M,I,J,K)
  604           CONTINUE
  504        CONTINUE
  
		     DO 505 M=1,5
		        RVE(M,I,J,K)=0
		        DO 605 N=1,5 
			       RVE(M,I,J,K)=SE(M,N)*IVE(N,I,J,K)+RVE(M,I,J,K)
  605           CONTINUE
  505        CONTINUE

		     DO 506 M=1,5
		        RVF(M,I,J,K)=0
		        DO 606 N=1,5 
			       RVF(M,I,J,K)=SF(M,N)*IVF(N,I,J,K)+RVF(M,I,J,K)
  606           CONTINUE
  506        CONTINUE
  
  140      CONTINUE   
  130     CONTINUE
  120    CONTINUE
      
	
         DO 20 I=SX,ENDX        
          DO 30 J=SY,ENDY
	       DO 40 K=SZ,ENDZ
		      IVA(1,I,J,K)=RVE(4,I,J,K-1)
		      IVA(2,I,J,K)=RVF(3,I,J,K)
		      IVA(3,I,J,K)=RVE(2,I,J,K)
		      IVA(4,I,J,K)=RVF(1,I,J+1,K)
		      IVA(5,I,J,K)=RVA(5,I,J,K)
    		  
		      IVB(1,I,J,K)=RVF(4,I-1,J,K)
		      IVB(2,I,J,K)=RVD(3,I,J,K-1)
		      IVB(3,I,J,K)=RVF(2,I,J,K)
		      IVB(4,I,J,K)=RVD(1,I,J,K)
		      IVB(5,I,J,K)=RVB(5,I,J,K)
    		  
		      IVC(1,I,J,K)=RVD(4,I,J,K)
		      IVC(2,I,J,K)=RVE(3,I-1,J,K)
		      IVC(3,I,J,K)=RVD(2,I,J+1,K)
		      IVC(4,I,J,K)=RVE(1,I,J,K)
		      IVC(5,I,J,K)=RVC(5,I,J,K)
    		  
		      IVD(1,I,J,K)=RVB(4,I,J,K)
		      IVD(2,I,J,K)=RVC(3,I,J-1,K)
		      IVD(3,I,J,K)=RVB(2,I,J,K+1)
		      IVD(4,I,J,K)=RVC(1,I,J,K)
		      IVD(5,I,J,K)=RVD(5,I,J,K)
    		  
		      IVE(1,I,J,K)=RVC(4,I,J,K)
		      IVE(2,I,J,K)=RVA(3,I,J,K)
		      IVE(3,I,J,K)=RVC(2,I+1,J,K)
		      IVE(4,I,J,K)=RVA(1,I,J,K+1)
		      IVE(5,I,J,K)=RVE(5,I,J,K)

		      IVF(1,I,J,K)=RVA(4,I,J-1,K)
		      IVF(2,I,J,K)=RVB(3,I,J,K)
		      IVF(3,I,J,K)=RVA(2,I,J,K)
		      IVF(4,I,J,K)=RVB(1,I+1,J,K)
		      IVF(5,I,J,K)=RVF(5,I,J,K)
		      
              !BL=1 代表电壁情况

		      IF (BL.EQ.1) THEN
		         IF (J.EQ.SY) THEN        
                    IVA(2,I,J,K)=-RVA(2,I,J,K)
			        IVC(1,I,J,K)=-RVC(1,I,J,K)
		         END IF
		         IF (J.EQ.ENDY) THEN
			    IVA(4,I,J,K)=-RVA(4,I,J,K)
			    IVC(3,I,J,K)=-RVC(3,I,J,K)
		         END IF
		         IF (K.EQ.SZ) THEN
			    IVA(1,I,J,K)=-RVA(1,I,J,K)
			    IVB(2,I,J,K)=-RVB(2,I,J,K)
		         END IF
		         IF (K.EQ.ENDZ) THEN    
			    IVA(3,I,J,K)=-RVA(3,I,J,K)
			    IVB(4,I,J,K)=-RVB(4,I,J,K)
		         END IF
		         IF (I.EQ.SX) THEN
			    IVB(1,I,J,K)=-RVB(1,I,J,K)
			    IVC(2,I,J,K)=-RVC(2,I,J,K) 
		         END IF
		         IF (I.EQ.ENDX) THEN 
			    IVB(3,I,J,K)=-RVB(3,I,J,K)
			    IVC(4,I,J,K)=-RVC(4,I,J,K)
		         END IF
		      END IF

              !BL=0 代表吸收边界情况
              
		      IF (BL.EQ.0) THEN
			    !IF (K.EQ.BP) THEN
                   !IVB(1,I,J,K)=0
				   !IVB(2,I,J,K)=0
				!ENDIF
		         IF (J.EQ.SY) THEN        
			    IVA(2,I,J,K)=0
			    IVC(1,I,J,K)=0
		         END IF
		         IF (J.EQ.ENDY) THEN
			    IVA(4,I,J,K)=0
			    IVC(3,I,J,K)=0
		         END IF
		         IF (K.EQ.SZ) THEN
			    IVA(1,I,J,K)=0
			    IVB(2,I,J,K)=0
		         END IF
		         IF (K.EQ.ENDZ) THEN    
			    IVA(3,I,J,K)=0
			    IVB(4,I,J,K)=0
		         END IF
		         IF (I.EQ.SX) THEN
			    IVB(1,I,J,K)=0
			    IVC(2,I,J,K)=0 
		         END IF
		         IF (I.EQ.ENDX) THEN 
			    IVB(3,I,J,K)=0
			    IVC(4,I,J,K)=0
		         END IF
		      END IF
		  
   40          CONTINUE   
   30       CONTINUE
   20    CONTINUE
            
      
      IF (SPLIT .EQ. 1) THEN
          IF(DO_EX .EQ. 1) THEN         
            FILE_UNIT(1)=UNIT_BASE+1+T
            OPEN(FILE_UNIT(1),FILE='EX/EX'//NAME_COUNT//'.out',FORM='FORMATTED')
          ENDIF
          IF(DO_EY .EQ. 1) THEN    
            FILE_UNIT(2)=UNIT_BASE+2+T
            OPEN(FILE_UNIT(2),FILE='EY/EY'//NAME_COUNT//'.out',FORM='FORMATTED')
          ENDIF
          IF(DO_EZ .EQ. 1) THEN         
            FILE_UNIT(3)=UNIT_BASE+3+T
            OPEN(FILE_UNIT(3),FILE='EZ/EZ'//NAME_COUNT//'.out',FORM='FORMATTED')
          ENDIF
          IF(DO_HX .EQ. 1) THEN         
            FILE_UNIT(4)=UNIT_BASE+4+T
            OPEN(FILE_UNIT(4),FILE='HX/HX'//NAME_COUNT//'.out',FORM='FORMATTED')
          ENDIF
          IF(DO_HY .EQ. 1) THEN         
            FILE_UNIT(5)=UNIT_BASE+5+T
            OPEN(FILE_UNIT(5),FILE='HY/HY'//NAME_COUNT//'.out',FORM='FORMATTED')
          ENDIF
          IF(DO_HZ .EQ. 1) THEN         
            FILE_UNIT(6)=UNIT_BASE+6+T          
            OPEN(FILE_UNIT(6),FILE='HZ/HZ'//NAME_COUNT//'.out',FORM='FORMATTED')
          ENDIF
      ENDIF
         
        DO 17910 FUI=1,6
          IF(FILE_UNIT(FUI) .GT. 0) THEN
            WRITE(FILE_UNIT(FUI),*) "#T=",T
          ENDIF
          DO 101 II=SX,ENDX        
	           DO 102 KK=SZ,ENDZ
	             IF(FILE_UNIT(FUI) .GT. 0) THEN
	              IF(FUI .EQ. 1) THEN
                    CALL EJ(EH(II,Y,KK),IVA,YX,GX,II,Y,KK,U,NX,NY,NZ)		!将电压转换为电场
                  ELSE IF(FUI .EQ. 2) THEN
                    CALL EJ(EH(II,Y,KK),IVB,YY(II,KK),GY,II,Y,KK,V,NX,NY,NZ)		
                  ELSE IF(FUI .EQ. 3) THEN
                    CALL EJ(EH(II,Y,KK),IVC,YZ,GZ,II,Y,KK,W,NX,NY,NZ)		
                  ELSE IF(FUI .EQ. 4) THEN
                    CALL HJ(EH(II,Y,KK),IVD,ZX,II,Y,KK,Z0,NX,NY,NZ)
                  ELSE IF(FUI .EQ. 5) THEN
                    CALL HJ(EH(II,Y,KK),IVE,ZY,II,Y,KK,Z0,NX,NY,NZ)
                  ELSE IF(FUI .EQ. 6) THEN
                    CALL HJ(EH(II,Y,KK),IVF,ZZ,II,Y,KK,Z0,NX,NY,NZ)
                  ENDIF 

                  DO 16384 LI=1,LN
                       IF(LN .EQ. 16384) THEN
                          LL=LI
                       ELSE
                          LL=L(LI)
                       ENDIF
                       IF(FUI .EQ. 1) THEN
                          CALL FT(REEX(LI,II,Y,KK),IMEX(LI,II,Y,KK),EH(II,Y,KK),LL,T) !进行傅立叶变换
                       ELSE IF(FUI .EQ. 2) THEN
                          CALL FT(REEY(LI,II,Y,KK),IMEY(LI,II,Y,KK),EH(II,Y,KK),LL,T) 
                       ELSE IF(FUI .EQ. 3) THEN
                          CALL FT(REEZ(LI,II,Y,KK),IMEZ(LI,II,Y,KK),EH(II,Y,KK),LL,T) 
                       ELSE IF(FUI .EQ. 4) THEN
                          CALL FT(REHX(LI,II,Y,KK),IMHX(LI,II,Y,KK),EH(II,Y,KK),LL,T) 
                       ELSE IF(FUI .EQ. 5) THEN
                          CALL FT(REHY(LI,II,Y,KK),IMHY(LI,II,Y,KK),EH(II,Y,KK),LL,T) 
                       ELSE IF(FUI .EQ. 6) THEN
                          CALL FT(REHZ(LI,II,Y,KK),IMHZ(LI,II,Y,KK),EH(II,Y,KK),LL,T) 
                       ENDIF

    16384         CONTINUE
	            ENDIF	                       
       102	  CONTINUE
       101 CONTINUE

           IF(FILE_UNIT(FUI) .GT. 0) THEN
		          
			   CALL NORMALIZE(EMAX(FUI),EH,SX,ENDX,Y,Y,SZ,ENDZ,NX,NY,NZ)
			   DO 111 II=SX,ENDX
				  DO 112 KK=SZ,ENDZ
						IF(YYY(II,KK) .EQ. ER1_COLOR)  THEN     !绘制介质图像
						  WRITE(FILE_UNIT(FUI),117) KK,II,EH(II,Y,KK),0.01
						ELSE
						  WRITE(FILE_UNIT(FUI),117) KK,II,EH(II,Y,KK),EH(II,Y,KK)
						ENDIF
		   112    CONTINUE
				  WRITE(FILE_UNIT(FUI),*)        !TLM_DFT.py需要修改适应此变化
		   111 CONTINUE

             WRITE(FILE_UNIT(FUI),*)
             WRITE(FILE_UNIT(FUI),*)
             IF (SPLIT .EQ. 1) THEN
               CLOSE(FILE_UNIT(FUI))
             ENDIF
           ENDIF
    17910 CONTINUE
           
      N1=MOD(T,10)
      IF (N1.EQ.0) THEN
	  WRITE(*,*)T
      END IF
   10 CONTINUE     
   
      DO 16386 LI=1,LN
          IF(LN .EQ. 16384) THEN
             LL=LI
          ELSE
             LL=L(LI)
          ENDIF
          WRITE(NAME_COUNT,'(I5)') LL
          IF(DO_FEX .EQ. 1) THEN
            OPEN(UNIT_BASE2+LL,FILE='FEX/FEX'//NAME_COUNT//'.out',FORM='FORMATTED')
          ENDIF
          IF(DO_FEY .EQ. 1) THEN
            OPEN(UNIT_BASE2+LL+1,FILE='FEY/FEY'//NAME_COUNT//'.out',FORM='FORMATTED')
          ENDIF
          IF(DO_FEZ .EQ. 1) THEN
            OPEN(UNIT_BASE2+LL+2,FILE='FEZ/FEZ'//NAME_COUNT//'.out',FORM='FORMATTED')
          ENDIF
          IF(DO_FHX .EQ. 1) THEN
            OPEN(UNIT_BASE2+LL+3,FILE='FHX/FHX'//NAME_COUNT//'.out',FORM='FORMATTED')
          ENDIF
          IF(DO_FHY .EQ. 1) THEN
            OPEN(UNIT_BASE2+LL+4,FILE='FHY/FHY'//NAME_COUNT//'.out',FORM='FORMATTED')
          ENDIF
          IF(DO_FHZ .EQ. 1) THEN
            OPEN(UNIT_BASE2+LL+5,FILE='FHZ/FHZ'//NAME_COUNT//'.out',FORM='FORMATTED')
          ENDIF
16386 CONTINUE


      DO 1011 II=SX,ENDX        
         DO 1012 KK=SZ,ENDZ      
            DO 16385 LI=1,LN
               IF(LN .EQ. 16384) THEN
                  LL=LI
               ELSE
                  LL=L(LI)
               ENDIF
               
               IF(DO_FEX .EQ. 1) THEN
                 FF=SQRT(REEX(LI,II,Y,KK)**2 + IMEX(LI,II,Y,KK)**2)
                 WRITE(UNIT_BASE2+LL,117),KK,II,FF
               ENDIF
               IF(DO_FEY .EQ. 1) THEN
                 FF=SQRT(REEY(LI,II,Y,KK)**2 + IMEY(LI,II,Y,KK)**2)
                 WRITE(UNIT_BASE2+LL+1,117),KK,II,FF
               ENDIF
               IF(DO_FEZ .EQ. 1) THEN
                 FF=SQRT(REEZ(LI,II,Y,KK)**2 + IMEZ(LI,II,Y,KK)**2)
                 WRITE(UNIT_BASE2+LL+2,117),KK,II,FF
               ENDIF
               IF(DO_FHX .EQ. 1) THEN
                 FF=SQRT(REHX(LI,II,Y,KK)**2 + IMHX(LI,II,Y,KK)**2)
                 WRITE(UNIT_BASE2+LL+3,117),KK,II,FF
               ENDIF
               IF(DO_FHY .EQ. 1) THEN
                 FF=SQRT(REHY(LI,II,Y,KK)**2 + IMHY(LI,II,Y,KK)**2)
                 WRITE(UNIT_BASE2+LL+4,117),KK,II,FF
               ENDIF
               IF(DO_FHZ .EQ. 1) THEN
                 FF=SQRT(REHZ(LI,II,Y,KK)**2 + IMHZ(LI,II,Y,KK)**2)
                 WRITE(UNIT_BASE2+LL+5,117),KK,II,FF
               ENDIF
               
16385       CONTINUE
1012     CONTINUE
1011  CONTINUE
   
      DO 16387 LI=1,LN
          IF(LN .EQ. 16384) THEN
             LL=LI
          ELSE
             LL=L(LI)
          ENDIF
          
          IF(DO_FEX .EQ. 1) THEN
            CLOSE(UNIT_BASE2+LL)
          ENDIF
          IF(DO_FEY .EQ. 1) THEN
            CLOSE(UNIT_BASE2+LL+1)
          ENDIF
          IF(DO_FEZ .EQ. 1) THEN
            CLOSE(UNIT_BASE2+LL+2)
          ENDIF
          IF(DO_FHX .EQ. 1) THEN
            CLOSE(UNIT_BASE2+LL+3)
          ENDIF
          IF(DO_FHY .EQ. 1) THEN
            CLOSE(UNIT_BASE2+LL+4)
          ENDIF
          IF(DO_FHZ .EQ. 1) THEN
            CLOSE(UNIT_BASE2+LL+5)
          ENDIF
          
16387 CONTINUE
   
  117 format(I, I, 7f12.7)
  118 format(7f12.7)
  119 format(I)


      IF (SPLIT .EQ. 0) THEN
         IF(FILE_UNIT(FUI) .GT. 0) THEN
            CLOSE(FILE_UNIT(FUI))
         ENDIF
      ENDIF
	  
	  CLOSE(8133)
	  CALL CPU_TIME ( time_end )
	  
	  PRINT *, 'Time of operation was ',time_end - time_begin, ' seconds'
	  STOP
      END          

      SUBROUTINE SBL(S,Y,G)  !生成[S]散射矩阵
      REAL S(5,5)
	  REAL Y,G
      DO 21 I=1,5
	  DO 22 J=1,5
	    IF (J.EQ.5) THEN 
	       S(I,J)=Y
	    ELSE 
	       S(I,J)=1
	    END IF
	    S(I,J)=S(I,J)*2/(4+Y+G)
	    IF (J.EQ.I) S(I,J)=S(I,J)-1
   22    CONTINUE
   21 CONTINUE
      RETURN
      END
      
      SUBROUTINE SCL(S,Z)
      REAL S(5,5)
	  REAL Z
      S(1,1)=-1.0
      S(1,2)=1.0
      S(1,3)=1.0
      S(1,4)=-1.0
      S(1,5)=-1.0
      S(2,1)=1.0
      S(2,2)=-1.0
      S(2,3)=-1.0
      S(2,4)=1.0
      S(2,5)=1.0
      S(5,1)=-Z
      S(5,2)=Z
      S(5,3)=Z
      S(5,4)=-Z
      S(5,5)=-Z
      DO 33 J=1,5
	    S(3,J)=S(2,J)
	    S(4,J)=S(1,J)
   33 CONTINUE
      DO 31 I=1,5
	   DO 32 J=1,5
	    S(I,J)=S(I,J)*2.0/(4.0+Z)
	    IF (J.EQ.I) S(I,J)=S(I,J)+1
   32    CONTINUE
   31 CONTINUE
      RETURN
      END

      SUBROUTINE EJ(E,IV,Y,G,I,J,K,L,NX,NY,NZ)
	  INTEGER NX,NY,NZ,I,J,K,L
      REAL IV(5,NX,NY,NZ)
	  REAL E,Y,G
      E=0.0
      DO 100 M=1,4
	     E=E+IV(M,I,J,K)
  100 CONTINUE
      E=E+Y*IV(5,I,J,K)
      E=E*2/(L*(4+Y+G))
      RETURN
      END
      
      SUBROUTINE HJ(H,IV,Z,I,J,K,Z0,NX,NY,NZ)
      REAL H,Z,Z0
      INTEGER I,J,K,NX,NY,NZ
      REAL IV(5,NX,NY,NZ)
      H=IV(1,I,J,K)-IV(2,I,J,K)-IV(3,I,J,K)+IV(4,I,J,K)+IV(5,I,J,K)
      H=H*2/(Z0*(4+Z))
      RETURN
      END
      
      SUBROUTINE FT(FREAL,FIM,E,LL,T)
      INTEGER LL,T
	  REAL TLBB,FREAL,FIM
 !    进行傅立叶变换，其中LL和T分别为主函数中L和T，表示频域的第几个点和时域的第几个点
      TLBB=(LL*(0.1-0)/16384.0+0)
      FREAL=FREAL+E*COS(2*3.14159*T*TLBB/2.0)
      FIM=FIM+E*SIN(2*3.14159*T*TLBB/2.0)
      END

      SUBROUTINE COLOR(GTYPE,S1,S2,I,K,ZY,ZYY,NX,NZ,C,CC, SHRINK)
      INTEGER GTYPE,S1,S2,I,K,NX,NZ,SIDE1,SIDE2
	  REAL SHRINK
      INTEGER CC
      REAL ZY(NX,NZ)
      INTEGER ZYY(NX,NZ)
      REAL C
      REAL CROW,CCOL,A,B,AX
      INTEGER IGAP,KGAP

	  SIDE1=S1*SHRINK
	  SIDE2=S2*SHRINK

      IGAP=S1-SIDE1
	  KGAP=S2-SIDE2

      A=SIDE1
      B=SIDE2
      CROW=A/2
      CCOL=B/2
      DO 3333 III=0,S1-1-IGAP
         DO 3334 KKK=0,S2-1-KGAP
             IF(GTYPE .EQ. 0) THEN     !矩形
                 ZY(I+III,K+KKK)=C
                 ZYY(I+III,K+KKK)=CC
             ENDIF
             IF(GTYPE .EQ. 1) THEN     !椭圆
                 IF((III-CROW)*(III-CROW)/(CROW*CROW)+(KKK-CCOL)*(KKK-CCOL)/(CCOL*CCOL) .LE. 1) THEN
                     ZY(I+III,K+KKK)=C
                     ZYY(I+III,K+KKK)=CC
                 ENDIF
             ENDIF
             IF(GTYPE .EQ. 2) THEN     !三角形
                 IF(2*A*(B/2-KKK)/B .LE. III .AND. 2*A*(KKK-B/2)/B .LE. III) THEN
                     ZY(I+III,K+KKK)=C
                     ZYY(I+III,K+KKK)=CC
                 ENDIF
             ENDIF            
 3334    CONTINUE
 3333 CONTINUE
      END

      SUBROUTINE HEXAGON(GTYPE,S1,S2,I,K,ZY,ZYY,NX,NZ,SX,ENDX,SZ,ENDZ,C,CC, SHRINK)
      INTEGER GTYPE,S1,S2,I,K,NX,NZ,SX,ENDX,SZ,ENDZ,SIDE1,SIDE2
	  REAL SHRINK
      INTEGER CC
      REAL ZY(NX,NZ)
      INTEGER ZYY(NX,NZ)
      REAL C
      REAL CROW,CCOL,A,B,AX
      INTEGER IGAP,KGAP

	  SIDE1=S1*SHRINK
	  SIDE2=S2*SHRINK

      IGAP=S1-SIDE1
	  KGAP=S2-SIDE2

      A=SIDE1
      B=SIDE2
      CROW=A/2
      CCOL=B/2
      DO 3333 III=0,S1-1-IGAP
         DO 3334 KKK=0,S2-1-KGAP
             IF(GTYPE .EQ. 3) THEN     !六边形                                  
                 IF((B/4-KKK)/(B/4) .LE. III/(A/2) .AND. (KKK-3*B/4)/(B/4) .LT. III/(A/2) .AND. (B/4-KKK)/(B/4) .LE. (A-III)/(A/2) .AND. (KKK-3*B/4)/(B/4) .LT. (A-III)/(A/2)) THEN
                   IF(I+III .LE. ENDX .AND. K+KKK .LE. ENDZ .AND. I+III .GE. SX .AND. K+KKK .GE. SZ) THEN
                     ZY(I+III,K+KKK)=C
                     ZYY(I+III,K+KKK)=CC
                   ENDIF
                 ENDIF
             ENDIF             
 3334    CONTINUE
 3333 CONTINUE
      END


      SUBROUTINE GENGAUSS(GAUSS,NX,NY,NZ,XP,YP,ZP,W0)
        INTEGER X0,Y0,Z0,NX,NY,NZ,XP,YP,ZP
        REAL GAUSS(-XP+1:NX-XP,NY,-ZP+1:NZ-ZP)
	    REAL X,Y,Z
        REAL C0,W0,Wz,PI,Zr,Rz,Lambda
	    REAL AMAX
        
     
        PI=3.1415926
        Y0=YP
 	    Y=Y0
        C0=1
        Lambda=31E-10
        Yr=PI*W0*W0/Lambda
     
        DO 1007 X0=-XP+1,NX-XP
          DO 2007 Z0=-ZP+1,NZ-ZP
			    X=X0
			    Z=Z0
			    Wy=W0*sqrt(1+((Lambda*Y)/(PI*W0*W0))*((Lambda*Y)/(PI*W0*W0)))
			    GAUSS(X0,Y0,Z0)=exp(-(X*X+Z*Z)/(Wy*Wy))
			    Ry=Y*(1+(Yr/Y)*(Yr/Y))
                IF(X0 .EQ. 0 .AND. Z0 .EQ. 0 ) THEN
			      AMAX=GAUSS(X0,Y0,Z0)
		        ENDIF
    2007   CONTINUE
    1007 CONTINUE
    
        DO 1008 X0=-XP+1,NX-XP
          DO 2008 Y0=YP,YP
            DO 3008 Z0=-ZP+1,NZ-ZP
              GAUSS(X0,Y0,Z0)=GAUSS(X0,Y0,Z0)/AMAX
   3008     CONTINUE
   2008  CONTINUE
   1008 CONTINUE
      END

	  SUBROUTINE LINE(X,Y,Z,PSX,PSZ,PENDZ,SX,ENDX,ANGLE,ZYY,NX,NZ,PULSE,PIND,PLEN,P_COLOR)
      INTEGER PSX,PSZ,PENDZ,SX,ENDX,NX,NZ,PIND,PLEN,X,Y,Z
      REAL PP !脉冲直线计算参数
      REAL PANGLE,ANGLE
      INTEGER ZYY(NX,NZ)
      INTEGER P_COLOR
      INTEGER PULSE(NX,3)
      INTEGER II,KK
      II=PSX
      KK=PSZ
      ZYY(II,KK)=P_COLOR
      PIND=1
      PULSE(PIND,1)=II
      PULSE(PIND,2)=Y
      PULSE(PIND,3)=KK
      PANGLE=TAND(ANGLE)
      IF(PANGLE .GT. 1) THEN
        PP=2/PANGLE-1
      ELSE
        PP=2*PANGLE-1
      ENDIF
      DO WHILE(II .GT. SX .AND. KK .GE. PENDZ)
         IF(PANGLE .GT. 1) THEN
            II=II-1
         ELSE
            KK=KK-1
         ENDIF
         IF(PP .GE. 0) THEN
            IF(PANGLE .GT. 1) THEN
               KK=KK-1
               PP=PP+2*(1/PANGLE-1)
            ELSE
               II=II-1
               PP=PP+2*(PANGLE-1)
            ENDIF
         ELSE
            IF(PANGLE .GT. 1) THEN
               PP=PP+2/PANGLE
            ELSE
               PP=PP+2*PANGLE
            ENDIF
         ENDIF
         PIND=PIND+1
         PULSE(PIND,1)=II
         PULSE(PIND,2)=Y
         PULSE(PIND,3)=KK
         ZYY(II,KK)=P_COLOR
      ENDDO
      PLEN=PIND
      END

      SUBROUTINE NORMALIZE(MAX,E,SX,ENDX,SY,ENDY,SZ,ENDZ,NX,NY,NZ)
	  INTEGER NX,NY,NZ,SX,ENDX,SY,ENDY,SZ,ENDZ,I,J,K
	  REAL E(NX,NY,NZ)
	  REAL EMAX,MAX
      EMAX=MAX
      DO 1107 I=SX,ENDX
	     DO 2107 J=SY,ENDY
            DO 3107 K=SZ,ENDZ
              IF(E(I,J,K) .GT. EMAX) THEN
			     EMAX=E(I,J,K)
			  ENDIF
   3107     CONTINUE
   2107  CONTINUE
   1107 CONTINUE
      IF(EMAX .LT. 0.000001) THEN
	      RETURN
      ENDIF
      DO 1106 I=SX,ENDX
	      DO 2106 J=SY,ENDY
            DO 3106 K=SZ,ENDZ
			     E(I,J,K)=E(I,J,K)/EMAX
   3106     CONTINUE
   2106   CONTINUE
   1106 CONTINUE
      IF(EMAX .GT. MAX) THEN
	     WRITE(*,*) "MAX REPLACED!",EMAX,MAX
	  ENDIF
	  MAX=EMAX
	  END

      
      

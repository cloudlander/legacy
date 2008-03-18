!  TLM.f90 
!
!  FUNCTIONS:
!	TLM      - Entry point of console application.
!
!	Example of displaying 'Hello World' at execution time.
!

!****************************************************************************
!
!  PROGRAM: TLM
!
!  PURPOSE:  Entry point for 'Hello World' sample console application.
!
!****************************************************************************

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
      INTEGER, ALLOCATABLE:: L(:)
      REAL, ALLOCATABLE:: YX(:,:),YY(:,:),YZ(:,:)
      INTEGER, ALLOCATABLE:: PULSE(:,:)   !脉冲点
      CHARACTER(LEN=12), ALLOCATABLE:: YYX(:,:),YYY(:,:),YYZ(:,:)      
      REAL, ALLOCATABLE:: GAUSS(:,:,:)
      
      REAL SA(5,5),SB(5,5),SC(5,5),SD(5,5),SE(5,5),SF(5,5)
      
      CHARACTER(LEN=12) ER_COLOR,ER1_COLOR,ER2_COLOR,P_COLOR  !介质的颜色
      
      REAL EH,FF      
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
      INTEGER START_ROW,START_COL,GRID_ROW,GRID_COL   !网格开始位置
      INTEGER COL  !网格总列数
      REAL PART  !网格所占百分比
      REAL NANGLE !网格偏移角度(与Z轴张角,取值范围为(0,180) 90为不倾斜
      INTEGER GTYPE !网格类型 0为矩形, 1为内切椭圆(正圆), 2为等腰三角形
      INTEGER PSX,PSZ !输入脉冲开始位置
      INTEGER PENDZ !输入脉冲结束的Z坐标
      REAL PANGLE  !输入脉冲直线与Z轴的张角(角度)
      REAL PLENGTH !输入脉冲直线的长度
      INTEGER PN !输入脉冲数组(0:使用PSX,PSZ,PENDZ和PLENGTH自动生成脉冲直线  >0: 使用输入文件的X,Z坐标作为脉冲点)
      
      REAL PP !脉冲直线计算参数
      INTEGER PIND, PLEN !脉冲点序号和总数

      !是否输出对应的数据文件: 1为输出, 0为不输出
      INTEGER DO_EX,DO_EY,DO_EZ,DO_HX,DO_HY,DO_HZ,DO_FEX,DO_FEY,DO_FEZ,DO_FHX,DO_FHY,DO_FHZ
      
      CHARACTER(LEN=5) NAME_COUNT

      UNIT_BASE=100
      UNIT_BASE2=16384
      ER_COLOR="255 255 255 "  !默认为白色
      ER1_COLOR="255 0 0     "
      ER2_COLOR="0 0 255     "
      P_COLOR="0 255 0     "   !激发为绿色

      OPEN(11,FILE='twotlme.in',FORM='FORMATTED')
      OPEN(111,FILE='grid.ppm',FORM='FORMATTED')
      
      READ(11,*)SX,SY,SZ,ENDX,ENDY,ENDZ,X,Y,Z,U,V,W,H,ER,UR,SGM,NT,BL,SPLIT,SIDE1,SIDE2,ER1,ER2,NANGLE,GTYPE,COL,FRE,PN,PSX,PSZ,PENDZ,PANGLE,DO_EX,DO_EY,DO_EZ,DO_HX,DO_HY,DO_HZ,LN,DO_FEX,DO_FEY,DO_FEZ,DO_FHX,DO_FHY,DO_FHZ
!读入数据固定参数计算结构参数时使用
      
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
      OPEN(10,FILE='set.gnu',FORM='FORMATTED')
      WRITE(10,*) "set samples 100, 100"
      WRITE(10,*) "set isosamples 10, 10"
      WRITE(10,*) "set autoscale"
      WRITE(10,*) "set size square"
      WRITE(10,*) "set surface"
      WRITE(10,*) 'set title "TLM 2D"'
      WRITE(10,*) 'set xlabel "Z"'
      WRITE(10,*) 'set ylabel "X"'
      WRITE(10,*) "set xrange [",SZ,":",ENDZ,"] noreverse nowriteback"
      WRITE(10,*) "set yrange [",SX,":",ENDX,"] noreverse nowriteback"
      WRITE(10,*) "set zrange [ -0.300000 : 0.300000 ] noreverse nowriteback"
      WRITE(10,*) "set cbrange [ -0.300000 : 0.300000 ] noreverse nowriteback"
      WRITE(10,*) "set zero 1e-0020"
      WRITE(10,*) "set pm3d at s"
      WRITE(10,*) "set dgrid3d ",ENDZ-SZ+1,",",ENDX-SX+1
      WRITE(10,*) "set style data dots"
	  !"set xrange" 对应（ey.out第一列），为介质空间Z轴； "set yrange"对应（ey.out第二列），为介质空间X轴
	  !"set zrange" 0.03变为1.03为了画图不出现白色块
	  !"set pm3d map"为2D画图，"set pm3d at s"为3D画图
	  CLOSE(10)
            
      U0=12566.3706144		!真空中的磁导率
      E0=8.854187818E-2		!真空中的介电常数
      Z0=SQRT(U0/E0)
      PI=3.1415926
      
      COL=COL-1
      PART=REAL(COL*SIDE2)/(ENDZ-SZ)
      START_ROW=SX
      START_COL=SZ+(ENDZ-SZ)*(1-PART)/2
      GRID_ROW=(ENDX-SX+1)/SIDE1
      GRID_COL=(ENDZ-SZ)*PART/SIDE2
      IF (GRID_COL+1 .EQ. COL) THEN
           GRID_COL=COL
      ELSE
          IF (GRID_COL .NE. COL) THEN
               WRITE(*,*) "COL NOT MATCH"
               STOP
          ENDIF
      ENDIF
      ENDX=SX+GRID_ROW*SIDE1-1

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
      ALLOCATE (GAUSS(NX,NY,NZ))
      ALLOCATE (YX(NX,NZ),YY(NX,NZ),YZ(NX,NZ))
      ALLOCATE (YYX(NX,NZ),YYY(NX,NZ),YYZ(NX,NZ))
      ALLOCATE (PULSE(NX,3))

      !初始化X-Z平面ER
      DO 2221 II=SX,ENDX
         DO 2222 KK=SZ,ENDZ
            YY(II,KK)=2*(U*W*SQRT(H)*ER/V-2)  !介电常数支线结构参数的计算(方向5即为介电常数支线)
            YYY(II,KK)=ER_COLOR
 2222    CONTINUE
 2221 CONTINUE

      !读入脉冲点位置数组X,Z坐标
      IF(PN .GT. 0) THEN
        DO 230 PIND=1,PN
          READ(11,*)PULSE(PIND,1),PULSE(PIND,3)
          PULSE(PIND,2)=Y
          YYY(PULSE(PIND,1),PULSE(PIND,3))=P_COLOR
  230   CONTINUE
        PLEN=PN
      ENDIF
      
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
                        
!  开始绘制网格            
      DO 3331 II=0,GRID_ROW-1
         DO 3332 KK=0,GRID_COL-1
            IF ( MOD((II+KK),2) .EQ. 1 ) THEN
               CALL COLOR(GTYPE,SIDE1,SIDE2,START_ROW+II*SIDE1,START_COL+KK*SIDE2,YX,YYX,NX,NZ,2*(V*W*SQRT(H)*ER1/V-2),ER1_COLOR)
               CALL COLOR(GTYPE,SIDE1,SIDE2,START_ROW+II*SIDE1,START_COL+KK*SIDE2,YY,YYY,NX,NZ,2*(U*W*SQRT(H)*ER1/V-2),ER1_COLOR)
               CALL COLOR(GTYPE,SIDE1,SIDE2,START_ROW+II*SIDE1,START_COL+KK*SIDE2,YZ,YYZ,NX,NZ,2*(V*U*SQRT(H)*ER1/V-2),ER1_COLOR)
            ELSE
               CALL COLOR(GTYPE,SIDE1,SIDE2,START_ROW+II*SIDE1,START_COL+KK*SIDE2,YX,YYX,NX,NZ,2*(V*W*SQRT(H)*ER2/V-2),ER2_COLOR)
               CALL COLOR(GTYPE,SIDE1,SIDE2,START_ROW+II*SIDE1,START_COL+KK*SIDE2,YY,YYY,NX,NZ,2*(U*W*SQRT(H)*ER2/V-2),ER2_COLOR)
               CALL COLOR(GTYPE,SIDE1,SIDE2,START_ROW+II*SIDE1,START_COL+KK*SIDE2,YZ,YYZ,NX,NZ,2*(V*U*SQRT(H)*ER2/V-2),ER2_COLOR)            
            ENDIF
 3332    CONTINUE
 3331 CONTINUE

      !  根据倾斜角度绘制介质网格
      CALL INCLINE(PSX,PSZ,NANGLE,START_COL,GRID_COL*SIDE2,SX,ENDX,SZ,ENDZ,NX,NZ,YY,YYY,2*(U*W*SQRT(H)*ER/V-2),ER_COLOR)
      
      !  开始绘制脉冲空间位置(直线),仅当不使用输入文件中指点的脉冲点时自动绘制直线
      IF(PN .EQ. 0) THEN
          II=PSX
          KK=PSZ
          YYY(II,KK)=P_COLOR
          PIND=1
          PULSE(PIND,1)=II
          PULSE(PIND,2)=Y
          PULSE(PIND,3)=KK
          PANGLE=TAND(PANGLE)
          IF(PANGLE .GT. 1) THEN
            PP=2/PANGLE-1
          ELSE
            PP=2*PANGLE-1
          ENDIF
          DO WHILE(II .GT. SX .AND. KK .GT. PENDZ)
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
             YYY(II,KK)=P_COLOR
          ENDDO
          PLEN=PIND
      ENDIF

!  输出全X-Z平面
      WRITE (111,'(A2)')"P3"
      WRITE (111,'(5I )',advance='no')ENDZ-SZ+1
      WRITE (111,'(5I)')ENDX-SX+1
      WRITE (111,'(A3)')"255"
      DO 55 I=ENDX,SX,-1
         DO 66 K=SZ,ENDZ
            WRITE (111,'(A12)',advance='no') YYY(I,K)
   66    CONTINUE
   55 CONTINUE
      WRITE (111,*)
      CLOSE(111)
      WRITE(*,*) "grid.ppm generated"
      WRITE(*,*) "PULSE POINTS:"
      DO 77 PIND=1,PLEN
        WRITE(*,*) PULSE(PIND,1),PULSE(PIND,3)
   77 CONTINUE
!      STOP

      ZX=2*(V*W*SQRT(H)*UR/U-2)
      ZY=2*(U*W*SQRT(H)*UR/V-2)
      ZZ=2*(V*U*SQRT(H)*UR/W-2)
      GX=SGM*V*W*Z0/U
      GY=SGM*U*W*Z0/V              !损耗支线结构参数计算[SGM取0记不计该Gjj参数（程序中由GY表示）]
      GZ=SGM*V*U*Z0/W
      
      CALL SCL(SD,ZX)      !生成并联结点散射矩阵
      CALL SCL(SE,ZY)	
      CALL SCL(SF,ZZ)	    
      
      ! 应用用高斯函数
      CALL GENGAUSS(GAUSS,NX,NY,NZ,X,Y,Z)
      OPEN(131,FILE='gausA.out',FORM='FORMATTED')
      DO 11111 I=SX,ENDX
        WRITE(131,*) Y,I,GAUSS(I,Y,Z)
11111 CONTINUE
      CLOSE(131)

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
     
!     DO 5 I=1,1
!	    IVB(1,X,Y,Z)=1	!激励（选择激励点支线方向位置，赋激励值1，代表1v电压）
!   5 CONTINUE
       
      CALL CPU_TIME ( time_begin )
      
      
      DO 10 T=1,NT		!开始迭代
       
       DO 1110 I=SX,ENDX
         DO 6 III=2,2  !1上2右3下4左
!             DO 992 PIND=1,PLEN
!                IVB(III,PULSE(PIND,1),PULSE(PIND,2),PULSE(PIND,3))=IVB(III,PULSE(PIND,1),PULSE(PIND,2),PULSE(PIND,3))+sin(PI*T/15)  !为sin激发单色波形式
!                IVD(III,PULSE(PIND,1),PULSE(PIND,2),PULSE(PIND,3))=IVD(III,PULSE(PIND,1),PULSE(PIND,2),PULSE(PIND,3))+sin(PI*T/15)  !为sin激发单色波形式                
! 992         CONTINUE
             IVB(III,I,Y,Z)=IVB(III,I,Y,Z)+GAUSS(I,Y,Z)*sin(PI*T/15)  !为sin激发单色波形式    
  6      CONTINUE
 1110  CONTINUE

	     DO 120 I=SX,ENDX		!结点散射的实施
	      DO 130 J=SY,ENDY
	       DO 140 K=SZ,ENDZ
	       
             CALL SBL(SA,YX(I,K),GX)	!生成并联结点散射矩阵
             CALL SBL(SB,YY(I,K),GY)	
             CALL SBL(SC,YZ(I,K),GZ)	
  
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
            
      WRITE(NAME_COUNT,'(I5)') T      
      
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
	           
	              IF(FUI .EQ. 1) THEN
                    CALL EJ(EH,IVA,YX,GX,II,Y,KK,U,NX,NY,NZ)		!将电压转换为电场
                  ELSE IF(FUI .EQ. 2) THEN
                    CALL EJ(EH,IVB,YY,GY,II,Y,KK,V,NX,NY,NZ)		
                  ELSE IF(FUI .EQ. 3) THEN
                    CALL EJ(EH,IVC,YZ,GZ,II,Y,KK,W,NX,NY,NZ)		
                  ELSE IF(FUI .EQ. 4) THEN
                    CALL HJ(EH,IVD,ZX,II,Y,KK,Z0,NX,NY,NZ)
                  ELSE IF(FUI .EQ. 5) THEN
                    CALL HJ(EH,IVE,ZY,II,Y,KK,Z0,NX,NY,NZ)
                  ELSE IF(FUI .EQ. 6) THEN
                    CALL HJ(EH,IVF,ZZ,II,Y,KK,Z0,NX,NY,NZ)
                  ENDIF 

                  IF(FILE_UNIT(FUI) .GT. 0) THEN
                    WRITE(FILE_UNIT(FUI),117) KK,II,EH
                  ENDIF
                    
                  DO 16384 LI=1,LN
                       IF(LN .EQ. 16384) THEN
                          LL=LI
                       ELSE
                          LL=L(LI)
                       ENDIF
                       IF(FUI .EQ. 1) THEN
                          CALL FT(REEX(LI,II,Y,KK),IMEX(LI,II,Y,KK),EH,LL,T) !进行傅立叶变换
                       ELSE IF(FUI .EQ. 2) THEN
                          CALL FT(REEY(LI,II,Y,KK),IMEY(LI,II,Y,KK),EH,LL,T) 
                       ELSE IF(FUI .EQ. 3) THEN
                          CALL FT(REEZ(LI,II,Y,KK),IMEZ(LI,II,Y,KK),EH,LL,T) 
                       ELSE IF(FUI .EQ. 4) THEN
                          CALL FT(REHX(LI,II,Y,KK),IMHX(LI,II,Y,KK),EH,LL,T) 
                       ELSE IF(FUI .EQ. 5) THEN
                          CALL FT(REHY(LI,II,Y,KK),IMHY(LI,II,Y,KK),EH,LL,T) 
                       ELSE IF(FUI .EQ. 6) THEN
                          CALL FT(REHZ(LI,II,Y,KK),IMHZ(LI,II,Y,KK),EH,LL,T) 
                       ENDIF

    16384         CONTINUE
	                       
       102	  CONTINUE
       101 CONTINUE
       
           IF(FILE_UNIT(FUI) .GT. 0) THEN
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
   
!      DO 222 T=1,NT
!      WRITE(13,117) EY(T)		!保存模拟结果
  222 CONTINUE

  117 format(I, I, 7f12.7)
  118 format(7f12.7)
  119 format(I)


      IF (SPLIT .EQ. 0) THEN
         IF(FILE_UNIT(FUI) .GT. 0) THEN
            CLOSE(FILE_UNIT(FUI))
         ENDIF
      ENDIF
	  
	  CALL CPU_TIME ( time_end )
	  
	  PRINT *, 'Time of operation was ',time_end - time_begin, ' seconds'
	  STOP
      END          

      SUBROUTINE SBL(S,Y,G)  !生成[S]散射矩阵
      REAL S(5,5)
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
	  INTEGER NX,NY,NZ,I,J,K
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

      SUBROUTINE GENGAUSS(GAUSS,NX,NY,NZ,XP,YP,ZP)
        INTEGER X0,Y0,Z0,NX,NY,NZ,XP,YP,ZP
        REAL GAUSS(-NX/2+(1-MOD(NX,2)):NX/2,NY,NZ)
	    REAL X,Y,Z
        REAL C0,W0,Wz,PI,Zr,Rz,Lambda
	    REAL AMAX
        
        PI=3.1415926
        Z0=ZP
 	    Z=Z0
        C0=1
        W0=3.0
        Lambda=1E-10
        Zr=PI*W0*W0/Lambda
     
        DO 1007 X0=-NX/2+(1-MOD(NX,2)),NX/2
          DO 2007 Y0=YP,YP
			    X=X0
			    Y=Y0
			    Wz=W0*sqrt(1+((Lambda*Z)/(PI*W0*W0))*((Lambda*Z)/(PI*W0*W0)))
			    GAUSS(X0,Y0,Z0)=exp(-(X*X+Y*Y)/(Wz*Wz))
			    Rz=Z*(1+(Zr/Z)*(Zr/Z))
                IF(X0 .EQ. 0 ) THEN
			      AMAX=GAUSS(X0,Y0,Z0)
		        ENDIF
    2007   CONTINUE
    1007 CONTINUE
    
        DO 1008 X0=-NX/2+(1-MOD(NX,2)),NX/2
          DO 2008 Y0=YP,YP
            GAUSS(X0,Y0,Z0)=GAUSS(X0,Y0,Z0)/AMAX
   2008  CONTINUE
   1008 CONTINUE
      END

      SUBROUTINE COLOR(GTYPE,SIDE1,SIDE2,I,K,YY,YYY,NX,NZ,C,CC)
      INTEGER GTYPE,SIDE1,SIDE2,I,K,NX,NZ
      CHARACTER(LEN=12) CC
      REAL YY(NX,NZ)
      CHARACTER(LEN=12) YYY(NX,NZ)
      REAL C
      REAL CROW,CCOL,A,B
      A=SIDE1
      B=SIDE2
      CROW=A/2
      CCOL=B/2
      DO 3333 III=0,SIDE1-1
         DO 3334 KKK=0,SIDE2-1
             IF(GTYPE .EQ. 0) THEN     !矩形
                 YY(I+III,K+KKK)=C
                 YYY(I+III,K+KKK)=CC
             ENDIF
             IF(GTYPE .EQ. 1) THEN     !椭圆
                 IF((III-CROW)*(III-CROW)/(CROW*CROW)+(KKK-CCOL)*(KKK-CCOL)/(CCOL*CCOL) .LE. 1) THEN
                     YY(I+III,K+KKK)=C
                     YYY(I+III,K+KKK)=CC
                 ENDIF
             ENDIF
             IF(GTYPE .EQ. 2) THEN     !三角形
                 IF(2*A*(B/2-KKK)/B .LE. III .AND. 2*A*(KKK-B/2)/B .LE. III) THEN
                     YY(I+III,K+KKK)=C
                     YYY(I+III,K+KKK)=CC
                 ENDIF
             ENDIF
 3334    CONTINUE
 3333 CONTINUE
      END
      
      SUBROUTINE INCLINE(PSX,PSY,ANGLE,START_COL,GRID,SX,ENDX,SZ,ENDZ,NX,NZ,YY,YYY,C,CC)
      REAL ANGLE
      INTEGER START_COL,GRID,NX,NZ
      INTEGER PSX,PSY,SX,ENDX,SZ,ENDZ
      REAL YY(NX,NZ)
      CHARACTER(LEN=12) YYY(NX,NZ)
      CHARACTER(LEN=12) CC,CCTMP
      REAL C,CTMP
      REAL D,P
      INTEGER SC,EC,DIR
      INTEGER L,LI,LJ
      INTEGER CROP
      
      CROP=0  !标志是否被裁剪
      PSY=-1  !标志是否得到有效激发点
      
      IF(ANGLE .EQ. 90) THEN
        PSY=START_COL-1
        RETURN
      ENDIF
      
      IF(ANGLE .LT. 90) THEN
        D=1/tand(ANGLE)
        SC=START_COL+GRID
        EC=START_COL
        DIR=-1
      ELSE
        D=1/tand(180-ANGLE)
        SC=START_COL
        EC=START_COL+GRID
        DIR=1
      ENDIF
      
      DO 3389 L=SC,EC,DIR
        LJ=L
        LI=SX
        P=2*D-1
        CTMP=YY(LI,L)
        CCTMP=YYY(LI,L)
        DO WHILE (LI .LT. ENDX .AND. LJ .LT. ENDZ .AND. LJ .GT. SZ)
          LI=LI+1
          IF(P .GE. 0) THEN
            LJ=LJ+(-1)*DIR
            P=P+2*(D-1)
            CTMP=YY(LI,L)
            CCTMP=YYY(LI,L)
          ELSE
            P=P+2*D
          ENDIF
          YY(LI,LJ)=CTMP      
          YYY(LI,LJ)=CCTMP    
          YY(LI,L)=C
          YYY(LI,L)=CC
          
          IF(L .EQ. START_COL .AND. LI .EQ. PSX) THEN
            PSY=LJ
          ENDIF
        ENDDO
          
        DO WHILE (LI .LE. ENDX)
          YY(LI,L)=C
          YYY(LI,L)=CC
          LI=LI+1
        ENDDO
        
        IF(LJ .GE. ENDZ .OR. LJ .LE. SZ) THEN
          CROP=1
        ENDIF

3389  CONTINUE

      IF(PSY .EQ. -1) THEN
        WRITE(*,*) "WARNING: PULSE LINE OVERLAPPED!"
        PSY=START_COL-1
      ENDIF
      IF(CROP .EQ. 1) THEN
        WRITE(*,*) "WARNING: ANGLE NOT APPROPRIATE, CROPPED!"
      ENDIF
      END

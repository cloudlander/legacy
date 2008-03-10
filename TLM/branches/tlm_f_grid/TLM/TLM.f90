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
      REAL RVB(5,200,200,200)
      REAL IVB(5,200,200,200)
      
   	  REAL REEY(10,200,200,200),IMEY(10,200,200,200)

      REAL SB(5,5)
      REAL EY
      REAL YY(200,200)
      INTEGER YYY(200,200)
      REAL ER,ER1,ER2
	  INTEGER U,V,W
	  INTEGER LN,LI,LL
	  INTEGER L(10)
	  INTEGER SPLIT !SPLIT=1 按T分割输出文件, SPLIT=0 不分割
	  REAL FRE
      REAL time_begin, time_end
      INTEGER UNIT_BASE,UNIT_BASE2
      INTEGER FILE_UNIT
      INTEGER START_ROW,START_COL,GRID_ROW,GRID_COL   !网格开始位置
      INTEGER COL  !网格总列数
      REAL PART  !网格所占百分比
      INTEGER PSX !输入脉冲开始位置
      REAL PANGLE !输入脉冲直线与Z轴张角(角度参数)
      REAL PP !脉冲直线计算参数
      INTEGER PULSE(200,3) !脉冲点
      INTEGER PIND, PLEN !脉冲点序号和总数
      
      CHARACTER(LEN=5) NAME_COUNT

      UNIT_BASE=100
      UNIT_BASE2=16384
      
      OPEN(11,FILE='twotlme.in',FORM='FORMATTED')
      OPEN(111,FILE='grid.dat',FORM='FORMATTED')
      
      READ(11,*)SX,SY,SZ,ENDX,ENDY,ENDZ,X,Y,Z,U,V,W,H,ER,UR,SGM,NT,BL,SIDE1,SIDE2,ER1,ER2,FRE,LN,SPLIT,COL,PSX,PANGLE
!读入数据固定参数计算结构参数时使用
      IF (LN .NE. 0) THEN
        DO 220 III=1,LN
          READ(11,*)L(III)
  220   CONTINUE
      ELSE
        LN=16384
      ENDIF
      CLOSE(11)
      
      IF(SPLIT .EQ. 0) THEN
        FILE_UNIT=13
        OPEN(13,FILE='EY.out',FORM='FORMATTED')
      ENDIF
      
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
      WRITE(15,*) "set pm3d at s"
      WRITE(15,*) "set dgrid3d ",ENDZ-SZ+1,",",ENDX-SX+1
      WRITE(15,*) "set style data dots"
	  !"set xrange" 对应（ey.out第一列），为介质空间Z轴； "set yrange"对应（ey.out第二列），为介质空间X轴
	  !"set zrange" 0.03变为1.03为了画图不出现白色块
	  !"set pm3d map"为2D画图，"set pm3d at s"为3D画图
      
      J=10
      
      NX=200
      NY=200
      NZ=200
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
      
      !WRITE(*,*) ENDX,GRID_ROW,GRID_COL
!      YY=2*(U*W*SQRT(H)*ER/V-2)    !介电常数支线结构参数的计算(方向5即为介电常数支线)

      DO 2221 II=SX,ENDX
         DO 2222 KK=SZ,ENDZ
            YY(II,KK)=2*(U*W*SQRT(H)*ER/V-2)
            YYY(II,KK)=0
 2222    CONTINUE
 2221 CONTINUE


!  开始绘制脉冲空间位置(直线)
      II=PSX
      KK=START_COL
      YYY(II,KK)=-1
      PIND=1
      PULSE(PIND,1)=II
      PULSE(PIND,2)=J
      PULSE(PIND,3)=KK
      PANGLE=TAND(PANGLE)
      IF(PANGLE .GT. 1) THEN
        PP=2/PANGLE-1
      ELSE
        PP=2*PANGLE-1
      ENDIF
      DO WHILE(II .GT. SX .AND. KK .GT. SZ)
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
         PULSE(PIND,2)=J
         PULSE(PIND,3)=KK
         YYY(II,KK)=-1
      ENDDO
      PLEN=PIND
            
            
!  开始绘制网格            
      DO 3331 II=0,GRID_ROW
         DO 3332 KK=0,GRID_COL
            IF ( MOD((II+KK),2) .EQ. 1 ) THEN
               DO 3333 III=0,SIDE1-1
                  DO 3334 KKK=0,SIDE2-1
                      YY(START_ROW+II*SIDE1+III,START_COL+KK*SIDE2+KKK)=2*(U*W*SQRT(H)*ER1/V-2)
                      YYY(START_ROW+II*SIDE1+III,START_COL+KK*SIDE2+KKK)=1
 3334             CONTINUE
 3333          CONTINUE
            ELSE
               DO 3335 III=0,SIDE1-1
                  DO 3336 KKK=0,SIDE2-1
                      YY(START_ROW+II*SIDE1+III,START_COL+KK*SIDE2+KKK)=2*(U*W*SQRT(H)*ER2/V-2)
                      YYY(START_ROW+II*SIDE1+III,START_COL+KK*SIDE2+KKK)=2
 3336             CONTINUE
 3335          CONTINUE 
            ENDIF
 3332    CONTINUE
 3331 CONTINUE

!  输出全X-Z平面
      DO 55 I=SX,ENDX
         DO 66 K=SZ,ENDZ
            WRITE (111,*) K,I,YYY(I,K)
   66    CONTINUE
   55 CONTINUE
      CLOSE(111)
!      STOP

      GY=SGM*U*W*Z0/V              !损耗支线结构参数计算[SGM取0记不计该Gjj参数（程序中由GY表示）]
      

      DO 1 I=SX,ENDX           !初始化五个支线的输入向量，理解为五个不同的向量，分别代表物理上不同方向的支线
	    DO 3 K=SZ,ENDZ
	       DO 4 T=1,5   
		        IVB(T,I,J,K)=0
   4       CONTINUE
   3    CONTINUE
   1  CONTINUE
     
!     DO 5 I=1,1
!	    IVB(1,X,Y,Z)=1	!激励（选择激励点支线方向位置，赋激励值1，代表1v电压）
!   5 CONTINUE
       
      CALL CPU_TIME ( time_begin )
      
      
      DO 10 T=1,NT		!开始迭代
       
         DO 6 III=1,4
                PIND=PLEN/2
!             DO 992 PIND=1,PLEN
                IVB(III,PULSE(PIND,1),PULSE(PIND,2),PULSE(PIND,3))=IVB(III,PULSE(PIND,1),PULSE(PIND,2),PULSE(PIND,3))+sin(PI*T/15)  !为sin激发单色波形式
! 992         CONTINUE
!             IVB(III,X,Y,Z)=IVB(III,X,Y,Z)+sin(PI*T/15)  !为sin激发单色波形式    
  6      CONTINUE
    

	     DO 120 I=SX,ENDX		!结点散射的实施
	       DO 140 K=SZ,ENDZ
	       
             CALL SBL(SB,YY(I,K),GY)	!生成并联结点散射矩阵
      
		     DO 502 M=1,5
		        RVB(M,I,J,K)=0
		        DO 602 N=1,5 
			       RVB(M,I,J,K)=IVB(N,I,J,K)*SB(M,N)+RVB(M,I,J,K)
  602           CONTINUE
  502        CONTINUE
  140      CONTINUE   
  120    CONTINUE
      
	
         DO 20 I=SX,ENDX        
	       DO 40 K=SZ,ENDZ
		     IVB(1,I,J,K)=RVB(3,I-1,J,K)		!矩阵的实施
		     IVB(2,I,J,K)=RVB(4,I,J,K-1)
		     IVB(3,I,J,K)=RVB(1,I+1,J,K)
		     IVB(4,I,J,K)=RVB(2,I,J,K+1)
		     IVB(5,I,J,K)=RVB(5,I,J,K)      !介电常数/电容率  支线
		  
		     IF (BL.EQ.1) THEN		!电壁边界条件的处理
		        IF (K.EQ.SZ) THEN
			       IVB(2,I,J,K)=-RVB(2,I,J,K)
		        END IF
		        IF (K.EQ.ENDZ) THEN    
			       IVB(4,I,J,K)=-RVB(4,I,J,K)
		        END IF
		        IF (I.EQ.SX) THEN
			       IVB(1,I,J,K)=-RVB(1,I,J,K)
		        END IF
		        IF (I.EQ.ENDX) THEN 
			       IVB(3,I,J,K)=-RVB(3,I,J,K)
		        END IF
		     END IF
	         IF (BL.EQ.0) THEN		!吸收边界条件的处理
	            IF (K.EQ.SZ) THEN
		           IVB(2,I,J,K)=0
	            END IF
	            IF (K.EQ.ENDZ) THEN    
		           IVB(4,I,J,K)=0
	            END IF
	            IF (I.EQ.SX) THEN
		           IVB(1,I,J,K)=0
	            END IF
	            IF (I.EQ.ENDX) THEN 
		           IVB(3,I,J,K)=0
	            END IF
	         END IF
   40          CONTINUE   
   20    CONTINUE
      
      N1=MOD(T,10)
      IF (N1.EQ.0) THEN
	  WRITE(*,*)T
      END IF
      
      WRITE(NAME_COUNT,'(I5)') T
      IF (SPLIT .EQ. 1) THEN
          FILE_UNIT=UNIT_BASE+T
          OPEN(FILE_UNIT,FILE='EY/EY'//NAME_COUNT//'.out',FORM='FORMATTED')
      ENDIF
         
      WRITE(FILE_UNIT,*) "#T=",T
      DO 101 II=SX,ENDX        
	       DO 102 KK=SZ,ENDZ
                CALL EJ(EY,IVB,YY,GY,II,J,KK,V,NX,NY,NZ)		!将电压转换为电场
                WRITE(FILE_UNIT,117) KK,II,EY
                
                DO 16384 LI=1,LN
                   IF(LN .EQ. 16384) THEN
                      LL=LI
                   ELSE
                      LL=L(LI)
                   ENDIF
                   CALL FT(REEY(LI,II,J,KK),IMEY(LI,II,J,KK),EY,LL,T) !进行傅立叶变换
16384           CONTINUE
     
   102	  CONTINUE
   101 CONTINUE
       WRITE(FILE_UNIT,*)
       WRITE(FILE_UNIT,*)
       IF (SPLIT .EQ. 1) THEN
         CLOSE(FILE_UNIT)
       ENDIF


   10 CONTINUE     
   
      DO 16386 LI=1,LN
          IF(LN .EQ. 16384) THEN
             LL=LI
          ELSE
             LL=L(LI)
          ENDIF
          WRITE(NAME_COUNT,'(I5)') LL
          OPEN(UNIT_BASE2+LL,FILE='FEY/FEY'//NAME_COUNT//'.out',FORM='FORMATTED')
16386 CONTINUE

      DO 1011 II=SX,ENDX        
         DO 1012 KK=SZ,ENDZ      
            DO 16385 LI=1,LN
               IF(LN .EQ. 16384) THEN
                  LL=LI
               ELSE
                  LL=L(LI)
               ENDIF
               FEY=SQRT(REEY(LI,II,J,KK)**2 + IMEY(LI,II,J,KK)**2)
               WRITE(UNIT_BASE2+LL,117),KK,II,FEY
16385       CONTINUE
1012     CONTINUE
1011  CONTINUE
   
      DO 16387 LI=1,LN
          IF(LN .EQ. 16384) THEN
             LL=LI
          ELSE
             LL=L(LI)
          ENDIF
          CLOSE(UNIT_BASE2+LL)
16387 CONTINUE
   
!      DO 222 T=1,NT
!      WRITE(13,117) EY(T)		!保存模拟结果
  222 CONTINUE

  117 format(I, I, 7f12.7)
  118 format(7f12.7)
  119 format(I)


      IF (SPLIT .EQ. 0) THEN
         CLOSE(FILE_UNIT)
      ENDIF
	  CLOSE(15)
	  
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
      
      SUBROUTINE FT(FREAL,FIM,E,LL,T)
      INTEGER LL,T
	  REAL TLBB,FREAL,FIM
 !    进行傅立叶变换，其中LL和T分别为主函数中L和T，表示频域的第几个点和时域的第几个点
      TLBB=(LL*(0.1-0)/16384.0+0)
      FREAL=FREAL+E*COS(2*3.14159*T*TLBB/2.0)
      FIM=FIM+E*SIN(2*3.14159*T*TLBB/2.0)
      END

      FUNCTION GAUSS(T)
      INTEGER T
      REAL T0,TAO
      REAL PI
      PI=3.1415926
      T0=0
      TAO=1.0
      GAUSS=EXP(-4.0*PI*(T-T0)*(T-T0)/(TAO*TAO))
      END

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
      INTEGER SX,SY,SZ,ENDX,ENDY,ENDZ,NX,NY,NZ,X,Y,Z,T,BL
!网格单位长度，起始网格位置结束网格位置，边界条件标志参数
      REAL RVB(0:9,100,100,100)
      REAL IVB(0:9,100,100,100)
      REAL SB(0:5,0:5)
      REAL SB2(9,9)
      REAL EY
      REAL CC,PI,HSTEP,FRE !CC为光速，HSTEP为YY计算时所用的步长,FRE为频率设定
      INTEGER U,V,W
	  INTEGER CL !CL=0 不加载负折射率介质,  CL=1 加载负折射率介质
	  INTEGER SPLIT !SPLIT=1 按T分割输出文件, SPLIT=0 不分割


      REAL SQUARE2,Z0L,VL,DL,EN,UN,EM,UM,POW2W,C0,L0,ZS,ZP,ZN1,ZN2
      
      REAL time_begin, time_end
      INTEGER UNIT_BASE
      INTEGER FILE_UNIT
      
      CHARACTER*(*) NAME_BASE
      CHARACTER(LEN=5) NAME_COUNT
      PARAMETER (NAME_BASE = 'img/ey')

      UNIT_BASE=100
            
      OPEN(11,FILE='twotlme.in',FORM='FORMATTED')            
	  READ(11,*)SX,SY,SZ,ENDX,ENDY,ENDZ,X,Y,Z,U,V,W,HSTEP,ER,UR,SGM,NT,BL,ERM,URM,CL,FRE,SPLIT
!读入数据固定参数计算结构参数时使用
      CLOSE(11)
      
      IF(SPLIT .EQ. 0) THEN
        FILE_UNIT=13
        OPEN(13,FILE='ey.out',FORM='FORMATTED')
      ENDIF
      
      OPEN(14,FILE='result.gnu',FORM='FORMATTED')
      OPEN(15,FILE='set.gnu',FORM='FORMATTED')
      
      WRITE(14,*) "set samples 100, 100"
      WRITE(14,*) "set isosamples 10, 10"
      WRITE(14,*) "set autoscale"
      WRITE(14,*) "set size square"
      WRITE(14,*) "set surface"
      WRITE(14,*) 'set title "TLM 2D"'
      WRITE(14,*) 'set xlabel "Z"'
      WRITE(14,*) 'set ylabel "X"'
      WRITE(14,*) "set xrange [",SZ,":",ENDZ,"] noreverse nowriteback"
      WRITE(14,*) "set yrange [",SX,":",ENDX,"] noreverse nowriteback"
      WRITE(14,*) "set zrange [ -0.300000 : 0.300000 ] noreverse nowriteback"
      WRITE(14,*) "set cbrange [ -0.300000 : 0.300000 ] noreverse nowriteback"
      WRITE(14,*) "set zero 1e-0020"
      WRITE(14,*) "set pm3d at s"
      WRITE(14,*) "set dgrid3d ",ENDZ-SZ+1,",",ENDX-SX+1
      write(14,*) "set terminal png size 1024,768"
      write(14,*) "set style data dots"
  
      WRITE(15,*) "set samples 100, 100"
      WRITE(15,*) "set isosamples 10, 10"
      WRITE(15,*) "set autoscale"
      WRITE(15,*) "set size square"
      WRITE(15,*) "set surface"
      WRITE(15,*) 'set title "TLM 2D"'
      WRITE(14,*) 'set xlabel "Z"'
      WRITE(14,*) 'set ylabel "X"'
      WRITE(15,*) "set xrange [",SZ,":",ENDZ,"] noreverse nowriteback"
      WRITE(15,*) "set yrange [",SX,":",ENDX,"] noreverse nowriteback"
      WRITE(15,*) "set zrange [ -0.300000 : 0.300000 ] noreverse nowriteback"
      WRITE(15,*) "set cbrange [ -0.300000 : 0.300000 ] noreverse nowriteback"
      WRITE(15,*) "set zero 1e-0020"
      WRITE(15,*) "set pm3d at s"
      WRITE(15,*) "set dgrid3d ",ENDZ-SZ+1,",",ENDX-SX+1
      write(15,*) "set style data dots"
       
	  !"set xrange" 对应（ey.out第一列），为介质空间Z轴； "set yrange"对应（ey.out第二列），为介质空间X轴
	  !"set zrange" 0.03变为1.03为了画图不出现白色块
	  !"set pm3d map"为2D画图，"set pm3d at s"为3D画图
      
      
      CC=3E8             !CC为光速
      PI=3.1415926

      NX=100
      NY=100
      NZ=100
      
      U0=4*PI*1E-7		!真空中的磁导率
      E0=8.854187818E-12		!真空中的介电常数
      Z0=SQRT(U0/E0)
      YY=2*(U*W*SQRT(HSTEP)*ER/V-2)    !介电常数支线结构参数的计算(方向5即为介电常数支线)
      GY=SGM*U*W*Z0/V              !损耗支线结构参数计算[SGM取0记不计该Gjj参数（程序中由GY表示）]

      SQUARE2=sqrt(2.0)
      Z0L=SQUARE2*Z0
      VL=SQUARE2*CC
      DL=CC/(30*FRE) 
      EN=E0
      UN=U0
      EM=E0*ERM
      UM=U0*URM
      POW2W=(2*PI*FRE)**2
      C0=-1/(POW2W*(UM-UN)*DL)
      L0=-1/(POW2W*(EM-EN)*DL)

      ZS=DL/(4*C0*VL)
      ZP=2*L0*VL/DL;
      ZN1=ZP*(ZS+Z0L)/(3*ZP+ZS+Z0L);
      ZN2=(ZS+Z0L)/4; 

      J=10
      
      DO 1 I=SX,ENDX           !初始化五个支线的输入向量，理解为五个不同的向量，分别代表物理上不同方向的支线
	    DO 3 K=SZ,ENDZ
	       DO 4 T=0,9 
	             IVB(T,I,J,K)=0
   4       CONTINUE
   3    CONTINUE
   1  CONTINUE
     
!      DO 5 I=1,1    !激励（选择激励点支线方向位置，赋激励值1，代表1v电压）
!	       IVB(I,X,Y,Z)=1	 !dirac函数激励（只激励一次）
!  5   CONTINUE

      CALL SBL(SB,YY,GY)	!生成并联结点散射矩阵
      CALL SBL2(SB2,Z0,ZP,ZS,Z0L,ZN1,ZN2)	!生成并联结点散射矩阵
      
      CALL CPU_TIME ( time_begin )
      
      DO 10 T=1,NT		!开始迭代
           
               DO 335 III=1,4
    !		      IVB(III,X,Y,Z)=1  !为点光源持续一个幅度激励
    !             IVB(III,X,Y,Z)=sin(2*PI*FRE*T)  !为sin激发 单色波形式
    !             IVB(III,X,Y,Z)=sin(6.283E+10*(0.33333E-11)*T)  !为sin激发单色波形式
    !             0.3333E-11=1/3E-11=dl/CC=deltaL/C(一个格点宽度/光速)
                 IVB(III,X,Y,Z)=sin(PI*T/15)  !为sin激发单色波形式

       335     CONTINUE

               DO 120 I=SX,ENDX		!结点散射的实施
                  DO 140 K=SZ,ENDZ
                     IF (CL .EQ. 1 .AND. K .GE. SZ+(ENDZ-SZ)/3 .AND. K .LE. SZ+(ENDZ-SZ)/3*2) THEN
	                     DO 502 M=1,9
	                         RVB(M,I,J,K)=0
	                         DO 602 N=1,9 
		                         RVB(M,I,J,K)=IVB(N,I,J,K)*SB2(M,N)+RVB(M,I,J,K)
          602                CONTINUE
          502            CONTINUE
                     ELSE
	                     DO 503 M=0,4
	                         RVB(M,I,J,K)=0
	                         DO 603 N=0,4
		                        RVB(M,I,J,K)=IVB(N,I,J,K)*SB(M,N)+RVB(M,I,J,K)
          603                CONTINUE
          503            CONTINUE
                     ENDIF
     140          CONTINUE   
     120       CONTINUE
      
	
              DO 20 I=SX,ENDX        
               DO 40 K=SZ,ENDZ
               
		          IVB(1,I,J,K)=RVB(3,I-1,J,K)		!矩阵的实施
		          IVB(2,I,J,K)=RVB(4,I,J,K-1)
		          IVB(3,I,J,K)=RVB(1,I+1,J,K)
		          IVB(4,I,J,K)=RVB(2,I,J,K+1)

                  IF (CL .EQ. 1 .AND. K .GE. SZ+(ENDZ-SZ)/32 .AND. K .LE. SZ+(ENDZ-SZ)/3*2) THEN
		             IVB(5,I,J,K)=RVB(5,I,J,K)      
			         IVB(6,I,J,K)=RVB(6,I,J,K)
			         IVB(7,I,J,K)=RVB(7,I,J,K)
			         IVB(8,I,J,K)=RVB(8,I,J,K)
			         IVB(9,I,J,K)=-RVB(9,I,J,K)
		          ELSE
		             IVB(0,I,J,K)=RVB(0,I,J,K)      
                  ENDIF
    				  
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

       40      CONTINUE   
       20    CONTINUE
      
      N1=MOD(T,10)
      IF (N1.EQ.0) THEN
      WRITE(*,*)T
      END IF

!      DO 555 III=SX,ENDX
!	    DO 666 KKK=SZ,ENDZ
!		   IF( IVB(0,III,J,KKK) .NE. IVB(5,III,J,KKK) ) THEN
!		      WRITE (*,*) "DIFF: ",III,KKK,"IVB(0)=",IVB(0,III,J,KKK),"IVB(5)=",IVB(5,III,J,KKK)
!		   ENDIF
!  666   CONTINUE
!  555 CONTINUE

      WRITE(NAME_COUNT,'(I5)') T
      IF (SPLIT .EQ. 1) THEN
          FILE_UNIT=UNIT_BASE+T
          OPEN(FILE_UNIT,FILE='ey/ey'//NAME_COUNT//'.out',FORM='FORMATTED')
      ENDIF
         
      WRITE(FILE_UNIT,*) "#T=",T
      DO 101 II=SX,ENDX        
	       DO 102 KK=SZ,ENDZ
                  IF (CL .EQ. 1 .AND. KK .GE. SZ+(ENDZ-SZ)/3 .AND. KK .LE. SZ+(ENDZ-SZ)/3*2) THEN
                     CALL EJ2(EY,IVB,ZP,ZS,Z0L,ZN1,ZN2,II,J,KK,NX,NY,NZ)		!将电压转换为电场
                    WRITE(FILE_UNIT,117) KK,II,EY
!				      WRITE(13,118) EY
                  ELSE
                     CALL EJ(EY,IVB,YY,GY,II,J,KK,V,NX,NY,NZ)		!将电压转换为电场
                    WRITE(FILE_UNIT,117) KK,II,EY
!                     WRITE(13,118) EY
                  ENDIF
   102	  CONTINUE
   101 CONTINUE
       WRITE(FILE_UNIT,*)
       WRITE(FILE_UNIT,*)
       IF (SPLIT .EQ. 1) THEN
         CLOSE(FILE_UNIT)
         write(14,*) 'set output '//'"img/img'//NAME_COUNT//'.png"'
         WRITE(14,*) 'splot '//'"ey/ey'//NAME_COUNT//'.out"'
       ELSE
         write(14,*) 'set output '//'"img/img'//NAME_COUNT//'.png"'
         WRITE(14,*) 'splot "ey.out" index ',T-1
       ENDIF

   10 CONTINUE     
!      DO 222 T=1,NT
!      WRITE(13,117) EY(T)		!保存模拟结果
!  222 CONTINUE

  117 format(I, I, 7f12.7)
  118 format(7f12.7)
!  119 format(I, I, 7f12.7, 7f12.7, 7f12.7, 7f12.7, 7f12.7, 7f12.7, 7f12.7, 7f12.7, 7f12.7, 7f12.7 )

      IF (SPLIT .EQ. 0) THEN
         CLOSE(FILE_UNIT)
      ENDIF
      WRITE(14,*) "exit"      
      CLOSE(14)
	  CLOSE(15)
	  
	  CALL CPU_TIME ( time_end )
	  
	  PRINT *, 'Time of operation was ',time_end - time_begin, ' seconds'
	  
      STOP
      END          

      SUBROUTINE SBL(S,Y,G)  !生成[S]散射矩阵
      REAL S(0:5,0:5)
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

      DO 222 J=1,5
	     S(0,J)=S(5,J)
  222 CONTINUE
      DO 223 I=0,5
	     S(I,0)=S(I,5)
  223 CONTINUE
      RETURN
      END

      SUBROUTINE SBL2(SB2,Z0,ZP,ZS,Z0L,ZN1,ZN2)
        REAL SB2(9,9)
        INTEGER I,J
        REAL a,b,c,d,e,f,g,h,p,q
        REAL Z0,ZP,ZS,Z0L,ZN1,ZN2

        a=(ZS+ZN1-Z0L)/(ZS+ZN1+Z0L)
        b=(Z0L+ZN1-ZS)/(Z0L+ZN1+ZS)
        c=(ZN2-ZP)/(ZN2+ZP)
        d=2*ZN1*Z0L/((Z0L+ZS+ZN1)*(Z0L+ZS))
        e=2*Z0L/(ZS+ZN1+Z0L)
        f=2*ZS/(ZS+ZN1+Z0L)
        g=2*ZN1*ZS/((Z0L+ZS+ZN1)*(Z0L+ZS))
        h=2*ZN1/(Z0L+ZS+ZN1)
        p=2*ZN2*Z0L/((Z0L+ZS)*(ZP+ZN2))
        q=2*ZN2*ZS/((Z0L+ZS)*(ZP+ZN2))

        DO 10001 I=1,4
            DO 10002 J=1,4
                IF(I.EQ.J) THEN
                    SB2(I,J)=a
                ELSE
                    SB2(I,J)=d
                ENDIF
  10002     CONTINUE
  10001 CONTINUE

        DO 10003 I=1,4
            DO 10004 J=5,8
                  IF (I+4 .EQ. J) THEN
                    SB2(I,J)=e
                  ELSE
                    SB2(I,J)=-d
                  ENDIF
  10004     CONTINUE
  10003 CONTINUE

        DO 10005 I=5,8
            DO 10006 J=1,4
                IF(I-4 .EQ. J) THEN
                    SB2(I,J)=f
                ELSE
                    SB2(I,J)=-g
                ENDIF
  10006     CONTINUE
  10005 CONTINUE

        DO 10007 I=5,8
            DO 10008 J=5,8
                IF(I .EQ. J) THEN
                    SB2(I,J)=b
                ELSE
                    SB2(I,J)=g
                ENDIF
  10008     CONTINUE
  10007 CONTINUE

        DO 10009 I=1,4
            SB2(I,9)=p
  10009 CONTINUE
        DO 10010 I=5,8
            SB2(I,9)=-q
  10010 CONTINUE
        DO 10011 I=1,4
            SB2(9,I)=h
  10011 CONTINUE
        DO 10012 I=5,8
            SB2(9,I)=-h
  10012 CONTINUE
        SB2(9,9)=c
      END

      SUBROUTINE EJ(E,IV,Y,G,I,J,K,L,NX,NY,NZ)
	  INTEGER NX,NY,NZ,I,J,K
      REAL IV(0:9,NX,NY,NZ)
	  REAL E,Y,G
	  INTEGER L
      E=0.0
      DO 100 M=1,4
	     E=E+IV(M,I,J,K)
  100 CONTINUE
      E=E+Y*IV(0,I,J,K)
      E=E*2/(L*(4+Y+G))
      RETURN
      END

      SUBROUTINE EJ2(E,IV,ZP,ZS,Z0L,ZN1,ZN2,I,J,K,NX,NY,NZ)
        INTEGER NX,NY,NZ,I,J,K,M
        REAL E,E1,E2,ZP,ZS,Z0L,ZN1,ZN2
        REAL IV(0:9,NX,NY,NZ)
        E=0.0
        E1=0.0
        E2=0.0
        DO 20001 M=1,8
            E1=E1+IV(M,I,J,K);
  20001 CONTINUE
        E2=2*(Z0L+ZS)*IV(9,I,J,K);
        E=(2*ZP*E1+E2)/(4*ZP+Z0L+ZS);
!		E=(2*ZP*E1)/(4*ZP+Z0L+ZS);
      END

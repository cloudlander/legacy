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
      REAL RVB(5,100,100,100)
      REAL IVB(5,100,100,100)
      REAL SB(5,5)
      REAL EY(7000)
	  INTEGER U,V,W
      
      OPEN(11,FILE='twotlme.in',FORM='FORMATTED')
      OPEN(13,FILE='ey.out',FORM='FORMATTED')
	  OPEN(14,FILE='eyJ.out',FORM='FORMATTED')

      READ(11,*)SX,SY,SZ,ENDX,ENDY,ENDZ,X,Y,Z,U,V,W,H,ER,UR,SGM,NT,BL
!读入数据固定参数计算结构参数时使用
      CLOSE(11)
      NX=100
      NY=100
      NZ=100
      U0=12566.3706144		!真空中的磁导率
      E0=8.854187818E-2		!真空中的介电常数
      Z0=SQRT(U0/E0)
      YY=2*(U*W*SQRT(H)*ER/V-2)    !介电常数支线结构参数的计算(方向5即为介电常数支线)
      GY=SGM*U*W*Z0/V              !损耗支线结构参数计算[SGM取0记不计该Gjj参数（程序中由GY表示）]
      J=10
      DO 1 I=SX,ENDX           !初始化五个支线的输入向量，理解为五个不同的向量，分别代表物理上不同方向的支线
	    DO 3 K=SZ,ENDZ
	       DO 4 T=1,5   
		  IVB(T,I,J,K)=0
   4           CONTINUE
   3        CONTINUE
   1  CONTINUE
     
! DO 5 I=1,1
!	  IVB(1,X,Y,Z)=1	!激励（选择激励点支线方向位置，赋激励值1，代表1v电压）
!   5  CONTINUE

      CALL SBL(SB,YY,GY)	!生成并联结点散射矩阵
      
      DO 10 T=1,NT		!开始迭代
       
         DO 6 I=1,4
	       IVB(I,X,Y,Z)=1	!激励（选择激励点支线方向位置，赋激励值1，代表1v电压）
    6    CONTINUE

	  DO 120 I=SX,ENDX		!结点散射的实施
	       DO 140 K=SZ,ENDZ
		     DO 502 M=1,5
		       RVB(M,I,J,K)=0
		        DO 602 N=1,5 
			       RVB(M,I,J,K)=IVB(N,I,J,K)*SB(M,N)+RVB(M,I,J,K)
  602           CONTINUE
  502        CONTINUE
  140      CONTINUE   
  120 CONTINUE
      
	
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
      CALL EJ(EY(T),IVB,YY,GY,X,Y,Z,V,NX,NY,NZ)		!将电压转换为电场
	  write(14,118) EY(T)

      WRITE(13,*) "#T=",T
      DO 101 II=SX,ENDX        
	       DO 102 KK=SZ,ENDZ
               CALL EJ(EY(T),IVB,YY,GY,II,J,KK,V,NX,NY,NZ)		!将电压转换为电场
                WRITE(13,117) KK,II,EY(T)
!				write(13,118) EY(T)
   102	   CONTINUE
   101 CONTINUE
   	  WRITE(13,*)
   	  WRITE(13,*)


   10 CONTINUE     
!      DO 222 T=1,NT
!      WRITE(13,117) EY(T)		!保存模拟结果
  222 CONTINUE

  117 format(I, I, 7f12.7)
  118 format(7f12.7)


      CLOSE(13)
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
	  INTEGER L
      E=0.0
      DO 100 M=1,4
	     E=E+IV(M,I,J,K)
  100 CONTINUE
      E=E+Y*IV(5,I,J,K)
      E=E*2/(L*(4+Y+G))
      RETURN
      END


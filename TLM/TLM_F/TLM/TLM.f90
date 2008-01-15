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
!����λ���ȣ���ʼ����λ�ý�������λ�ã��߽�������־����
      REAL RVB(5,100,100,100)
      REAL IVB(5,100,100,100)
      REAL SB(5,5)
      REAL EY(7000)
	  INTEGER U,V,W
      
      OPEN(11,FILE='twotlme.in',FORM='FORMATTED')
      OPEN(13,FILE='ey.out',FORM='FORMATTED')
	  OPEN(14,FILE='eyJ.out',FORM='FORMATTED')

      READ(11,*)SX,SY,SZ,ENDX,ENDY,ENDZ,X,Y,Z,U,V,W,H,ER,UR,SGM,NT,BL
!�������ݹ̶���������ṹ����ʱʹ��
      CLOSE(11)
      NX=100
      NY=100
      NZ=100
      U0=12566.3706144		!����еĴŵ���
      E0=8.854187818E-2		!����еĽ�糣��
      Z0=SQRT(U0/E0)
      YY=2*(U*W*SQRT(H)*ER/V-2)    !��糣��֧�߽ṹ�����ļ���(����5��Ϊ��糣��֧��)
      GY=SGM*U*W*Z0/V              !���֧�߽ṹ��������[SGMȡ0�ǲ��Ƹ�Gjj��������������GY��ʾ��]
      J=10
      DO 1 I=SX,ENDX           !��ʼ�����֧�ߵ��������������Ϊ�����ͬ���������ֱ���������ϲ�ͬ�����֧��
	    DO 3 K=SZ,ENDZ
	       DO 4 T=1,5   
		  IVB(T,I,J,K)=0
   4           CONTINUE
   3        CONTINUE
   1  CONTINUE
     
! DO 5 I=1,1
!	  IVB(1,X,Y,Z)=1	!������ѡ������֧�߷���λ�ã�������ֵ1������1v��ѹ��
!   5  CONTINUE

      CALL SBL(SB,YY,GY)	!���ɲ������ɢ�����
      
      DO 10 T=1,NT		!��ʼ����
       
         DO 6 I=1,4
	       IVB(I,X,Y,Z)=1	!������ѡ������֧�߷���λ�ã�������ֵ1������1v��ѹ��
    6    CONTINUE

	  DO 120 I=SX,ENDX		!���ɢ���ʵʩ
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
		  IVB(1,I,J,K)=RVB(3,I-1,J,K)		!�����ʵʩ
		  IVB(2,I,J,K)=RVB(4,I,J,K-1)
		  IVB(3,I,J,K)=RVB(1,I+1,J,K)
		  IVB(4,I,J,K)=RVB(2,I,J,K+1)
		  IVB(5,I,J,K)=RVB(5,I,J,K)      !��糣��/������  ֧��
		  
		  IF (BL.EQ.1) THEN		!��ڱ߽������Ĵ���
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
		  IF (BL.EQ.0) THEN		!���ձ߽������Ĵ���
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
      CALL EJ(EY(T),IVB,YY,GY,X,Y,Z,V,NX,NY,NZ)		!����ѹת��Ϊ�糡
	  write(14,118) EY(T)

      WRITE(13,*) "#T=",T
      DO 101 II=SX,ENDX        
	       DO 102 KK=SZ,ENDZ
               CALL EJ(EY(T),IVB,YY,GY,II,J,KK,V,NX,NY,NZ)		!����ѹת��Ϊ�糡
                WRITE(13,117) KK,II,EY(T)
!				write(13,118) EY(T)
   102	   CONTINUE
   101 CONTINUE
   	  WRITE(13,*)
   	  WRITE(13,*)


   10 CONTINUE     
!      DO 222 T=1,NT
!      WRITE(13,117) EY(T)		!����ģ����
  222 CONTINUE

  117 format(I, I, 7f12.7)
  118 format(7f12.7)


      CLOSE(13)
      STOP
      END          

      SUBROUTINE SBL(S,Y,G)  !����[S]ɢ�����
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


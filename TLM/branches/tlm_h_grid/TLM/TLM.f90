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

      PROGRAM TWOTLMH
      INTEGER SX,SY,SZ,ENDX,ENDY,ENDZ,NX,NY,NZ,X,Y,Z,T,BL,SIDE1,SIDE2   
!����λ���ȣ���ʼ����λ�ý�������λ�ã��߽�������־����

      REAL, ALLOCATABLE:: RVE(:,:,:,:),IVE(:,:,:,:)
      REAL, ALLOCATABLE:: REHY(:,:,:,:),IMHY(:,:,:,:)

      REAL SE(5,5)
      REAL HY,FHY

      INTEGER, ALLOCATABLE:: PULSE(:,:)   !�����
      INTEGER, ALLOCATABLE:: L(:)
      REAL, ALLOCATABLE:: ZY(:,:)
	  CHARACTER(LEN=12), ALLOCATABLE:: ZYY(:,:)
      REAL, ALLOCATABLE:: GAUSS(:,:,:),GAUSS2(:,:,:)
      CHARACTER(LEN=12) ER_COLOR,ER1_COLOR,ER2_COLOR,P_COLOR  !���ʵ���ɫ

	  REAL GMAX,W0
      REAL ER,ER1,ER2
	  INTEGER U,V,W
	  INTEGER LN,LI,LL

	  INTEGER SPLIT !SPLIT=1 ��T�ָ�����ļ�, SPLIT=0 ���ָ�
	  REAL FRE   !�Ⲩʵ��Ƶ��
	  REAL NORMAL_FRE  !��һ��Ƶ��
      REAL time_begin, time_end
      INTEGER UNIT_BASE,UNIT_BASE2
      INTEGER FILE_UNIT
      INTEGER START_ROW,START_COL,GRID_ROW,GRID_COL   !����ʼλ��
      INTEGER COL  !����������
      REAL PART  !������ռ�ٷֱ�
      
	  !!!!!!!!!!!!!        NANGLE������2D�汾�в�����Ч       !!!!!!!!!!!!!!1
	  REAL NANGLE !����ƫ�ƽǶ�(��Z���Ž�,ȡֵ��ΧΪ(0,180) 90Ϊ����б

      INTEGER GTYPE !�������� 0Ϊ����, 1Ϊ������Բ(��Բ), 2Ϊ����������
      INTEGER PSX,PSZ !�������忪ʼλ��
	  INTEGER PENDZ !�������������Z����
      REAL PANGLE !��������ֱ����Z���Ž�(�ǶȲ���)
      REAL PLENGTH !��������ֱ�ߵĳ���
      REAL PP !����ֱ�߼������
      INTEGER PIND, PLEN !�������ź�����
      
	  !������������,����Ϊȡֵ˵��:
      !=0: ʹ��PSX,PSZ,PENDZ��PLENGTH�Զ���������ֱ��,ֱ����תPANGLE�Ƕ�,��������2��Ч���ļ�
      !>0: ʹ�������ļ���X,Z������Ϊ�����,������תPANGLE�Ƕ�,����һ��Ч���ļ�
      INTEGER PN 

      CHARACTER(LEN=5) NAME_COUNT

      UNIT_BASE=100
      UNIT_BASE2=16384
      ER_COLOR="255 255 255 "  !Ĭ��Ϊ��ɫ
      ER1_COLOR="255 0 0     "
      ER2_COLOR="0 0 255     "
      P_COLOR="0 255 0     "   !����Ϊ��ɫ
	        
      OPEN(11,FILE='twotlmh.in',FORM='FORMATTED')

      !�������ݹ̶���������ṹ����ʱʹ��      
      READ(11,*)SX,SY,SZ,ENDX,ENDY,ENDZ,X,Y,Z,U,V,W,H,ER,UR,SGM,NT,BL,SPLIT,SIDE1,SIDE2,ER1,ER2,NANGLE,GTYPE,COL,FRE,NORMAL_FRE,W0,PN,PSX,PSZ,PENDZ,PANGLE,LN
      
      IF(SPLIT .EQ. 0) THEN
        FILE_UNIT=13
        OPEN(13,FILE='HY.out',FORM='FORMATTED')
      ENDIF
      
      !set.gnu����ʵ��Ӧ��Ч��,���������tlm.py��
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
	  !"set xrange" ��Ӧ��HY.out��һ�У���Ϊ���ʿռ�Z�᣻ "set yrange"��Ӧ��HY.out�ڶ��У���Ϊ���ʿռ�X��
	  !"set zrange" 0.03��Ϊ1.03Ϊ�˻�ͼ�����ְ�ɫ��
	  !"set pm3d map"Ϊ2D��ͼ��"set pm3d at s"Ϊ3D��ͼ
      
      
      NX=200
      NY=200
      NZ=200
      U0=12566.3706144		!����еĴŵ���
      E0=8.854187818E-2		!����еĽ�糣��
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

      ALLOCATE (RVE(5,NX,NY,NZ),IVE(5,NX,NY,NZ))
      ALLOCATE (REHY(10,NX,NY,NZ),IMHY(10,NX,NY,NZ))

      ALLOCATE (GAUSS(NX,NY,NZ),GAUSS2(NX,NY,NZ))
      ALLOCATE (ZY(NX,NZ))
	  ALLOCATE (ZYY(NX,NZ))
      ALLOCATE (PULSE(NX,3))

      !  ��ʼ��X-Zƽ��ER
      DO 2221 II=SX,ENDX
         DO 2222 KK=SZ,ENDZ
            ZY(II,KK)=2*(U*W*SQRT(H)*ER/V-2)  !��糣��֧�߽ṹ�����ļ���(����5��Ϊ��糣��֧��)
            ZYY(II,KK)=ER_COLOR
 2222    CONTINUE
 2221 CONTINUE

!  ��ʼ��������            
      DO 3331 II=0,GRID_ROW-1
         DO 3332 KK=0,GRID_COL-1
            IF ( MOD((II+KK),2) .EQ. 1 ) THEN
               CALL COLOR(GTYPE,SIDE1,SIDE2,START_ROW+II*SIDE1,START_COL+KK*SIDE2,ZY,ZYY,NX,NZ,2*(U*W*SQRT(H)*ER1/V-2),ER1_COLOR)
            ELSE
               CALL COLOR(GTYPE,SIDE1,SIDE2,START_ROW+II*SIDE1,START_COL+KK*SIDE2,ZY,ZYY,NX,NZ,2*(U*W*SQRT(H)*ER2/V-2),ER2_COLOR)
            ENDIF
 3332    CONTINUE
 3331 CONTINUE

      !���������λ������X,Z����
      IF(PN .GT. 0) THEN

        DO 230 PIND=1,PN
          READ(11,*)PULSE(PIND,1),PULSE(PIND,3)
          PULSE(PIND,2)=Y
          ZYY(PULSE(PIND,1),PULSE(PIND,3))=P_COLOR
  230   CONTINUE
        PLEN=PN
        
      ELSE IF(PN .EQ. 0) THEN
      
      !  ��ʼ��������ռ�λ��(ֱ��),������ʹ�������ļ���ָ��������ʱ�Զ�����ֱ��
          CALL LINE(X,Y,Z,PSX,PSZ,PENDZ,SX,ENDX,PANGLE,ZYY,NX,NZ,PULSE,PIND,PLEN,P_COLOR)      
          
      ENDIF


!  ���ȫX-Zƽ���ʵ�ʼ���Ч��ͼ
      OPEN(111,FILE='grid.ppm',FORM='FORMATTED')
      WRITE (111,'(A2)')"P3"
      WRITE (111,'(5I )',advance='no')ENDZ-SZ+1
      WRITE (111,'(5I)')ENDX-SX+1
      WRITE (111,'(A3)')"255"
      DO 56 I=ENDX,SX,-1
         DO 67 K=SZ,ENDZ
            WRITE (111,'(A12)',advance='no') ZYY(I,K)
   67    CONTINUE
   56 CONTINUE
      WRITE (111,*)
      CLOSE(111)
      WRITE(*,*) "grid.ppm generated"
	  WRITE(*,*) "START_COL:", START_COL
      WRITE(*,*) "PULSE POINTS:"
      DO 77 PIND=1,PLEN
        WRITE(*,*) PULSE(PIND,1),PULSE(PIND,3)
   77 CONTINUE

      !����Ƶ�������
	  ALLOCATE (L(LN))
      IF (LN .NE. 0) THEN
        DO 220 III=1,LN
          READ(11,*)L(III)
  220   CONTINUE
      ELSE
        LN=16384
      ENDIF
      CLOSE(11)
     
      ! Ӧ���ø�˹����
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


      !J����̶�Ϊ�������Yin
      J=Y 
	  
      GY=SGM*U*W*Z0/V              !���֧�߽ṹ��������[SGMȡ0�ǲ��Ƹ�Gjj��������������GY��ʾ��]
      
      DO 1 I=SX,ENDX           !��ʼ�����֧�ߵ��������������Ϊ�����ͬ���������ֱ���������ϲ�ͬ�����֧��
	    DO 3 K=SZ,ENDZ
	       DO 4 T=1,5   
		        RVE(T,I,J,K)=0
   4       CONTINUE
   3    CONTINUE
   1  CONTINUE

      CALL CPU_TIME ( time_begin )
      
      OPEN(8133,FILE="sin.out",FORM='FORMATTED')

      DO 10 T=1,NT		!��ʼ����

       WRITE(NAME_COUNT,'(I5)') T      

	   OPEN(8134+T,FILE="GAUSS/GAUSS"//NAME_COUNT//".out",FORM='FORMATTED')
	   OPEN(8135+T,FILE="ONLYSIN/ONLYSIN"//NAME_COUNT//".out",FORM='FORMATTED')

         DO 6 III=1,5
             DO 992 PIND=1,PLEN
    !		      RVE(III,X,Y,Z)=1  !Ϊ���Դ����һ�����ȼ���
    !             RVE(III,X,Y,Z)=sin(2*PI*FRE*t)  !Ϊsin���� ��ɫ����ʽ
    !             ����t=(deltaL/C)*T   ,TΪNTѭ���ĵ�������,deltaL/C(һ�������/����=��λ�������õ�ʱ��),deltaLΪ��λ���룬CΪ���� 
	!             ���ԣ�RVE(III,X,Y,Z)=sin(2*PI*FRE*t)=sin(2*PI*FRE*(deltaL/C)*T)=sin(2*PI*NORMAL_FRE*T) ����FRE*(deltaL/C)=FRE*deltaL/CΪ��һ��Ƶ�� NORMAL_FRE

	!             ����FRE=10GHzʱ������TLMɫɢ���� deltaL/lamd<0.1  ,�˴�ȡdeltaL/lamd=(1/3)*0.1=3.33*E-2
	!             !RVE(III,X,Y,Z)=sin(6.283E+10*(0.33333E-11)*T)  !Ϊsin������ɫ����ʽ FRE=10GHz
    !             !����0.3333E-11=1/3E-11=dl/CC=deltaL/C(һ�������/����)  !dlȡ(3*E-2)*lamd=(3*E-2)*C/FRE ��ȡFRE=10GHz, CΪ����
                RVE(III,PULSE(PIND,1),PULSE(PIND,2),PULSE(PIND,3))=RVE(III,PULSE(PIND,1),PULSE(PIND,2),PULSE(PIND,3))+sin(2*PI*NORMAL_FRE*T)  !Ϊsin������ɫ����ʽ
	!            RVE(III,PULSE(PIND,1),PULSE(PIND,2),PULSE(PIND,3))=RVE(III,PULSE(PIND,1),PULSE(PIND,2),PULSE(PIND,3))+sin(PI*T/15)  !Ϊsin������ɫ����ʽ
 992         CONTINUE 
  6      CONTINUE

         DO 374 III=2,2
             DO 993 I=SX,ENDX
			   DO 994 K=SZ,PSZ
			   	  write(8134+T,*) K,I,GAUSS(I,Y,K)*RVE(III,I,Y,K)
				  write(8135+T,*) K,I,RVE(III,I,Y,K)
    !              RVE(III,I,J,K)=GAUSS(I,J,K)*RVE(III,I,J,K)    
 994           CONTINUE  
 993         CONTINUE
 374     CONTINUE


       CLOSE(8134+T)			 
	   CLOSE(8135+T)
       WRITE(8133,*) T,sin(PI*T/15)

       DO 20 I=SX,ENDX        
	       DO 40 K=SZ,ENDZ
		     IVE(1,I,J,K)=RVE(3,I-1,J,K)		!�����ʵʩ
		     IVE(2,I,J,K)=RVE(4,I,J,K-1)
		     IVE(3,I,J,K)=RVE(1,I+1,J,K)
		     IVE(4,I,J,K)=RVE(2,I,J,K+1)
		     IVE(5,I,J,K)=RVE(5,I,J,K)      !��糣��/������  ֧��
		  
		     IF (BL.EQ.1) THEN		!��ڱ߽������Ĵ���
		        IF (K.EQ.SZ) THEN
			       IVE(2,I,J,K)=-RVE(2,I,J,K)
		        END IF
		        IF (K.EQ.ENDZ) THEN    
			       IVE(4,I,J,K)=-RVE(4,I,J,K)
		        END IF
		        IF (I.EQ.SX) THEN
			       IVE(1,I,J,K)=-RVE(1,I,J,K)
		        END IF
		        IF (I.EQ.ENDX) THEN 
			       IVE(3,I,J,K)=-RVE(3,I,J,K)
		        END IF
		     END IF
	         IF (BL.EQ.0) THEN		!���ձ߽������Ĵ���
	            IF (K.EQ.SZ) THEN
		           IVE(2,I,J,K)=0
	            END IF
	            IF (K.EQ.ENDZ) THEN    
		           IVE(4,I,J,K)=0
	            END IF
	            IF (I.EQ.SX) THEN
		           IVE(1,I,J,K)=0
	            END IF
	            IF (I.EQ.ENDX) THEN 
		           IVE(3,I,J,K)=0
	            END IF
	         END IF
   40          CONTINUE   
   20    CONTINUE
       
         DO 120 I=SX,ENDX		!���ɢ���ʵʩ
	       DO 140 K=SZ,ENDZ
	       
             CALL SCL(SE,ZY(I,K))	!���ɲ������ɢ�����
      
		     DO 502 M=1,5
		        RVE(M,I,J,K)=0
		        DO 602 N=1,5 
			       RVE(M,I,J,K)=IVE(N,I,J,K)*SE(M,N)+RVE(M,I,J,K)
  602           CONTINUE
  502        CONTINUE
  140      CONTINUE   
  120    CONTINUE
              
      IF (SPLIT .EQ. 1) THEN
          FILE_UNIT=UNIT_BASE+T
          OPEN(FILE_UNIT,FILE='HY/HY'//NAME_COUNT//'.out',FORM='FORMATTED')
      ENDIF
         
      WRITE(FILE_UNIT,*) "#T=",T
      DO 101 II=SX,ENDX        
	       DO 102 KK=SZ,ENDZ
                CALL HJ(HY,IVE,ZY,II,J,KK,Z0,NX,NY,NZ)		!����ѹת��Ϊ�糡
                WRITE(FILE_UNIT,117) KK,II,HY
                
                DO 16384 LI=1,LN
                   IF(LN .EQ. 16384) THEN
                      LL=LI
                   ELSE
                      LL=L(LI)
                   ENDIF
                   CALL FT(REHY(LI,II,J,KK),IMHY(LI,II,J,KK),HY,LL,T) !���и���Ҷ�任
16384           CONTINUE
     
   102	  CONTINUE
   101 CONTINUE
       WRITE(FILE_UNIT,*)
       WRITE(FILE_UNIT,*)
       IF (SPLIT .EQ. 1) THEN
         CLOSE(FILE_UNIT)
       ENDIF

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
          OPEN(UNIT_BASE2+LL,FILE='FHY/FHY'//NAME_COUNT//'.out',FORM='FORMATTED')
16386 CONTINUE

      DO 1011 II=SX,ENDX        
         DO 1012 KK=SZ,ENDZ      
            DO 16385 LI=1,LN
               IF(LN .EQ. 16384) THEN
                  LL=LI
               ELSE
                  LL=L(LI)
               ENDIF
               FHY=SQRT(REHY(LI,II,J,KK)**2 + IMHY(LI,II,J,KK)**2)
               WRITE(UNIT_BASE2+LL,117),KK,II,FHY
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
!      WRITE(13,117) HY(T)		!����ģ����
  222 CONTINUE

  117 format(I, I, 7f12.7)
  118 format(7f12.7)
  119 format(I)


      IF (SPLIT .EQ. 0) THEN
         CLOSE(FILE_UNIT)
      ENDIF
	  CLOSE(15)
	  
	  CLOSE(8133)
	  CALL CPU_TIME ( time_end )
	  
	  PRINT *, 'Time of operation was ',time_end - time_begin, ' seconds'
	  STOP
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
      DO 13 J=1,5
	    S(3,J)=S(2,J)
	    S(4,J)=S(1,J)
   13 CONTINUE
      DO 11 I=1,5
	  DO 12 J=1,5
	    S(I,J)=S(I,J)*2/(4+Z)
	    IF (J.EQ.I) S(I,J)=S(I,J)+1
   12    CONTINUE
   11 CONTINUE
      RETURN
      END

      SUBROUTINE HJ(H,IV,Z,I,J,K,Z0,NX,NY,NZ)
      REAL IV(5,NX,NY,NZ)
      H=0.0
      H=IV(1,I,J,K)-IV(2,I,J,K)-IV(3,I,J,K)+IV(4,I,J,K)+IV(5,I,J,K)
      H=H*2/(Z0*(4+Z))
      RETURN
      END
      
      SUBROUTINE FT(FREAL,FIM,E,LL,T)
      INTEGER LL,T
	  REAL TLBB,FREAL,FIM
 !    ���и���Ҷ�任������LL��T�ֱ�Ϊ��������L��T����ʾƵ��ĵڼ������ʱ��ĵڼ�����
      TLBB=(LL*(0.1-0)/16384.0+0)
      FREAL=FREAL+E*COS(2*3.14159*T*TLBB/2.0)
      FIM=FIM+E*SIN(2*3.14159*T*TLBB/2.0)
      END

      SUBROUTINE COLOR(GTYPE,SIDE1,SIDE2,I,K,ZY,ZYY,NX,NZ,C,CC)
      INTEGER GTYPE,SIDE1,SIDE2,I,K,NX,NZ
      CHARACTER(LEN=12) CC
      REAL ZY(NX,NZ)
      CHARACTER(LEN=12) ZYY(NX,NZ)
      REAL C
      REAL CROW,CCOL,A,B
      A=SIDE1
      B=SIDE2
      CROW=A/2
      CCOL=B/2
      DO 3333 III=0,SIDE1-1
         DO 3334 KKK=0,SIDE2-1
             IF(GTYPE .EQ. 0) THEN     !����
                 ZY(I+III,K+KKK)=C
                 ZYY(I+III,K+KKK)=CC
             ENDIF
             IF(GTYPE .EQ. 1) THEN     !��Բ
                 IF((III-CROW)*(III-CROW)/(CROW*CROW)+(KKK-CCOL)*(KKK-CCOL)/(CCOL*CCOL) .LE. 1) THEN
                     ZY(I+III,K+KKK)=C
                     ZYY(I+III,K+KKK)=CC
                 ENDIF
             ENDIF
             IF(GTYPE .EQ. 2) THEN     !������
                 IF(2*A*(B/2-KKK)/B .LE. III .AND. 2*A*(KKK-B/2)/B .LE. III) THEN
                     ZY(I+III,K+KKK)=C
                     ZYY(I+III,K+KKK)=CC
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
      REAL PP !����ֱ�߼������
      REAL PANGLE,ANGLE
      CHARACTER(LEN=12) ZYY(NX,NZ)
      CHARACTER(LEN=12) P_COLOR
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

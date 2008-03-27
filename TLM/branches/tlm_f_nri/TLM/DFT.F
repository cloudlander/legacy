    


	dimension reey(16384),timey(16384),ey(60000)
	dimension rehx(16384),timhx(16384)
	dimension eyy(16384),hxx(16384),zor(16384)
	dimension eyyc(16384),hxxc(16384)
	INTEGER   N1
        open(15,file='fin.dat',form='formatted')
        open(17,file='fabs.dat',form='formatted')
        open(18,file='fang.dat',form='formatted')
        open(19,file='fime.dat',form='formatted')
        open(20,file='frea.dat',form='formatted')
	write(*,301)
        read(*,302) nss
301	format(1x,' read nss') 
302	format(1i5)

	
	tta=0.0
	ttb=0.1
	
	do 10 i=1,nss
	read(15,100) ey(i)
10	continue
	
	do 50 j=1,16384
	tlbb=(j*(ttb-tta)/16384.0+tta)
	do 30 i=1,nss
        reey(j)=reey(j)+ey(i)*cos(2*3.14159*i*tlbb/2.0)
        timey(j)=timey(j)+ey(i)*sin(2*3.14159*i*tlbb/2.0)
30	continue
	eyy(j)=sqrt(reey(j)**2+timey(j)**2)
	eyyc(j)=atan(timey(j)/reey(j))

      N1=MOD(j,500)
      IF (N1.EQ.0) THEN
	 WRITE(*,*)j
      END IF

50	continue

	write(17,200) eyy
	write(18,200) eyyc
	write(19,200) timey
	write(20,200) reey
100	format(e15.8)
200	format(e12.5)
	stop
	end

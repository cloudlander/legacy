#!/usr/bin/python
import sys,os,re,signal
import subprocess

X=0
Z=0

while X<=0 or Z<=0:
    try:
        print "\rInput X:",
        X=int(sys.stdin.readline())
        print "\rInput Z:",
        Z=int(sys.stdin.readline())
    except KeyboardInterrupt:
        sys.exit(0)
    except:
        print "Please give reasonable input!"

def DFT(NT):
    if sys.platform.find("win")==0:
        dft=subprocess.Popen("DFT.exe",stdin=subprocess.PIPE)
    else:
        dft=subprocess.Popen("./dft",stdin=subprocess.PIPE)
    dft.stdin.write(str(NT)+"\n")
    dft.wait()

def bi_search(lines,X,Z):
    i=0
    j=len(lines)-1
    while(i<=j):
        m=(i+j)/2
        line=lines[m]
        a=line.split()
        try:
            x=int(a[1])
            z=int(a[0])
        except:
            return None
        if x==X:
            if z==Z:
                print line
                return (m,a[2])
            elif z>Z:
                for iter in range(m-1,i-1,-1):
                    line=lines[iter]
                    a=line.split()
                    if Z==int(a[0]):
                        print lines[iter]
                        return (iter,a[2])
            else:
                for iter in range(m+1,j+1):
                    line=lines[iter]
                    a=line.split()
                    if Z==int(a[0]):
                        print lines[iter]
                        return (iter,a[2])
            print "Fatal error!"
        elif x<X:
            i=m+1
        else:
            j=m-1
    print "Fatal error!"

for dir in ("EX","EY","EZ","HX","HY","HZ"):
    input_files=[]
    for root, dirs, files in os.walk(dir, topdown=False):
        for name in files:
            if name.find(dir)==0:
                input_files.append(root+"/"+name)
    input_files.sort()
    NT=len(input_files)
    if NT > 0:
        fin=open("fin.dat","w")
        found=False
        index=0
        pat=re.compile(" +%d +%d +(.+)"%(Z,X))
        for file in input_files:
            print "collecting data from %s ...\r"%(file),
            input=open(file,"r")
            lines=input.readlines()
            if 0==index: # count the index of pattern
                ret=bi_search(lines,X,Z)
                if ret!=None:
                    (index,data)=ret
                    found=True
                    print index
                    print >>fin,data
            else:
                mat=pat.match(lines[index])
                print >>fin,mat.group(1)
            input.close()
            if not found:
                print "%s output data error, or input (%d,%d) not correct!"%(dir,X,Z)
                fin.close()
                os.unlink("fin.dat")
                sys.exit(1)
        fin.close()
        print
        DFT(NT)
        try:
            os.unlink(dir+"_%d_%d_fin.dat"%(X,Z))
            os.unlink(dir+"_%d_%d_fabs.dat"%(X,Z))
            os.unlink(dir+"_%d_%d_fang.dat"%(X,Z))
            os.unlink(dir+"_%d_%d_fime.dat"%(X,Z))
            os.unlink(dir+"_%d_%d_frea.dat"%(X,Z))
        except:
            pass
        os.rename("fin.dat",dir+"_%d_%d_fin.dat"%(X,Z))
        os.rename("fabs.dat",dir+"_%d_%d_fabs.dat"%(X,Z))
        os.rename("fang.dat",dir+"_%d_%d_fang.dat"%(X,Z))
        os.rename("fime.dat",dir+"_%d_%d_fime.dat"%(X,Z))
        os.rename("frea.dat",dir+"_%d_%d_frea.dat"%(X,Z))

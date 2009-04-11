#!/usr/bin/python
import time,subprocess,threading,Queue,os,sys,signal,copy
import StringIO
import re
import zipfile

class Config:
    def __init__(self):
        self._config={}
    def parseConfig(self):
        self._config['threads']=4
        self._config['PNG_SIZE']='1024,768'
        self._config['+RANGE']=0.0001
        self._config['-RANGE']=-0.0001
        self._config['+G_RANGE']=1.0
        self._config['-G_RANGE']=-1.0
        self._config['+H_RANGE']=3.0
        self._config['-H_RANGE']="0"
#        self._config['PALETTE']="set palette defined (-1.0 \"black\", -0.5 \"dark-blue\", -0.2 \"medium-blue\", 0 \"#CCE8CF\", 0.01 \"light-yellow\", 0.2 \"yellow\", 0.5 \"red\", 1.0 \"dark-red\")"
        self._config['PALETTE']="set palette defined (-0.0001 \"black\", -0.00005 \"dark-blue\", -0.00002 \"medium-blue\", 0 \"#CCE8CF\", 0.00002 \"yellow\", 0.00005 \"red\", 0.0001 \"dark-red\")"
        self._config['G_PALETTE']="set palette defined (-1.0 \"black\", -0.5 \"dark-blue\", -0.2 \"medium-blue\", 0 \"#CCE8CF\", 0.01 \"light-yellow\", 0.2 \"yellow\", 0.5 \"red\", 1.0 \"dark-red\")"
        self._config['H_PALETTE']="set palette defined (0 \"#CCE8CF\", 0.01 \"light-yellow\", 0.1 \"black\", 0.5 \"dark-blue\", 1 \"medium-blue\", 1.5 \"blue\", 2 \"yellow\", 2.5 \"red\", 3.0 \"dark-red\")"
        self._config['TLM']=True
        self._config['MAP']=True
        self._config['SURFACE']=True
        self._config['GAUSS']=False
        self._config['ONLY_SIN']=False
        self._config['HYBRID']=False
        self._config['HYBRID_EXPR']="sqrt(x*x+y*y)"
        self._config['HYBRID_LIST']=["EX,EZ","HY,HZ"]
        self._config['SHOW_MEDIUM']=True
        self._config['ANI']=True
        self._config['ZIP']=True
        self._config['ZIPNAME']="data_%s.zip"%(time.strftime("%y-%m-%d-%H-%M", time.localtime()))
        self._config['DO_EX']=True
        self._config['DO_EY']=True
        self._config['DO_EZ']=True
        self._config['DO_HX']=True
        self._config['DO_HY']=True
        self._config['DO_HZ']=True
        if sys.platform.find("win")==0:
            self._config['GEN_ANI']="gen_ani.bat"
            self._config['TLM_EXE']="Release/TLM.exe"
            self._config['GNUPLOT']="wgnuplot.exe"
            self._config['PASTE']="paste.exe"
        else:
            self._config['GEN_ANI']="./gen_ani.sh"
            self._config['TLM_EXE']="./tlm"
            self._config['GNUPLOT']="gnuplot"
            self._config['PASTE']="paste"
    def resetConfig(self):
        return True
    def getConfig(self):
        return self._config

class FileConfig(Config):
    def __init__(self,f):
        Config.__init__(self)
        self._input=f
        self.parseConfig()
    def parseConfig(self):
        Config.parseConfig(self)
        input=open(self._input,"rb")
        data_line=input.readline()
        data=re.split("[,\s]+",data_line)
        while True:
            line=input.readline()
            if line:
                try:
                    comment=re.split("[,\s]+",line)
                    for (i,v) in enumerate(comment):
                        self._config[v]=data[i]
                except:
                    pass
            else:
                break
        input.close()
        for task in ('EX','EY','EZ','HX','HY','HZ'):
            if self._config['GEN_'+task]=='1' and self._config['DO_'+task]:
                self._config['DO_'+task]='1'
            else:
                self._config['DO_'+task]='0'
        #verify HYBRID_LIST:
        hybrid_list=copy.copy(self._config['HYBRID_LIST'])
        self._config['HYBRID_TASK_MAP']={}
        for hybrid in self._config['HYBRID_LIST']:
            list=hybrid.split(",")
            valid=True
            for task in list:
                if self._config['DO_'+task]=='0':
                    print task+" not generated for hybrid: "+hybrid+"("+self._config['HYBRID_EXPR']+") REMOVED!"
                    valid=False
                    break
            if not valid:
                hybrid_list.remove(hybrid)
            else:
                self._config['HYBRID_TASK_MAP'][hybrid]=None
        if self._config.has_key('HYBRID_LIST'):
            self._config['HYBRID_NUM']=len(self._config['HYBRID_LIST'][0].split(","))
        if len(hybrid_list)==0:
            self._config['HYBRID']=False

class ILineAware:
    def encounterLine(self,line):
        pass

class Visualizer(ILineAware):
    class RefCount:
        def __init__(self):
            self._refcount=0
        def addRef(self,val=1):
            self._refcount+=val
        def release(self,val=1):
            self._refcount-=val
        def value(self):
            return self._refcount
    class Worker(threading.Thread):
        def __init__(self,producer,request,config):
            threading.Thread.__init__(self,None,self._worker)
            self._request=request
            self._producer=producer
            self._config=config
            self._enabled=True
            self._event=threading.Event()
            self._collector=None
        def __del__(self):
            pass
        def enable(self,status):
            self._enabled=status
        def clearEvent(self):
#            print self.getName()+":clearEvent"
            self._event.clear()
        def setEvent(self):
#            print self.getName()+":setEvent"
            self._event.set()
        def waitEvent(self):
#            print self.getName()+":waitEvent"
            self._event.wait()
        def setCollector(self,collector):
            self._collector=collector
        def prepareWorker(self):
            pass
        def getPlotOption(self):
            return ""
        def removefile(self,filename):
            while True:
                  try:
                        os.unlink(filename)
                        return
                  except:
                        time.sleep(1)
        def _worker(self):
            s=self.prepareWorker().readlines()
            while not self._producer.killed :
                try:
                    trunk=self._request()
                    if trunk != None:
                        if self._enabled:
                            filename=self.getName()+"_"+str(trunk['start'])+".gnu"
                            commands=open(filename,"wb")
                            commands.writelines(s)
                            for i in range(trunk['start'],trunk['end']+1):
                                print >>commands,"set output \"%s_%s/IMG%5d.png\""%(trunk['prefix'],trunk['dir'],i)
                                if self._config['SPLIT']=='0':
                                    print >>commands,"splot \"%s.out\" index %d %s"%(trunk['prefix'],i-1,self.getPlotOption())
                                else:
                                    print >>commands,"splot \"%s/%s%5d.out\" %s"%(trunk['prefix'],trunk['prefix'],i,self.getPlotOption())
                            commands.write("exit\n")
                            commands.close()
                            plot=subprocess.Popen([self._config['GNUPLOT'],filename],universal_newlines=True)
                            plot.wait()
                            self.removefile(filename)
                        trunk['ref'].release()
                    elif self._producer.joining:
                        break
                except:
                    print "Exceptions in %s!"%(self.getName())

    class Collector(Worker):
        def __init__(self,producer,request,config,threads):
            threading.Thread.__init__(self,None,self._worker)
            self._request=request
            self._producer=producer
            self._config=config
            self._enabled=True
            self._event=threading.Event()
            self._threads=threads
            for thread in self._threads:
                thread.setCollector(self)
        def doCollection(self,trunk):
            pass
        def _worker(self):
            while not self._producer.killed :
               try:
                    trunk=self._request()
                    if trunk != None:
                        if self._enabled:
                            while True:
                                if trunk['ref'].value() == 0:
                                    break
                                else:
                                    time.sleep(1)
                            self.doCollection(trunk)
                    elif self._producer.joining:
                        break
               except:
                    print "Exceptions in %s!"%(self.getName())

    class ZipArchiver(Collector):
        def __init__(self,producer,request,config):
            Visualizer.Worker.__init__(self,producer,request,config)
            self._z=None
        def __del__(self):
            if self._z != None:
                self._z.close()
        def _zip_and_remove(self,trunk):
            for i in range(trunk['start'],trunk['end']+1):
                filename="%s/%s%5d.out"%(trunk['prefix'],trunk['prefix'],i)
                if os.path.isfile(filename):
                    self._z.write(filename)
                    self.removefile(filename)
        def doCollection(self,trunk):
            if None==self._z:
                filename=self.getName()+self._config['ZIPNAME']
                self._z=zipfile.ZipFile(filename,mode='w',compression=zipfile.ZIP_DEFLATED,allowZip64=True)
            self._zip_and_remove(trunk)


    class MapWorker(Worker):
        def getPlotOption(self):
            if self._config['SHOW_MEDIUM']:
                return "using 1:2:4"
            else:
                return "using 1:2:3"
        def prepareWorker(self):
            ratio=float(int(self._config['ENDX'])-int(self._config['SX']))/(int(self._config['ENDZ'])-int(self._config['SZ']))
            env_set=StringIO.StringIO()
            env_set.writelines(
                    [#"set samples 100, 100","\n",
                     #"set isosamples 10, 10","\n"
                     #"set palette model HSV functions gray, 1, 1","\n"
                     #"set autoscale","\n",
                     #"set size square","\n",
                     "set size ratio ",str(ratio)," \n",
                     self._config['PALETTE'],"\n",
                     "set surface","\n",
                     'set title "TLM 3D"',"\n",
                     'set xlabel "Z"',"\n",
                     'set ylabel "X"',"\n",
                     'set xrange [',self._config['SZ'],":",self._config['ENDZ'],'] noreverse nowriteback',"\n",
                     'set yrange [',self._config['SX'],":",self._config['ENDX'],'] noreverse nowriteback',"\n",
                     'set zrange [',self._config['-RANGE'],":",self._config['+RANGE'],'] noreverse nowriteback',"\n",
                     'set cbrange [',self._config['-RANGE'],":",self._config['+RANGE'],'] noreverse nowriteback',"\n",
                     'set zero 1e-0020',"\n",
                     'set pm3d map',"\n",
                     #'set dgrid3d ',str(int(self._config['ENDX'])-int(self._config['SX'])+1),",",str(int(self._config['ENDZ'])-int(self._config['SZ'])+1),"\n",
                     'set terminal png size ',self._config['PNG_SIZE'],"\n",
                    ])
            env_set.seek(0)
            return env_set


    class SurfaceWorker(Worker):
        def getPlotOption(self):
            if self._config['SHOW_MEDIUM']:
                return "using 1:2:4"
            else:
                return "using 1:2:3"
        def prepareWorker(self):
            ratio=float(int(self._config['ENDX'])-int(self._config['SX']))/(int(self._config['ENDZ'])-int(self._config['SZ']))
            env_set=StringIO.StringIO()
            env_set.writelines(
                    [#"set samples 100, 100","\n",
                     #"set isosamples 10, 10","\n"
                     #"set palette model HSV functions gray, 1, 1","\n"
                     #"set autoscale","\n",
                     #"set size square","\n",
                     "set size ratio ",str(ratio)," \n",
                     self._config['PALETTE'],"\n",
                     "set surface","\n",
                     'set title "TLM 3D"',"\n",
                     'set xlabel "Z"',"\n",
                     'set ylabel "X"',"\n",
                     'set xrange [',self._config['SZ'],":",self._config['ENDZ'],'] noreverse nowriteback',"\n",
                     'set yrange [',self._config['SX'],":",self._config['ENDX'],'] noreverse nowriteback',"\n",
                     'set zrange [',self._config['-RANGE'],":",self._config['+RANGE'],'] noreverse nowriteback',"\n",
                     'set cbrange [',self._config['-RANGE'],":",self._config['+RANGE'],'] noreverse nowriteback',"\n",
                     'set zero 1e-0020',"\n",
                     'set pm3d at s',"\n",
                     #'set dgrid3d ',str(int(self._config['ENDX'])-int(self._config['SX'])+1),",",str(int(self._config['ENDZ'])-int(self._config['SZ'])+1),"\n",
                     'set terminal png size ',self._config['PNG_SIZE'],"\n",
                     'set style data pm3d',"\n",
                    ])
            env_set.seek(0)
            return env_set

    class GaussWorker(Worker):
        def prepareWorker(self):
            ratio=float(int(self._config['ENDX'])-int(self._config['SX']))/(int(self._config['ENDZ'])-int(self._config['SZ']))
            env_set=StringIO.StringIO()
            env_set.writelines(
                    [#"set samples 100, 100","\n",
                     #"set isosamples 10, 10","\n"
                     #"set palette model HSV functions gray, 1, 1","\n"
                     #"set autoscale","\n",
                     #"set size square","\n",
                     "set size ratio ",str(ratio)," \n",
                     self._config['G_PALETTE'],"\n",
                     "set surface","\n",
                     'set title "TLM 2D Gauss Distribution"',"\n",
                     'set xlabel "Z"',"\n",
                     'set ylabel "X"',"\n",
                     'set xrange [',self._config['SZ'],":",self._config['ENDZ'],'] noreverse nowriteback',"\n",
                     'set yrange [',self._config['SX'],":",self._config['ENDX'],'] noreverse nowriteback',"\n",
                     'set zrange [',self._config['-G_RANGE'],":",self._config['+G_RANGE'],'] noreverse nowriteback',"\n",
                     'set cbrange [',self._config['-G_RANGE'],":",self._config['+G_RANGE'],'] noreverse nowriteback',"\n",
                     'set zero 1e-0020',"\n",
                     'set pm3d map',"\n",
                     #'set dgrid3d ',str(int(self._config['ENDX'])-int(self._config['SX'])+1),",",str(int(self._config['ENDZ'])-int(self._config['SZ'])+1),"\n",
                     'set terminal png size ',self._config['PNG_SIZE'],"\n",
                     #'set style data dots',"\n",
                     #'set style data pm3d',"\n",
                     #'set contour',"\n",
                     #'set cntrparam levels incremental -0.2,0.01,0.2',"\n"
                     #'unset surface',"\n",
                     #'set view 0,0',"\n"
                    ])
            env_set.seek(0)
            return env_set

    class OnlySinWorker(Worker):
        def prepareWorker(self):
            ratio=float(int(self._config['ENDX'])-int(self._config['SX']))/(int(self._config['ENDZ'])-int(self._config['SZ']))
            env_set=StringIO.StringIO()
            env_set.writelines(
                    [#"set samples 100, 100","\n",
                     #"set isosamples 10, 10","\n"
                     #"set palette model HSV functions gray, 1, 1","\n"
                     #"set autoscale","\n",
                     #"set size square","\n",
                     "set size ratio ",str(ratio)," \n",
                     self._config['G_PALETTE'],"\n",
                     "set surface","\n",
                     'set title "TLM 2D Only SIN Distribution"',"\n",
                     'set xlabel "Z"',"\n",
                     'set ylabel "X"',"\n",
                     'set xrange [',self._config['SZ'],":",self._config['ENDZ'],'] noreverse nowriteback',"\n",
                     'set yrange [',self._config['SX'],":",self._config['ENDX'],'] noreverse nowriteback',"\n",
                     'set zrange [',self._config['-G_RANGE'],":",self._config['+G_RANGE'],'] noreverse nowriteback',"\n",
                     'set cbrange [',self._config['-G_RANGE'],":",self._config['+G_RANGE'],'] noreverse nowriteback',"\n",
                     'set zero 1e-0020',"\n",
                     'set pm3d map',"\n",
                     #'set dgrid3d ',str(int(self._config['ENDX'])-int(self._config['SX'])+1),",",str(int(self._config['ENDZ'])-int(self._config['SZ'])+1),"\n",
                     'set terminal png size ',self._config['PNG_SIZE'],"\n",
                     #'set style data dots',"\n",
                     #'set style data pm3d',"\n",
                     #'set contour',"\n",
                     #'set cntrparam levels incremental -0.2,0.01,0.2',"\n"
                     #'unset surface',"\n",
                     #'set view 0,0',"\n"
                    ])
            env_set.seek(0)
            return env_set

    class HybridWorker(Worker):
        def getPlotOption(self):
            if self._config['SHOW_MEDIUM']:
                if self._config['HYBRID_NUM']==2:
                    return "using 1:2:(hybrid($4,$8))"
                else:
                    return "using 1:2:(hybrid($4,$8,$12))"
            else:
                if self._config['HYBRID_NUM']==2:
                    return "using 1:2:(hybrid($3,$7))"
                else:
                    return "using 1:2:(hybrid($3,$7,$11))"
        def prepareWorker(self):
            if self._config.has_key('H_PALETTE'):
                palette=self._config['H_PALETTE']
            else:
                palette=""
            if self._config['HYBRID_NUM']==2:
                expr_head="hybrid(x,y)="
            else:
                expr_head="hybrid(x,y,z)="
            ratio=float(int(self._config['ENDX'])-int(self._config['SX']))/(int(self._config['ENDZ'])-int(self._config['SZ']))
            env_set=StringIO.StringIO()
            env_set.writelines(
                    [#"set samples 100, 100","\n",
                     #"set isosamples 10, 10","\n"
                     #"set palette model HSV functions gray, 1, 1","\n"
                     #"set autoscale","\n",
                     #"set size square","\n",
                     "set size ratio ",str(ratio)," \n",
                     palette,"\n",
                     "set surface","\n",
                     'set title "TLM 3D -- Modulized: ',self._config['HYBRID_EXPR'],'"',"\n",
                     'set xlabel "Z"',"\n",
                     'set ylabel "X"',"\n",
                     'set xrange [',self._config['SZ'],":",self._config['ENDZ'],'] noreverse nowriteback',"\n",
                     'set yrange [',self._config['SX'],":",self._config['ENDX'],'] noreverse nowriteback',"\n",
                     'set zrange [',self._config['-H_RANGE'],":",self._config['+H_RANGE'],'] noreverse nowriteback',"\n",
                     'set cbrange [',self._config['-H_RANGE'],":",self._config['+H_RANGE'],'] noreverse nowriteback',"\n",
                     'set zero 1e-0020',"\n",
                     'set pm3d map',"\n",
                     #'set dgrid3d ',str(int(self._config['ENDX'])-int(self._config['SX'])+1),",",str(int(self._config['ENDZ'])-int(self._config['SZ'])+1),"\n",
                     'set terminal png size ',self._config['PNG_SIZE'],"\n",
                     expr_head,self._config['HYBRID_EXPR'],"\n"
                    ])
            env_set.seek(0)
            return env_set
        def _worker(self):
            s=self.prepareWorker().readlines()
            while not self._producer.killed :
                try:
                    trunk=self._request()
                    if trunk != None:
                        if self._enabled:
                            filename=self.getName()+"_"+str(trunk['start'])+"_"+trunk['prefix']+".gnu"
                            commands=open(filename,"wb")
                            commands.writelines(s)
                            for i in range(trunk['start'],trunk['end']+1):
                                print >>commands,"set output \"%s/%s_IMG%5d.png\""%(trunk['dir'],trunk['prefix'].replace(',','_'),i)
                                if self._config['SPLIT']=='0':
                                    print >>commands,"splot \"-\" index %d %s"%(i-1,self.getPlotOption())
                                else:
                                    print >>commands,"splot \"-\" %s"%(self.getPlotOption())
                                files=trunk['prefix'].split(",")
                                params=[self._config['PASTE']]
                                for file in files:
                                    params.append("%s/%s%5d.out"%(file,file,i))
                                paste=subprocess.Popen(params,stdout=subprocess.PIPE,universal_newlines=True)
                                line=paste.stdout.readline()
                                while line:
                                    print >>commands,line,
                                    line=paste.stdout.readline()
                                paste.wait()
                                print >>commands,"e"
                            commands.write("exit\n")
                            commands.close()
                            plot=subprocess.Popen([self._config['GNUPLOT'],filename],universal_newlines=True)
                            plot.wait()
                            self.removefile(filename)
                        for task in trunk['refs'].values():
                            task.release()
                    elif self._producer.joining:
                        break
                except:
                    print "Exceptions in %s!"%(self.getName())

    def __init__(self,config=Config()):
        self._config=config.getConfig()
        #self._num_threads=self._config['threads']*2
        self._num_threads=self._config['threads']*5
        self._threads=range(self._num_threads + 3)
        self._map_queue=Queue.Queue()
        self._surface_queue=Queue.Queue()
        self._gauss_queue=Queue.Queue()
        self._only_sin_queue=Queue.Queue()
        self._hybrid_queue=Queue.Queue()
        self._archiver1_queue=Queue.Queue()
        self._archiver2_queue=Queue.Queue()
        self._archiver3_queue=Queue.Queue()
        self._trunk_start=1
        self.joining=False
        self.killed=False

    def __del__(self):
        for thread in self._threads:
            thread.__del__()

    def getTotal(self):
        nt=float(self._config['NT'])
        ntask=0
        for task in ('DO_EX','DO_EY','DO_EZ','DO_HX','DO_HY','DO_HZ'):
            ntask+=int(self._config[task])
        return nt*ntask

    def getTLMStatus(self):
        total=float(self._config['NT'])
        if self._trunk_start > total:
            return 100
        else:
            return (self._trunk_start-1)/total*100

    def getMapPlotStatus(self):
        total=float(self.getTotal())
        if self._trunk_start > int(self._config['NT']):
            return (total-self._map_queue.qsize()*10)/total*100
        else:
            return ((self._trunk_start-1)/float(self._config['NT'])-self._map_queue.qsize()*10/total)*100

    def getSurfacePlotStatus(self):
        total=float(self.getTotal())
        if self._trunk_start > int(self._config['NT']):
            return (total-self._surface_queue.qsize()*10)/total*100
        else:
            return ((self._trunk_start-1)/float(self._config['NT'])-self._surface_queue.qsize()*10/total)*100

    def getGaussPlotStatus(self):
        total=float(self._config['NT'])
        if self._trunk_start > int(self._config['NT']):
            return 100
        else:
            return ((self._trunk_start-1)/float(self._config['NT'])-self._gauss_queue.qsize()*10/total)*100

    def getOnlySinPlotStatus(self):
        total=float(self._config['NT'])
        if self._trunk_start > int(self._config['NT']):
            return 100
        else:
            return ((self._trunk_start-1)/float(self._config['NT'])-self._only_sin_queue.qsize()*10/total)*100

    def getHybridPlotStatus(self):
        total=float(self._config['NT']) * len(self._config['HYBRID_LIST'])
        if self._trunk_start > int(self._config['NT']):
            return (total-self._hybrid_queue.qsize()*10)/total*100
        else:
            return ((self._trunk_start-1)/float(self._config['NT'])-self._hybrid_queue.qsize()*10/total)*100

    def printStatus(self):
        print "TLM %.0f%% calculated,surface %.0f%% plotted,map %.0f%% plotted,Gauss %.0f%% plotted,OnlySin %.0f%% plotted,Hybrid %.0f%% plotted!\r"%(self.getTLMStatus(),self.getSurfacePlotStatus(),self.getMapPlotStatus(),self.getGaussPlotStatus(),self.getOnlySinPlotStatus(),self.getHybridPlotStatus()),

    def encounterLine(self,line):
        try:
            end=int(line)
            if end % 10 == 0:
                for hybrid in self._config['HYBRID_LIST']:
                    refcounts={}
                    for task in hybrid.split(','):
                        if not refcounts.has_key(task):
                            refcounts[task]=1
                        else:
                            refcounts[task]+=1
                    self._config['HYBRID_TASK_MAP'][hybrid]=refcounts
                for task in ('EX','EY','EZ','HX','HY','HZ'):
                    if self._config["DO_"+task] == '1':
                        refcount=Visualizer.RefCount()
                        refcount.addRef(2)
                        self.map_deposit({'start':self._trunk_start,'end':end,'prefix':task,'dir':'MAP','ref':refcount})
                        self.surface_deposit({'start':self._trunk_start,'end':end,'prefix':task,'dir':'3D','ref':refcount})
                        for hybrid in self._config['HYBRID_LIST']:
                            if self._config['HYBRID_TASK_MAP'].has_key(hybrid):
                                refcounts=self._config['HYBRID_TASK_MAP'][hybrid]
                                if refcounts.has_key(task):
                                    refcount.addRef(refcounts[task])
                                    refcounts[task]=refcount
                        self.archiver1_deposit({'start':self._trunk_start,'end':end,'prefix':task,'dir':'3D','ref':refcount})
                    else:
                        for hybrid in self._config['HYBRID_LIST']:
                            if self._config['HYBRID_TASK_MAP'].has_key(hybrid):
                                refcounts=self._config['HYBRID_TASK_MAP'][hybrid]
                                if refcounts.has_key(task):
                                    del refcounts[task]
                for hybrid in self._config['HYBRID_LIST']:
                    self.hybrid_deposit({'start':self._trunk_start,'end':end,'prefix':hybrid,'dir':'HYBRID','refs':self._config['HYBRID_TASK_MAP'][hybrid]})
                refcount=Visualizer.RefCount()
                refcount.addRef()
                self.gauss_deposit({'start':self._trunk_start,'end':end,'prefix':"GAUSS",'dir':'SIN','ref':refcount})
                self.archiver2_deposit({'start':self._trunk_start,'end':end,'prefix':"GAUSS",'dir':'SIN','ref':refcount})
                refcount=Visualizer.RefCount()
                refcount.addRef()
                self.only_sin_deposit({'start':self._trunk_start,'end':end,'prefix':"ONLYSIN",'dir':'3D','ref':refcount})
                self.archiver3_deposit({'start':self._trunk_start,'end':end,'prefix':"ONLYSIN",'dir':'3D','ref':refcount})
                self._trunk_start=end+1
        except:
            #print "Exceptions in encounterLine: %s"%(line)
            pass

    def map_request(self):
        trunk=None
        try:
            trunk=self._map_queue.get(True,1)
        finally:
            return trunk

    def map_deposit(self,trunk):
        self._map_queue.put(trunk,True)

    def surface_request(self):
        trunk=None
        try:
            trunk=self._surface_queue.get(True,1)
        finally:
            return trunk

    def surface_deposit(self,trunk):
        self._surface_queue.put(trunk,True)

    def gauss_request(self):
        trunk=None
        try:
            trunk=self._gauss_queue.get(True,1)
        finally:
            return trunk

    def gauss_deposit(self,trunk):
        self._gauss_queue.put(trunk,True)


    def only_sin_request(self):
        trunk=None
        try:
            trunk=self._only_sin_queue.get(True,1)
        finally:
            return trunk

    def only_sin_deposit(self,trunk):
        self._only_sin_queue.put(trunk,True)

    def hybrid_request(self):
        trunk=None
        try:
            trunk=self._hybrid_queue.get(True,1)
        finally:
            return trunk

    def hybrid_deposit(self,trunk):
        self._hybrid_queue.put(trunk,True)

    def archiver1_request(self):
        trunk=None
        try:
            trunk=self._archiver1_queue.get(True,1)
        finally:
            return trunk

    def archiver1_deposit(self,trunk):
        self._archiver1_queue.put(trunk,True)

    def archiver2_request(self):
        trunk=None
        try:
            trunk=self._archiver2_queue.get(True,1)
        finally:
            return trunk

    def archiver2_deposit(self,trunk):
        self._archiver2_queue.put(trunk,True)

    def archiver3_request(self):
        trunk=None
        try:
            trunk=self._archiver3_queue.get(True,1)
        finally:
            return trunk

    def archiver3_deposit(self,trunk):
        self._archiver3_queue.put(trunk,True)

    def start(self):
        try:
            for i in range(self._num_threads/5):
                self._threads[i]=self.MapWorker(self,self.map_request,self._config)
                self._threads[i].setName("MapWorker"+str(i))
                self._threads[i].start()
                self._threads[i].enable(self._config['MAP'])
            for i in range(self._num_threads/5,self._num_threads*2/5):
                self._threads[i]=self.SurfaceWorker(self,self.surface_request,self._config)
                self._threads[i].setName("SurfaceWorker"+str(i))
                self._threads[i].start()
                self._threads[i].enable(self._config['SURFACE'])
            for i in range(self._num_threads*2/5,self._num_threads*3/5):
                self._threads[i]=self.HybridWorker(self,self.hybrid_request,self._config)
                self._threads[i].setName("HybridWorker"+str(i))
                self._threads[i].start()
                self._threads[i].enable(self._config['HYBRID'])
            for i in range(self._num_threads*3/5,self._num_threads*4/5):
                self._threads[i]=self.GaussWorker(self,self.gauss_request,self._config)
                self._threads[i].setName("GaussWorker"+str(i))
                self._threads[i].start()
                self._threads[i].enable(self._config['GAUSS'])
            for i in range(self._num_threads*4/5,self._num_threads):
                self._threads[i]=self.OnlySinWorker(self,self.only_sin_request,self._config)
                self._threads[i].setName("OnlySinWorker"+str(i))
                self._threads[i].start()
                self._threads[i].enable(self._config['ONLY_SIN'])
            self._threads[self._num_threads]=self.ZipArchiver(self,self.archiver1_request,self._config)
            self._threads[self._num_threads].setName("EH_")
            self._threads[self._num_threads].start()
            self._threads[self._num_threads].enable(self._config['ZIP'])
            self._num_threads+=1
            self._threads[self._num_threads]=self.ZipArchiver(self,self.archiver2_request,self._config)
            self._threads[self._num_threads].setName("Gauss_")
            self._threads[self._num_threads].start()
            self._threads[self._num_threads].enable(self._config['ZIP'])
            self._num_threads+=1
            self._threads[self._num_threads]=self.ZipArchiver(self,self.archiver3_request,self._config)
            self._threads[self._num_threads].setName("OnlySin_")
            self._threads[self._num_threads].start()
            self._threads[self._num_threads].enable(self._config['ZIP'])
            self._num_threads+=1
        except:
            print "Exceptions in start!"

    def join(self):
        try:
            self.joining=True
            count=0
            while not self.killed:
                self.printStatus()
                for i in range(self._num_threads):
                    if self._threads[i].isAlive():
                        count+=1
                if count == 0:
                    break
                else:
                    time.sleep(1)
                    count=0
            if not self.killed:
                if self._config['ANI']:
                    ani=subprocess.Popen(self._config['GEN_ANI'],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
                    ani.stdout.readlines()
                    ani.stderr.readlines()
                    ani.wait()
        except:
            if not self.killed:
                print "Exceptions in join!"

    def killall(self):
        try:
            self.killed=True
            count=0
            while True:
                for i in range(self._num_threads):
                    if self._threads[i].isAlive():
                        count+=1
                if count == 0:
                    break
                else:
                    time.sleep(1)
                    count=0
        except:
            print "Exceptions in killall!"


class TLM:
    def __init__(self,visual=Visualizer,config=Config()):
        self._visualizer=visual(config)
        self._config=config.getConfig()
        self._exe=self._config['TLM_EXE']
        self._config['instance']=self
        self._killed=False

    def run(self):
        self._visualizer.start()
        if self._config['TLM']:
            self._tlm=subprocess.Popen(self._exe,stdout=subprocess.PIPE,stderr=subprocess.PIPE,universal_newlines=True)
            while not self._killed:
                try:
                    line=self._tlm.stdout.readline()
                    if line:
                        print line,
                        self._visualizer.encounterLine(line)
                    else:
                        break
                except IOError:
                    if not self._killed:
                        print "Exceptions in run"
        else:
            for i in range(10,int(self._config['NT'])+1,10):
                self._visualizer.encounterLine(str(i))
        if not self._killed:
            self._visualizer.join()
        else:
            if sys.platform.find("win")==0:
                try:
                    import win32api
                    h=win32api.OpenProcess(1,True,self._tlm.pid)
                    win32api.TerminateProcess(h,1)
                except:
                    pass
            else:
                os.kill(self._tlm.pid,signal.SIGINT)
            self._tlm.wait()
        self._visualizer.__del__()

    def killall(self):
        print "Terminating all calculating threads, this may take some minutes..."
        self._killed=True
        self._visualizer.killall()
        print "TLM aborted!"

def handle_interrupt(sig,stack):
    if sig!=signal.SIGINT:
        print "caught unknown signal"
        sys.exit(1)
    else:
        tlm_config.getConfig()['instance'].killall()

def main(tlm_config):
    signal.signal(signal.SIGINT,handle_interrupt)
    tlm=TLM(config=tlm_config)
    tlm.run()


tlm_config=FileConfig("twotlme.in")
if __name__ == "__main__":
    main(tlm_config)

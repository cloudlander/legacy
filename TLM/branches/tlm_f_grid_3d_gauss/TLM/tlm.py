#!/usr/bin/python
import time,subprocess,threading,Queue,os,sys,signal
import StringIO
import re


class Config:
    def __init__(self):
        self._config={}
    def parseConfig(self):
        self._config['threads']=1
        self._config['PNG_SIZE']='1024,768'
        self._config['+RANGE']=0.3
        self._config['-RANGE']=-0.3
        self._config['TLM']=True
        self._config['MAP']=True
        self._config['SURFACE']=False
        self._config['ANI']=True
        self._config['+G_RANGE']=1.0
        self._config['-G_RANGE']=-1.0
        self._config['GAUSS']=True
        self._config['DO_EX']='0'
        self._config['DO_EY']='1'
        self._config['DO_EZ']='0'
        self._config['DO_HX']='0'
        self._config['DO_HY']='0'
        self._config['DO_HZ']='0'
        if sys.platform.find("win")==0:
            self._config['GEN_ANI']="gen_ani.bat"
            self._config['TLM_EXE']="Release/TLM.exe"
            self._config['GNUPLOT']="gnuplot.exe"
        else:
            self._config['GEN_ANI']="./gen_ani.sh"
            self._config['TLM_EXE']="./tlm"
            self._config['GNUPLOT']="gnuplot"
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

class ILineAware:
    def encounterLine(self,line):
        pass

class Visualizer(ILineAware):
    class Worker(threading.Thread):
        def __init__(self,producer,request,config):
            threading.Thread.__init__(self,None,self._worker)
            self._request=request
            self._producer=producer
            self._config=config
            self._enabled=True
        def enable(self,status):
            self._enabled=status
        def prepareWorker():
            pass
        def _worker(self):
            filename=None
            s=self.prepareWorker().readlines()
            while not self._producer.killed :
                try:
                    trunk=self._request()
                    if trunk != None:
                        if self._enabled:
                            commands=open(str(plot.pid)+".gnu","wb")
                            commands.writelines(s)
                            for i in range(trunk['start'],trunk['end']+1):
                                print >>commands,"set output \"%s_%s/IMG%5d.png\""%(trunk['prefix'],trunk['dir'],i)
                                if self._config['SPLIT']=='0':
                                    print >>commands,"splot \"%s.out\" index %d"%(trunk['prefix'],i-1)
                                else:
                                    print >>commands,"splot \"%s/%s%5d.out\""%(trunk['prefix'],trunk['prefix'],i)
                            commands.write("exit\n")
                            commands.close()
                            plot=subprocess.Popen(self._config['GNUPLOT'],stdin=subprocess.PIPE,universal_newlines=True)
                            plot.stdin.write("load \""+str(plot.pid)+".gnu\"\n")
                            plot.stdin.close()
                            plot.wait()
                            if filename != None:
                                os.unlink(filename)
                            filename=str(plot.pid)+".gnu"
                    elif self._producer.joining:
                        break
                except:
                    print "Exceptions in %s!"%(self.getName())
            try:
                if filename != None:
                    os.unlink(filename)
            except:
                print "Unable to remove .gnu file(%s)"%(self.getName())

    class MapWorker(Worker):
        def prepareWorker(self):
            env_set=StringIO.StringIO()
            env_set.writelines(
                    ["set samples 100, 100","\n",
                     "set isosamples 10, 10","\n"
                     "set palette model HSV functions gray, 1, 1","\n"
                     "set autoscale","\n",
                     "set size square","\n",
                     "set surface","\n",
                     'set title "TLM 2D"',"\n",
                     'set xlabel "Z"',"\n",
                     'set ylabel "X"',"\n",
                     'set xrange [',self._config['SX'],":",self._config['ENDX'],'] noreverse nowriteback',"\n",
                     'set yrange [',self._config['SZ'],":",self._config['ENDZ'],'] noreverse nowriteback',"\n",
                     'set zrange [',self._config['-RANGE'],":",self._config['+RANGE'],'] noreverse nowriteback',"\n",
                     'set cbrange [',self._config['-RANGE'],":",self._config['+RANGE'],'] noreverse nowriteback',"\n",
                     'set zero 1e-0020',"\n",
                     'set pm3d map',"\n",
                     'set dgrid3d ',str(int(self._config['ENDX'])-int(self._config['SX'])+1),",",str(int(self._config['ENDZ'])-int(self._config['SZ'])+1),"\n",
                     'set terminal png size ',self._config['PNG_SIZE'],"\n",
                     'set xlabel "Z"',"\n",
                     'set ylabel "X"',"\n"
                    ])
            env_set.seek(0)
            return env_set


    class SurfaceWorker(Worker):
        def prepareWorker(self):
            env_set=StringIO.StringIO()
            env_set.writelines(
                    ["set samples 100, 100","\n",
                     "set isosamples 10, 10","\n"
                     "set palette model HSV functions gray, 1, 1","\n"
                     "set autoscale","\n",
                     "set size square","\n",
                     "set surface","\n",
                     'set title "TLM 2D"',"\n",
                     'set xlabel "Z"',"\n",
                     'set ylabel "X"',"\n",
                     'set xrange [',self._config['SX'],":",self._config['ENDX'],'] noreverse nowriteback',"\n",
                     'set yrange [',self._config['SZ'],":",self._config['ENDZ'],'] noreverse nowriteback',"\n",
                     'set zrange [',self._config['-RANGE'],":",self._config['+RANGE'],'] noreverse nowriteback',"\n",
                     'set cbrange [',self._config['-RANGE'],":",self._config['+RANGE'],'] noreverse nowriteback',"\n",
                     'set zero 1e-0020',"\n",
                     'set pm3d at s',"\n",
                     'set dgrid3d ',str(int(self._config['ENDX'])-int(self._config['SX'])+1),",",str(int(self._config['ENDZ'])-int(self._config['SZ'])+1),"\n",
                     'set terminal png size ',self._config['PNG_SIZE'],"\n",
                     'set style data dots',"\n",
                     'set xlabel "Z"',"\n",
                     'set ylabel "X"',"\n"
                    ])
            env_set.seek(0)
            return env_set

    class GaussWorker(Worker):
        def prepareWorker(self):
            env_set=StringIO.StringIO()
            env_set.writelines(
                    ["set samples 100, 100","\n",
                     "set isosamples 10, 10","\n"
                     "set palette model HSV functions gray, 1, 1","\n"
                     "set autoscale","\n",
                     "set size square","\n",
                     "set surface","\n",
                     'set title "TLM 2D Gauss Distribution"',"\n",
                     'set xlabel "Z"',"\n",
                     'set ylabel "X"',"\n",
                     'set xrange [',self._config['SX'],":",self._config['ENDX'],'] noreverse nowriteback',"\n",
                     'set yrange [',self._config['SZ'],":",self._config['ENDZ'],'] noreverse nowriteback',"\n",
                     'set zrange [',self._config['-G_RANGE'],":",self._config['+G_RANGE'],'] noreverse nowriteback',"\n",
                     'set cbrange [',self._config['-G_RANGE'],":",self._config['+G_RANGE'],'] noreverse nowriteback',"\n",
                     'set zero 1e-0020',"\n",
                     'set pm3d at s',"\n",
                     'set dgrid3d ',str(int(self._config['ENDX'])-int(self._config['SX'])+1),",",str(int(self._config['ENDZ'])-int(self._config['SZ'])+1),"\n",
                     'set terminal png size ',self._config['PNG_SIZE'],"\n",
                     'set style data dots',"\n",
                     'set xlabel "Z"',"\n",
                     'set ylabel "X"',"\n"
                    ])
            env_set.seek(0)
            return env_set

    def __init__(self,config=Config()):
        self._config=config.getConfig()
        #self._num_threads=self._config['threads']*2
        self._num_threads=self._config['threads']*3
        self._threads=range(self._num_threads)
        self._map_queue=Queue.Queue()
        self._surface_queue=Queue.Queue()
        self._gauss_queue=Queue.Queue()
        self._trunk_start=1
        self.joining=False
        self.killed=False

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
        total=float(self.getTotal())
        if self._trunk_start > int(self._config['NT']):
            return (total-self._gauss_queue.qsize()*10)/total*100
        else:
            return ((self._trunk_start-1)/float(self._config['NT'])-self._gauss_queue.qsize()*10/total)*100

    def printStatus(self):
        print "TLM %.0f%% calculated, surface %.0f%% plotted, map %.0f%% plotted, Gauss %.0f%% plotted!\r"%(self.getTLMStatus(),self.getSurfacePlotStatus(),self.getMapPlotStatus(),self.getGaussPlotStatus()),

    def encounterLine(self,line):
        try:
            end=int(line)
            if end % 10 == 0:
                for task in ('EX','EY','EZ','HX','HY','HZ'):
                    if self._config["DO_"+task] == '1':
                        self.map_deposit({'start':self._trunk_start,'end':end,'prefix':task,'dir':'MAP'})
                        self.surface_deposit({'start':self._trunk_start,'end':end,'prefix':task,'dir':'3D'})
                self.gauss_deposit({'start':self._trunk_start,'end':end,'prefix':"GAUSS",'dir':'SIN'})
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

    def start(self):
        try:
            for i in range(self._num_threads/3):
                self._threads[i]=self.MapWorker(self,self.map_request,self._config)
                self._threads[i].setName("MapWorker"+str(i))
                self._threads[i].start()
                self._threads[i].enable(self._config['MAP'])
            for i in range(self._num_threads/3,self._num_threads*2/3):
                self._threads[i]=self.SurfaceWorker(self,self.surface_request,self._config)
                self._threads[i].setName("SurfaceWorker"+str(i))
                self._threads[i].start()
                self._threads[i].enable(self._config['SURFACE'])
            for i in range(self._num_threads*2/3,self._num_threads):
                self._threads[i]=self.GaussWorker(self,self.gauss_request,self._config)
                self._threads[i].setName("GaussWorker"+str(i))
                self._threads[i].start()
                self._threads[i].enable(self._config['GAUSS'])
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

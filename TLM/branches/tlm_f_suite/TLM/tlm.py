#!/usr/bin/python
import time,subprocess,threading,Queue,os,sys,signal
import StringIO
import re

class Config:
    def parseConfig(self):
        return True
    def resetConfig(self):
        return True
    def getConfig(self):
        return {}

class FileConfig(Config):
    def __init__(self,f):
        self._input=f
        self._config={}
        self.parseConfig()
    def parseConfig(self):
        input=open(self._input,"rb")
        data_line=input.readline()
        cmt_line=input.readline()
        input.close()
        data=re.split("[,\s]+",data_line)
        comment=re.split("[,\s]+",cmt_line)
        for (i,v) in enumerate(comment):
            self._config[v]=data[i]
    def getConfig(self):
        return self._config

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
        def prepareWorker():
            pass
        def _worker(self):
            try:
                s=self.prepareWorker().readlines()
                while not self._producer.killed :
                    trunk=self._request()
                    if trunk != None:
                        #print self.getName(),":",trunk
                        plot=subprocess.Popen("gnuplot",stdin=subprocess.PIPE,universal_newlines=True)
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
                        plot.stdin.write("load \""+str(plot.pid)+".gnu\"\n")
                        plot.stdin.close()
                        plot.wait()
                        os.unlink(str(plot.pid)+".gnu")
                    elif self._producer.joining:
                        break

            except:
                print "Exceptions in %s!"%(self.getName())

    class MapWorker(Worker):
        def prepareWorker(self):
            env_set=StringIO.StringIO()
            env_set.writelines(
                    ["set samples 100, 100","\n",
                     "set isosamples 10, 10","\n"
                     "set autoscale","\n",
                     "set size square","\n",
                     "set surface","\n",
                     'set title "TLM 2D"',"\n",
                     'set xlabel "Z"',"\n",
                     'set ylabel "X"',"\n",
                     'set xrange [',self._config['SX'],":",self._config['ENDX'],'] noreverse nowriteback',"\n",
                     'set yrange [',self._config['SZ'],":",self._config['ENDZ'],'] noreverse nowriteback',"\n",
                     'set zrange [-1.0:1.0] noreverse nowriteback',"\n",
                     'set cbrange[-0.3:0.3] noreverse nowriteback',"\n",
                     'set zero 1e-0020',"\n",
                     'set pm3d map',"\n",
                     'set dgrid3d ',str(int(self._config['ENDX'])-int(self._config['SX'])+1),",",str(int(self._config['ENDZ'])-int(self._config['SZ'])+1),"\n",
                     'set terminal png size 1024,768',"\n",
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
                     "set autoscale","\n",
                     "set size square","\n",
                     "set surface","\n",
                     'set title "TLM 2D"',"\n",
                     'set xlabel "Z"',"\n",
                     'set ylabel "X"',"\n",
                     'set xrange [',self._config['SX'],":",self._config['ENDX'],'] noreverse nowriteback',"\n",
                     'set yrange [',self._config['SZ'],":",self._config['ENDZ'],'] noreverse nowriteback',"\n",
                     'set zrange [-1.0:1.0] noreverse nowriteback',"\n",
                     'set cbrange[-0.3:0.3] noreverse nowriteback',"\n",
                     'set zero 1e-0020',"\n",
                     'set pm3d at s',"\n",
                     'set dgrid3d ',str(int(self._config['ENDX'])-int(self._config['SX'])+1),",",str(int(self._config['ENDZ'])-int(self._config['SZ'])+1),"\n",
                     'set terminal png size 1024,768',"\n",
                     'set style data dots',"\n",
                     'set xlabel "Z"',"\n",
                     'set ylabel "X"',"\n"
                    ])
            env_set.seek(0)
            return env_set

    def __init__(self,config=Config(),threads=1):
        self._config=config.getConfig()
        self._num_threads=threads*2
        self._threads=range(threads*2)
        self._map_queue=Queue.Queue()
        self._surface_queue=Queue.Queue()
        self._trunk_start=1
        self.joining=False
        self.killed=False

    def getTLMStatus(self):
        total=float(self._config['NT'])
        if self._trunk_start > total:
            return 100
        else:
            return (self._trunk_start-1)/total*100

    def getMapPlotStatus(self):
        total=float(self._config['NT'])
        if self._trunk_start > total:
            return (total-self._map_queue.qsize()*10)/total*100
        else:
            return (self._trunk_start-1-self._map_queue.qsize()*10)/total*100

    def getSurfacePlotStatus(self):
        total=float(self._config['NT'])
        if self._trunk_start > total:
            return (total-self._surface_queue.qsize()*10)/total*100
        else:
            return (self._trunk_start-1-self._surface_queue.qsize()*10)/total*100

    def printStatus(self):
        print "TLM %.2f%% calculated, surface %.2f%% plotted, map %.2f%% plotted!\r"%(self.getTLMStatus(),self.getSurfacePlotStatus(),self.getMapPlotStatus()),

    def encounterLine(self,line):
        try:
            end=int(line)
            if end % 10 == 0:
                self.map_deposit({'start':self._trunk_start,'end':end,'prefix':'EY','dir':'MAP'})
                self.surface_deposit({'start':self._trunk_start,'end':end,'prefix':'EY','dir':'3D'})
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

    def start(self):
        try:
            for i in range(self._num_threads/2):
                self._threads[i]=self.MapWorker(self,self.map_request,self._config)
                self._threads[i].setName("MapWorker"+str(i))
                self._threads[i].start()
            for i in range(self._num_threads/2,self._num_threads):
                self._threads[i]=self.SurfaceWorker(self,self.surface_request,self._config)
                self._threads[i].setName("SurfaceWorker"+str(i))
                self._threads[i].start()

        except:
            print "Exceptions in start!"

    def join(self):
        try:
            self.joining=True
            count=0
            while True:
                self.printStatus()
                for i in range(self._num_threads):
                    if self._threads[i].isAlive():
                        count+=1
                if count == 0:
                    break
                else:
                    time.sleep(1)
                    count=0
            ani=subprocess.Popen("./gen_ani.sh",stdout=subprocess.PIPE,stderr=subprocess.PIPE)
            ani.stdout.readlines()
            ani.stderr.readlines()
        except:
            print "Exceptions in join!"

    def killall(self):
        try:
            self.killed=True
            count=0
            while True:
                self.printStatus()
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
    def __init__(self,exe="./tlm",visual=Visualizer,config=Config()):
        self._exe=exe
        self._visualizer=visual(config)
        self._config=config.getConfig()
        self._killed=False

    def run(self):
        self._visualizer.start()
        self._tlm=subprocess.Popen(self._exe,stdout=subprocess.PIPE,universal_newlines=True)
        while not self._killed:
            line=self._tlm.stdout.readline()
            if line:
                print line,
                self._visualizer.encounterLine(line)
            else:
                break
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
            print "TLM aborted!"

    def killall(self):
        print "Terminating all calculating threads, this may take some minutes..."
        self._killed=True
        self._visualizer.killall()

global tlm

def handle_interrupt(sig,stack):
    if sig!=signal.SIGINT:
        print "caught unknown signal"
        sys.exit(1)
    else:
        tlm.killall()

signal.signal(signal.SIGINT,handle_interrupt)
tlm=TLM(config=FileConfig("twotlme.in"))
tlm.run()

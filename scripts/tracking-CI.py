#!/usr/bin/python
#This program is to get the tracking results of check-in events
#It will create a html file named tracking-check-in-events.html and a check_in_diffs fold in storage directory

#Author: Allan Zhang 2004-07-16
    

import time, os, sys, string, socket, errno, re, MimeWriter, smtplib, mimetools, cStringIO


#This class is used to process the arugments inputed by command line, then, run the cvs_diffs.ksh
#then, read the diffs.txt created by cvs_diffs.ksh into a list of records. Each record represents
#a change submitted to CVS sever. And the diffs of each changed files are save as text files in
#check_in_diffs directory that is created in a specified path.
#The records contains the fields: User, File, Time, Log and Diffs
class Inputor:
    def __init__(self, value):
        #It's the list of records
        self.MyTable = value
########
	self.revMap={}
	self.newRevMap={}
########	
        self.storageDir = os.getcwd()
        version = sys.version;
        ver = string.split(version)
        numbers = string.split(ver[0], ".")
        if (numbers[0]>='2'):
            if (numbers[1]>='3'):
                if(numbers[2]<'2'):
                    print "The python version is at least 2.3.2\n"
                    sys.exit(0)
            else:
                print "The python version is at least 2.3.2\n"
                sys.exit(0)
        else:
            print "The python version is at least 2.3.2\n"
            sys.exit(0)
                
########
    def revMapInit(self):
	if(self.cached==0):
		return
	try:
		source=open("rev-cache","r")
		print "rev-cache found, reading"
	except IOError:
		print "rev-cache not found"
		return
	txt=source.readline()
	while(txt):
		list=string.split(txt)
		self.revMap[list[0]]=list[1]
		txt=source.readline()
#	print revMap
	return
	
    def revMapDeinit(self):
#	print newRevMap
	if(self.cached==0):
		return
	if(len(self.newRevMap) > 0):
		try:
			source=open("rev-cache","a")
		except IOError:
			print "rev-cache error"
			raise
		for revi in self.newRevMap:
			source.write(revi+"\t"+self.newRevMap[revi]+"\n")
	return
########

    #process the arugments from command line        
    def dealArgument(self):
        self.emailRecv = ""
        self.emailTitle = ""
        self.storageDir2 = ""
########
	self.cached=0
########	
        args = ""
        isDSetted = False
        isRSetted = False
        i = 1
        while (i < len(sys.argv)):
            arg = sys.argv[i]
            if (arg == "-s"):
                self.storageDir = sys.argv[i+1]
                self.storageDir = os.path.abspath(self.storageDir)
                if ((not os.path.exists(self.storageDir)) and (not os.path.ismount(self.storageDir))):
                    try:
                        os.mkdir(self.storageDir)
                    except OSError, err:
                        # Reraise the error unless it's about an already existing directory 
                        if err.errno != errno.EEXIST or not os.path.isdir(self.storageDir): 
                            raise
                if (len(sys.argv)> i+2 and sys.argv[i+2][0] != '-'):
                    self.storageDir2 = sys.argv[i+2]
                    self.storageDir2 = os.path.abspath(self.storageDir2)
                    if ((not os.path.exists(self.storageDir2)) and (not os.path.ismount(self.storageDir2))):
                        try:
                            os.mkdir(self.storageDir2)
                        except OSError, err:
                            # Reraise the error unless it's about an already existing directory 
                            if err.errno != errno.EEXIST or not os.path.isdir(self.storageDir2): 
                                raise                    
                    i = i+1
                    
            elif (arg == "-d"):
                args = args + " " + arg + " " + "\"" + sys.argv[i+1] +"\""
                isDSetted = True
            elif (arg == "-r"):
                args = args + " " + arg + sys.argv[i+1]
                isRSetted = True
            elif (arg == "-e"):
                self.emailRecv = sys.argv[i+1]
            elif (arg == "-t"):
                self.emailTitle = sys.argv[i+1]
########
	    elif (arg == "--local-cache"):
		self.cached = 1
########		
            else:
                print "Please use Python 2.3.2 or higher version\n"
                print "Usage: ./tracking-CI.py <-r branch-name> <-d specified dates> <-s storage-directory>\n"
                print "Description:"
                print "  This program is to get the tracking results of check-in events"
                print "  It will create a html file named tracking-check-in-events.html \n  and a check_in_diffs fold in storage directory\n"
                print "Options:\n"
                print " -d <d>  Specify dates (d1<d2). Example:\n           \">2 days ago\"\n           \"2000-7-20 > 2000-7-20 14:20:01\", \n              means dates between 2000-7-20 14:20:01 and 2000-7-20, exclusively\n           \"2000-7-20 >= 2000-7-20 14:20:01\",\n              means dates between 2000-7-20 14:20:01 and 2000-7-20, inclusively\n            The default is \">= 1 weeks ago\""
                print " -r <r>  Specify branch.  The default will be the branch-merge_uc"
                print " -s <s1> <s2>  Specify storage dirs(2 at most) to save the results. Example:\n           \"//allan/share\"\n           \"result/checkin\"\n            \"//allan/share  result/checkin\"\n            The default will be \".\""
                print " -e <email recievers> Specified the email reievers.\n                    If no this option, no email will be sent."
                print " -t <email's title>. Example: \"The check-in records in the last week\""
########
		print " --local-cache.  Load and store map of (File,Branch Number)->(Branch Name) to a file named \"rev-cache\""
########		
                sys.exit(0)
                
            i += 2
        
        if (not isRSetted):
            args += " -r branch-merge_uc"
        if (not isDSetted):
            args += " -d \">= 1 week ago\""
#        os.system("cd $MECAREA/dev")
        try:
#            print "mkdir " + self.storageDir + "/check_in_diffs"
            os.mkdir(self.storageDir + "/check_in_diffs")
        except OSError, err:
            # Reraise the error unless it's about an already existing directory 
            if err.errno != errno.EEXIST or not os.path.isdir(self.storageDir + "/check_in_diffs"): 
                raise
        os.system("rm -rf " + self.storageDir + "/check_in_diffs/*")
	os.chdir(os.environ['MECAREA']+'/dev')
        args = "sh.exe cvs_diffs.ksh " + args
        print args

        if (os.system(args)!=0):
            print "call to cvs_diffs.ksh " + args + "failed"
        
        

    #write the diffs of each file into a text file respectively.
    def writeDiffs(self, MyRecord):
            file = MyRecord["File"]
            list = string.split(file)
            subList = string.split(list[0],"/")
            fileName = self.storageDir + "/check_in_diffs/" + subList[len(subList)-1] + list[1]+".txt"
            logFile = open(fileName, "w")
            logFile.write(MyRecord["Diffs"])
            logFile.close()
            patt = re.compile("^//")
            if (patt.match(self.storageDir)):
                MyRecord["Diffs"] = "file:"+ fileName
            else:
                MyRecord["Diffs"] = fileName
            if (self.storageDir2 != ""):
                fileName2 = self.storageDir2 + "/check_in_diffs/" + subList[len(subList)-1] + list[1]+".txt"
                if (patt.match(self.storageDir)):
                    MyRecord["Diffs2"] = "file:"+ fileName2
                else:
                    MyRecord["Diffs2"] = fileName2
    
########    
    def getBranch(self,FileName,revision):
	patt=re.compile(r"(.*)\.[0-9]*$")
	revision=re.sub(patt,r"\1",revision)
	print FileName+revision
	if(revision in self.newRevMap):
		print self.newRevMap[revision]+" found\n"
		return self.newRevMap[revision]
	if(revision in self.revMap):
		print self.revMap[revision]+" found\n"
		return self.revMap[revision]
	branch="(none)"
	print "cvs -q status -v "+ FileName + " > tmp.status"
	os.system("cvs -q status -v "+ FileName + " > tmp.status")
   	source=open("tmp.status","r")
	txt=source.readline()
	while(txt and "Existing Tags" not in txt):
	    txt=source.readline()
	txt=source.readline()
	while(txt):
	    if("branch" not in txt):
		    txt=source.readline()
		    continue
	    list=string.split(txt)
	    list[2]=string.replace(list[2],")",'')
	    if(list[2] == revision):
		    branch=list[0]
		    print branch+"\n"
		    self.newRevMap[revision]=branch
		    return branch
            txt=source.readline()
        if(branch == "(none)"):
	    branch="MAIN"
	    print branch+"\n"
	    self.newRevMap[revision]=branch
	    return branch
	raise   
########	    
		    
    
    #read data from diffs.txt into list of records that is contain all contents we need processed.               
    def readData(self):
        MyRecord = {}
        source = open('diffs.txt', 'r')
        txt = source.readline()
        i=0
        while (txt):
            print txt + "\n"
#            MyRecord={"User":"", "File":"", "Time":"", "Log":"", "Diffs":"","Diffs2":""};
########
	    MyRecord={"User":"", "File":"", "Time":"", "Log":"", "Diffs":"","Diffs2":"","Branch":""};
########	    
            #process each block of text begining with ===BEGIN CHANGESET
            if ("===BEGIN CHANGESET" in txt):
                list = string.split(txt)
                MyRecord["User"]=list[len(list)-1];
                MyRecord["Time"]=list[len(list)-4] + " " + list[len(list)-3]
                txt = source.readline()
                patt = re.compile("^/.*")
                #read the log information
                while (not patt.match(txt) and "===BEGIN CHANGESET" not in txt):
#                    txt = string.replace(txt,'\n','')
                    MyRecord["Log"]+= txt
                    txt = source.readline()
                sublist=[]
                #read the names of files that are changed in this change set
                while (patt.match(txt)):
                    txt = string.replace(txt, '\n', '')
                    MyRecord["File"]=txt
########
		    revi=string.split(MyRecord["File"])
                    list1 = string.split(MyRecord["File"])
                    list1[0] = string.replace(list1[0], '/','',1)
		    MyRecord["Branch"]=self.getBranch(list1[0],revi[1])
########		    	    
#                    MyRecord["Diffs"] = ""
                    sublist.append(MyRecord)
                    MyRecord=MyRecord.copy()
                    txt=source.readline()
########
		print "rm -f ./tmp.status"
		os.system("rm -f ./tmp.status")
########		
                #read the differences of these changed files
                isNewLine = True
                for MyRecord in sublist:
                    if (isNewLine):
                        isNewLine = False
                        if ("===BEGIN CHANGESET" in txt):
                            break                    
                        differs = ""
                    list = string.split(MyRecord["File"])
                    list[0] = string.replace(list[0], '/','',1)
                    if (list[0] not in txt):
                        continue
                    while ("=== END FILE" not in txt):
                        differs = differs + txt
                        txt = source.readline()
                    differs = differs + txt
                    MyRecord["Diffs"] = differs
                    self.writeDiffs(MyRecord)
                    txt = source.readline()
                    isNewLine = True
                    
                #append the result records to list
                for MyRecord in sublist:
                    self.MyTable.append(MyRecord)
                if (len(sublist) == 0):
                    self.MyTable.append(MyRecord)
            else:
                #just go ahead
                txt = source.readline()
        source.close()
        
    def process(self):
        self.dealArgument()
########
	self.revMapInit()
########	
        self.readData()
########
	self.revMapDeinit()
########	

#To sort these records by user, file and time        
class Sortor:
    def __init__(self, value):
        self.MyTable = value
        self.mapByUser={}
        self.mapByFile={}
        self.mapByTime={}
########	
	self.mapByBranch={}
########	
        for MyRecorder in self.MyTable:
            #create the mapping between (User, Time) and records
            if (not self.mapByUser.has_key((MyRecorder["User"],MyRecorder["Time"]))):
                self.mapByUser[(MyRecorder["User"],MyRecorder["Time"])] = [MyRecorder]
            else:
                self.mapByUser[(MyRecorder["User"],MyRecorder["Time"])].append(MyRecorder)
            #create the mapping between File and records
            self.mapByFile[(MyRecorder["File"],MyRecorder["Time"])]= [MyRecorder]

            #create the mapping between Time and records           
            if (not self.mapByTime.has_key(MyRecorder["Time"])):
                self.mapByTime[MyRecorder["Time"]] = [MyRecorder]
            else:
                self.mapByTime[MyRecorder["Time"]].append(MyRecorder)
		
########
            #create the mapping between Branch and records
            if (not self.mapByBranch.has_key(MyRecorder["Branch"])):
                self.mapByBranch[MyRecorder["Branch"]] = [MyRecorder]
            else:
                self.mapByBranch[MyRecorder["Branch"]].append(MyRecorder)
########	

    #sort the records by user
    def getRecordsByUser(self):
        keys = self.mapByUser.keys()
        keys.sort()
        return [self.mapByUser[key] for key in keys]
    #sort the records by file
    def getRecordsByFile(self):
        keys = self.mapByFile.keys()
        keys.sort()
        return [self.mapByFile[key] for key in keys]
    #sort the records by time
    def getRecordsByTime(self):
        keys = self.mapByTime.keys()
        keys.sort()
        keys.reverse()
        return [self.mapByTime[key] for key in keys]

########
    def getRecordsByBranch(self):
        keys = self.mapByBranch.keys()
        keys.sort()
        keys.reverse()
        return [self.mapByBranch[key] for key in keys]
########
	
class Outputor:
########    
#    def __init__(self, listByUser, listByFile, listByTime):
    def __init__(self, listByUser, listByFile, listByTime, listByBranch):
########
        self.listByUser = listByUser
        self.listByFile = listByFile
        self.listByTime = listByTime
########
	self.listByBranch = listByBranch
########	
    #iterate to output the records that is ordered by file
    def iterationByFile(self, htmlReport, list):
        htmlReport.write("<table cellSpacing=1 cellPadding=1 border=1 width=150%>\n")
#        htmlReport.write("<tr valign=top bgColor=turquoise> <td><strong>Filename & Diffs</strong></td> <td><strong>User</strong></td> <td><strong>Check-in time</strong></td> <td><strong>Log Message</strong></td></tr> \n")
########
        htmlReport.write("<tr valign=top bgColor=turquoise> <td><strong>Filename & Diffs</strong></td> <td><strong>Branch</strong></td> <td><strong>User</strong></td> <td><strong>Check-in time</strong></td> <td><strong>Log Message</strong></td></tr> \n")
########	
        i = 0
        for MyRecords in list:
              for MyRecord in MyRecords:
                anchor2 = ""
                i = i+1
                if (i%2==0):
                    color = "bgColor=lightgrey"
                else:
                    color = "bgColor=palegoldenrod"                  
                if MyRecord["Diffs"] == "":
                    anchor = MyRecord["File"]
                else:
                    anchor = "<a href=\"" + MyRecord["Diffs"] + "\">" + MyRecord["File"] + "</a>"
                    if (MyRecord["Diffs2"] == ""):
                        anchor2 = ""
                    else:
                        anchor2 = "<br><a href=\"" + MyRecord["Diffs2"] + "\">" + MyRecord["File"] + "</a>"
#                line = "<td>"+ anchor + anchor2 + "</td><td>" + MyRecord["User"] + "</td><td>" +  MyRecord["Time"] + "</td><td>" + MyRecord["Log"] +" </td>"
########			
                line = "<td>"+ anchor + anchor2 + "</td><td>" + MyRecord["Branch"] + "</td><td>" + MyRecord["User"] + "</td><td>" +  MyRecord["Time"] + "</td><td>" + MyRecord["Log"] +" </td>"
########		
                htmlReport.write("<tr " + color +">" + line + " </tr>\n")
        htmlReport.write("</table>\n")

    #iterate to output the records that is ordered by user or time    
    def iterationByOthers(self, htmlReport, list, kind):
        
        htmlReport.write("<table cellSpacing=1 cellPadding=1 border=1 width=150%>\n")
        if (kind == "By User"):
########
#            htmlReport.write("<tr valign=top bgColor=turquoise> <td><strong>User</strong></td> <td><strong>Check-in time</strong></td> <td><strong>Filename & Diffs</strong></td> <td><strong>Log Message</strong></td></tr> \n")
            htmlReport.write("<tr valign=top bgColor=turquoise> <td><strong>User</strong></td> <td><strong>Branch</strong></td> <td><strong>Check-in time</strong></td> <td><strong>Filename & Diffs</strong></td> <td><strong>Log Message</strong></td></tr> \n")
#        else:
	elif (kind == "By Time"):
########	     
#            htmlReport.write("<tr valign=top bgColor=turquoise> <td><strong>Check-in time</strong></td> <td><strong>User</strong></td> <td><strong>Filename & Diffs</strong></td> <td><strong>Log Message</strong></td></tr> \n")
            htmlReport.write("<tr valign=top bgColor=turquoise> <td><strong>Check-in time</strong></td> <td><strong>Branch</strong></td> <td><strong>User</strong></td> <td><strong>Filename & Diffs</strong></td> <td><strong>Log Message</strong></td></tr> \n")
########	
	else:
            htmlReport.write("<tr valign=top bgColor=turquoise> <td><strong>Branchs</strong></td> <td><strong>Check-in time</strong></td> <td><strong>User</strong></td> <td><strong>Filename & Diffs</strong></td> <td><strong>Log Message</strong></td></tr> \n")
        i = 0
########	
        
        for MyRecords in list:
            prevRecord = {}
            fileField = ""
            for MyRecord in MyRecords:
                anchor2 = ""
                if MyRecord["Diffs"] == "":
                    anchor = MyRecord["File"]
                else:
                    anchor = "<a href=\"" + MyRecord["Diffs"] + "\">" + MyRecord["File"] + "</a>"
                    if (MyRecord["Diffs2"] != ""):
                        anchor2 = "<br><a href=\"" + MyRecord["Diffs2"] + "\">" + MyRecord["File"] + "</a>"
                    else:
                        anchor2 = ""
                #the codes below are to cluster the records with same user or the same time into single row of the table in HTML format.
########
                if (prevRecord == {}):
########                        
                    savedUser=MyRecord["User"]
                    savedLog=MyRecord["Log"]
                    savedTime=MyRecord["Time"]
                    savedBranch=MyRecord["Branch"]
########                    
                    fileField = "<table>\n<tr><td>"+anchor+anchor2+"</td></tr>\n"
                elif(prevRecord["Branch"] == MyRecord["Branch"] and prevRecord["Time"] == MyRecord["Time"]):
                    fileField += "<tr><td>"+ anchor + anchor2 + "</td></tr>\n"
                else:
                    fileField +="</table>\n"
                    if (kind == "By User"):
########			
#                        line = "<td>"+ MyRecord["User"] + "</td><td>" + MyRecord["Time"] + "</td><td>" + fileField + "</td><td>" + MyRecord["Log"] +" </td>"
                        line = "<td>"+ savedUser + "</td><td>" + savedBranch + "</td><td>" + savedTime + "</td><td>" + fileField + "</td><td>" + savedLog +" </td>"
                    elif (kind == "By Time"):
#                        line = "<td>"+ MyRecord["Time"] + "</td><td>" + MyRecord["User"] + "</td><td>" + fileField + "</td><td>" + MyRecord["Log"] +" </td>"
                        line = "<td>"+ savedTime + "</td><td>" + savedBranch + "</td><td>" + savedUser + "</td><td>" + fileField + "</td><td>" + savedLog +" </td>"
		    else: 
		    	line = "<td>"+ MyRecord["Branch"] + "</td><td>" + savedTime + "</td><td>" + savedUser + "</td><td>" + fileField + "</td><td>" + savedLog +" </td>"
#		    	line = "<td>"+ MyRecord["Branch"] + "</td><td>" + MyRecord["Time"] + "</td><td>" + MyRecord["User"] + "</td><td>" + fileField + "</td><td>" + MyRecord["Log"] +" </td>"
########			
                    i = i + 1
                    if (i%2==0):
                        color = "bgColor=lightgrey"
                    else:
                        color = "bgColor=palegoldenrod" 
                    htmlReport.write("<tr " + color +">" + line + " </tr>\n")
                    fileField = "<table>\n<tr><td>"+anchor+anchor2+"</td></tr>\n"
                prevRecord = MyRecord
########                
                savedUser=MyRecord["User"]
                savedLog=MyRecord["Log"]
                savedTime=MyRecord["Time"]
                savedBranch=MyRecord["Branch"]
########                
            if (fileField != ""):
                fileField +="</table>"
                if (kind == "By User"):
########			
#                        line = "<td>"+ MyRecord["User"] + "</td><td>" + MyRecord["Time"] + "</td><td>" + fileField + "</td><td>" + MyRecord["Log"] +" </td>"
                        line = "<td>"+ MyRecord["User"] + "</td><td>" + MyRecord["Branch"] + "</td><td>" + MyRecord["Time"] + "</td><td>" + fileField + "</td><td>" + MyRecord["Log"] +" </td>"
                elif (kind == "By Time"):
#                        line = "<td>"+ MyRecord["Time"] + "</td><td>" + MyRecord["User"] + "</td><td>" + fileField + "</td><td>" + MyRecord["Log"] +" </td>"
                        line = "<td>"+ MyRecord["Time"] + "</td><td>" + MyRecord["Branch"] + "</td><td>" + MyRecord["User"] + "</td><td>" + fileField + "</td><td>" + MyRecord["Log"] +" </td>"
		else: 
		    	line = "<td>"+ MyRecord["Branch"] + "</td><td>" + MyRecord["Time"] + "</td><td>" + MyRecord["User"] + "</td><td>" + fileField + "</td><td>" + MyRecord["Log"] +" </td>"
########			
                i = i + 1
                if (i%2==0):
                     color = "bgColor=lightgrey"
                else:
                     color = "bgColor=palegoldenrod" 
                htmlReport.write("<tr " + color +">" + line + " </tr>\n")
        htmlReport.write("</table>\n")
                
                      
                    
                    
    #output 3 tables in HTML format that are check-in events by user, check-in events by file and check-in events by time respectively    
    def output(self):
        htmlReport = open(MyInput.storageDir + "/tracking-check-in-events.html","w")
########
        htmlReport.write("<a name = \"top\" href= \"#By Branch\"> Tracking by Branch </a> <br> \n")
########	
        htmlReport.write("<a name = \"top\" href= \"#By Time\"> Tracking by Time </a> <br> \n")
        htmlReport.write("<a href= \"#By User\"> Tracking by User </a> <br> \n")
        htmlReport.write("<a href= \"#By File\"> Tracking by File </a> <br> \n")
       

########
        htmlReport.write("<br><br><br>")
        htmlReport.write("<a name=\"By Branch\"><P style=\"FONT-WEIGHT: bold; FONT-SIZE: larger; COLOR: salmon; FONT-STYLE: italic; FONT-VARIANT: normal\"> Check-in Events by Branch </p></a>\n")
        self.iterationByOthers(htmlReport, self.listByBranch, "By Branch")
        htmlReport.write("<br><br><br>")
        htmlReport.write("\n<a name=\"By Branch\" href=\"#top\"> Return to Top </a><br>\n")
########
	
        htmlReport.write("<br><br><br>")
        htmlReport.write("<a name=\"By Time\"><P style=\"FONT-WEIGHT: bold; FONT-SIZE: larger; COLOR: salmon; FONT-STYLE: italic; FONT-VARIANT: normal\"> Check-in Events by Time </p></a>\n")
        self.iterationByOthers(htmlReport, self.listByTime, "By Time")
        

        htmlReport.write("<br><br><br>")
        htmlReport.write("\n<a name=\"By User\" href=\"#top\"> Return to Top </a><br>\n")
        htmlReport.write("<P style=\"FONT-WEIGHT: bold; FONT-SIZE: larger; COLOR: salmon; FONT-STYLE: italic; FONT-VARIANT: normal\">Check-in Events by User </p>")
        self.iterationByOthers(htmlReport, self.listByUser, "By User")

        htmlReport.write("<br><br><br>")
        htmlReport.write("<a name=\"By File\" href=\"#top\"> Return to Top </a><br>\n")
        htmlReport.write("<P style=\"FONT-WEIGHT: bold; FONT-SIZE: larger; COLOR: salmon; FONT-STYLE: italic; FONT-VARIANT: normal\">Check-in Events by File</p>\n")
        self.iterationByFile(htmlReport, self.listByFile)        
        
        
        htmlReport.write("\n<br><br><a href=\"#top\"> Return to Top </a><br>\n")
        htmlReport.close()
        if (MyInput.storageDir2 != ""):
            dir1 = string.replace(MyInput.storageDir,"\\","/")
            dir2 = string.replace(MyInput.storageDir2,"\\","/")
            os.system("rm -rf " + dir2 + "/check_in_diffs")
#            print "cp " + dir1 + "/tracking-check-in-events.html " + dir2
            os.system("cp " + dir1 + "/tracking-check-in-events.html " + dir2)
#            print "cp -R " + dir1 + "/check_in_diffs " + dir2
            os.system("cp -R " + dir1 + "/check_in_diffs " + dir2)
        #send emais to specified recipants
        if (MyInput.emailRecv != ""):
            htmlReport = open(MyInput.storageDir + "/tracking-check-in-events.html","r")
            html = htmlReport.read()
            htmlReport.close()
            From = "Xianfeng.Zhang@intel.com"
            To = MyInput.emailRecv
#            date = time.ctime(time.time())
#            Head = ('From: %s\nTo: %s\nDate: %s\nSubject: %s\n' 
#                         % (From, To, date, "Check-in Events in the recent week"))
            out = cStringIO.StringIO(  ) # output buffer for our message
            writer = MimeWriter.MimeWriter(out)
            htmlin = cStringIO.StringIO(html)
            writer.addheader("Subject", MyInput.emailTitle)
            writer.addheader("To", To)
            writer.addheader("From", From)
            writer.addheader("MIME-Version", "1.0")
            writer.flushheaders()
            writer.startmultipartbody("alternative")
            subpart = writer.nextpart()
            subpart.addheader("Content-Transfer-Encoding", "quoted-printable")
            pout = subpart.startbody("text/html", [("charset", 'us-ascii')])
            mimetools.encode(htmlin, pout, 'quoted-printable')
            htmlin.close()
            writer.lastpart()
            msg = out.getvalue( )
            out.close()
               
            server = smtplib.SMTP("mipos2.intel.com")              # connect, no login step
            failed = server.sendmail(From, To, msg)
            server.quit() 
            if failed:                                     # smtplib may raise exceptions
               print 'Failed recipients:', failed         # too, but let them pass here
        
  




        
        
#main program entry:
MyTable = []    #The list of records
MyInput = Inputor(MyTable) 
MyInput.process()   #Fill the list with the contents from "diffs.txt"
MySortor = Sortor(MyTable) 
listByUser = MySortor.getRecordsByUser() #Sort the records by user
listByFile = MySortor.getRecordsByFile() #Sort the records by file
listByTime = MySortor.getRecordsByTime() #Sort the records by time
########
listByBranch = MySortor.getRecordsByBranch()
#MyOutputor = Outputor(listByUser, listByFile, listByTime)
MyOutputor = Outputor(listByUser, listByFile, listByTime,listByBranch)
########    
MyOutputor.output() #Output the result in HTML format
print "The result file(tracking-check-in-events.html) is in " + MyInput.storageDir + "  " + MyInput.storageDir2 + "\n"



#dest = open("testreslut.txt", "w")
#dest.write("\n\n\n.................Records in MyTable.....................*\n")
#for MyRecord in MyTable:
#    dest.write(MyRecord["User"] +"      "+ MyRecord["Time"] +"      "+ MyRecord["File"] + "     "+MyRecord["Log"] + "       " + MyRecord["Diffs"]);
#    dest.write("\n")
#dest.write("\n\n\n.................List By User.....................*\n")
#for MyRecords in listByUser:
#    for MyRecord in MyRecords:
#        dest.write(MyRecord["User"] +"      "+ MyRecord["Time"] +"      "+ MyRecord["File"] + "     "+MyRecord["Log"] + "       " + MyRecord["Diffs"]);
#        dest.write("\n")
#dest.write("\n\n\n.................List By File.....................*\n")
#for MyRecords in listByFile:
#    for MyRecord in MyRecords:
#        dest.write(MyRecord["User"] +"      "+ MyRecord["Time"] +"      "+ MyRecord["File"] + "     "+MyRecord["Log"] + "       " + MyRecord["Diffs"]);
#        dest.write("\n")
#dest.write("\n\n\n.................List By Time.....................*\n")
#for MyRecords in listByTime:
#    for MyRecord in MyRecords:
#        dest.write(MyRecord["User"] +"      "+ MyRecord["Time"] +"      "+ MyRecord["File"] + "     "+MyRecord["Log"] + "       " + MyRecord["Diffs"]);
#        dest.write("\n")
#    
#dest.close()
    
    


        

    









            
                
            
            
            
            
    
    
    
    
    
    
    

  





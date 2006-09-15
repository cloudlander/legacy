if CDBl(WScript.Version) < 5.6 then
    MsgBox "This program only runs on Windows XP and above!",vbCritical,"Checking Version"
else
    Shutdown
end if

sub Shutdown
    dim when
    dim ret
    when = ShutdownScheduled
    if when <> "" then
        ret = MsgBox ( "Keep the scheduled shutdown at " & when &"?",vbYesNo or vbQuestion,"Well")
        if ret = vbNo then
            DeleteShutdown
            when = ShutdownSheduled
            if when = "" then
                MsgBox "scheduled shutdown is cancelled",vbInformation,"Great"
            else
                MsgBox "failed to cancel shutdown task",vbCritical,"My God!"
            end if
        end if
    else
        default = DateAdd("n",30,CDate(Year(Now) & "-" & Month(Now) & "-" & Day(Now) & " " & Hour(Now) & ":" & Minute(Now) & ":00"))
        when = InputBox("When to shutdown your computer?" &  Chr(13) & Chr(10) & "Default is after 30 minutes ","Only 1 question :)",default)
        if when <> "" then
            ret = MsgBox("You want to shutdown computer at " & when & "?",vbOKCancle or vbQuestion,"Are you sure")
            if ret = vbCancel then
                MsgBox "Shutdown cancelled",vbInformation,"Well"
            else
                msg = "halted"
                how = "shutdown -s -t 5 -c " & Time
                ScheduleShutdown when,how
                if when = ShutdownScheduled then
                    MsgBox "System will be " & msg & " at " & when & "!",vbOKOnly or vbInformation,"Just a reminder"
                else
                    MsgBox "failed to schedule shutdown task",vbCritical,"My God!"
                end if
            end if
        else
            MsgBox "Shutdown cancelled",vbInformation,"Well"
        end if
    end if
end sub

function ShutdownScheduled
    dim output
    dim ret
    set WshShell = WScript.CreateObject("WScript.Shell")
    set schtasks = WshShell.Exec( "schtasks" )
    output=schtasks.StdOut.ReadAll
    ret=InStr(output,"SHUTDOWN")
    On Error resume next
    if ret > 0 then
        dim regEx,Match,Matches
        set regEx = New RegExp
        regEx.Pattern = ".*SHUTDOWN\s*(\d{2}:\d{2}:\d{2}),\s*(\d{4}-\d+-\d+).*"
        regEx.IgnoreCase = false
        regEx.Global = true
        set Matches = regEx.Execute(output)
        set Match = Matches(0)
        ShutdownScheduled = Match.SubMatches(1) & " " & Match.SubMatches(0)
        if Err.Description <> "" then   ' Need to clean all NEVER RUN died shutdown tasks.
            ShutdownScheduled = ""
            MsgBox "There are inactive shutdown tasks which never run." & Chr(13) & Chr(10) & "I'll clean them now",vbOKOnly or vbInformation,"Just a reminder"
            DeleteShutdown
        end if
    else
        ShutdownScheduled = ""
    end if
end function

sub ScheduleShutdown(when,how)
    set WshShell = WScript.CreateObject("WScript.Shell")
    date_time = Split(when)
    arrSchDate = Split(date_time(0),"-")
    schYear = arrSchDate(0)
    schMonth = arrSchDate(1)
    schDay = arrSchDate(2)
    ZeroPrefix schMonth,2
    ZeroPrefix schDay,2
    schDate = schYear & "/" & schMonth & "/" & schDay
    schTime = date_time(1)
    ZeroPrefix schTime,8
    when = date_time(0) & " " & schTime
    WshShell.Run "schtasks /create /TR """ & how &""" /ST """ & schTime & """ /sd """ & schDate & """ /sc once /tn SHUTDOWN /RU "" "" ",0,true
end sub

sub DeleteShutdown
    set WshShell = WScript.CreateObject("WScript.Shell")
    WshShell.Run "schtasks /delete /tn SHUTDOWN /F",0,true
end sub

sub ZeroPrefix(strng,length)
    if len(strng) + 1 = length then
        strng = "0" & strng
    end if
end sub

main

sub main
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
        when = InputBox("When to shutdown your computer?" &  Chr(13) & Chr(10) & "Default is after 30 minutes ","Only 1 question :)",DateAdd("n",30,CDate(Hour(Now) & ":" & Minute(Now) & ":00")))
        if when <> "" then
            ret = MsgBox("You want to hibernate or shutdown computer at " & when &"?" &  Chr(13) & Chr(10) & "Press Yes to hibernate,No to shutdown :)",vbYesNoCancel or vbQuestion,"Are you sure")
            if ret = vbCancel then
                MsgBox "Shutdown cancelled",vbInformation,"Well"
            else
                select case ret
                    case vbYes
                        msg = "hibernated"
                        DirectoryOfScript=WScript.CreateObject("WScript.Shell").CurrentDirectory
                        how =  DirectoryOfScript & "\sleep.exe /h"
                    case vbNo
                        msg = "halted"
                        how = "shutdown -s -t 5 -c " & Time
                end select
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
        regEx.Pattern = ".*SHUTDOWN\s*(\d{2}:\d{2}:\d{2}).*"
        regEx.IgnoreCase = false
        regEx.Global = true
        set Matches = regEx.Execute(output)
        set Match = Matches(0)
        ShutdownScheduled = Match.SubMatches(0)
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
    if len(when) = 7 then   'schtasks requires "HH:MM:SS" format
        when = "0" & when
    end if
    WshShell.Run "schtasks /create /TR """ & how &""" /ST """ & when & """ /sc once /tn SHUTDOWN /RU "" "" ",0,true
end sub

sub DeleteShutdown
    set WshShell = WScript.CreateObject("WScript.Shell")
    WshShell.Run "schtasks /delete /tn SHUTDOWN /F",0,true
end sub

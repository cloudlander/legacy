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
        when = InputBox("When to shutdown your computer?" &  Chr(13) & Chr(10) & "Default is after 30 minutes ","Only 1 question :)",DateAdd("n",30,Time))
        if when <> "" then
            ret = MsgBox("You want to shutdown computer at " & when &"?",vbYesNo or vbQuestion,"Are you sure")
            if ret = vbYes then
                ScheduleShutdown(when)
            else
                MsgBox "Shutdown cancelled",vbInformation,"Well"
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
    if ret > 0 then
        dim regEx,Match,Matches
        set regEx = New RegExp
        regEx.Pattern = ".*SHUTDOWN\s*(\d{2}:\d{2}:\d{2}).*"
        regEx.IgnoreCase = false
        regEx.Global = true
        set Matches = regEx.Execute(output)
        set Match = Matches(0)
        ShutdownScheduled = Match.SubMatches(0)
    else
        ShutdownScheduled = ""
    end if
end function

sub ScheduleShutdown(when)
    set WshShell = WScript.CreateObject("WScript.Shell")
    WshShell.Run "schtasks /create /TR ""shutdown -s -t 5 -c " & Time & """ /ST """ & when & """ /sc once /tn SHUTDOWN /RU "" "" ",0,true
end sub

sub DeleteShutdown
    set WshShell = WScript.CreateObject("WScript.Shell")
    WshShell.Run "schtasks /delete /tn SHUTDOWN /F",0,true
end sub

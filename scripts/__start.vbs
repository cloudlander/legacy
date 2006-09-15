Set objArgs = WScript.Arguments
If objArgs.Count = 0 then
   MsgBox "No Programs to run!",vbInformation,"Well"
   WScript.Quit (1)
end if
RunProgram = objArgs(0)
ProgramArgs = objArgs(1)
For I = 2 to objArgs.Count - 1
   ProgramArgs = ProgramArgs & " " & objArgs(I)
Next
On Error Resume Next
ret = MsgBox("Start " & RunProgram & " with these arguments: " & ProgramArgs & "?",vbYesNo or vbQuestion,"Are you sure")
if ret = vbYes then
    set WshShell = WScript.CreateObject("WScript.Shell")
    WshShell.Run """" & RunProgram & """ """ & ProgramArgs & """",0,true
end if

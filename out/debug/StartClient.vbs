Set WshShell = CreateObject("WScript.Shell")
WScript.Sleep 4000
WshShell.Run chr(34) & ".\StartClient.bat"  & Chr(34), 0
Set WshShell = Nothing
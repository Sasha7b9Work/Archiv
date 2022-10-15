Set WshShell = CreateObject("WScript.Shell")
WshShell.Run chr(34) & "c:\Keil_v5\UV4\UV4 -c S8-53\Boot53.uvprojx -j0"  & Chr(34), 0
WshShell.Run chr(34) & "c:\Keil_v5\UV4\UV4 -b S8-53\Boot53.uvprojx -j0 -z -o Boot53.out"  & Chr(34), 0
Set WshShell = Nothing

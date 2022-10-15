@echo off

@echo Build All...

echo Compile Boot53   , 1 / 6 ... %TIME%

c:\Keil_v5\UV4\UV4 -b S8-53\Boot53.uvprojx -j0 -z -o Boot53.out
if ERRORLEVEL 1 goto BadBoot53

echo Compile Device53 , 2 / 6 ... %TIME%

:FineBoot53
c:\Keil_v5\UV4\UV4 -b S8-53\Device53.uvprojx -j0 -z -o Device53.out
if ERRORLEVEL 1 goto BadDevice53

echo Compile Boot54   , 3 / 6 ... %TIME%

:FineDevice53
c:\Keil_v5\UV4\UV4 -b S8-54\Boot54.uvprojx -j0 -z -o Boot54.out
if ERRORLEVEL 1 goto BadBoot54

echo Compile Device54 , 4 / 6 ... %TIME%

:FineBoot54
c:\Keil_v5\UV4\UV4 -b S8-54\Device54.uvprojx -j0 -z -o Device54.out
if ERRORLEVEL 1 goto BadDevice54

echo Compile Panel    , 5 / 6 ... %TIME%

:FineDevice54
c:\Keil_v5\UV4\UV4 -b Kornet\Panel.uvprojx -j0 -z -o Panel.out
if ERRORLEVEL 1 goto BadPanel

echo Compile Device   , 6 / 6 ... %TIME%

:FinePanel
c:\Keil_v5\UV4\UV4 -b Kornet\Device.uvprojx -j0 -z -o Device.out
if ERRORLEVEL 1 goto BadDevice

:FineDevice


exit

:BadBoot53
echo ERROR!!! Boot53 failed!!!
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop S8-53\Boot53.out -n1000 -x0 -y0
goto FineBoot53

:BadDevice53
echo ERROR!!! Device53 failed!!!
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop S8-53\Device53.out -n1000 -x0 -y0
goto FineDevice53

:BadBoot54
echo ERROR!!! Boot54 failed!!!
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop S8-54\Boot54.out -n1000 -x0 -y0
goto FineBoot54

:BadDevice54
echo ERROR!!! Device54 failed!!!
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop S8-54\Device54.out -n1000 -x0 -y0
goto FineDevice54

:BadPanel
echo ERROR!!! Panel failed!!!
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop Kornet\Panel.out -n1000 -x0 -y0
goto FinePanel

:BadDevice
echo ERROR!!! Device failed!!!
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop Kornet\Device.out -n1000 -x0 -y0
goto FineDevice

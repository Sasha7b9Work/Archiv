del release\ /s /q

set PATH=d:\Sasha\Qt4\bin\;"e:\Program Files\Microsoft Visual Studio 9.0\VC\bin\"
set QMAKESPEC=win32-msvc2008
rem qmake -project -config "shared release"
rem qmake Device129Multi.pro
qmake -win32 -tp vc Device129Multi.pro
pause
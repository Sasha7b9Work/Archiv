del release\ /s /q

set PATH=d:\Sasha\Qt4\bin\;"e:\Program Files\Microsoft Visual Studio 9.0\VC\bin\"
set QMAKESPEC=win32-msvc2008
rem qmake -project -nopwd -config "shared qt release"
rem qmake Registrator.pro
qmake -win32 -tp vc Registrator.pro
pause
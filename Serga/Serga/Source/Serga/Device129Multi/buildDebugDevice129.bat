del debug\ /s /q

set PATH=d:\Sasha\Qt4\bin\;"e:\Program Files\Microsoft Visual Studio 9.0\VC\bin\"
set QMAKESPEC=win32-msvc2008
qmake -project -nopwd -config "shared console qt warn_on release"
qmake Device129Multi.pro
qmake -win32 -tp vc Device129Multi.pro
pause
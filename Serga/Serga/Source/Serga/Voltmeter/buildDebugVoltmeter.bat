del debug\ /s /q

set PATH=d:\Sasha\Qt4\bin\;"e:\Program Files\Microsoft Visual Studio 9.0\VC\bin\"
set QMAKESPEC=win32-msvc2008

rem qmake -project -config "shared console qt warn_on release"

rem qmake -project -nopwd -config "shared console qt warn_on release"

rem qmake Voltmeter.pro

rem qmake -win32 -tp vc Voltmeter.pro

pause
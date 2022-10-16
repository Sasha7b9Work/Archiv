del release\ /s /q

set PATH=d:\Sasha\Qt4\bin\;"e:\Program Files\Microsoft Visual Studio 9.0\VC\bin\";"D:\Sasha\Projects\Serga\Source\Serga\Analyzer\"
set QMAKESPEC=win32-msvc2008
rem qmake -project -config "shared qt release"
rem qmake Analyzer.pro
qmake -win32 -tp vc Analyzer.pro
pause
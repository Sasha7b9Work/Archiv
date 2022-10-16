del release\ /s /q

set PATH=c:\Qt482\bin\;"c:\Program Files\Microsoft Visual Studio 9.0\VC\bin\"
set QMAKESPEC=win32-msvc2008
qmake -project -config "shared console qt warn_on release"
qmake Replacer.pro
qmake -tp vc Replacer.pro
pause
set PATH=D:\Sasha\Qt4_Release\bin\;"e:\Program Files\Microsoft Visual Studio 9.0\VC\bin\"
set QMAKESPEC=win32-msvc2008
qmake -project -config release
qmake Voltimetr.pro
qmake -win32 -tp vc Baikal.pro
pause
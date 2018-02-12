@pushd ..\..\..\..\blds
@call make.bat %1 %2 %3 %4 %5 %6
set el=%ERRORLEVEL%
@popd
exit /b %el%
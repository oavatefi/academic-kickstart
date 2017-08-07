@echo off
set PATHSAV=%PATH%
set ERRORLEVEL=

if not exist mingw (
    mkdir mingw
    xcopy \\BIE-PVCSBS-01\Tool\mingw mingw /e /s /f /y
)

if not exist ..\tool\IntegrationTests\Valeo\RuntimeWorstCaseTest\RuntimeDocumentationTool\tools (
    mkdir ..\tool\IntegrationTests\Valeo\RuntimeWorstCaseTest\RuntimeDocumentationTool\tools
    mkdir ..\tool\IntegrationTests\Valeo\RuntimeWorstCaseTest\RuntimeDocumentationTool\tools\gnuplot
    xcopy /V /E /F /Y \\BIE-PVCSBS-01\Tool\gnuplot ..\tool\IntegrationTests\Valeo\RuntimeWorstCaseTest\RuntimeDocumentationTool\tools\gnuplot
    xcopy /V /E /F /Y \\BIE-PVCSBS-01\Tool\saxon\saxonhe9-5-1-2j ..\tool\IntegrationTests\Valeo\RuntimeWorstCaseTest\RuntimeDocumentationTool\tools\
    xcopy /V /E /F /Y \\BIE-PVCSBS-01\Tool\xerces ..\tool\IntegrationTests\Valeo\RuntimeWorstCaseTest\RuntimeDocumentationTool\tools\
)

@REM adjust the following path to the directory of your make program and you unix tools
set PATH=.\mingw\bin;%PATH%

set TUAM_SVN_REV=1721
set TUAM_SVN_URL=http://tua1-svn.vnet.valeo.com/svn/APGEN3/
set LM_LICENSE_FILE=4736@BIE2-FLEXLM1;4736@BIE2-FLEXLM2;4736@BIE2-FLEXLM3

set MAKEFILE=build_configs/makefile
set BUILDLOG=logs/build.log
if NOT EXIST build_configs/%1.mk goto nomk
set MAKEFILE=build_configs/%1.mk
set BUILDLOG=logs/build_%1.log
shift
:nomk

IF NOT EXIST logs (
	mkdir logs
)

if not exist trunk_daimler_%TUAM_SVN_REV%\software\sw\vayu\dependencies.txt (
    svn export -r %TUAM_SVN_REV% %TUAM_SVN_URL%/trunk_daimler/software/sw/vayu/dependencies.txt trunk_daimler_%TUAM_SVN_REV%\software\sw\vayu\dependencies.txt
)

@REM The following line is for speeding up dependency generation. Remove the "::" if you want to
@REM use it. Adjust the jobs limit (-j) as you like. Be aware that no errors are reported to build
@REM log because the output is scrambled (make uses multiple jobs in parallel).
:: make.exe -j5 -f %MAKEFILE% deps

@REM The following line executes the make program. Change this if your program is named differently.
@REM The stdout and stderr of the build process are joined together to stdout (2>&1). The "tee"
@REM program is used to store the build messages in the file "build.log" and print them to stdout.
make.exe -f %MAKEFILE% objects.txt %1 %2 %3 %4 %5 %6 %7 %8 %9 "SVN_REV=%TUAM_SVN_REV%" 2>&1 | tee -a %BUILDLOG%
make.exe -f %MAKEFILE% include_paths.txt %1 %2 %3 %4 %5 %6 %7 %8 %9 "SVN_REV=%TUAM_SVN_REV%" 2>&1 | tee -a %BUILDLOG%
make.exe -f %MAKEFILE% %1 %2 %3 %4 %5 %6 %7 %8 %9 "SVN_REV = %TUAM_SVN_REV%" 2>&1 | tee -a %BUILDLOG%

set PATH=%PATHSAV%

if %ERRORLEVEL% equ 0 (
  goto End
) else (
  goto Error
)


:Error
echo ************************************************************************
echo error level is %ERRORLEVEL%
echo ************************************************************************
exit /b 1


:End
exit /b 0
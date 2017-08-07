@echo off

if not exist mingw (
    mkdir mingw
    xcopy \\BIE-PVCSBS-01\Tool\mingw mingw /e /s /f /y
)

echo. > build_all_failed.txt
set fail=0
set /a total=0
for /F %%f in ('"mingw\bin\ls.exe build_configs/*mk"') do set /a total+=1
set /a current=0
Setlocal EnableDelayedExpansion
for /F %%f in ('"mingw\bin\ls.exe build_configs/*mk"') do (
	call make.bat %%~nf %1 %2 %3 %4 %5 %6 %7 %8 %9
	IF NOT EXIST build_configs/%%~nf/%%~nf.elf (
        set fail=1
		echo %%~nf >> build_all_failed.txt
	)
    set /a current += 1
    echo !current! ^| %total% > build_all_progress.txt
)

if %fail% equ 1 goto error
exit /b 0

:error
exit /b 1
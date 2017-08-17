@echo off

set omit_root=
if [%1] == [--omit-root] (
    set  omit_root=TRUE
    shift
)
if [%1] == [] goto wrong_usage
if [%2] == [] goto wrong_usage
if [%3] == [] goto wrong_usage

git rev-parse %1^^^^0 > nul 2>&1

if ERRORLEVEL 1 (
    echo Error: Cannot find hash %1!
    goto co_fail
)
    
for /f %%i in ('git rev-parse %1^^^^0') do (
    if ERRORLEVEL 1 (
        goto co_fail
    )
    set hash=%%i
)

setlocal enabledelayedexpansion
For /R %2 %%G IN (*) do (
    set filepath=%%~pG
    for /f %%i in ('echo ^| set ^/p^=%2^| perl -ne "use Data::Dumper; @dirs = split /\\/; print $dirs[-1];"') do (
        set directory_part=%%i
    )
    
    if [%omit_root%] == [TRUE] (
        for /f %%i in ('echo^|set ^/p^=!filepath!^| perl -pe "s/.*!directory_part!(.*)/\1/g" ^| perl -pe "s/\\/\//g" ') do (
            set sub_path=%%i
        )
    ) else (
        for /f %%i in ('echo^|set ^/p^=!filepath!^| perl -pe "s/.*(!directory_part!.*)/\1/g" ^| perl -pe "s/\\/\//g" ') do (
            set sub_path=%%i
        )
    )
    curl --netrc-file my-nexus-creds -k --upload-file %%G -o http.log -O https://forge-nexus.vnet.valeo.com/nexus/content/sites/%3/%1_%hash%/!sub_path!/%%~nxG
)
setlocal disabledelayedexpansion
exit /b 0

:co_fail
exit /b 1

:wrong_usage
echo Usage: please remane TEMPLATE_my-nexus-creds to my-nexus-creds and adapt it with your user and password
echo Usage: dir2nexus.cmd [--omit-root] ^<tag^> ^<directory^> ^<reponame^>
exit /b 1
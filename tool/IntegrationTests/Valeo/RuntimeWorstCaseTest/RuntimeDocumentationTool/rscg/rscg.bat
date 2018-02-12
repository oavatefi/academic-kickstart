@echo off

if "%1" == "" goto usage
if "%2" == "" goto usage

set script_path=%~dp0
set config_path=%~dp1
set "config_path_nix=%config_path:\=/%"

java -cp "%script_path%\..\tools\xercesSamples.jar;%script_path%\..\tools\xercesImpl.jar" jaxp.SourceValidator -i %1 -a "%script_path%\rscc-schema.xsd" 2> xerces_out.log
type xerces_out.log | findstr /C:"[Error]" && del xerces_out.log && exit /B 1
del xerces_out.log
java -jar "%script_path%\..\tools\saxon9he.jar" %1 "%script_path%\xsl\rscc2cmd.xsl" > commands.bat
call commands.bat
java -jar "%script_path%\..\tools\saxon9he.jar" %1 "%script_path%\xsl\rscc2rsct.xsl" config_path=%config_path_nix% > template.rsct
java -jar "%script_path%\..\tools\saxon9he.jar" template.rsct "%script_path%\xsl\rsct2rsc.xsl" > %2

del commands.bat
del template.rsct

exit /B 0

:usage
echo "rscg.bat <runtime_score_card_config.xml> <runtime_score_card_out.html>"
exit /B 1

@echo off

mkdir iSystemAutomation
xcopy /V /E /F /Y \\bie-pvcsbs-01\Tool\iSystemAutomation iSystemAutomation
mkdir ..\..\IntegrationTests\Valeo\RuntimeWorstCaseTest\RuntimeDocumentationTool\tools
mkdir ..\..\IntegrationTests\Valeo\RuntimeWorstCaseTest\RuntimeDocumentationTool\tools\gnuplot
xcopy /V /E /F /Y \\bie-pvcsbs-01\Tool\gnuplot ..\..\IntegrationTests\Valeo\RuntimeWorstCaseTest\RuntimeDocumentationTool\tools\gnuplot
xcopy /V /E /F /Y \\bie-pvcsbs-01\Tool\saxon\saxonhe9-5-1-2j ..\..\IntegrationTests\Valeo\RuntimeWorstCaseTest\RuntimeDocumentationTool\tools\
xcopy /V /E /F /Y \\bie-pvcsbs-01\Tool\xerces ..\..\IntegrationTests\Valeo\RuntimeWorstCaseTest\RuntimeDocumentationTool\tools\
pause

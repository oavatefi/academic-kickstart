<?xml version="1.0" encoding="iso-8859-1"?>
<xsl:stylesheet
    version="2.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:html="http://www.w3.org/1999/xhtml"
    xmlns="http://www.w3.org/1999/xhtml"
    exclude-result-prefixes="html">
    <xsl:output
        method="text"
        doctype-system="http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd"
        doctype-public="-//W3C//DTD XHTML 1.1//EN"
	indent="yes"
	omit-xml-declaration="no" />
	<xsl:template match="rscc">
if NOT Exist %config_path%\<xsl:value-of select="tmp-path"/> (mkdir %config_path%\<xsl:value-of select="tmp-path"/>)
		<xsl:apply-templates select="./criteria/criterion" /><xsl:text>
</xsl:text>
	</xsl:template>
	<xsl:template match="criterion">
perl "%script_path%\asc2gpl.pl" --can-signal-path=<xsl:value-of select="can-signal-path" /> <xsl:text> </xsl:text>--can-dbc=%config_path%/<xsl:value-of select="can-dbc-file" /><xsl:text> </xsl:text>%config_path%/<xsl:value-of select="../../can-trace-file" /> &gt; %config_path%/<xsl:value-of select="../../tmp-path"/>/out.dat
IF %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%
"%script_path%\..\tools\gnuplot\bin\gnuplot" -e "output_path=\"%config_path_nix%/<xsl:value-of select="../../tmp-path"/>\"" "%script_path%\gpl_std_cmds_pre.gpl" -e "<xsl:if test="y-label">set ylabel \"<xsl:value-of select="y-label" />\";</xsl:if>set title \"<xsl:value-of select="@name" />\" ;plot output_path . '/out.dat' lt rgb \"blue\" with lines title \"<xsl:value-of select="@name" />\"<xsl:if test="max-value">,<xsl:value-of select="max-value" /> lt rgb \"red\" title \"Max Value\"</xsl:if><xsl:if test="min-value">,<xsl:value-of select="min-value" /> lt rgb \"red\" title \"Min Value\"</xsl:if>" &gt; %config_path_nix%/<xsl:value-of select="../../tmp-path"/>/<xsl:value-of select="@id" />.svg 
IF %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%
</xsl:template>
</xsl:stylesheet>

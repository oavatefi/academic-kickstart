<?xml version="1.0" encoding="iso-8859-1"?>
<xsl:stylesheet
    version="2.0"
    xmlns:xs="http://www.w3.org/2001/XMLSchema"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:html="http://www.w3.org/1999/xhtml"
    xmlns="http://www.w3.org/1999/xhtml"
    exclude-result-prefixes="html">
    <xsl:param name="config_path" as="xs:string" required="yes"/>
    <xsl:output
        method="xml"
        doctype-system="http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd"
        doctype-public="-//W3C//DTD XHTML 1.1//EN"
	indent="yes"
	omit-xml-declaration="no" />
	<xsl:template match="rscc">
		<html>
			<head>
				<title><xsl:value-of select="./meta-info/project-name" /> - Runtime Score Card</title>
				<style type="text/css">
				#header {
			        background-color: lightgrey;
			        border-style: solid;
			        padding: 2pt;
			        width: 690pt;
				}
				#image_desc {
					vertical-align: top;
				}
				#criterion_box {
					margin-top: 20pt;
					padding: 5pt;
					border-style: solid;
					border-width: thin;
					width: 680pt;
				}
				.page_break {
					page-break-after: always;
				}
				.info {
				        font-weight: bold;
				}
				.spacer {
				        padding-right: 70pt
				}
				</style>
			</head>
			
			<body>
				<table id="header">
					<tr>
						<td class="info">Project:</td>
						<td class="spacer"><xsl:value-of select="./meta-info/project-name" /></td>
						<td class="info">Measured by:</td>
						<td><xsl:value-of select="./meta-info/author" /></td>
					</tr>
					<tr>
						<td class="info">Baseline:</td>
						<td class="spacer"><xsl:value-of select="./meta-info/baseline" /></td>
						<td class="info">Date:</td>
						<td><xsl:value-of select="current-date()" /></td>
					</tr>
					<tr>
						<td class="info">P2-Baseline:</td>
						<td class="spacer"><xsl:value-of select="./meta-info/p2-baseline" /></td>
						<td class="info">VS6-Scenario:</td>
						<td><xsl:value-of select="./meta-info/vs6-scenario" /></td>
					</tr>
				</table>
				<xsl:apply-templates select="./criteria/criterion" />
			</body>
		</html>
	</xsl:template>
	<xsl:template match="criterion">
	<div id="criterion_box">
		<xsl:if test="(position() mod 2 = 0) and not(position() = last())"><xsl:attribute name="class">page_break</xsl:attribute></xsl:if>
		<span id="image_desc"><xsl:value-of select="description" /></span>
		<div><svg><xsl:value-of select="$config_path" /><xsl:value-of select="../../tmp-path" />/<xsl:value-of select="@id" />.svg</svg></div>
	</div>
	</xsl:template>
</xsl:stylesheet>

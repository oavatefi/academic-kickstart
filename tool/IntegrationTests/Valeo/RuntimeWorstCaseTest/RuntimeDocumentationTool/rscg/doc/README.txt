RSCG - Runtime Score Card Generator
===================================

1.) Introduction
----------------
The RSCG tool transforms a CAN-trace in ASC format and an XML configuration
file into a nice looking HTML documentation. It is possible to define several
runtime criteria within the XML configuration file. For each criterion a chart
will be outputted within the resulting HTML documentation file.

2.) Running the script
----------------------
The script (batch file) will generate the HTML documentation. It must be called
in the following way:
    rscg.bat <in.xml> <out.html>
where in.xml is the XML configuration file and out.html is the resulting
documentation.

3.) The XML configuration file language
---------------------------------------
The following is a skeleton of the configuration file.
    <?xml version="1.0" encoding="UTF-8"?>
    <rscc>
        <meta-info>
            <project-name>Application projects name</project-name>
            <baseline>projects baseline</baseline>
            <p2-baseline>application projects P2 baseline</p2-baseline>
            <author>The authors name</author>
            <vs6-scenario>Explaination of the VS6 parking scenario</vs6-scenario>
        </meta-info>
        <can-trace-file>The CAN trace file in ASC format</can-trace-file>
        <tmp-path>The path where to store temporary generated files</tmp-path>
        <criteria>
            <criterion id="A unique ID over all criteria" name="Criterions name">
                <can-message-id>1BFC520C</can-message-id>
                <can-message-byte-number>0</can-message-byte-number>
                <can-mux-id>2</can-mux-id>
                <can-physical-factor>1</can-physical-factor>
                <min-value>3000</min-value>
                <max-value>100</max-value>
                <y-label>Task Load / percent</y-label>
                <description>Task load &lt; 100 %</description>
            </criterion>
            ...
            <criterion id="A unique ID over all criteria" name="Criterions name">
                ...
            </criterion>
        </criteria>
    </rscc>
The following tags are optional:
    * can-physical-factor - defaults to 1
    * min-value, max-value
    * y-label
    * can-mux-id - defaults to no-mux
The min-value, max-value is displayed in the HTML as red constants within the
criterions chart. They refer to the maximum and minimum allowed values of the
defined criterion. If they are omitted no min / max constant is displayed in
the outcoming chart.

4.) The CAN trace file
----------------------
The CAN trace file be in ASC format. It can be saved by configuring a CAN
logging block in CANoe.
The signal which shall be documented itself must be within one single byte
within the specified CAN message within the trace. Signals longer than one
byte are not supported.
The handling of multiplexed CAN messages are currently not supported.

5.) Internals
-------------
The script uses GNUplot to generate the runtime chart. The runtime charts are
represented as SVG-images embedded into the outcoming HTML. The tmp-path also
contains the intermediate images as native SVG files.

rem @echo off
setlocal enabledelayedexpansion

set descriptions[1]=TC1: Walls Left And Right \/ Moving Objects Ahead And Behind
set descriptions[2]=TC2: Symmetric Perpendicular Slots Left And Right \/ Moving Objects Ahead And Behind
set descriptions[3]=TC3: Symmetric Alternating Perpendicular And Parallel Slots Left And Right \/ Moving Objects Ahead And Behind
set descriptions[4]=TC4: Parking Into Garage Object Following
set descriptions[5]=TC5: Parallel parking while objects overtake
set descriptions[6]=TC6: Perpendicular parking while objects overtake
set descriptions[7]=TC7: Park Slots In Circle
set descriptions[8]=TC8: Inner And Outer Curb In Circle
set descriptions[9]=TC9: OBJD Worst Case Test  for ULS
set descriptions[10]=TC10: Symmetric Alternating Perpendicular And Parallel Slots Left And Right \/ Moving Objects Ahead And Behind with Cam Objects

set subs_cmd="s/(.*<vs6-scenario>).*(<\/vs6-scenario>.*)/\1!descriptions[%1]!\2/g"
type rscc_DAS.xml | perl -pe "s/(.*<can-trace-file>).*(<\/can-trace-file>.*)/\1Traces\\TraceRSC_TC%1.asc\2/g" >tmp.xml
type tmp.xml | perl -pe !subs_cmd! > tmp1.xml

if not exist .\RSCOutput\nul md .\RSCOutput

setlocal disabledelayedexpansion
rem @echo off
rscg.bat tmp1.xml RSCOutput\\%2


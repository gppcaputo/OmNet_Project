#!/bin/bash

# Read filenames from input.txt file
while read filename
do
  # Extract name without extension
  filename=$(echo "$filename" | sed 's/\-#0.sca$//')

  filename2="${filename}-#*"
  
  # Run scavetool on each file
  opp_scavetool.exe x "$filename2" -o "$filename.json"
done < list.txt

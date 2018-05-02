import sys
import os

if len(sys.argv) < 2:
    print "This script takes a file with a list of audio files and youtube video ID's, as well as titles, see exampleSounds.txt"
    sys.exit()

f = open(sys.argv[1])
for line in f:
    if line == '':
        continue
    song = line.split(':')
    inputFile = song[0].strip()
    outputFile = song[1].strip()
    if '.' not in inputFile:
        os.system("youtube-dl " + inputFile +
                  " -q -o \"yt-temp.%(ext)s\" -x --audio-format wav")
        inputFile = "yt-temp.wav"
    print "Converting", outputFile, "to raw low quality audio... ",
    os.system(
        "sox " +
        inputFile +
        " -r 8000 -b 8 -c 1 -e unsigned-integer " +
        outputFile +
        ".wav vol $(sox " +
        inputFile +
        " -n stat -v 2>&1)")
    print "Finished"

os.system("rm yt-temp.wav")

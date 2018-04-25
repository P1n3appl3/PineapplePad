import sys
import os

if len(sys.argv) < 2:
    print "convert.py takes a list of audio files and youtube video ID's as arguments"
    sys.exit()

songNum = 0
for song in sys.argv[1:]:
    inputFile = song
    outputFile = "output" + str(songNum)
    if '.' not in song:
        os.system("youtube-dl " + song +
                  " -o \"yt-temp.%(ext)s\" -x --audio-format wav")
        inputFile = "yt-temp.wav"
    print "Converting to raw low quality audio... ",
    os.system(
        "sox " +
        inputFile +
        " -r 8000 -b 8 -c 1 -e unsigned-integer badq_" +
        outputFile +
        ".wav vol $(sox " +
        inputFile +
        " -n stat -v 2>&1)")
    print "Finished"
    songNum += 1

import sys
import os

if len(sys.argv) < 2:
    print "Usage: python wavconvert.py input_file(s)"
    sys.exit()

for song in sys.argv[1:]:
    print "Converting", song, "...",
    os.system("sox " + song + " -r 4000 -b 8 -c 1 -e unsigned-integer badq_" +
        song.split('.')[0] + ".raw vol $(sox " + song + " -n stat -v 2>&1)")
    print "Finished"

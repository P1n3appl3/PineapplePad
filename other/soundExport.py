import sys

if len(sys.argv) == 1:
    print "give me a file"

inFile = open(sys.argv[1])
outFile = open("sound.h", 'w')

outFile.write("#include <stdint>\nconst uint8_t testSound[4000] = {")

limit = 11000
inFile.read(10000)
byte = inFile.read(1)
while byte != '' and limit > 0:
    outFile.write( str(int(ord(byte))) + ",")
    byte = inFile.read(1)
    limit -= 1

outFile.write("};\n")
inFile.close()
outFile.close()

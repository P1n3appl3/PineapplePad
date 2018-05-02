import sys

if len(sys.argv) == 1:
    print "give me a file"
    sys.exit()

outFile = open("sfx.h", 'w')
outFile.write("#include <stdint.h>\n")

for i in sys.argv[1:]:
    outFile.write("const uint8_t " + i.split('.')[0] + "[] = {\n")
    inFile = open(i)

    limit = 20000
    #  inFile.read(10000) # silence at the beginning of sound
    byte = inFile.read(1)
    while byte != '' and limit > 0:
        outFile.write( str(int(ord(byte))) + ",")
        byte = inFile.read(1)
        limit -= 1
    inFile.close()
    outFile.write("};\n")

outFile.close()

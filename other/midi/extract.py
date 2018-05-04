import json
import sys

# to run: python3 extract.py <infile>.json <name>

infile = sys.argv[1]
name = sys.argv[2]
track = 1

f = open(name + '.h', 'w')
f.write('//' + name + '\n')
f.write("#include <stdint.h>\n\n")
f.write("const uint32_t pitch[] = {")

music = json.load(open(infile))
divisors = []
durations = []
velocities = []
time = 0
restVelocity = '0'
restDuration = '100'
restdivisor = '0'
for note in music["tracks"][track]["notes"]:
    noteTime = note["time"]
    if(time < noteTime):
        divisors.append(restdivisor)
        velocities.append(restVelocity)
        durations.append(str(int(100 * float("{0:.2f}".format(noteTime - time)))))
        time = noteTime
    velocity = str(int(100 * float("{0:.2f}".format(note['velocity']))))
    duration = str(int(100 * float("{0:.2f}".format(note['duration']))))
    frequency = (440 / 32) * (pow(2, ((note["midi"] - 9) / 12)))
    divisor = int(80e6 / (64 * frequency))
    divisors.append(str(divisor))
    durations.append(duration)
    velocities.append(velocity)

    divisors.append('0')
    durations.append('1')
    velocities.append('0')
divisors.append('0')
durations.append('1')
velocities.append('0')

print(divisors)
f.write(','.join(divisors))
f.write('};\n\nconst uint16_t dur[] = {')
f.write(','.join(durations))
f.write('};\n\nconst uint8_t velocity[] = {')
f.write(','.join(velocities))
f.write('};')

f.close()

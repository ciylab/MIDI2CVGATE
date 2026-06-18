import mido
import time

port = mido.get_output_names()[1]
outport = mido.open_output(port)
d = mido.bpm2tempo(120) / 24
while True:
    msg = mido.Message('clock')
    outport.send(msg)
    time.sleep(d/1000000.0)

outport.close()

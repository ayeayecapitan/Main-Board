# open UDP socket on 1
import random
from socket import *
import struct
import time

gcs_addr = ("172.16.18.182", 2000)
main_board_addr = ("172.16.18.181", 1000)

sock = socket(AF_INET, SOCK_DGRAM)
sock.bind(gcs_addr)
sock.settimeout(0)



# # // Ground Control Station (GCS) command

# namespace probe
# {
#   const uint8_t COUNT = 4;
# }

# enum class State : uint8_t
# {
#     UNSET = 0,
#     OFF,
#     ON
# };

# struct GroundCommand
# {
#     uint64_t timestamp_us = 0;
#     probe::State probe_desired_state[probe::COUNT];
# };

# https://docs.python.org/3/library/struct.html
# We pack the data into a binary format which matches the main board's struct layout




while True:
    now_us = int(time.time() * 1e6)
    command = struct.pack('<QBBBB', now_us, 1, 1, 1, 1)
    sock.sendto(command, main_board_addr)
    time.sleep(3)
    # check if
    try:
        data, _ = sock.recvfrom(22)
        # print data size
        print(len(data))

        # should have 22 bytes of data
        state = struct.unpack('<QfffBB', data)
        print(state)
        print(f"Timestamp: {state[0]}")
        print(f"Altitude: {state[1]}")
        print(f"Latitude: {state[2]}")
        print(f"Longitude: {state[3]}")
        print(f"Satellite numbers: {state[4]}")
        print(f"Temperature: {state[5]}")

    except timeout:
        continue
    except BlockingIOError:
        continue
    
    

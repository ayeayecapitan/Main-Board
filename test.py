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



# // Ground Control Station (GCS) command
# struct GcsCommand {
#     static const uint8_t TEST = 0;

#     uint64_t timestamp_ns;
#     uint8_t type;
#     uint8_t param;
# };

# // State which gcs receives from main board
# struct MainBoardState {
#     uint64_t timestamp_ns;

#     float altitude = 0;
#     float latitude = 0;
#     float longitude = 0; 
#     uint8_t satellite_numbers = 0;
#     uint8_t temperature;
# };


# https://docs.python.org/3/library/struct.html
# We pack the data into a binary format which matches the main board's struct layout

command = struct.pack('QBB', 123, 1, 2)

while True:
    sock.sendto(command, main_board_addr)
    
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
    
    time.sleep(3)

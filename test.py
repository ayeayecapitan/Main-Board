# open UDP socket on 1
import random
from socket import *
import struct
import threading
import time

gcs_addr = ("10.0.0.100", 2000)
main_board_addr = ("10.0.0.181", 1000)

sock = socket(AF_INET, SOCK_DGRAM)
sock.bind(gcs_addr)
sock.settimeout(0)



# # // Ground Control Station (GCS) command

# namespace probe
# {
#   const uint8_t COUNT = 4;
# }

# namespace temperature
# {
#   const uint8_t COUNT = 8;
# }

# enum class State : uint8_t
# {
#     UNSET = 0,
#     OFF,
#     ON
# };

# struct SystemState
# {
#     uint64_t timestamp_us = 0;

#     struct SensorsData {
#         float temperatures_c[temperature::COUNT] = {0};
#         float pressure_pa = 0;

#         struct Gps
#         {
#             float altitude = 0;
#             float latitude = 0;
#             float longitude = 0;
#             uint8_t satellites = 0;
#         } gps;

#         struct PowerSupply
#         {
#             float voltage = 0;
#             float current = 0;
#             float power = 0;
#         } power_supply;
#     } sensors;

#     struct Devices {
#         valve::state valve_state[valve::COUNT] = { valve::state::UNSET };
#         pump::State pump_state[pump::COUNT] = { pump::State::UNSET };
#     } devices;
# };


# struct GroundCommand
# {
#     uint64_t timestamp_us = 0;
#     probe::State probe_desired_state[probe::COUNT];
# };

# https://docs.python.org/3/library/struct.html
# We pack the data into a binary format which matches the main board's struct layout

probe_desired_state = [1, 1, 1, 1]

def send_command():
    while True:
        now_us = int(time.time() * 1e6)
        command = struct.pack('<Q4B', now_us, *probe_desired_state)
        sock.sendto(command, main_board_addr)
        time.sleep(1)

def receive_state():
    data, _ = sock.recvfrom(79)
    state = struct.unpack('<Q8f1f3f1B3f8B2B', data)
    return state

send_thread = threading.Thread(target=send_command)
send_thread.start()

while True:
    # Get commant from user in form [probe_index] [ON/OFF]
    command = input("Enter command: ")
    if command == "":
        continue
    command = command.split()
    probe_index = int(command[0]) - 1
    if probe_index < 0 or probe_index > 3:
        print("Invalid probe index")
        continue
    probe_state = 2 if command[1] == "ON" else 1
    probe_desired_state[probe_index] = probe_state
    
    

    # check if
    
    # try:
    #     state = receive_state()
    #     print(state)

    # except timeout:
    #     continue
    # except BlockingIOError:
    #     continue
    
    

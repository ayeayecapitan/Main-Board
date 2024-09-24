# open UDP socket on 1
from dataclasses import dataclass, field
import random
from socket import *
import struct
import threading
import time
from enum import Enum
from typing import Callable, List
import numpy


# file://./src/shared/data.hpp
class ProbeState(Enum):
    UNSET = 0
    OFF = 1
    ON = 2
    
class ValveState(Enum):
    UNSET = 0
    CLOSED = 1
    OPEN = 2
    INCOMPLETE = 3
    ERROR = 4
    
class PumpState(Enum):
    UNSET = 0
    OFF = 1
    ON = 2


@dataclass
class Command:
    timestamp_us: int = 0
    spe_probes_desired_states: List[ProbeState] = field(default_factory=lambda: [ProbeState.UNSET] * 4)
    chemical_probe_desired_state: ProbeState = ProbeState.UNSET

    def pack(self):
        return struct.pack('<Q4BB', self.timestamp_us, *[s.value for s in self.spe_probes_desired_states], self.chemical_probe_desired_state.value)

@dataclass
class SystemState:
    timestamp_us: int = 0
    
    @dataclass
    class SensorsData:
        temperatures_c: List[float] = field(default_factory=lambda: [0] * 8)
        pressure_pa: float = 0
        
        @dataclass
        class Gps:
            altitude: float = 0
            latitude: float = 0
            longitude: float = 0
            satellites: int = 0
        gps : Gps = field(default_factory=Gps)
        
        @dataclass
        class PowerSupply:
            voltage: float = 0
            current: float = 0
            power: float = 0
        power_supply : PowerSupply = field(default_factory=PowerSupply)
        
    sensors : SensorsData = field(default_factory=SensorsData)
    
    @dataclass
    class Devices:
        valve_state: List[ValveState] = field(default_factory=lambda: [ValveState.UNSET] * 4)
        pump_state: List[PumpState] = field(default_factory=lambda: [PumpState.UNSET] * 4)
    devices : Devices = field(default_factory=Devices)

    @staticmethod
    def unpack(data) -> 'SystemState':
        state = SystemState()
        
        unpacked = struct.unpack('<Q8f1f3f1B3f8B2B', data)
        state.timestamp_us = unpacked[0]
        state.sensors.temperatures_c = unpacked[1:9]
        state.sensors.pressure_pa = unpacked[9]
        
        state.sensors.gps.altitude = unpacked[10]
        state.sensors.gps.latitude = unpacked[11]
        state.sensors.gps.longitude = unpacked[12]
        state.sensors.gps.satellites = unpacked[13]
        
        state.sensors.power_supply.voltage = unpacked[14]
        state.sensors.power_supply.current = unpacked[15]
        state.sensors.power_supply.power = unpacked[16]
        
        state.devices.valve_state = [ValveState(v) for v in unpacked[17:21]]
        state.devices.pump_state = [PumpState(p) for p in unpacked[21:25]]
        
        return state


class MainBoardInterface:    
    def __init__(self, on_new_state_callback: Callable[[SystemState], None]):
        self.__gcs_addr = ("172.16.18.182", 2000)
        self.__main_board_addr = ("172.16.18.181", 1000)

        self.__sock = None
        
        self.__command = None
        self.__on_new_state_callback = on_new_state_callback
        
        self.__should_stop = False
        self.__send_thread = None
        self.__receive_thread = None
        
    def start(self):
        self.__sock = socket(AF_INET, SOCK_DGRAM)
        self.__sock.bind(self.__gcs_addr)
        self.__sock.settimeout(0.1)
        self.__should_stop = False
        self.__send_thread = threading.Thread(target=self.__send_command)
        self.__send_thread.start()
        self.__receive_thread = threading.Thread(target=self.__receive_state)
        self.__receive_thread.start()
        
    def stop(self):
        self.__should_stop = True
        if self.__send_thread is not None:
            self.__send_thread.join()
        if self.__receive_thread is not None:
            self.__receive_thread.join()
        self.__sock.close()
        
    def set_command(self, command: Command):
        self.__command = command
        
    def __del__(self):
        self.stop()
        
    def __send_command(self):
        while not self.__should_stop:
            if self.__command is not None:
                self.__command.timestamp_us = int(time.time() * 1e6)
                self.__sock.sendto(self.__command.pack(), self.__main_board_addr)
                # print(f"Sending: {self.__command}")
            time.sleep(1)
        
    def __receive_state(self):
        while not self.__should_stop:
            try:
                data, _ = self.__sock.recvfrom(79)
                state = SystemState.unpack(data)
                self.__on_new_state_callback(state)
            except timeout:
                continue
            except BlockingIOError:
                continue
    

def main():
    main_board_interface = MainBoardInterface(lambda state: print(state))
    main_board_interface.start()
    command = Command()

    import keyboard

    def toggle_spe(index: int):
        command.spe_probes_desired_states[index] = ProbeState.ON if command.spe_probes_desired_states[index] == ProbeState.OFF else ProbeState.OFF
        main_board_interface.set_command(command)
        
    def toggle_chemical_probe():
        command.chemical_probe_desired_state = ProbeState.ON if command.chemical_probe_desired_state == ProbeState.OFF else ProbeState.OFF
        main_board_interface.set_command(command)

    keyboard.on_press_key('1', lambda _: toggle_spe(0))
    keyboard.on_press_key('2', lambda _: toggle_spe(1))
    keyboard.on_press_key('3', lambda _: toggle_spe(2))
    keyboard.on_press_key('4', lambda _: toggle_spe(3))
    keyboard.on_press_key('c', lambda _: toggle_chemical_probe())

    keyboard.wait('q')
    print("done")
    keyboard.unhook_all()
    main_board_interface.stop()
    
if __name__ == "__main__":
    main()
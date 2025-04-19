# Python script to get the physical MAC address of the device.

import uuid

mac = ':'.join(['{:02x}'.format((uuid.getnode() >> i) & 0xff) 
                for i in range(0, 8*6, 8)][::-1])

print(mac)
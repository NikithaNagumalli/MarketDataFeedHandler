import socket
import struct
import time
# import binascii


MCAST_GRP = '239.255.0.1'
MCAST_PORT = 5000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
ttl = struct.pack('b', 1)
sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, ttl)

for i in range(10):
    timestamp = int(time.time_ns())
    instrument_id = i
    price = 100.5 + i
    qty = 10 + i
    msg = struct.pack("<QIdI", timestamp, instrument_id, price, qty)
    sock.sendto(msg, (MCAST_GRP, MCAST_PORT))
    print(f"Sending: ts={timestamp} id={instrument_id} price={price} qty={qty}")
    # print("Raw bytes:", binascii.hexlify(msg).decode())
    time.sleep(1)

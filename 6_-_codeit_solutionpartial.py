# written by Asuna Amawaka for FlareOn7 6_-_codeit

from binascii import hexlify, unhexlify

computername = b"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
with open("sprite.bmp","rb") as spritebmp:
    file_contents = spritebmp.read()
counter = 54
final_string = ""
intermediate_string = ""
for char in computername:
    t = char
    for shift_position in reversed(range(7)):
        t += (file_contents[counter] & 0x1) << shift_position
        counter += 1
    intermediate_string += chr(t)
    final_string += chr((t >> 1) + ((t & 0x1) << 7))
print(intermediate_string)
print(final_string)
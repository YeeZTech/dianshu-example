hex_s = ''
b = bytes.fromhex(hex_s)
print(type(b))
s = b.decode('utf8')
print(s)

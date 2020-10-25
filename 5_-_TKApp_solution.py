# written by Asuna Amawaka for FlareOn7 5_-_TKApp

from Crypto.Hash import SHA256
from Crypto.Cipher import AES
import base64

Desc = "water"
Password = "mullethat"
Step = "magic"
Note = "keep steaks for dinner"

text = Desc[2]+Password[6]+Password[4]+Note[4]+Note[0]+Note[17]+Note[18]+Note[16]+Note[11]+Note[13]+Note[12]+Note[15]+Step[4]+Password[6]+Desc[1]+Password[2]+Password[2]+Password[4]+Note[18]+Step[2]+Password[4]+Note[5]+Note[4]+Desc[0]+Desc[3]+Note[15]+Note[8]+Desc[4]+Desc[3]+Note[4]+Step[2]+Note[13]+Note[18]+Note[18]+Note[8]+Note[4]+Password[0]+Password[7]+Note[0]+Password[4]+Note[11]+Password[6]+Password[4]+Desc[4]+Desc[3]
print("text formed = ", text)
#the kind of challenges we are gonna make here
key = SHA256.new(bytearray(text,"utf-8")).digest()
iv = bytearray("NoSaltOfTheEarth","utf-8")
print("key = hash(text) = ", key)

cipher = AES.new(key, AES.MODE_CBC, iv)
with open("Runtime.dll","rb") as rt:
    runtimedll = rt.read()
msg = cipher.decrypt(runtimedll)
imgdata = base64.b64decode(msg)
#print(msg)

with open("imgdata.jpg","wb") as i:
    i.write(imgdata)
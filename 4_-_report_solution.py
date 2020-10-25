# written by Asuna Amawaka for FlareOn7 4_-_report

def Mid(txt, start, len):
    return txt[start:start+len]


def rigmarole(es):
    furphy = ""
    for i in range(0, len(es)-1, 4):
        c = int(Mid(es,i,2),16)
        s = int(Mid(es,i+2,2),16)
        cc = c - s
        furphy += chr(cc)
    return furphy

        
def canoodle(panjandrum, ardylo, s, bibble):
    kerfuffle = [0]*s
    quean = 0
    for cattywampus in range(0, len(panjandrum)-1, 4):
        kerfuffle[quean] = int(Mid(panjandrum, cattywampus+ardylo, 2),16) ^ bibble[quean % len(bibble)]
        quean += 1
        if quean == len(kerfuffle):
            break
    return kerfuffle


def folderol():
    with open("F.L", "r") as FL:
        onzo = FL.read().split(".")

    counter = 0
    for s in onzo:
        counter +=1
        print("rigmarole(onzo[{}])  =  {}".format(counter,rigmarole(s)))
    """ 
    rigmarole(onzo[1])  =  AppData
    rigmarole(onzo[2])  =  \Microsoft\stomp.mp3
    rigmarole(onzo[3])  =  play
    rigmarole(onzo[4])  =  FLARE-ON
    rigmarole(onzo[5])  =  Sorry, this machine is not supported.
    rigmarole(onzo[6])  =  FLARE-ON
    rigmarole(onzo[7])  =  Error
    rigmarole(onzo[8])  =  winmgmts:\\.\root\CIMV2
    rigmarole(onzo[9])  =  SELECT Name FROM Win32_Process
    rigmarole(onzo[10])  =  vbox
    rigmarole(onzo[11])  =  WScript.Network
    rigmarole(onzo[12])  =  \Microsoft\v.png
    """
    
    with open("F.T.Text", "r") as ftt:
        fttext = ftt.read()
        
    xertz = bytearray(b'\x11\x22\x33\x44\x55\x66\x77\x88\x99\xAA\xBB\xCC\xDD\xEE')
    wabbit = canoodle(fttext, 0, 168667, xertz)
    
    with open("stomp.mp3", "wb") as stomp:
        for item in wabbit:
            stomp.write((item).to_bytes(1,"little"))
    
    key = bytearray("NO-ERALF", "utf-8")    
    flareon = canoodle(fttext, 2, 468336, key)
    
    with open("v.png", "wb") as v:
        for item in flareon:
            v.write((item).to_bytes(1,"little"))
   
   
folderol()
# written by Asuna Amawaka for FlareOn7 1_-_fidler

def decode_flag():
    target_amount = (2**36) + (2**35)
    last_value = int((target_amount - 2**20)/ 10**8)
    encoded_flag = [1135, 1038, 1126, 1028, 1117, 1071, 1094, 1077, 1121, 1087, 1110, 1092, 1072, 1095, 1090, 1027,
                    1127, 1040, 1137, 1030, 1127, 1099, 1062, 1101, 1123, 1027, 1136, 1054]
    decoded_flag = []

    for i in range(len(encoded_flag)):
        c = encoded_flag[i]
        val = (c - ((i%2)*1 + (i%3)*2)) ^ last_value
        decoded_flag.append(val)
        last_value = c

    print(''.join([chr(x) for x in decoded_flag]))

decode_flag()



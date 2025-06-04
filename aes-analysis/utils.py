from Cryptodome.Cipher import AES
import base64

def get_mode_name(mode):
    if mode == AES.MODE_ECB:
        return "ECB"
    
    if mode == AES.MODE_CBC:
        return "CBC"
    
    if mode == AES.MODE_CFB:
        return "CFB"
    
    if mode == AES.MODE_OFB:
        return "OFB"
    
    if mode == AES.MODE_CTR:
        return "CTR"

    raise Exception(f"Invalid mode = {mode}")

def get_avg(times):
    return sum(times) / len(times)

def hex_to_base64(hex_string):
    return base64.b64encode(bytes.fromhex(hex_string)).decode('utf-8')

def print_bytes(name, hex):
    print(name + ": Hex (" + hex + '), B64 (' + hex_to_base64(hex) + ')')
    return

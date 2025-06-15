import time
import utils
from Cryptodome.Cipher import AES
from Cryptodome.Random import get_random_bytes
from utils import *

def encrypt_ecb(key, message):
    return AES.new(key, AES.MODE_ECB).encrypt(message)

def decrypt_ecb(key, ciphertext):
    return AES.new(key, AES.MODE_ECB).decrypt(ciphertext)


def encrypt_cbc(key, iv, message):
    return AES.new(key, AES.MODE_CBC, iv).encrypt(message)

def decrypt_cbc(key, iv, ciphertext):
    return AES.new(key, AES.MODE_CBC, iv).decrypt(ciphertext)


def encrypt_cfb(key, iv, message):
    return AES.new(key, AES.MODE_CFB, iv, segment_size = 128).encrypt(message)

def decrypt_cfb(key, iv, ciphertext):
    return AES.new(key, AES.MODE_CFB, iv, segment_size = 128).decrypt(ciphertext)


def encrypt_ofb(key, iv, message):
    return AES.new(key, AES.MODE_OFB, iv).encrypt(message)

def decrypt_ofb(key, iv, ciphertext):
    return AES.new(key, AES.MODE_OFB, iv).decrypt(ciphertext)


def encrypt_ctr(key, nonce, message):
    return AES.new(key, AES.MODE_CTR, nonce = nonce).encrypt(message)

def decrypt_ctr(key, nonce, ciphertext):
    return AES.new(key, AES.MODE_CTR, nonce = nonce).decrypt(ciphertext)


def encrypt(key, message, mode, iv = None, nonce = None):
    if mode == AES.MODE_ECB:
        return encrypt_ecb(key, message)

    if mode == AES.MODE_CBC:
        return encrypt_cbc(key, iv, message)

    if mode == AES.MODE_CFB:
        return encrypt_cfb(key, iv, message)

    if mode == AES.MODE_OFB:
        return encrypt_ofb(key, iv, message)

    if mode == AES.MODE_CTR:
        return encrypt_ctr(key, nonce, message)

    raise Exception(f"Invalid mode = {mode}")

def decrypt(key, message, mode, iv = None, nonce = None):
    if mode == AES.MODE_ECB:
        return decrypt_ecb(key, message)
    
    if mode == AES.MODE_CBC:
        return decrypt_cbc(key, iv, message)
    
    if mode == AES.MODE_CFB:
        return decrypt_cfb(key, iv, message)
    
    if mode == AES.MODE_OFB:
        return decrypt_ofb(key, iv, message)
    
    if mode == AES.MODE_CTR:
        return decrypt_ctr(key, nonce, message)

    raise Exception(f"Invalid mode = {mode}")

def test_mode(key, message, mode, iv = None, nonce = None, debug_texts = False):

    print(f"Testing mode: {get_mode_name(mode)}")

    cipher_text = None
    enc_times = []

    dec_text = None
    dec_times = []
    
    exec_times = []

    for _ in range(100):

        start_enc = time.perf_counter_ns()
        cipher_text = encrypt(key, message, mode, iv, nonce)
        end_enc = time.perf_counter_ns()
        enc_times.append(end_enc - start_enc)

        start_dec = time.perf_counter_ns()
        dec_text = decrypt(key, cipher_text, mode, iv, nonce)
        end_dec = time.perf_counter_ns()
        dec_times.append(end_dec - start_dec)

        exec_times.append(dec_times[-1] + enc_times[-1])

    if debug_texts:
        print_bytes("Ciphertext", cipher_text.hex())
        print_bytes("Decrypted message", dec_text.hex())

    print(f"Average encryption time: {get_avg(enc_times)} ns")
    print(f"Average decryption time: {get_avg(dec_times)} ns")
    print(f"Average of total time: {get_avg(exec_times)} ns\n")

    assert dec_text == message
    print("Test passed!\n")

# key = get_random_bytes(16)
# iv = get_random_bytes(16)
# nonce = get_random_bytes(12)

key = bytes.fromhex('af3ff749bbfd9a8a3dc791b2cceb193c')
iv = bytes.fromhex('7ac4b2b76533f1a702de0c1660192bfb')
nonce = bytes.fromhex('6102500a1e90abcab67f620d')

files = [
    '../messages/hex/16_bytes',
    '../messages/hex/4096_bytes',
    '../messages/hex/1048576_bytes',
    '../messages/hex/268435456_bytes',
]

with open(files[0], 'r') as f:
    message = bytes.fromhex(f.read().strip())

# print_bytes("Key", key.hex())
# print_bytes("Message", message.hex())
# print_bytes("IV", iv.hex())
# print_bytes("Nonce", nonce.hex())

print()

test_mode(key, message, AES.MODE_ECB)
test_mode(key, message, AES.MODE_CBC, iv)
test_mode(key, message, AES.MODE_CFB, iv)
test_mode(key, message, AES.MODE_OFB, iv)
test_mode(key, message, AES.MODE_CTR, nonce = nonce)
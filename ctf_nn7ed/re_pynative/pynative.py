#!/usr/bin/python

from ctypes import *

c = bytearray((b-i ^ 0xCC) & 0xFF for i,b in enumerate([
	0x99, 0x48, 0x22, 0x52, 0x24, 0x8D, 0xA0, 0x12, 0x91, 0x41, 0xD7, 0xD7, 0xD8, 0xD9, 0x55, 0x98,
    0xD4, 0x56, 0x9B, 0x47, 0x5B, 0x96, 0xDA, 0x5C, 0x99, 0x55, 0x61, 0xB4, 0x58, 0x6C, 0x2C, 0xEC,
    0x65, 0xBA, 0x2E, 0x6A, 0xAD, 0x61, 0x6C, 0xEB, 0x6C, 0xAA, 0x5D, 0x7A, 0xB5, 0x69, 0xFB, 0x7B,
    0xE1, 0x64, 0xFE, 0xEC, 0x56, 0x7C, 0xCF, 0x73, 0x1F, 0xD2, 0x46, 0x80, 0xD5, 0xAD, 0x8D, 0xF0,
    0xB0, 0x25, 0xFB, 0x08, 0x4F, 0xCE, 0x66, 0x14, 0x14, 0x15, 0x16, 0x72, 0x17, 0x58, 0xD7, 0x6F,
    0x1C, 0x1D, 0x1E, 0x1F, 0x9B, 0xDE, 0x8E, 0x46, 0xE1, 0x79, 0x9F, 0xE4, 0x94, 0xD2, 0x26, 0x2B,
    0x2C, 0x2D, 0x09, 0x80, 0x30, 0xAC, 0xEF, 0x9B, 0xB5, 0x59, 0x46, 0x0D, 0x0E, 0x68, 0x17, 0x28,
    0x35, 0x7C, 0xFB, 0x97, 0x41, 0x41, 0x42, 0x43, 0x9F, 0x44, 0x85, 0x04, 0xA0, 0x49, 0x4A, 0x4B,
    0x4C, 0xC8, 0x03, 0xBB, 0x73, 0x06, 0xAA, 0xCC, 0x09, 0xC1, 0x00, 0x53, 0x58, 0x59, 0x5A, 0x36,
    0x9E, 0x5D, 0x07, 0x5B, 0x60, 0x61, 0x62, 0x3E, 0xB5, 0x5E, 0xE1, 0x1C, 0xD0, 0xE4, 0x57, 0xD3,
    0xE7, 0x69, 0x6F, 0xA2, 0x6C, 0x7F, 0x97, 0x78, 0x2B, 0x83, 0x7E, 0x64, 0x71, 0xB8, 0x37, 0xD7,
    0x7D, 0x7D, 0x7E, 0x7F, 0xDB, 0x80, 0xC1, 0x40, 0xE0, 0x85, 0x86, 0x87, 0x88, 0x04, 0x3F, 0xF7,
    0xAF, 0x42, 0xEA, 0x08, 0x45, 0xFD, 0x3C, 0x8F, 0x94, 0x95, 0x96, 0x72, 0xDA, 0x92, 0x15, 0x50,
    0x04, 0x18, 0xAA, 0xA0, 0x21, 0xFB, 0x99, 0x41, 0x3A, 0x5E, 0x4E, 0xA3, 0xA8, 0xA9, 0xAA, 0xEC,
    0x0C, 0xAD, 0x29, 0x64, 0x18, 0xDC, 0xBE, 0xB4, 0xA1, 0xAE, 0xF5, 0x74, 0x18, 0xBA, 0xBA, 0xBB,
    0xBC, 0x18, 0xBD, 0xFE, 0x7D, 0x21, 0xC2, 0xC3, 0xC4, 0xC5, 0x41, 0x94, 0x34, 0xEC, 0x97, 0x2B,
    0x45, 0x9A, 0x3A, 0x77, 0xCC, 0xD1, 0xD2, 0xD3, 0x25, 0x35, 0x51, 0x8C, 0x40, 0x54, 0xE6, 0xDC,
    0x1D, 0x37, 0xD4, 0x87, 0xDC, 0xE1, 0xE2, 0xE3, 0x35, 0x45, 0x61, 0x9C, 0x50, 0x64, 0xE6, 0xEC,
    0x2D, 0x4D, 0x00, 0xEA, 0x40, 0x72, 0x5C, 0x2B, 0x29, 0x3E, 0x2B, 0xE4, 0xF1, 0x38, 0xB7, 0x3F,
    0xFD, 0xFD, 0xFE, 0xFF, 0x5B, 0x00, 0x41, 0xC0, 0x48, 0x05, 0x06, 0x07, 0x08, 0x84, 0xBF, 0x77,
    0x2F, 0xC2, 0x52, 0x88, 0xC5, 0x7D, 0xBC, 0x0F, 0x14, 0x15, 0x16, 0xF2, 0x5A, 0x1C, 0x95, 0xD0,
    0x84, 0x98, 0x2A, 0x20, 0x61, 0x83, 0x32, 0xCB, 0x20, 0x25, 0x26, 0x27, 0x03, 0x61, 0x2D, 0xA6,
    0xE9, 0x95, 0xA9, 0x23, 0x28, 0x72, 0x8B, 0x43, 0x2F, 0x86, 0xB7, 0xA1, 0xF7, 0x00, 0x6B, 0x11,
    0x29, 0x36, 0x7D, 0xFC, 0x88, 0x42, 0x42, 0x43, 0x44, 0xA0, 0x45, 0x86, 0x05, 0x91, 0x4A, 0x4B,
    0x4C, 0x4D, 0xC9, 0x1C, 0xBC, 0x74, 0x1F, 0x9B, 0xCD, 0x22, 0xC2, 0xFF, 0x54, 0x59, 0x5A, 0x5B,
    0x9D, 0xBD, 0x60, 0xDA, 0x15, 0xC9, 0xDD, 0x6F, 0x65, 0xA6, 0xC8, 0x79, 0x10, 0x65, 0x6A, 0x6B,
    0x6C, 0xAE, 0xCE, 0x71, 0xEB, 0x26, 0xDA, 0xEE, 0x70, 0x76, 0xB7, 0xCF, 0x6E, 0x74, 0xCA, 0xBC,
    0xD6, 0x79, 0x27, 0x7B, 0x80, 0x81, 0x82, 0xC4, 0xE5, 0x87, 0x01, 0x44, 0xF0, 0x04, 0x7E, 0x83,
    0xCD, 0xEE, 0xA0, 0x37, 0x8C, 0x91, 0x92, 0x93, 0xD5, 0xF5, 0x98, 0x12, 0x85, 0x01, 0x15, 0x97,
    0x9A, 0xDE, 0xF6, 0x95, 0x9B, 0xD9, 0xE3, 0x04, 0xA8, 0xA0, 0xF7, 0x28, 0x12, 0x9D, 0xF5, 0xAF,
    0xF6, 0x9A, 0xA7, 0xEE, 0x6D, 0xFD, 0xB3, 0xB3, 0xB4, 0xB5, 0x11, 0xB6, 0xF7, 0x76, 0x06, 0xBB,
    0xBC, 0xBD, 0xBE, 0x3A, 0x75, 0x2D, 0xE5, 0x78, 0x10, 0x3E, 0x7B, 0x33, 0x72, 0xC5, 0xCA, 0xCB,
    0xCC, 0xA8, 0x10, 0xCC, 0x4B, 0x86, 0x3A, 0x4E, 0xE0, 0xD6, 0x45, 0x29, 0x59, 0x43, 0x6D, 0x5F,
    0x13, 0x68, 0xCB, 0xD8, 0x1F, 0x9E, 0x32, 0xE4, 0xE4, 0xE5, 0xE6, 0x42, 0xE7, 0x28, 0xA7, 0x3B,
    0xEC, 0xED, 0xEE, 0xEF, 0x6B, 0xAE, 0x5E, 0x16, 0xB1, 0x45, 0x6F, 0xB4, 0x64, 0xA2, 0xF6, 0xFB,
    0xFC, 0xFD, 0xD9, 0x50, 0xFD, 0xA9, 0xFE, 0x03, 0x04, 0x05, 0xE1, 0x3F, 0x06, 0x84, 0xC7, 0x73,
    0x87, 0xFA, 0x76, 0x8A, 0x1C, 0x21, 0x2D, 0x1F, 0x0A, 0x84, 0x68, 0x98, 0x82, 0x81, 0x11, 0x32,
    0x51, 0x0A, 0x17, 0x5E, 0xDD, 0x55, 0x23, 0x23, 0x24, 0x25, 0x81, 0x26, 0x67, 0xE6, 0x5E, 0x2B,
    0x2C, 0x2D, 0x2E, 0xAA, 0xED, 0x9D, 0x55, 0xF0, 0x68, 0xAE, 0xF3, 0xA3, 0xE1, 0x35, 0x3A, 0x3B,
    0x3C, 0x18, 0x8F, 0x3F, 0xBB, 0xFE, 0xAA, 0xBE, 0x38, 0x55, 0x87, 0xA8, 0x40, 0xF3, 0x46, 0x4B,
    0x4C, 0x4D, 0x29, 0x91, 0x50, 0xCC, 0x1F, 0xBB, 0xCF, 0x51, 0x58, 0x98, 0xB0, 0x61, 0x55, 0x93,
    0x06, 0x59, 0x5E, 0x5F, 0x60, 0x3C, 0xA4, 0x62, 0xDF, 0x32, 0xCE, 0x6A, 0x5C, 0x6B, 0xEB, 0xD4,
    0xB4, 0xA2, 0xD0, 0x47, 0x5D, 0x6A, 0xB1, 0x30, 0xAC, 0x76, 0x76, 0x77, 0x78, 0xD4, 0x79, 0xBA,
    0x39, 0xB5, 0x7E, 0x7F, 0x80, 0x81, 0xFD, 0x40, 0xF0, 0xA8, 0x43, 0xBF, 0x01, 0x46, 0xF6, 0x34,
    0x88, 0x8D, 0x8E, 0x8F, 0xD1, 0xF2, 0x95, 0x3D, 0x90, 0x95, 0x96, 0x97, 0x73, 0xDB, 0x93, 0x16,
    0x69, 0x05, 0x19, 0x8C, 0x08, 0x1C, 0x96, 0x9D, 0xC7, 0x99, 0xA4, 0x82, 0xF9, 0x0F, 0x52, 0xA7,
    0xAC, 0xAD, 0xAE, 0xF0, 0x10, 0xB4, 0x5B, 0xAF, 0xB4, 0xB5, 0xB6, 0x92, 0xF1, 0xB2, 0x35, 0xA8,
    0x24, 0x38, 0xBA, 0xBD, 0x3B, 0xAE, 0x2A, 0xC6, 0xC0, 0xBB, 0xC9, 0x17, 0x49, 0x33, 0xD9, 0xDB,
    0xDD, 0xC5, 0xBB, 0xC8, 0x0F, 0x8E, 0x0E, 0xD4, 0xD4, 0xD5, 0xD6, 0x32, 0xD7, 0x18, 0x97, 0x17,
    0xDC, 0xDD, 0xDE, 0xDF, 0x5B, 0x96, 0x4E, 0x06, 0x99, 0x21, 0x5F, 0x9C, 0x54, 0x64, 0xA7, 0x57,
    0x47, 0xEA, 0x96, 0xF0, 0xF0, 0xF1, 0xF2, 0xB9, 0x6F, 0x52, 0xBB, 0x3A 
]))

proto = CFUNCTYPE(c_bool, c_char_p)
func_ptr = windll.kernel32.VirtualAlloc(0, len(c), 0x1000, 0x40)
memmove(func_ptr, bytes(c), len(c))
chk = proto(func_ptr)

print("""
        ___          
    . -^   `--,      
   /# =========`-_   
  /# (--====___====\ 
 /#   .- --.  . --.| 
/##   |  * ) (   * ),
|##   \    /\ \   / |
|###   ---   \ ---  |
|####      ___)    #|
|######           ##|
 \##### ---------- / 
  \####           (  
   `\###          |  
     \###         |  
      \##        |   
       \###.    .)   
        `======/    

""")

if (chk(raw_input("Show me what you got: ").encode('utf-8'))):
	print("Greaat! :)")


#!/usr/bin/env python2

import angr
import claripy

binary = './valid_flag'
avoid_paths = [
  0x0804980D,
  0x0804983A,
  0x08049876,
  0x080498B3,
  0x080498F6,
  0x0804993B,
  0x080499AC,
  0x080499DD,
  0x08049A1C,
  0x08049A6F,
  0x08049ACD,
  0x08048491   # Invalid flag
]

proj = angr.Project(binary)

input_size = 40
argv1 = claripy.BVS("argv1", input_size * 8)
initial_state = proj.factory.entry_state(args=[binary, argv1]) 
initial_state.libc.buf_symbolic_bytes=input_size + 1 

sm = proj.factory.simgr(initial_state)
sm.explore(find=0x0804847F, avoid=avoid_paths)

found = sm.found[0]
flag  = found.se.eval(argv1, cast_to=str)

print "Flag: %s" % flag


catman
======

catman is a network framework with which you can implement game server or game client easily.

catman is comprised of 3 modules: common module, net module and thread module.

common module provides the transport protocol and byte stream, etc.

net module is OO implementation of Berkeley socket, which was driven by select.

thread module is OO implementation of pthread library. Meanwhile, a runtime fixed-size thread
pool is provided.

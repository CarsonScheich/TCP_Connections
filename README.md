This should run on both Mac and Windows. I use a Mac and have never tested on Windows.
To compile on mac it should be 
'''
g++ -std=c++17 main.cpp -o tcp_connections
'''

and on Windows it might be
'''
g++ -std=c++17 main.cpp -o tcp_connections.exe -lws2_32
'''

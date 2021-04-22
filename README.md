# gige_test

# 0. add submodule
git clone git@github.com:aileenfun/GigeVisionCam.git
git submodule add git@github.com:aileenfun/GigeVisionCam.git

# 1. udp test speed
cd udp && make
sudo ifconfig lo mtu 65536
./recv 9999  (start receive process)
./send 127.0.0.1 9999 0 (start send process, ip, port, not loop)
cat 60k | ./send 127.0.0.1 9999 1 (start send process, loop sending 60k, with args: ip, port, loop)

# 2. udp test speed log message
send log:
$ cat 60k | ./send 127.0.0.1 9999 1
Please input broadcast msg:
tid:140303557416704
[total:0 KB, speed: 0 KB/s]
[total:7099269 KB, speed: 7099269 KB/s]
[total:14503772 KB, speed: 7404503 KB/s]
[total:21907845 KB, speed: 7404072 KB/s]
[total:29432709 KB, speed: 7524864 KB/s]
[total:36966789 KB, speed: 7534080 KB/s]
[total:44464128 KB, speed: 7497338 KB/s]
[total:51846021 KB, speed: 7381893 KB/s]
[total:59127152 KB, speed: 7281131 KB/s]
[total:66373939 KB, speed: 7246786 KB/s]
[total:73612431 KB, speed: 7238492 KB/s]
[total:80977059 KB, speed: 7364628 KB/s]
[total:88558325 KB, speed: 7581265 KB/s]

recv log:
$ ./recv 9999
recv ready!!!
[ip: 127.0.0.1, port: 57874, len: 61440, total: 78585016 KB]

# 3. ./submodules/udp-image-streaming
mkdir build && cd build && cmake .. . && make -j
./server 9999
./client 127.0.0.1 9999 cam (using pc camera)
./client 127.0.0.1 9999 xxx.mp4 (using mp4 video)

# 4. ./submodules/rc_genicam_api
mkdir build && cd build && cmake .. . && make -j
using GenICam to contact GigeVision camera, including discovery

# 5. ./doc
documents


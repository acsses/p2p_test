
gcc request.c p2ptest/http.c p2ptest/parser.c p2ptest/ssdp.c p2ptest/timeutil.c p2ptest/natpmp.c p2ptest/portmapping.c p2ptest/network.c p2ptest/id.c p2ptest/sha256.c -L/usr/local/opt/expat/lib -I/usr/local/opt/expat/include -lexpat -L/usr/local/opt/json-c/lib -I/usr/local/opt/json-c/include -ljson-c -L/usr/local/opt/openssl@3.1/lib -I/usr/local/opt/openssl@3.1/include -lssl -lcrypto -o request
./request

##gcc test2.c p2ptest/http.c p2ptest/parser.c p2ptest/ssdp.c p2ptest/timeutil.c p2ptest/natpmp.c p2ptest/portmapping.c p2ptest/network.c -L/usr/local/opt/expat/lib -I/usr/local/opt/expat/include -lexpat -L/usr/local/opt/json-c/lib -I/usr/local/opt/json-c/include -ljson-c -L/usr/local/opt/openssl@3.1/lib -I/usr/local/opt/openssl@3.1/include -lssl -lcrypto
##./a.out

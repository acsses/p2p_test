##gcc test.c \
##    p2ptest/http.c \
##    p2ptest/parser.c \
##    p2ptest/ssdp.c \
##    p2ptest/util.c \
##    p2ptest/natpmp.c \
##    p2ptest/portmapping.c \
##    p2ptest/network.c \
##    p2ptest/id.c \
##    p2ptest/sha256.c \
##    p2ptest/broadcast.c \
##    p2ptest/message.c \
##    p2ptest/node.c \
##    p2ptest/cache.c \
##    p2ptest/job.c \
##    -L/usr/local/opt/expat/lib -I/usr/local/opt/expat/include -lexpat -L/usr/local/opt/json-c/lib -I/usr/local/opt/json-c/include -ljson-c -L/usr/local/opt/openssl@3.1/lib -I/usr/local/opt/openssl@3.1/include -lssl -lcrypto -lpthread -w
##./a.out
##rm -r -f ./a.out

gcc test.c \
    p2ptest/http.c \
    p2ptest/parser.c \
    p2ptest/ssdp.c \
    p2ptest/util.c \
    p2ptest/natpmp.c \
    p2ptest/portmapping.c \
    p2ptest/network.c \
    p2ptest/id.c \
    p2ptest/sha256.c \
    p2ptest/broadcast.c \
    p2ptest/message.c \
    p2ptest/node.c \
    p2ptest/cache.c \
    p2ptest/job.c \
    p2ptest/block.c \
    -L/usr/local/opt/expat/lib -I/usr/local/opt/expat/include -lexpat \
    -L/usr/local/opt/json-c/lib -I/usr/local/opt/json-c/include -ljson-c \
    -L/usr/local/opt/openssl@3.1/lib -I/usr/local/opt/openssl@3.1/include -lssl -lcrypto \
    -lpthread -w

./a.out
rm -r -f ./a.out


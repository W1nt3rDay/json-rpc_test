TARGET=rpc
CC=gcc

TOP_DIR=$(shell pwd)/
LIBEV_DIR=${TOP_DIR}/libev_pc/tmp/
JSONRPC_DIR=${TOP_DIR}/jsonrpc-c_pc/tmp/

CFLAGS=-I${LIBEV_DIR}/include -I${JSONRPC_DIR}/include
LDFLAGS=${JSONRPC_DIR}/lib/libjsonrpcc.a  ${LIBEV_DIR}/lib/libev.a

c_files = cJSON.c rpc.c 

all:
	${CC} ${CFLAGS} -o ${TARGET} ${c_files} ${LDFLAGS}

clean:
	rm -f *.o ${TARGET}
	

INC=/usr/local/ssl/include/
LIB=/usr/local/ssl/lib/

all:
		gcc -I$(INC) -L$(LIB) -o enc try2.c -lcrypto -ldl

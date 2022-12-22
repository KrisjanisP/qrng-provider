LIB_DIR=$(CURDIR)/lib
SRC_DIR=$(CURDIR)/src
CNF_DIR=$(CURDIR)/cnf

compile: $(LIB_DIR)/libqrng.so

$(LIB_DIR)/libqrng.so: $(SRC_DIR)/qrng-provider-rand.c $(SRC_DIR)/qrng-provider.c
	mkdir -p $(LIB_DIR)
	gcc -shared -fPIC $^ -o $@

install: $(LIB_DIR)/libqrng.so $(CNF_DIR)/
	python3 $(CNF_DIR)/gen-conf.py
	cp $(LIB_DIR)/lib/librqrng.so /usr/lib/librqrng.so
	cp $(CNF_DIR)/config/modified.cnf /etc/ssl/openssl.cnf
	cp $(CNF_DIR)/config/modified.cnf /usr/local/ssl/openssl.cnf
LIB_DIR=$(CURDIR)/lib
SRC_DIR=$(CURDIR)/src
CNF_DIR=$(CURDIR)/cnf

compile: $(LIB_DIR)/libqrng.so

$(LIB_DIR)/libqrng.so: $(SRC_DIR)/qrng-provider-rand.c $(SRC_DIR)/qrng-provider.c
	mkdir -p $(LIB_DIR)
	gcc -shared -fPIC $^ -o $@

install: $(LIB_DIR)/libqrng.so $(CNF_DIR)/modified.cnf
	cp $(LIB_DIR)/librqrng.so /usr/lib/librqrng.so
	cp $(CNF_DIR)/modified.cnf /etc/ssl/openssl.cnf
	cp $(CNF_DIR)/modified.cnf /usr/local/ssl/openssl.cnf

$(CNF_DIR)/modified.cnf: $(CNF_DIR)/default.cnf $(CNF_DIR)/old-providers.txt $(CNF_DIR)/new-providers.txt
	python3 $(CNF_DIR)/gen-conf.py
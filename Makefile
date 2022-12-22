LIB_DIR=$(CURDIR)/lib
SRC_DIR=$(CURDIR)/src
CNF_DIR=$(CURDIR)/conf

compile: $(LIB_DIR)/librqrng.so

$(LIB_DIR)/librqrng.so: $(SRC_DIR)/qrng-provider-rand.c $(SRC_DIR)/qrng-provider.c
	mkdir -p $(LIB_DIR)
	gcc -shared -fPIC $^ -o $@

install: $(LIB_DIR)/librqrng.so $(CNF_DIR)/modified.cnf
	cp $(LIB_DIR)/librqrng.so /usr/lib/librqrng.so
	cp $(CNF_DIR)/modified.cnf /etc/ssl/openssl.cnf
	cp $(CNF_DIR)/modified.cnf /usr/local/ssl/openssl.cnf

$(CNF_DIR)/modified.cnf: $(CNF_DIR)/default.cnf $(CNF_DIR)/old-providers.txt $(CNF_DIR)/new-providers.txt
	cd $(CNF_DIR); python3 gen-conf.py

$(CNF_DIR)/default.cnf:
	cp /etc/ssl/openssl.cnf $(CNF_DIR)/default.cnf

uninstall: $(CNF_DIR)/default.cnf
	cp $(CNF_DIR)/default.cnf /etc/ssl/openssl.cnf
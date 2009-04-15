#include <iostream>

#ifdef HAVE_GNUTLS
#include <gnutls/gnutls.h>
#include <gcrypt.h>
#include <errno.h>
#include <pthread.h>

GCRY_THREAD_OPTION_PTHREAD_IMPL;
#endif // HAVE_GNUTLS

#include "debug.h"
#include "season.h"
#include "client_socket.h"

int main(int argc, char *argv[]) {
#ifdef HAVE_GNUTLS
	gcry_control (GCRYCTL_SET_THREAD_CBS, &gcry_threads_pthread);
	gnutls_global_init();
#endif // HAVE_GNUTLS

	std::cout << "Hello world!!" << std::endl;
	PSProxy::ClientSocket socket("127.0.0.1", 10000);
	return 0;

	PSProxy::Season season(PSProxy::SeasonConfig("~/.psproxy-client.conf"));

	while(!season.getLines2()) {
		pDebug("%s\n", "Yet offline");
	}
	
	// 3B 24 00 30 FF 3B 24 00 30 42 30 30
	PSProxy::Data_t tmp;
	season.read(tmp);

	PSProxy::Data_t a;
	a.push_back(0x3B);
	a.push_back(0x24);
	a.push_back(0x00);
	a.push_back(0x30);
	a.push_back(0x42);
	a.push_back(0x30);
	a.push_back(0x30);
	season.write(a);
	if(season.readDataAvail()) {
		pDebug("%s\n", "Got answer to ATR");
		season.read(a);
		for(unsigned i = 0; i < a.size(); i++) {
			pDebug("Got byte: %X\n", a[i]);
		}

	}

	return 0;
}

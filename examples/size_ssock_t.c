#include <stdio.h>
#include <ssock/ssock.h>

int main () {
  ssock_t sock;
  printf("size of ssock_t: %lu\n", sizeof sock);

  printf("size of ssock_t settings: %lu\n",
	 sizeof sock.settings);

  printf("size of ssock_t settings af_inet: %lu\n",
	 sizeof sock.settings.af_inet);

  printf("size of ssock_t settings af_unix: %lu\n",
	 sizeof sock.settings.af_unix);

  printf("size of ssock_t settings af_unix address: %lu\n",
	 sizeof sock.settings.af_unix.address);

  printf("size of ssock_t settings af_unix address sun_path: %lu\n",
	 sizeof sock.settings.af_unix.address.sun_path);
}

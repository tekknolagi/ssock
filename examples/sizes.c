#include <stdio.h>
#include <ssock/ssock.h>

int main () {
  ssock_settings_t settings;
  printf("%lu\n", sizeof settings.type);
}

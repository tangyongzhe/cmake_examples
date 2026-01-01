// network.cpp - 网络功能演示
#include <iostream>
#include <string>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#endif

void demonstrate_openssl_usage();

void demonstrate_network_capabilities();
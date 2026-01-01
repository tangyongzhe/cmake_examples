// network.cpp - 网络功能演示
#include <iostream>
#include <string>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#endif

void demonstrate_openssl_usage() {
#ifdef HAVE_OPENSSL
    std::cout << "=== OpenSSL库演示 ===" << std::endl;
    
    // 初始化OpenSSL
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
    
    // 打印OpenSSL版本
    std::cout << "OpenSSL版本: " << OpenSSL_version(OPENSSL_VERSION) << std::endl;
    
    // 创建SSL上下文
    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
    if (ctx) {
        std::cout << "SSL上下文创建成功" << std::endl;
        
        // 设置最小TLS版本
        SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);
        std::cout << "设置最小TLS版本: 1.2" << std::endl;
        
        // 生成随机数
        unsigned char random_bytes[16];
        if (RAND_bytes(random_bytes, sizeof(random_bytes)) == 1) {
            std::cout << "随机数生成成功: ";
            for (int i = 0; i < 8; ++i) {
                printf("%02x", random_bytes[i]);
            }
            std::cout << "..." << std::endl;
        }
        
        // 清理
        SSL_CTX_free(ctx);
        std::cout << "SSL上下文已释放" << std::endl;
    }
    
    // 清理OpenSSL
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();
    
    std::cout << std::endl;
#endif
}

void demonstrate_network_capabilities() {
    std::cout << "=== 网络功能演示 ===" << std::endl;
    
#ifdef HAVE_OPENSSL
    std::cout << "支持的网络功能:" << std::endl;
    std::cout << "  ✓ HTTPS/TLS加密通信" << std::endl;
    std::cout << "  ✓ SSL证书验证" << std::endl;
    std::cout << "  ✓ 安全随机数生成" << std::endl;
#else
    std::cout << "网络功能: 基础TCP/UDP (无加密)" << std::endl;
#endif
    
    std::cout << std::endl;
}

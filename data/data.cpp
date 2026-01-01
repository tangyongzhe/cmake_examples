// data.cpp - 数据处理演示
#include <iostream>
#include <string>
#include <vector>

#ifdef HAVE_ZLIB
#include <zlib.h>
#endif

void demonstrate_compression_capabilities() {
    std::cout << "=== 数据压缩功能演示 ===" << std::endl;
    
#ifdef HAVE_ZLIB
    std::cout << "支持的压缩功能:" << std::endl;
    std::cout << "  ✓ DEFLATE压缩算法" << std::endl;
    std::cout << "  ✓ GZIP格式支持" << std::endl;
    std::cout << "  ✓ 内存压缩/解压" << std::endl;
    std::cout << "  ✓ 流式压缩" << std::endl;
    
    // 压缩测试
    const char* test_data = "这是一段用于压缩测试的数据。"
                           "This is data for compression testing.";
    uLong data_len = strlen(test_data) + 1;
    uLong max_compressed_len = compressBound(data_len);
    
    std::cout << std::endl;
    std::cout << "压缩测试:" << std::endl;
    std::cout << "  原始数据大小: " << data_len << " 字节" << std::endl;
    std::cout << "  最大压缩后大小: " << max_compressed_len << " 字节" << std::endl;
    
    // 计算压缩率
    double max_ratio = (1.0 - (double)max_compressed_len / data_len) * 100.0;
    std::cout << "  理论最大压缩率: " << max_ratio << "%" << std::endl;
#else
    std::cout << "压缩功能: 不可用" << std::endl;
#endif
    
    std::cout << std::endl;
}

void demonstrate_data_processing_features() {
    std::cout << "=== 数据处理功能总结 ===" << std::endl;
    
    std::cout << "可用的数据处理功能:" << std::endl;
    
#ifdef HAVE_JSON
    std::cout << "  ✓ JSON解析与生成" << std::endl;
    std::cout << "  ✓ 配置文件处理" << std::endl;
    std::cout << "  ✓ 数据序列化" << std::endl;
#endif
    
#ifdef HAVE_ZLIB
    std::cout << "  ✓ 数据压缩/解压" << std::endl;
    std::cout << "  ✓ 文件压缩" << std::endl;
#endif
    
#ifdef HAVE_SQLITE3
    std::cout << "  ✓ 关系型数据库" << std::endl;
    std::cout << "  ✓ SQL查询" << std::endl;
    std::cout << "  ✓ 事务支持" << std::endl;
#endif
    
    std::cout << std::endl;
}

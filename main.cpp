// main.cpp - 测试静态库别名空间
#include <iostream>
#include <string>
#include <vector>

// 第三方库头文件
#ifdef HAVE_JSON
#include <json/json.h>
#endif

#ifdef HAVE_ZLIB
#include <zlib.h>
#endif

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

#ifdef HAVE_SQLITE3
#include <sqlite3.h>
#endif

// 演示函数：检查库是否可用
void check_libraries_availability() {
    std::cout << "=== 检查第三方静态库可用性 ===" << std::endl;
    
#ifdef HAVE_JSON
    std::cout << "JSON库: 可用" << std::endl;
#else
    std::cout << "JSON库: 不可用" << std::endl;
#endif

#ifdef HAVE_ZLIB
    std::cout << "ZLib库: 可用" << std::endl;
#else
    std::cout << "ZLib库: 不可用" << std::endl;
#endif

#ifdef HAVE_OPENSSL
    std::cout << "OpenSSL库: 可用" << std::endl;
#else
    std::cout << "OpenSSL库: 不可用" << std::endl;
#endif

#ifdef HAVE_SQLITE3
    std::cout << "SQLite3库: 可用" << std::endl;
#else
    std::cout << "SQLite3库: 不可用" << std::endl;
#endif
    
    std::cout << std::endl;
}

// 演示函数：使用JSON库
void demonstrate_json_usage() {
#ifdef HAVE_JSON
    std::cout << "=== JSON库演示 ===" << std::endl;
    
    // 创建JSON对象
    Json::Value root;
    root["name"] = "测试应用";
    root["version"] = "1.0.0";
    root["enabled"] = true;
    
    // 添加数组
    Json::Value modules(Json::arrayValue);
    modules.append("network");
    modules.append("database");
    modules.append("config");
    root["modules"] = modules;
    
    // 序列化为字符串
    Json::StreamWriterBuilder writer;
    std::string json_str = Json::writeString(writer, root);
    
    std::cout << "生成的JSON:" << std::endl;
    std::cout << json_str << std::endl;
    std::cout << std::endl;
#endif
}

// 演示函数：使用ZLib库
void demonstrate_zlib_usage() {
#ifdef HAVE_ZLIB
    std::cout << "=== ZLib库演示 ===" << std::endl;
    
    const char* original = "这是一段需要压缩的测试文本。This is a test text that needs to be compressed.";
    uLong original_len = strlen(original) + 1;
    uLong compressed_len = compressBound(original_len);
    
    // 分配内存
    Bytef* compressed = new Bytef[compressed_len];
    Bytef* uncompressed = new Bytef[original_len];
    
    // 压缩
    int compress_result = compress(compressed, &compressed_len, 
                                   (const Bytef*)original, original_len);
    
    if (compress_result == Z_OK) {
        std::cout << "压缩成功:" << std::endl;
        std::cout << "  原始大小: " << original_len << " 字节" << std::endl;
        std::cout << "  压缩后大小: " << compressed_len << " 字节" << std::endl;
        std::cout << "  压缩率: " 
                  << (100.0 - (compressed_len * 100.0 / original_len)) 
                  << "%" << std::endl;
        
        // 解压
        uLong uncompressed_len = original_len;
        int uncompress_result = uncompress(uncompressed, &uncompressed_len,
                                          compressed, compressed_len);
        
        if (uncompress_result == Z_OK) {
            std::cout << "解压成功，验证数据一致性: " 
                      << (strcmp(original, (char*)uncompressed) == 0 ? "true" : "false")
                      << std::endl;
        }
    }
    
    delete[] compressed;
    delete[] uncompressed;
    std::cout << std::endl;
#endif
}

// 演示函数：使用SQLite3库
void demonstrate_sqlite_usage() {
#ifdef HAVE_SQLITE3
    std::cout << "=== SQLite3库演示 ===" << std::endl;
    
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    
    if (rc == SQLITE_OK) {
        std::cout << "SQLite内存数据库创建成功" << std::endl;
        
        // 创建表
        const char* create_table_sql = 
            "CREATE TABLE users ("
            "id INTEGER PRIMARY KEY, "
            "name TEXT NOT NULL, "
            "email TEXT UNIQUE NOT NULL);";
        
        char* err_msg = nullptr;
        rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
        
        if (rc == SQLITE_OK) {
            std::cout << "用户表创建成功" << std::endl;
            
            // 插入数据
            const char* insert_sql = 
                "INSERT INTO users (name, email) VALUES "
                "('张三', 'zhangsan@example.com'), "
                "('李四', 'lisi@example.com');";
            
            rc = sqlite3_exec(db, insert_sql, nullptr, nullptr, &err_msg);
            
            if (rc == SQLITE_OK) {
                std::cout << "数据插入成功" << std::endl;
                
                // 查询数据
                const char* select_sql = "SELECT id, name, email FROM users;";
                sqlite3_stmt* stmt;
                rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
                
                if (rc == SQLITE_OK) {
                    std::cout << "查询结果:" << std::endl;
                    std::cout << "ID\t姓名\t邮箱" << std::endl;
                    std::cout << "--\t----\t------" << std::endl;
                    
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        int id = sqlite3_column_int(stmt, 0);
                        const char* name = (const char*)sqlite3_column_text(stmt, 1);
                        const char* email = (const char*)sqlite3_column_text(stmt, 2);
                        
                        std::cout << id << "\t" << name << "\t" << email << std::endl;
                    }
                    
                    sqlite3_finalize(stmt);
                }
            } else if (err_msg) {
                std::cerr << "SQL错误: " << err_msg << std::endl;
                sqlite3_free(err_msg);
            }
        } else if (err_msg) {
            std::cerr << "SQL错误: " << err_msg << std::endl;
            sqlite3_free(err_msg);
        }
        
        sqlite3_close(db);
    } else {
        std::cerr << "无法打开数据库: " << sqlite3_errmsg(db) << std::endl;
    }
    
    std::cout << std::endl;
#endif
}

// 主函数
int main(int argc, char* argv[]) {
    std::cout << "第三方静态库别名空间演示程序" << std::endl;
    std::cout << "============================" << std::endl;
    
    bool run_tests = false;
    
    // 简单的参数解析
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--test" || arg == "-t") {
            run_tests = true;
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "用法: " << argv[0] << " [选项]" << std::endl;
            std::cout << "选项:" << std::endl;
            std::cout << "  --test, -t    运行所有测试" << std::endl;
            std::cout << "  --help, -h    显示此帮助信息" << std::endl;
            return 0;
        }
    }
    
    // 检查库可用性
    check_libraries_availability();
    
    if (run_tests) {
        std::cout << "运行所有库测试..." << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        
        // 演示各库功能
        demonstrate_json_usage();
        demonstrate_zlib_usage();
        demonstrate_sqlite_usage();
        
        std::cout << "所有测试完成!" << std::endl;
    } else {
        std::cout << "使用 --test 参数运行所有库测试" << std::endl;
    }
    
    return 0;
}

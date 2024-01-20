#include "io.h"


// 其他必要的头文件和函数声明

int main(int argc, char* argv[]) {
    std::string inputFilename;
    std::string outputFilename = "a.sfs"; // 默认输出文件名

    // 检查是否只有一个参数（程序名）
    if (argc == 1) {
        std::cerr << "Usage: " << argv[0] << " <input_filename> [-o <output_filename>]" << std::endl;
        return 1;
    }

    // 解析命令行参数
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-o" && i + 1 < argc) {
            outputFilename = argv[++i]; // 设置自定义输出文件名
        } else if (arg == "-h") {
            // 显示帮助信息
            std::cout << "Usage: " << argv[0] << " <input_filename> [-o <output_filename>]\n";
            std::cout << "Options:\n";
            std::cout << "  -h           Show this help message\n";
            std::cout << "  -o <file>    Set output file name\n";
            return 0;
        } else {
            inputFilename = arg; // 设置输入文件名
        }
    }

    if (inputFilename.empty()) {
        std::cerr << "Error: No input file provided.\n";
        std::cerr << "Usage: " << argv[0] << " <input_filename> [-o <output_filename>]" << std::endl;
        return 1;
    }
    //初始化
    // 读取文件和编译过程
    read_file(inputFilename);
    output(outputFilename);
    
    return 0;
}
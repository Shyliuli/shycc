#include "memorylist.h" // 包含必要的头文件
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <iomanip>

extern std::vector<unsigned short>
    address; // 外部声明address向量，用于存储内存地址

struct index_s
{
  unsigned short index_of_data_start;
  unsigned short index_of_data_end;
  unsigned short index_of_code_start;
  unsigned short index_of_code_end;
}; // 索引结构体，用于存储数据和代码的起始和结束索引

// 此头文件的函数：
inline void
read_file (const std::string &filename); // 此头文件的读取“主函数”，调用这个即可
unsigned short ascii_to_hex (char c);
unsigned short string_to_hex (const std::string &str);
std::vector<std::string> readaline (const std::string &filename,
                                    unsigned short index);
index_s add_indexs (std::string file_name);
void read_data (std::string file_name, index_s indexs);
void read_code (std::string file_name, index_s indexs);
inline void output(const std::string &filename);
//输出”主函数“

inline void output(const std::string &filename){
  std::ofstream file(filename);
  if (!file.is_open()){
      std::cerr << "Failed to open file: " << filename << std::endl;
      return;
  }
  for (auto &instruction : address) {
      file << std::hex << std::setw(4) << std::setfill('0') << instruction << " ";

}

}


inline void
read_file (const std::string &filename)
{
  index_s index = add_indexs (filename);
  read_data (filename, index);
  read_code (filename, index);
}


// 命令和寄存器到16进制的映射
std::unordered_map<std::string, unsigned short> command_map = {
  { "adda", 0x10 }, { "addn", 0x11 }, { "suba", 0x12 },    { "subn", 0x13 },
  { "mula", 0x14 }, { "muln", 0x15 }, { "diva", 0x16 },    { "divn", 0x17 },
  { "lsa", 0x18 },  { "lsn", 0x19 },  { "lra", 0x1A },     { "lrn", 0x1B },
  { "anda", 0x1C }, { "andn", 0x1D }, { "ora", 0x1E },     { "orn", 0x1F },
  { "nota", 0x20 }, { "notn", 0x21 }, { "xora", 0x22 },    { "xorn", 0x23 },
  { "jmpa", 0x24 }, { "jmpn", 0x25 }, { "equa", 0x26 },    { "equn", 0x27 },
  { "biga", 0x28 }, { "bign", 0x29 }, { "bigequa", 0x2A }, { "bigequn", 0x2B },
  { "smaa", 0x2C }, { "sman", 0x2D }, { "smaequa", 0x2E }, { "smaequn", 0x2F },
  { "seta", 0x30 }, { "setn", 0x31 }, { "ina", 0x32 },     { "inn", 0x33 },
  { "outa", 0x34 }, { "outn", 0x35 }, { "outaasc", 0x36 }, { "outnasc", 0x37 }
};

std::unordered_map<std::string, unsigned short> register_map
    = { { "ax", 0x01 }, { "bx", 0x02 }, { "cx", 0x03 }, { "dx", 0x04 },
        { "ex", 0x05 }, { "fx", 0x06 }, { "gx", 0x07 }, { "hx", 0x08 },
        { "ix", 0x09 }, { "jx", 0x0A }, { "kx", 0x0B }, { "lx", 0x0C },
        { "mx", 0x0D }, { "nx", 0x0E }, { "ox", 0x0F } };

// 将ASCII字符转换成16进制数

// 将ASCII字符转换成16进制数的函数
unsigned short
ascii_to_hex (char c)
{
  return static_cast<unsigned short> (c);
}

// 将字符串转换成16进制数的函数
unsigned short
string_to_hex (const std::string &str)
{
  unsigned short value;
  std::stringstream ss;
  ss << std::hex << str;
  ss >> value;
  return value;
}
// 读取指定行的函数
inline std::vector<std::string>
readaline (std::string &filename, unsigned short index)
{
  index=index+1; // 行号从1开始，数组下标从0开始
  std::ifstream file (filename); // 读取一行
  std::vector<std::string> result;

  if (file.is_open ())
    {
      std::string line;
      unsigned short currentLine = 0;

      while (std::getline (file, line))
        {
          currentLine++;
          if (currentLine == index)
            {
              std::istringstream iss (line);
              std::string word;
              while (iss >> word)
                {
                  result.push_back (word);
                }
              break;
            }
        }
      file.close ();
    }
  return result;
}
// 增加索引函数
index_s
add_indexs (std::string file_name)
{
  index_s added_indexs;
  for (unsigned short i = 0; i < MAX; i++)
    {
      std::vector<std::string> aline= readaline (file_name, i);
      if (aline[0][0] == '_')
        {
          if (aline[0] == "_datastart_")
            {
              added_indexs.index_of_data_start = i;
            }
          if (aline[0] == "_dataend_")
            {
              added_indexs.index_of_data_end = i;
            }
          if (aline[0] == "_codestart_")
            {
              added_indexs.index_of_code_start = i;
            }
          if (aline[0] == "_codeend_")
            {
              added_indexs.index_of_code_end = i;
              break;
            }
        }
    }
  return added_indexs;
}
// 读取数据部分的函数
void
read_data (std::string file_name, index_s indexs)
{
  std::ifstream file (file_name);
  if (file.is_open ())
    {
      for (int i = indexs.index_of_data_start + 1;
           i < indexs.index_of_data_end; i++)
        {
          std::vector<std::string> aline = readaline (file_name, i);
          if (!aline.empty ())
            {
              unsigned short address_index
                  = string_to_hex (aline[0]); // 将地址转换为16进制
              for (size_t j = 1; j < aline.size (); j++)
                {
                  if (aline[j].find ("0x") == 0)
                    {
                      // 如果是以0x开头的字符串，将其转换为16进制数存储
                      address[address_index++] = string_to_hex (aline[j]);
                    }
                  else
                    {
                      // 如果是普通字符串或单个字符，将其转换为ASCII的16进制数存储
                      for (char c : aline[j])
                        {
                          address[address_index++] = ascii_to_hex (c);
                        }
                    }
                }
            }
        }
      file.close ();
    }
}
// 读取代码段并解析指令、参数，存储到address向量的函数
void
read_code (std::string file_name, index_s indexs)
{
  std::ifstream file (file_name); // 打开文件
  std::unordered_map<std::string, unsigned short>
      flag_map;                     // 存储标志和索引的映射
  unsigned short index = CODE_FROM; // 初始索引值

  if (file.is_open ())
    {
      for (int i = indexs.index_of_code_start +1 ;
           i < indexs.index_of_code_end; i++)
        {
          std::vector<std::string> aline
              = readaline (file_name, i); // 读取一行
          if (!aline.empty ())
            {
              if (aline[0].front () == '.')
                {
                  // 如果行以'.'开头，处理为标志，并存储索引
                  flag_map[aline[0].substr (1)] = index;
                  continue; // 不自增index
                }
              else
                {
                  // 处理命令，将命令字符串转换为16进制数并存储
                  address[index] = command_map[aline[0]];
                  // 预处理参数1，计算'[]'的数量并去除
                  int brackets = 0;
                  while (aline[1].find ('[') != std::string::npos)
                    {
                      aline[1].erase (aline[1].find ('['), 1);
                      aline[1].erase (aline[1].find (']'), 1);
                      brackets++;
                    }
                  address[index + 2] = brackets; // 存储'[]'的数量
                  // 处理参数1，转换为16进制数或查找标志索引
                  if (register_map.find (aline[1]) != register_map.end ())
                    {
                      address[index + 1] = register_map[aline[1]];
                    }
                  else if (aline[1].front () == '.')
                    {
                      address[index + 1] = flag_map[aline[1].substr (1)];
                    }
                  else
                    {
                      address[index + 1] = string_to_hex (aline[1]);
                    }
                  // 处理参数2，与参数1类似
                  while (aline[1].find ('[') != std::string::npos)
                    {
                      aline[1].erase (aline[1].find ('['), 1);
                      aline[1].erase (aline[1].find (']'), 1);
                      brackets++;
                    }
                  if (aline.size () > 2)
                    {
                      if (register_map.find (aline[2]) != register_map.end ())
                        {
                          address[index + 3] = register_map[aline[2]];
                        }
                      else if (aline[2].front () == '.')
                        {
                          address[index + 3] = flag_map[aline[2].substr (1)];
                        }
                      else
                        {
                          address[index + 3] = string_to_hex (aline[2]);
                        }
                    }
                  else
                    {
                      address[index + 3] = 0; // 如果参数2不存在，补0
                    }

                  index += 5; // 自增index以处理下一条指令
                }
            }
        }
      file.close (); // 关闭文件
    }
}

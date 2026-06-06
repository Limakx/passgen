// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Limakx

#define VERSION "1.0.0"

#include<iostream>
#include<fstream>
#include<algorithm>
#include<cstring>
#include<chrono>
#include<random>

bool quit_flag = false;     // Global quit flag, shared between InitRandom() and main()

/**
 * @brief Show help message
 * @param program_name The name of the program which is running (argv[0])
 */
void ShowHelp(const char* program_name)
{
    std::cout<<"Usage: "<<program_name<<" [OPTIONS] [LENGTH] [COUNT] [OUTPUT_FILE]"<<std::endl<<std::endl;
    std::cout<<"Generate random passwords with high entropy"<<std::endl<<std::endl;
    std::cout<<"Options:"<<std::endl;
    std::cout<<" -l <N>     Set password length (default: 64)"<<std::endl;
    std::cout<<" -c <N>     Set number of passwords to generate (default: 1)"<<std::endl;
    std::cout<<" -o <FILE>   Set passwords to file in specified file (default: print to console)"<<std::endl;
    std::cout<<" -h         Show help message"<<std::endl<<std::endl;
    std::cout<<"Examples:"<<std::endl;
    std::cout<<program_name<<"                           # 1 password, length 64 ,print to console"<<std::endl;
    std::cout<<program_name<<" -l 128                    # 1 password, length 128 ,print to console"<<std::endl;
    std::cout<<program_name<<" -l 128 -c 5               # 5 password, length 128 ,print to console"<<std::endl;
    std::cout<<program_name<<" -o output.txt             # 1 password, length 64 ,print to output.txt"<<std::endl;
    std::cout<<program_name<<" -l 128 -c 5 -o output.txt # 5 password, length 128 ,print to output.txt"<<std::endl<<std::endl;
    std::cout<<"Notes:"<<std::endl;
    std::cout<<" - Passwords contain A-Z, a-z, 1-9, !@#$%^&*_-=+?, but 0, I, l, 1 are excluded to avoid confusion"<<std::endl;
    std::cout<<" - Version: "<<VERSION<<std::endl;
    std::cout<<" - Author: Limakx<youlunkubiaoke@yeah.net>"<<std::endl;
}

/**
 * @brief Convert a string to a number, return -1 if any non-digit is encountered
 * @param source_string The string (C-style) which will be converted
 * @return The converted number or -1 if conversion fails
 */
int StringToNumber(const char* source_string)
{
    int result_number = 0;
    for (int i = 0; i < strlen(source_string); i++)
    {
        if (source_string[i] >= '0' && source_string[i] <= '9')
          result_number = result_number * 10 + (source_string[i] - '0');
        else
          return -1;
    }
    return result_number;
}

/**
 * @brief Initialize the random number generator
 * @return The initialized random number generator
 */
std::mt19937 InitRandom(void)
{
    std::random_device random_seed;
    if (random_seed.entropy() == 0)
    {
        std::cout<<"Warning: cannot obtain true random numbers form system entropy source."<<std::endl<<std::endl;
        std::cout<<"Possible causes:"<<std::endl;
        std::cout<<" -your CPU does not support RDRAND"<<std::endl;
        std::cout<<" -std::random_device is not supported by this compiler"<<std::endl<<std::endl;
        std::cout<<"Would you like to use time-base seed (may produce weaker passwords):(y/n) "<<std::endl;
        if (getchar() != 'y')
        {
            quit_flag = true;
            return std::mt19937(0);
        }
        else
          return std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count());     // If true random numbers cannot be obtained, use time-based seed
    }
    return std::mt19937(random_seed());
}

/**
 * @brief Generate a random password
 * @param length The length of the password to generate
 * @param random_engine The random number generator to use
 * @return The generated password
 */
std::string GeneratePassword(int length, std::mt19937& random_engine)
{
    const std::string character_set="ABCDEFGHJKLMNPQRSTUVWXYZ"
    "abcdefghijkmnpqrstuvwxyz"
    "23456789"
    "!@#$%^&*_-=+?";
    std::string password;
    std::uniform_int_distribution<size_t> distribution(0,character_set.length() - 1);
    for (int i = 0;i < length; i++)
    {
        password = password + character_set[distribution(random_engine)];
    }
    return password;
}

/**
 * @brief Clear the password from memory securely
 * @param password The password string which will be cleared
 */
void SecureClear(std::string& password)
{
    volatile char* p = &password[0];     // Avoid compiler optimization
    for (int i = 0; i < password.length(); i++)
    {
        p[i] = 0;
    }
    password.clear();
    password.shrink_to_fit();     // Release allocated memory
}

int main(int argc, char* argv[])
{
    bool enable_length = false, enable_count = false, has_outputfile = false;
    int length = 0, count = 0;
    std::string outputfile_name, password;
    std::ofstream output_file;
    std::mt19937 random_engine=InitRandom();

    if (quit_flag == true)
      return 1;

    if (argc == 1)     // When no command-line aguments are provided
    {
        std::cout<<"Length of passwords: (no default) ";
        std::cin>>length;
        if (length <= 0)
        {
            std::cout<<"Error: length must be positive"<<std::endl;
            return 1;
        }
        enable_length = true;
        std::cout<<"Count of passwords: (no default) ";
        std::cin>>count;
        if (count <= 0)
        {
            std::cout<<"Error: count must be positive"<<std::endl;
            return 1;
        }
        enable_count = true;
        std::cout<<"Name of output file: (Input \'n\' to skip) ";
        std::cin>>outputfile_name;
        if (outputfile_name != "n")
        {
            has_outputfile = true;
            output_file.open(outputfile_name, std::ios::out);
            if (output_file.is_open() != true)
            {
                std::cout<<"Error: cannot open file"<<std::endl;
                return 1;
            }
        }
    }

    for (int i = 1; i < argc; i++)     // Get CLI arguments
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            ShowHelp(argv[0]);
            return 0;
        }
        else
        {
            if (strcmp(argv[i], "-l") == 0)
            {
                enable_length = true;
                if (i + 1 < argc && StringToNumber(argv[i + 1]) != -1)
                  length = StringToNumber(argv[i + 1]);
                else
                {
                    std::cout<<"Error: option -l expects a positive integer argument"<<std::endl;
                    return 1;
                }
                i++;
                continue;
            }
            else
            {
                if (strcmp(argv[i], "-c") == 0)
                {
                    enable_count = true;
                    if (i + 1 < argc && StringToNumber(argv[i + 1]) != -1)
                    count = StringToNumber(argv[i + 1]);
                    else
                    {
                        std::cout<<"Error: option -c expects a positive integer argument"<<std::endl;
                        return 1;
                    }
                    i++;
                    continue;
                }
                else
                {
                    if (strcmp(argv[i], "-o") == 0)
                    {
                        has_outputfile = true;
                        if (i + 1 < argc)
                        {
                            output_file.open(argv[i + 1], std::ios::out);
                            outputfile_name = argv[i + 1];
                            if (output_file.is_open() != true)
                            {
                                std::cout<<"Error: cannot open file"<<std::endl;
                                return 1;
                            }
                        }
                        else
                        {
                            std::cout<<"Error: option -o expects a filename argument"<<std::endl;
                            return 1;
                        }
                        i++;
                        continue;
                    }
                }
            }
        }
    }

    if (enable_length == false)
      length = 64;
    if (enable_count == false)
      count =1;
    std::cout<<"Passwords:"<<std::endl<<std::endl;

    for (int i = 1; i <= count; i++)     // Provide passwords
    {
        password = GeneratePassword(length, random_engine);
        std::cout<<password<<std::endl<<std::endl;
        if (has_outputfile == true)
        {
            output_file<<password<<std::endl<<std::endl;
        }
        SecureClear(password);
    }

    output_file.close();
    return 0;
}
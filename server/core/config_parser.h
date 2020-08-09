#ifndef SERVER_CORE_CONFIG_PARSER_H_
#define SERVER_CORE_CONFIG_PARSER_H_
#include "yaml-cpp/yaml.h"
#define MAX_STRING_SIZE 100


struct ServerConfig{
    char log_dir[MAX_STRING_SIZE];
    char public_dir[MAX_STRING_SIZE];
    char cgi_dir[MAX_STRING_SIZE];

    int log_level=-1;
};


void ParseConfig(const char* config_file,
        ServerConfig* server_config);

ServerConfig* GetGlobalConfig();


#endif

#include "server/core/config_parser.h"
#include <string.h>
#include <string>




void ParseConfig(const char* config_file, ServerConfig* server_config){
    YAML::Node config = YAML::LoadFile(config_file);
    // set string
    strcpy(server_config->log_dir, config["log_dir"].as<std::string>().data());
    strcpy(server_config->public_dir, config["public_dir"].as<std::string>().data());
    strcpy(server_config->cgi_dir, config["cgi_dir"].as<std::string>().data());

    // set int
    server_config->log_level = config["log_level"].as<int>();
}


ServerConfig* GetGlobalConfig(){
    // singleton
    static ServerConfig server_config;
    return &server_config;
}

#include <stdlib.h>
#include <iostream>
#include <rpc/RcomClient.h>
#include "util/ClockAccessor.h"
#include "util/Logger.h"

void print(nlohmann::json& obj)
{
        std::string s = obj.dump(4);
        std::cout << s << std::endl;
}


nlohmann::json python_eval(const std::string& expression)
{
        nlohmann::json response;
        romi::RPCError error;
        
        nlohmann::json params{
                {"expression", expression}
        };

        
        r_debug("python.eval('%s')", expression.c_str());
        auto rpc = romi::RcomClient::create("python", 30);
        rpc->execute("eval", params, response, error);
        
        if (error.code == 0) {
                if (response["error"]["code"] == 0) {
                        return response.at("result");
                } else {
                        throw std::runtime_error(response["error"]["message"]);
                }
        } else {
                        throw std::runtime_error(error.message.c_str());
        }
}

const char *get_expression(int argc, char **argv)
{
        if (argc < 2) {
                throw std::runtime_error("Usage: rpython 'expression'");
        }
        return argv[1];
}

int main(int argc, char **argv)
{
        (void) argc;
        (void) argv;
        
        try {
                const char *expression = get_expression(argc, argv);
                nlohmann::json result = python_eval(expression);
                print(result);
                
        } catch (std::exception& e) {
                std::cerr << "Caught exception: " << e.what() << std::endl;
        }
        return 0;
}

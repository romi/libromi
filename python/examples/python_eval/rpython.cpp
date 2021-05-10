#include <stdlib.h>
#include <iostream>
#include <rpc/RcomClient.h>
#include <ClockAccessor.h>

void print(JsonCpp& obj)
{
        std::string s;
        obj.tostring(s, k_json_pretty);
        std::cout << s << std::endl;
}


JsonCpp python_eval(const std::string& expression)
{
        JsonCpp response;
        romi::RPCError error;
        
        JsonCpp params = JsonCpp::construct("{\"expression\": \"%s\"}",
                                            expression.c_str());
        
        r_debug("python.eval('%s')", expression.c_str());
        auto rpc = romi::RcomClient::create("python", 30);
        rpc->execute("eval", params, response, error);
        
        if (error.code == 0) {
                if (response.get("error").num("code") == 0) {
                        //print(response);
                        return response.get("result");
                } else {
                        throw std::runtime_error(response.get("error").str("message"));
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
                JsonCpp result = python_eval(expression);
                print(result);
                
        } catch (std::exception& e) {
                std::cerr << "Caught exception: " << e.what() << std::endl;
        }
        return 0;
}

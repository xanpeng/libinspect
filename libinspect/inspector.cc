#include <fstream>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "mime_types.h"
#include "inspector.h"

namespace libinspect {

std::map<std::string, Inspector::Callback> Inspector::commands_;
std::map<std::string, std::string> Inspector::help_info_;

Inspector::Inspector(std::string port) : server_(port) {
  server_.RegisterRequestHandler(Inspector::HandleRequest);
}

void Inspector::AddContent(
    const std::string& command,
    const std::string& help,
    const Callback& cb) {
  commands_[command] = cb;
  help_info_[command] = help;
}

void Inspector::RemoveContent(const std::string& command) {
  commands_.erase(command);
  help_info_.erase(command);
}

void Inspector::Start() {
  server_.Run();
}

void Inspector::HandleRequest(const Request& req, Reply& rep) {
  using std::string;

  string request_path;
  if (!RequestHandler::DecodeUrl(req.uri, request_path)) {
    rep = Reply::StockReply(Reply::bad_request);                                                                       
    return;                                                                                                            
  }                                                                                                                    

  if (request_path.empty() || request_path[0] != '/'
      || request_path.find("..") != std::string::npos) {                                                               
    rep = Reply::StockReply(Reply::bad_request);                                                                       
    return;                                                                                                            
  }                                                                                                                    

  string content;

  // TODO make this more reasonable
  // handle root path
  if (request_path[request_path.size() - 1] == '/') {                                                                  
    for (std::map<string, string>::iterator it = help_info_.begin();
        it != help_info_.end();
        ++it) {
      content += it->first;
      content += "\t";
      content += it->second;
      content += "\n";
    }
  }                                                                                                                    
  else {
    std::size_t last_slash_pos = request_path.find_last_of("/");
    string command = request_path.substr(last_slash_pos + 1);
    std::map<string, Callback>::const_iterator it = commands_.find(command);
    if (it != commands_.end() && it->second) {
      // TODO accept arguments from request
      ArgList args;
      const Callback& cb = it->second;
      content = cb(args);
    }
    else {
      content += "'" + command + "'" + " not implemented";
    }
  }

  // Fill out the reply to be sent to the client.                                                                      
  rep.status = Reply::ok;                                                                                              
  rep.content.append(content);

  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());                                         
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = mime_types::ExtensionToType();
}

}

#ifndef LIBINSPECT_INSPECTOR_H
#define LIBINSPECT_INSPECTOR_H

#include <map>
#include <vector>
#include <string>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include "http_server.h"

namespace libinspect {

class Inspector : private boost::noncopyable {
public:
  typedef std::vector<std::string> ArgList;
  typedef boost::function<std::string (const ArgList& args)> Callback;

  explicit Inspector(std::string port);

  void AddContent(
      const std::string& command,
      const std::string& help,
      const Callback& cb);
  void RemoveContent(const std::string& command);

  static void HandleRequest(const libinspect::Request& req, libinspect::Reply& rep);

  void Start();
private:

  HttpServer server_;
  // HandleRequest is static to fit RequestHandler's RegisterRealHandler,
  // so commands_ and help_info_ used by HandleRequest need to be static too.
  static std::map<std::string, Callback> commands_;
  static std::map<std::string, std::string> help_info_;
};

}

#endif


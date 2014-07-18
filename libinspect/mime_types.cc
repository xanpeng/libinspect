#include "mime_types.h"

namespace libinspect { namespace mime_types {

struct Mapping {
  const char* extension;
  const char* mime_type;
} mappings[] = {
  { "gif", "image/gif" },
  { "htm", "text/html" },
  { "html", "text/html" },
  { "jpg", "image/jpeg" },
  { "png", "image/png" },
  { 0, 0 }
};

std::string ExtensionToType(const std::string& extension) {
  for (Mapping* m = mappings; m->extension; ++m) {
    if (m->extension == extension)
      return m->mime_type;
  }
  return "text/plain";
}

}}

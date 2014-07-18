#ifndef LIBINSPECT_MIME_TYPES_H
#define LIBINSPECT_MIME_TYPES_H

#include <string>

namespace libinspect { namespace mime_types {

std::string ExtensionToType(const std::string& extension="plain");

}}

#endif

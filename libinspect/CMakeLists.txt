set(server_SRCS
  connection.cc
  http_server.cc
  inspector.cc
  message.cc
  mime_types.cc
  )

add_library(inspect ${server_SRCS})
target_link_libraries(inspect boost_system boost_thread pthread)


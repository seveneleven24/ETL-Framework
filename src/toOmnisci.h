#include <boost/tokenizer.hpp>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <arrow/api.h>
#include <arrow/io/api.h>
#include <arrow/ipc/api.h>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>
#include "gen-cpp/OmniSci.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

#ifdef HAVE_THRIFT_STD_SHAREDPTR
#include <memory>
namespace mapd 
{
	using std::make_shared;
	using std::shared_ptr;
}
#endif

void setToThrift(std::string user_name, std::string passwd, std::string db_name, std::string table_name, std::string inString);


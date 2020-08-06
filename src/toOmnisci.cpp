#include "toOmnisci.h"

void setToThrift(std::string user_name, std::string passwd, std::string db_name, std::string table_name, std::string inString)
{
	std::string server_host("localhost");  
	int port = 6274;
	int h;
	mapd::shared_ptr<TTransport> socket(new TSocket(server_host, port));
	mapd::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	mapd::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	OmniSciClient client(protocol);
	TSessionId session;
	try 
	{
		transport->open();                                    
		client.connect(session, user_name, passwd, db_name);  
		client.load_table_binary_arrow(session, table_name, inString);
		client.disconnect(session);  
		transport->close(); 

	} catch (TOmniSciException& e) {
		std::cerr << e.error_msg << std::endl;
		return;
	} catch (TException& te) {
		std::cerr << "Thrift error: " << te.what() << std::endl;
		return;
	}
	return;
}
						   

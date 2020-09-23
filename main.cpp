/*
1. int main					-> A simple terminal where an user can defined a server port
2. int initserver			-> Create the server listening connection
3. static void ev_handler	-> Serve static html files
*/


// Include Mongoose
#include "mongoose.h";
#include <string>
#include <iostream>


// Struct containg the setting for how to connect server HTTp eith mongoose;
static struct mg_serve_http_opts s_http_server_opts;


//Event handler
static void ev_handler(struct mg_connection *nc, int ev, void *p)
{
	//if event is a htt Request

	if (ev == MG_EV_HTTP_REQUEST)
	{
		//Server static html files
		mg_serve_http(nc,(struct http_message *)p, s_http_server_opts);
	}
}


int initserver(int port)
{
	//Mongoose Event manager
	struct mg_mgr mgr;

	//Mongoose connection
	struct mg_connection* nc;

	//Convert port to char
	std::string portToChar = std::to_string(port);
	static char const* sPort = portToChar.c_str();

	//Init mongoose
	mg_mgr_init(&mgr, NULL);
	std::cout << "Starting web server on port " << std::endl;

	nc = mg_bind(&mgr, sPort, ev_handler);

	//if the Connection Fails
	if (nc==NULL)
	{
		std::cout << "Failed to create listener" << std::endl;
		return 1;
	}

	//Set up HTTP server options
	mg_set_protocol_http_websocket(nc);
	s_http_server_opts.document_root = ".";
	s_http_server_opts.enable_directory_listing = "yes";
	for (;;)
	{
		mg_mgr_poll(&mgr, 1000);
	}

	//Free up all the memory that hase been Allocated up to this point
	mg_mgr_free(&mgr);
}

int main(void)
{
	int port;
	std::cout << "Select server port" << std::endl;

	std::cin >> port;

	//fail case
	if (std::cin.fail())
	{
		port = 1000;
	}

	initserver(port);

}
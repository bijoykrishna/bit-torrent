int StartPeer(ubyte2 srvPort, ubyte2 cliPort, ubyte2 cliDwnPort);

void ReceiveFile(int, char*);

void *connection_download_nb_handler(void *socket_desc);

void *connection_upload_nb_handler(void *socket_desc);



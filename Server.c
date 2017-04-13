#include "Utility.h"
#include "Server.h"
static char arrcFilename[] = "";

int StartFileOwner(ubyte2 usSrvPort)
{
    pthread_t thread_id;
    uint uiSocket = 0;
    int iRet = -1;

    iRet = CreateSocket(&uiSocket);
    if(iRet == CNT_FAILURE)
    {
        printf("Failed to create socket.\n");
    }

    iRet = BindAddress(uiSocket, "127.0.0.1", usSrvPort);
    if(iRet == CNT_FAILURE)
    {
        printf("Failed to bind socket.\n");
    }

    //    char arrcFilename[FILENAME_SIZE];

    printf("Enter Filename:\n");
    scanf("%s", arrcFilename);

    SplitFile(arrcFilename);

    uint uiCliSocket = AcceptConnectionInServer(uiSocket);

    while(uiCliSocket)
    {
        printf("Socket accept %d\n", iRet);
        /* Create a new thread */
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &uiCliSocket) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        uiCliSocket = AcceptConnectionInServer(uiSocket);
    }

    return CNT_SUCCESS;
}

void *connection_handler(void *socket_desc)
{
    uint uiCliSocket = *(int*)socket_desc;
    // SplitFile("pic.jpg");

    SendFiles(uiCliSocket, arrcFilename);

    return 0;
}

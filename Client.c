#include "Utility.h"
#include "Client.h"

int StartPeer(ubyte2 usFOSrvPort, ubyte2 usSelfSrvPort, ubyte2 usPeerSrvPort)
{
    uint uiSocket = 0;

    pthread_t thread_id;
    CreateSocket(&uiSocket);
    //char writefname[256];
    char arrcFilename[FILENAME_SIZE];

    /* Get file from file owner */
    if(CNT_SUCCESS == OpenConnection(uiSocket, "127.0.0.1", usFOSrvPort))
    {
        ReceiveFile( uiSocket, arrcFilename );
    }

    if( pthread_create( &thread_id , NULL ,  connection_download_nb_handler , (void*) &usSelfSrvPort) < 0)
    {
        perror("could not create thread");
        return 1;
    }
    if( pthread_create( &thread_id , NULL ,  connection_upload_nb_handler , (void*) &usPeerSrvPort) < 0)
    {
        perror("could not create thread");
        return 1;
    }


#if 0
    char temp[256];
    int val = 0;
    int len = strlen(arrcFilename);
    sscanf(arrcFilename, "%s.%d", temp, &val);
    printf("filename  = %s temp = %s VAL = %d\n",arrcFilename, temp, val);
    while(val >0)
    {
        val = val /10;
        len--;
    }
    len--;
    memcpy(temp, arrcFilename, len);
    printf("after\n");
    printf("join files\n");
    sprintf(writefname, "Complete/%s", temp);



    printf("joined name = %s\n", writefname);

   DIR *dir;
   struct dirent *ent;
   /*int fileno = 0;*/
   int file_count = 0;
   
   if ((dir = opendir ("Data")) != NULL)
   {

       while ((ent = readdir(dir)) != NULL) 
       {
           if (ent->d_type == DT_REG) 
           { /* If the entry is a regular file */
              file_count++;
           }
       }
   }
   else 
   {
      /* could not open directory */
      perror ("");
      return EXIT_FAILURE;
   }

   int k = 0;
   FILE *f, *ft;
   f = fopen(writefname, "wb");

   char ch;
   int m = 0;
   for(k = 1; k < file_count; k++)
   {

       sprintf(temp, "%s.%d", writefname, k);
       ft = fopen(temp, "rb");
       for(m = 0; m < 102400; m++)
       {
           ch = fgetc(ft);
           fputc(ch, f);
       }
       fclose(ft);
   }

   fclose(f);


#endif
    return CNT_SUCCESS;


}

void ReceiveFile(int uiSocket, char* pcFilename )
{
    char revbuf[LENGTH];
    char dest[256];
    char arrcSummary[FILENAME_SIZE];
    FILE *fr, *fs;
    int fr_block_sz = 0;

    sprintf(arrcSummary, "Data/Summary.txt");
    fs = fopen(arrcSummary, "wb");

    bzero(revbuf, LENGTH);
    while((fr_block_sz = recv(uiSocket, revbuf, LENGTH, 0)) > 0)
    {
        if(strncmp(revbuf, "Config details file start filename", 25) == 0)
        {
            sscanf(revbuf, "Config details file start filename = %s end", pcFilename);

            printf("rcvd file start\n");
            printf("Received filename at client = %s\n", pcFilename);
            sprintf(dest, "Data/%s",pcFilename);
            //fr_block_sz = sprintf(arrcSummary, "%s", pcFilename);

            //fwrite(arrcSummary, sizeof(char), fr_block_sz, fs);

            //fprintf(fs, "%s", pcFilename);
            fr = fopen(dest, "wb");

            bzero(revbuf, LENGTH);
            continue;
        }
        else if(strncmp(revbuf, "Config details file close filename", 25) == 0) /* or EOF */            
        {
            fclose(fr);

            printf("Here2\n");
            printf("File close\n");

            bzero(revbuf, LENGTH);
            continue;
        }
        
        
        if(strncmp(revbuf, "TCP close filename", 10) == 0) /* or EOF */           
        {

            printf("TCP close rcvd\n");
            bzero(revbuf, LENGTH);
            break;
        }

        int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
        if(write_sz < fr_block_sz)
        {
            printf("File write failed on server.Errno = %d\n", errno);
        }
        bzero(revbuf, LENGTH);
        /*if (fr_block_sz == 0 || fr_block_sz != 512) 
         * {
         * break;
         * }*/
         //write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fs);
    }

    bzero(revbuf, LENGTH);
    printf("Here\n");
    fclose(fs);
    return;
}

void *connection_download_nb_handler(void *socket_desc)
{
    uint uiCliSocket = *(int*)socket_desc;
    char sdbuf[LENGTH];
    int iSent = 0;
    int fs_block_sz = sprintf(sdbuf, "Request for Chunk Id list");
    
    if((iSent = send(uiCliSocket, sdbuf, fs_block_sz, 0)) < 0)
    {
        fprintf(stderr, "ERROR: Failed to send TCP closerrno = )\n");
    }
    if(iSent > 0)
    {
        fflush(stdout);
    }

    return 0;
}

void *connection_upload_nb_handler(void *socket_desc)
{
    uint uiCliSocket = *(int*)socket_desc;
    char sdbuf[LENGTH];
    int iSent = 0;
    int fs_block_sz;
    char revbuf[LENGTH];
    char arrcTemp[FILENAME_SIZE];
    //FILE *fs;

    sprintf(arrcTemp, "Data/Summary.txt");
    //fs = fopen(arrcTemp, "wb");

    while((fs_block_sz = recv(uiCliSocket, revbuf, LENGTH, 0)) > 0)
    {
        if(memcmp(revbuf, "Request for Chunk Id list", 25) == 0)
        {


            //fs_block_sz = sprintf(sdbuf, "");
            
            if((iSent = send(uiCliSocket, sdbuf, fs_block_sz, 0)) < 0)
            {
                fprintf(stderr, "ERROR: Failed to send TCP closerrno = )\n");
            }
            if(iSent > 0)
            {
                fflush(stdout);
            }
        }
    }



    return 0;
}

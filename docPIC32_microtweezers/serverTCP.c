#include "TCPIP Stack/TCPIP.h"
#include <string.h>
#include <p32xxxx.h>
#include "main.h"

#define SERVER_PORT	9760
#define CMDBUFFER_SIZE  100         //arbitrary, assuming no commands will exceed 100 chars

static enum _TCPServerState
{
        SM_HOME = 0,
        SM_LISTENING,
SM_CLOSING,
} TCPServerState = SM_HOME;

static TCP_SOCKET MySocket;
static BOOL lastTCPState = 0;

typedef struct RegisteredCmd RegisteredCmd;
struct RegisteredCmd
{
    char* cmd_str;
    typCmd cmd_typ;
    RegisteredCmd* pNext;
};
static RegisteredCmd* registeredCmd = NULL;


void registerCmd(typCmd cmdTyp, char* cmdStr)
{
    RegisteredCmd* newCmd = malloc(sizeof(RegisteredCmd));
    newCmd->cmd_str = cmdStr;
    newCmd->cmd_typ = cmdTyp;
    newCmd->pNext = registeredCmd;
    registeredCmd = newCmd;
}

void release()
{
    RegisteredCmd* tmp;
    while ( registeredCmd != NULL )
    {
        tmp = registeredCmd->pNext;
        free (registeredCmd);
        registeredCmd = tmp;
    }
}


void ServerCompute(void)
{
    BYTE i;         //i and w2 to test if a SM_CLOSING is required
    WORD w2;

    BYTE TcpBuffer[RT_BUFFERS_SIZE];    //the buffer used to fil the command, same size as the TCP buffer

    switch(TCPServerState)
    {
        case SM_HOME:
            // Allocate a socket for this server to listen and accept connections on
            MySocket = TCPOpen(0, TCP_OPEN_SERVER, SERVER_PORT, TCP_PURPOSE_GENERIC_TCP_SERVER);
            if(MySocket == INVALID_SOCKET)
                    return;
            TCPServerState = SM_LISTENING;
            break;
        case SM_LISTENING:
        {
            // See if anyone is connected to us
            BOOL TCPState = TCPIsConnected(MySocket);


            if (TCPState && !lastTCPState)
            {
                char msg[15]="Init\n";
                ServerSend(msg);
            }
            lastTCPState = TCPState;

            if(!TCPState) return;


            WORD nbrMax;
            WORD nbrRead;

            nbrMax = TCPIsGetReady(MySocket);           //how many char have to be read (<32)
            //printf("nbrMax %d \n",nbrMax);
            while(nbrMax>0)
            {
                nbrRead = TCPGetArray(MySocket,TcpBuffer,nbrMax>RT_BUFFERS_SIZE?RT_BUFFERS_SIZE:nbrMax);	//writting TcpBuffer
                //how many chars have actually been read

                cmdBuffering(TcpBuffer,nbrRead);        //sending the buffer chunk to build a command
                nbrMax = TCPIsGetReady(MySocket);       //getting the new amount of char to be read
                for(w2 = 0; w2 < nbrRead; w2++)         //cheking if their's a TCP problem (black box loop...)
                {
                    if(i == 0x1B)   //escape
                    {
                        TCPServerState = SM_CLOSING;
                    }
                }
            }

            // No need to perform any flush.  TCP data in TX FIFO will automatically transmit itself after it accumulates for a while.
            //If you want to decrease latency (at the expense of wasting network bandwidth on TCP overhead), perform and explicit flush via the TCPFlush() API.
            //TCP_SOCKET hTCP
            TCPFlush(MySocket);
            break;
        }
        case SM_CLOSING:
            // Close the socket connection.
            TCPClose(MySocket);

            TCPServerState = SM_HOME;
            break;
    }
}

//------------------------------------------------------------------------------
//function turning successive tcpBuffers in a complette commmand to be parsed and sent to main.c functions
//------------------------------------------------------------------------------
void cmdBuffering(char* tcpBuffer, int tcpBufferSize)
{
    //printf("%.*s\n", 32, tcpBuffer);
    static char cmdBuffer[CMDBUFFER_SIZE];
    static int  cmdBufferPos;
    int tcpBufferPos = 0;

    while(tcpBufferPos < tcpBufferSize)
    {
        if(cmdBufferPos>CMDBUFFER_SIZE)        //if we're building a too long command, ignorring everything untill the next '\n'
        {
            printf("cmdBufferPos>CMDBUFFER_SIZE\n");
            if(tcpBuffer[tcpBufferPos]=='\n')
            {
                printf("reset cmdBufferPos\n");
                cmdBufferPos=0;
            }
            tcpBufferPos++;
        }
        else
        {
            //printf("%.*s\n", 100, cmdBuffer);
            cmdBuffer[cmdBufferPos]=tcpBuffer[tcpBufferPos];
            if(cmdBuffer[cmdBufferPos]=='\n')
            {
                cmdBuffer[cmdBufferPos]='\0';
                Cmd cmd;
                if(*cmdBuffer != NULL)                  //not to send NULL stuff
                {
                    if (parseCmd(cmdBuffer,&cmd))       //cutting cmdBuffer
                        addCmd(cmd);                    //sendding the new command
                }
                cmdBufferPos = 0;                       //reinitializing the cmdBuffer building
            }
            else
            {
                cmdBufferPos++;
            }
            tcpBufferPos++;
        }
    }
}

void ServerStop(void)
{
    TCPServerState = SM_CLOSING;
}

void ServerSend(const char* msg)
{
    if (TCPServerState == SM_LISTENING)
    {
        TCPPutArray(MySocket, (BYTE*)msg, strlen(msg));
        TCPFlush(MySocket);
    }
}

//------------------------------------------------------------------------------
//cutting a command in arguments
//------------------------------------------------------------------------------
int parseCmd(char* cmd_str, Cmd* cmd)
{
    //count nb args
    int nbArgs = 0;
    char* str_ptr = cmd_str;

    while ( ( str_ptr = memchr(str_ptr,':', strlen(str_ptr)) ) != NULL )
    {
        nbArgs++;
        str_ptr++;
    }

    // get command string
    char *str_cmd = strtok(cmd_str, ":");
    RegisteredCmd* itRegCmd = registeredCmd;

    while ( itRegCmd != NULL && cmd_str != NULL && strcmp(str_cmd, itRegCmd->cmd_str) )
    {
        itRegCmd = itRegCmd->pNext;
    }
    if (itRegCmd != NULL)
    {
        cmd->cmd_typ = itRegCmd->cmd_typ;
        //printf("%s\n", (char*)itRegCmd->cmd_typ); how to ?
        cmd->argc = nbArgs;
        if (nbArgs > 0)
        {
            // get args
            char **str_arg = malloc(sizeof(char*)*nbArgs);
            char* arg;
            int i = 0;
            do
            {
                arg = strtok(NULL,":");
                if (arg != NULL)
                {
                    //printf("%s\n", arg);
                    str_arg[i] = malloc(sizeof(char)* (strlen(arg)+1));
                    strcpy(str_arg[i], arg);
                }
                i++;
            } while (arg);

            cmd->args = str_arg;
        }
        return 1;
    }
    return 0;
}
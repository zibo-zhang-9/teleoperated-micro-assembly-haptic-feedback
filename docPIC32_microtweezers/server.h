/** \file server.h
 * Header file containing all the function prototypes needed to deal with TCP buffers.
 */

#ifndef SERVER_H
#define	SERVER_H

#ifdef	__cplusplus
extern "C" {
#endif

/**********************************************************************//**
 * @brief  Turns successive tcpBuffers in a complete commmand to be parsed and sent to main.c functions.
 *
 * @ref addCmd(Cmd cmd) is used to send the command when it's complete
 *
 * @param [in] tcpBuffer the successive buffers read and turned into commands.
 * @param [in] tcpBufferSize the size of tcpBuffer.
**************************************************************************/
void cmdBuffering(char* tcpBuffer, int tcpBufferSize);


/**********************************************************************//**
 * @brief  Deals with the buffers received by TCP.
 *
 * uses a call to @ref cmdBuffering(TcpBuffer,nbrRead) to build commands from TCP messages.
**************************************************************************/
void ServerCompute(void);

/**********************************************************************//**
 * @brief Makes the PIC32 send a 32 characters array via TCP.
 *
 * @param [in] msg if longer than 32 char in the array, nothing will be sent.
**************************************************************************/
void ServerSend(const char* msg);

/**********************************************************************//**
 * @brief  Cuts commands into arguments.
 *
 * @param [in] cmd_str the command to be cut.
 * @param [out] cmd the buffer to store the arguments
**************************************************************************/
int parseCmd(char* cmd_str, Cmd* cmd);



void registerCmd(typCmd cmdTyp, char* cmdStr);
void release();
void ServerStop(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SERVER_H */


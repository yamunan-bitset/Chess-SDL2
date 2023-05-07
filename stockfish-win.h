#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <windows.h>
#include <stdio.h>
#include <memory.h>

STARTUPINFO sti = {0};
SECURITY_ATTRIBUTES sats = {0};
PROCESS_INFORMATION pi = {0};
HANDLE pipin_w, pipin_r, pipout_w, pipout_r;
BYTE buffer[2048];
DWORD writ, excode, read, available;

void ConnectToEngine(char* path)
{
   pipin_w = pipin_r = pipout_w = pipout_r = NULL;
   sats.nLength = sizeof(sats);
   sats.bInheritHandle = TRUE;
   sats.lpSecurityDescriptor = NULL;

   CreatePipe(&pipout_r, &pipout_w, &sats, 0);
   CreatePipe(&pipin_r, &pipin_w, &sats, 0);
         
   sti.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
   sti.wShowWindow = SW_HIDE;
   sti.hStdInput = pipin_r;
   sti.hStdOutput = pipout_w;
   sti.hStdError = pipout_w;

   CreateProcess(NULL, path, NULL, NULL, TRUE,0, NULL, NULL, &sti, &pi);
}


char* GetNextMove(const char* position)
{     
    char* str = (char*) malloc(strlen("bestmove aNxbM ponder cPxdQ"));
    char* str1 = "position startpos moves";
    char* str3 = "\ngo movetime 2000\n";
    char* strc = (char*) malloc(1 + strlen(str1) + strlen(position) + strlen(str3));
    strcpy(strc, str1);
    strcpy(strc, position);
    strcpy(strc, str3); 

    WriteFile(pipin_w, strc, strlen(strc), &writ, NULL);
    Sleep(500);
        
    PeekNamedPipe(pipout_r, buffer,sizeof(buffer), &read, &available, NULL);   
    do
    {   
        ZeroMemory(buffer, sizeof(buffer));
        if (!ReadFile(pipout_r, buffer, sizeof(buffer), &read, NULL) || !read) break; 
        buffer[read] = 0;
        strcpy(str, (char*) buffer);
    } while(read >= sizeof(buffer));

    if (strstr(str, "bestmove") != NULL)
    {
        char* ret = "";
        //sprintf(ret, "%c%c%c%c%c", str[9], str[10], str[11], str[12], str[13]);
        printf("%c%c%c%c%c\n", str[9], str[10], str[11], str[12], str[13]);
        return ret;
    }
    return (char*) "error";
}


void CloseConnection()
{
    WriteFile(pipin_w, "quit\n", 5,&writ, NULL);
    if(pipin_w != NULL) CloseHandle(pipin_w);
    if(pipin_r != NULL) CloseHandle(pipin_r);
    if(pipout_w != NULL) CloseHandle(pipout_w);
    if(pipout_r != NULL) CloseHandle(pipout_r);
    if(pi.hProcess != NULL) CloseHandle(pi.hProcess);
    if(pi.hThread != NULL) CloseHandle(pi.hThread);
}

#endif

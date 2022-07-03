#include "WorkerSDK/improbable/c_io.h"

void Io_Stream_Destroy(Io_Stream* StreamToDestroy){}

int Io_Stream_Flush(Io_Stream *Stream){return -1;}

char* Io_Stream_GetLastError(Io_Stream *Stream) {return nullptr;}

Io_Stream* Io_CreateRotatingFileStream(Io_RotatingFileStreamParameters *Param){return nullptr;}

Io_Stream* Io_CreateFileStream(char* fullname,Io_OpenMode mode){return nullptr;}
 

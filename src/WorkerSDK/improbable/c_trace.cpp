#include "WorkerSDK/improbable/c_trace.h"

Trace_SpanIdType Trace_SpanId_IsNull(const Trace_SpanIdType Id[TRACE_SPAN_ID_SIZE_BYTES]){return -1;}

Trace_SpanIdType* Trace_SpanId_Null() {return nullptr;}

void Trace_Query_Destroy(Trace_Query* Quer){}

uint32_t Trace_GetSerializedItemSize(const Trace_Item* Item){ return 0;}

int Trace_SerializeItemToStream(Io_Stream* Stream,const Trace_Item* Item, uint32_t ItemSize){return 0;}

char* Trace_GetLastError(){return nullptr;}

void Trace_EventTracer_ClearActiveSpanId(Trace_EventTracer *EventTracer){}

void Trace_EventTracer_SetActiveSpanId( Trace_EventTracer* InEventTracer,const Trace_SpanIdType* id ){}

Trace_Query* Trace_ParseSimpleQuery(char* str){ return nullptr;}

bool Trace_EventTracer_ShouldSampleSpan(Trace_EventTracer* EventTracer, const Trace_SpanIdType* Causes, int32_t NumCauses,Trace_Event* Event){return false;}

Trace_EventData* Trace_EventData_Create(){return nullptr;}

void Trace_EventData_Destroy(Trace_EventData *EventData) {}

void Trace_EventTracer_AddSpan(Trace_EventTracer* EventTracer, const Trace_SpanIdType* Causes, int32_t NumCauses,Trace_Event* Event, const Trace_SpanIdType* id){}

bool Trace_EventTracer_PreFilterAcceptsEvent(Trace_EventTracer* EventTracer, Trace_Event* Event){return false;}

Trace_EventTracer* Trace_EventTracer_Create(Trace_EventTracer_Parameters *Param){return nullptr;}

void Trace_EventTracer_AddEvent(Trace_EventTracer* EventTracer,Trace_Event* Event){}

void Trace_EventData_AddStringFields(Trace_EventData* EventData, int num, const char** Key, const char** Value){}

void Trace_EventTracer_Destroy(Trace_EventTracer* EventTracer){}

uint32_t Trace_GetNextSerializedItemSize(Io_Stream *Stream){return 0;}

Trace_Item* Trace_Item_GetThreadLocal(){return nullptr;}

int8_t Trace_DeserializeItemFromStream(Io_Stream *Stream, Trace_Item* Item, uint32_t BytesToRead){return 0;}

 

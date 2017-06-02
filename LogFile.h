#ifndef LogFile_h
#define LogFile_h

#include <inttypes.h>
#include <cstdio>
#include "Log.h"

/** Logging class. Default implementation ignores logging messages. This is suitable for something with no filesystem
 *  like an Arduino, but an actual implementation will be needed to record data in a file or set of files. Can cover
 *  any quadrant on the logging space, text/binary and single/multiple streams.
 *
 *  A packet consists of a packet identifier followed by a fixed structure of data. All packets with the same identifier
 *  should have the same structure. We follow the CCSDS specification and call this packet identifer the APID, Application
 *  Process ID
 *
 *  Text/binary is pretty much what it sounds like. Text could be something like CSV or NMEA sentences. Binary is something
 *  like CCSDS packets, where the binary data is just concatenated and stuffed in the packet stream.
 *
 *  For single/multiple streams, use the APID to identify which stream this packet goes into. So for instance, startPacket
 *  would look up which stream this packet goes into and makes subsequent writes go to it.
 */
class LogFile:public Log {
private:
  void getRecordPath();
  static char recordPath[256];
protected:
  FILE* stream;
public:
  LogFile(const char* name, bool bufEnabled);
  virtual ~LogFile();
};

#endif /* Log_File_h */

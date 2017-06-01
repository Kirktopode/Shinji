#include "LogCSV.h"

void LogCSV::start(int apid, const char* pktName) {
  pktApid=apid;
  firstField=true;
  if(hasDoc[apid]) {
    //Write directly to the stream
    fbuf=stream;
  } else {
    //Write the packet to the buffer, so we can write the documentation to the stream
    fbuf=fmemopen(buf,sizeof(buf),"w");
  }
}

void LogCSV::write(const char* value, int len, const char* fieldName) {
  writeDoc(fieldName);
  if(!firstField) fprintf(fbuf,",");
  for(int i=0;i<len;i++) fprintf(fbuf,"%02x",value[i]);
  firstField=false;
}

void LogCSV::write(const char* value, const char* fieldName) {
  writeDoc(fieldName);
  fprintf(fbuf,firstField?"%s":",%s",value);
  firstField=false;
}

void LogCSV::end() {
  if(hasDoc[pktApid]||!inDoc) { //True if the packet was already documented or we didn't have any documentation
    fprintf(stream,"\n"); //Write the linefeed for the packet
  } else {
    fprintf(stream,"\n"); //Write the linefeed for the packet documentation
    fclose(fbuf);         //close the in-memory buffer for the packet data
    if(inDoc) fprintf(stream,"%s\n",buf); //write the packet data to the stream, with its linefeed
  }
  hasDoc[pktApid]=true; //Either way, our chance to document has ended
}

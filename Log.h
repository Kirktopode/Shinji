#ifndef Log_h
#define Log_h

#include <inttypes.h>

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
class Log {
protected:
  /** Number of APIDs allowed. The possible APIDs run from 0 to n_apid-1
   *  inclusive. APIDs equal or greater than n_apid will write off the
   *  end of at least one array. */
  static const int nApid=64;
  /** Indicates whether a packet APID has been documented yet. Once
   *  the first packet of a given APID has been finished, the documentation
   *  for that packet is written in a packet-type specific way, and
   *  further writes of packets of this APID will not be documented again.
   */
  bool hasDoc[nApid];
public:
  Log() {};
  virtual ~Log() {};
  /** Start a packet with a particular APID
   \param[in] apid APID to use for this packet
   \param[in] ptkName Name of this packet, used for self-documentation purposes.
                      Default value is null, and if null, then no attempt is made
                      to document this packet and any previous definition remains.
                      If the packet is documented twice and the documentation is
                      the same, the packet writer is required to act as if the packet
                      was only documented once. This means that you can leave
                      the documentation on a packet writer in a loop.
   */
  virtual void start(int apid, const char* pktName=nullptr)=0;
  /** Add a signed 8-bit value to the packet
   \param[in] value value to write
   \param[in] fieldName Name of this field, used for self-documentation purposes.
                      Default value is null, and if null, then no attempt is made
                      to document this packet and any previous definition remains.
                      If the packet is documented twice and the documentation is
                      the same, the packet writer is required to act as if the packet
                      was only documented once. This means that you can leave
                      the documentation on a packet writer in a loop.
   */
  virtual void write(int8_t value, const char* fieldName=nullptr)=0;
  /** Add a signed 16-bit value to the packet
   \param[in] value value to write
   \param[in] fieldName Name of this field, used for self-documentation purposes.
                      Default value is null, and if null, then no attempt is made
                      to document this packet and any previous definition remains.
                      If the packet is documented twice and the documentation is
                      the same, the packet writer is required to act as if the packet
                      was only documented once. This means that you can leave
                      the documentation on a packet writer in a loop.
   */
  virtual void write(int16_t value, const char* fieldName=nullptr)=0;
  /** Add a signed 32-bit value to the packet
   \param[in] value value to write
   \param[in] fieldName Name of this field, used for self-documentation purposes.
                      Default value is null, and if null, then no attempt is made
                      to document this packet and any previous definition remains.
                      If the packet is documented twice and the documentation is
                      the same, the packet writer is required to act as if the packet
                      was only documented once. This means that you can leave
                      the documentation on a packet writer in a loop.
   */
  virtual void write(int32_t value, const char* fieldName=nullptr)=0;
  /** Add an unsigned 8-bit value to the packet
   \param[in] value value to write
   \param[in] fieldName Name of this field, used for self-documentation purposes.
                      Default value is null, and if null, then no attempt is made
                      to document this packet and any previous definition remains.
                      If the packet is documented twice and the documentation is
                      the same, the packet writer is required to act as if the packet
                      was only documented once. This means that you can leave
                      the documentation on a packet writer in a loop.
   */
  virtual void write(uint8_t value, const char* fieldName=nullptr)=0;
  /** Add an unsigned 16-bit value to the packet
   \param[in] value value to write
   \param[in] fieldName Name of this field, used for self-documentation purposes.
                      Default value is null, and if null, then no attempt is made
                      to document this packet and any previous definition remains.
                      If the packet is documented twice and the documentation is
                      the same, the packet writer is required to act as if the packet
                      was only documented once. This means that you can leave
                      the documentation on a packet writer in a loop.
   */
  virtual void write(uint16_t value, const char* fieldName=nullptr)=0;
  /** Add an unsigned 32-bit value to the packet
   \param[in] value value to write
   \param[in] fieldName Name of this field, used for self-documentation purposes.
                      Default value is null, and if null, then no attempt is made
                      to document this packet and any previous definition remains.
                      If the packet is documented twice and the documentation is
                      the same, the packet writer is required to act as if the packet
                      was only documented once. This means that you can leave
                      the documentation on a packet writer in a loop.
   */
  virtual void write(uint32_t value, const char* fieldName=nullptr)=0;
  /** Add a single-precision floating-point value to the packet
   \param[in] value value to write
   \param[in] fieldName Name of this field, used for self-documentation purposes.
                      Default value is null, and if null, then no attempt is made
                      to document this packet and any previous definition remains.
                      If the packet is documented twice and the documentation is
                      the same, the packet writer is required to act as if the packet
                      was only documented once. This means that you can leave
                      the documentation on a packet writer in a loop.
   */
  virtual void write(float value, const char* fieldName=nullptr)=0;
  /** Add a double-precision floating-point value to the packet
   \param[in] value value to write
   \param[in] fieldName Name of this field, used for self-documentation purposes.
                      Default value is null, and if null, then no attempt is made
                      to document this packet and any previous definition remains.
                      If the packet is documented twice and the documentation is
                      the same, the packet writer is required to act as if the packet
                      was only documented once. This means that you can leave
                      the documentation on a packet writer in a loop.
   */
  virtual void write(double value, const char* fieldName=nullptr)=0;
  /** Add a byte buffer to the packet. This is intended to be used for arbitrary
      binary data. The length information is not required to be written to the 
      packet, so in packets that are not self-delimiting (say CCSDS) you may
      need to write a length. ASCII packet generators will generally transform
      this to printable characters (such as hex or base85).
   \param[in] value value to write
   \param[in] len number of bytes to write
   \param[in] fieldName Name of this field, used for self-documentation purposes.
                      Default value is null, and if null, then no attempt is made
                      to document this packet and any previous definition remains.
                      If the packet is documented twice and the documentation is
                      the same, the packet writer is required to act as if the packet
                      was only documented once. This means that you can leave
                      the documentation on a packet writer in a loop.
   */
  virtual void write(const char* value, int len, const char* fieldName=nullptr)=0;
  /** Add a null-terminated string to the packet. This is intended to be used
      for ASCII text strings. This will generally be dumped to the packet as-is,
      so be careful if you are using a delimited packet such as CSV or NMEA. The 
      null terminator is not required to be written to the packet, so you may need
      to write a length before or null terminator after this string.
   \param[in] value value to write
   \param[in] fieldName Name of this field, used for self-documentation purposes.
                      Default value is null, and if null, then no attempt is made
                      to document this packet and any previous definition remains.
                      If the packet is documented twice and the documentation is
                      the same, the packet writer is required to act as if the packet
                      was only documented once. This means that you can leave
                      the documentation on a packet writer in a loop.
   */
  virtual void write(const char* value, const char* fieldName=nullptr)=0;
  /** End a packet */
  virtual void end()=0;
  //These follow IDL types when possible just as a reference
  static const int t_u8    = 1; ///< Field type is unsigned 8-bit int
  static const int t_i16   = 2; ///< Field type is signed 16-bit int
  static const int t_i32   = 3; ///< Field type is signed 32-bit int
  static const int t_u16   =12; ///< Field type is unsigned 16-bit int
  static const int t_u32   =13; ///< Field type is unsigend 32-bit int
  static const int t_float = 4; ///< Field type is IEEE754 single-precision 32-bit float
  static const int t_double= 5; ///< Field type is IEEE754 double-precision 64-bit float
  static const int t_string= 7; ///< Field is a byte string of arbitrary length. No length info provided, it must be provided elsewhere. Intent is ASCII text
  static const int t_binary=10; ///< Field is a byte string of arbitrary length. No length info provided, it must be provided elsewhere. We steal the pointer type from IDL
};

#endif /* LOG_H_ */

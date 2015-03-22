#ifndef CLICK_LISPGENMAPREPLY_HH
#define CLICK_LISPGENMAPREPLY_HH
#include "LISPStructs.hh"
#include <click/element.hh>
#include <clicknet/udp.h>
#include <clicknet/ether.h>
#include <click/args.hh>

CLICK_DECLS

/*
=c

LISPGenMapReplyOuter(ITRADDR)

=s LISPGenMapReplyOuter

Generates the outer common bytes of a MapReply packet.

=d
The outer common bytes for a MapReply request correspond to the first 4 bytes of the packet.

  Type:   2 (Map-Reply)

   P: This is the probe-bit, which indicates that the Map-Reply is in
      response to a Locator reachability probe Map-Request.  The 'Nonce'
      field MUST contain a copy of the nonce value from the original
      Map-Request.  See Section 6.3.2 for more details.

   E: This bit indicates that the ETR that sends this Map-Reply message
      is advertising that the site is enabled for the Echo-Nonce Locator
      reachability algorithm.  See Section 6.3.1 for more details.

   S: This is the Security bit.  When set to 1, the following
      authentication information will be appended to the end of the
      Map-Reply.  The detailed format of the Authentication Data Content
      is for further study.

     0                   1                   2                   3
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |    AD Type    |       Authentication Data Content . . .       |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

   Reserved:  This field MUST be set to 0 on transmit and MUST be
      ignored on receipt.

   Record Count:  This is the number of records in this reply message.
      A record is comprised of that portion of the packet labeled
      'Record' above and occurs the number of times equal to Record
      Count.
=a
https://tools.ietf.org/html/rfc6830#section-6.1.4
 */

class LISPGenMapReply : public Element {
public:
	LISPGenMapReply() ;
	~LISPGenMapReply() ;

	const char *class_name() const { return "LISPGenMapReplyOuter"; }
	const char *port_count() const { return "1/1"; }
	const char *processing() const { return PULL; }

	void setOuterHeader(LISPMapReply*); // set outer header fixed bytes
	void setInnerHeader(LISPMapReply*); // set inner header fixed bytes
	void setLocator(LISPMapReply*);	    // set locator field fixed bytes

	Packet *simple_action(Packet* p);
};

CLICK_ENDDECLS
#endif

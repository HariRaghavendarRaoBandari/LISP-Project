#ifndef LISPSTRUCTS_H
#define LISPSTRUCTS_H

#include <arpa/inet.h>

/*
 * Values for the Type field.
 */
#define LISP_H_TYPE_0 0	// Reserved
#define LISP_H_TYPE_1 1	// Map-Request
#define LISP_H_TYPE_2 2	// Map-Reply
#define LISP_H_TYPE_3 3	// Map-Register
#define LISP_H_TYPE_4 4	// Map-Notify
#define LISP_H_TYPE_8 8	// Encapsulated Control Message

/*
 * Values for AFI (Address Family Identifier) fields.
 * @see https://www.iana.org/assignments/address-family-numbers/address-family-numbers.xhtml
 */
#define AFI_IPV4 htons(1)
#define AFI_IPV6 htons(2)

/*
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Type=3 |P|            Reserved               |M| Record Count  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

struct LISPMapRegisterOuterHeader {
#if CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN
	/*
	 * The Reserved field is splitted to align the fields on 8bits.
	 */
	unsigned int Reserved_1 : 3; // max value 7
	unsigned int P : 1;
	unsigned int Type : 4;
	unsigned int Reserved_2 : 8; // max value 255
	unsigned int M : 1;
	unsigned int Reserved_3 : 7; // max value 127
	unsigned int Record_Count : 8;
#else
#error "Only little endian is supported"
#endif
} CLICK_SIZE_PACKED_ATTRIBUTE;

struct LISPMapRegister {
	struct LISPMapRegisterOuterHeader header;
	uint32_t nonce1;
	uint32_t nonce2;
} CLICK_SIZE_PACKED_ATTRIBUTE;

/*
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Type=1 |A|M|P|S|p|s|    Reserved     |   IRC   | Record Count  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
struct LISPMapRequestOuterHeader {
#if CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN
	unsigned int S : 1;
	unsigned int P : 1;
	unsigned int M : 1;
	unsigned int A : 1;
	unsigned int Type : 4;
	unsigned int Reserved_1 : 6; // max value 0x3F
	unsigned int p : 1;
	unsigned int s : 1;
	unsigned int IRC : 5;
	unsigned int Reserved_2 : 3; // max value 7
	unsigned int Record_Count : 8;
#else
#error "Only little endian is supported"
#endif
} CLICK_SIZE_PACKED_ATTRIBUTE;

/*
 *      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |                              ...                              |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |         Source-EID-AFI        |   Source EID Address  ...     |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |         ITR-RLOC-AFI 1        |    ITR-RLOC Address 1  ...    |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |                              ...                              |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |         ITR-RLOC-AFI n        |    ITR-RLOC Address n  ...    |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   / |   Reserved    | EID mask-len  |        EID-Prefix-AFI         |
 * Rec +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   \ |                       EID-Prefix  ...                         |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |                   Map-Reply Record  ...                       |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * If Source-EID-AFI is 0, then "Source EID Address" don't exist.
 * Here we assume that Source-EID-AFI will always be 0.
 */
struct LISPMapRequestInnerHeader {
#if CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN
	unsigned int Source_EID_AFI : 16;
	unsigned int ITR_RLOC_AFI : 16;
	unsigned int ITR_RLOC_Address : 32;
	unsigned int Reserved : 8;
	unsigned int EID_mask_len : 8;
	unsigned int EID_prefix_AFI : 16;
	unsigned int EID_prefix : 32;
	unsigned int Map_Reply_Record : 32; // Unused in a "pure" Map-Request
#else
#error "Only little endian is supported"
#endif
} CLICK_SIZE_PACKED_ATTRIBUTE;

struct LISPMapRequest {
	struct LISPMapRequestOuterHeader header;
	uint32_t nonce1;
	uint32_t nonce2;
	struct LISPMapRequestInnerHeader inner;
} CLICK_SIZE_PACKED_ATTRIBUTE;

/*
       Map-Reply Message Format
        0                   1                   2                   3
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |Type=2 |P|E|S|          Reserved               | Record Count  |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                         Nonce . . .                           |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                         . . . Nonce                           |
   +-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |   |                          Record TTL                           |
   |   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   R   | Locator Count | EID mask-len  | ACT |A|      Reserved         |
   e   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   c   | Rsvd  |  Map-Version Number   |       EID-Prefix-AFI          |
   o   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   r   |                          EID-Prefix                           |
   d   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  /|    Priority   |    Weight     |  M Priority   |   M Weight    |
   | L +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | o |        Unused Flags     |L|p|R|           Loc-AFI             |
   | c +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  \|                             Locator                           |
   +-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/



/*
 * struct Outer Header
 */

struct LISPMapReplyOuterHeader {

#if CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN
	unsigned int Reserved1:		1;
	unsigned int S:			1;
	unsigned int E:			1;
	unsigned int P:			1;
	unsigned int Type:		4;
	
#elif CLICK_BYTE_ORDER == CLICK_BIG_ENDIAN
	unsigned int Type:		4;
	unsigned int P:			1;
	unsigned int E:			1;
	unsigned int S:			1;
	unsigned int Reserved1:		1;
#else
#error "Undefined Byte Order!"
#endif
	unsigned int Reserved2:		16;
	unsigned int Record_Count:	8;

} CLICK_SIZE_PACKED_ATTRIBUTE;

/*
 *	struct Inner Header 
 */

struct LISPMapReplyInnerHeader {

	unsigned int Record_TTL: 	32;
	unsigned int Locator_Count: 	8;
	unsigned int EID_Mask_Len: 	8;

#if CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN
	unsigned int Reserved1:		4;
	unsigned int A:			1;
	unsigned int ACT:		3;

	unsigned int Reserved2:		8;

	unsigned int Map_Version1:	4;
	unsigned int Reserved3:		4; // Rsvd
	
	unsigned int Map_Version2:	8;

	unsigned int EID_Prefix_AFI:	8;

	unsigned int EID_Prefix:	16;

#elif CLICK_BYTE_ORDER == CLICK_BIG_ENDIAN
	unsigned int ACT:		3;
	unsigned int A:			1;
	unsigned int Reserved1:		4;

	unsigned int Reserved2:		8;

	unsigned int Reserved3:		4; // Rsvd
	unsigned int Map_Version1:	4;

	unsigned int Map_Version2:	8;

	unsigned int EID_Prefix_AFI:	8;

	unsigned int EID_Prefix:	16;
#else
#error "Undefined Byte Order!"
#endif
} CLICK_SIZE_PACKED_ATTRIBUTE;

struct LISPMapReplyLoc {

	unsigned int Priority:		8;

	unsigned int Weight:		8;

	unsigned int M_Priority:	8;

	unsigned int M_Weight:		8;

	unsigned int UnunsedFlags1:	8;

#if CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN

	unsigned int R:			1;
	unsigned int p:			1;
	unsigned int L:			1;
	unsigned int UnusedFlags2:	5;

#elif CLICK_BYTE_ORDER == CLICK_BIG_ENDIAN

	unsigned int UnusedFlags2:	5;
	unsigned int L:			1;
	unsigned int p:			1;
	unsigned int R:			1;
#else
#error "Undefined Byte Order!"
#endif

	unsigned int Loc_AFI:		16;
	unsigned int Locator:		32;
	

} CLICK_SIZE_PACKED_ATTRIBUTE;


struct LISPMapReply {
	struct LISPMapReplyOuterHeader oh;
	uint32_t nonce1;
	uint32_t nonce2;
	struct LISPMapReplyInnerHeader ih;
	struct LISPMapReplyLoc loc;
} CLICK_SIZE_PACKED_ATTRIBUTE;

#endif

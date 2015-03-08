#ifndef LISPSTRUCTS_H
#define LISPSTRUCTS_H

#include <arpa/inet.h>
#include <stdint.h>

/*
 * Values for the Key_ID field.
 */

#define KEY_ID_NONE 		htons(0)
#define KEY_ID_HMAC_SHA_1_96	htons(1) // defined in [RFC2404]
#define KEY_ID_HMAC_SHA_256_128 htons(2) // defined in [RFC4868]

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
 *      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |Type=1 |A|M|P|S|p|s|    Reserved     |   IRC   | Record Count  |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |                              ... Nonce                        |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |                         Nonce ...                             |
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
struct LISPMapRequest {
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
	uint32_t nonce1;
	uint32_t nonce2;
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

/*
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   |                          Record TTL                           |
 *  |   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  R   | Locator Count | EID mask-len  | ACT |A|      Reserved         |
 *  e   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  c   | Rsvd  |  Map-Version Number   |       EID-Prefix-AFI          |
 *  o   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  r   |                          EID-Prefix                           |
 *  d   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |  /|    Priority   |    Weight     |  M Priority   |   M Weight    |
 *  | L +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  | o |        Unused Flags     |L|p|R|           Loc-AFI             |
 *  | c +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |  \|                             Locator                           |
 *  +-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
struct LISPLocatorHeader {
/*
#if CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN
	unsigned int Priority : 8;
	unsigned int Weight : 8;
	unsigned int M_Priority : 8;
	unsigned int M_Weight : 8;
	unsigned int Unused_Flags : 13;
	unsigned int L : 1;
	unsigned int P : 1;
	unsigned int R : 1;
	unsigned int Loc_AFI : 16;
	unsigned int Locator : 32;
#else
#error "Only little endian is supported"
#endif
*/
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

struct LISPRecordLocatorHeader {
/*
#if CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN
	unsigned int Record_TTL : 32;
	unsigned int Locator_count : 8;
	unsigned int EID_mask_len : 8;
	unsigned int Rsvd1 : 4;
	unsigned int A : 1;
	unsigned int ACT : 3;
	unsigned int Reserved: 8;
	unsigned int Rsvd2 : 4;
	unsigned int Map_Version_number : 12;
	unsigned int EID_Prefix_AFI : 16;
	unsigned int EID_Prefix : 32; // 32 bits (4 bytes) for ipv4, 128 bits (16 bytes) for ipv6
	struct LISPLocatorHeader loc;
#else
#error "Only little endian is supported"
#endif
*/
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
#elif CLICK_BYTE_ORDER == CLICK_BIG_ENDIAN
	unsigned int ACT:		3;
	unsigned int A:			1;
	unsigned int Reserved1:		4;
	unsigned int Reserved2:		8;
	unsigned int Reserved3:		4; // Rsvd
	unsigned int Map_Version1:	4;
#else
#error "Undefined Byte Order!"
#endif
	unsigned int Map_Version2:	8;
	unsigned int EID_Prefix_AFI:	8;
	unsigned int EID_Prefix:	16;
} CLICK_SIZE_PACKED_ATTRIBUTE;

/*
 *      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |Type=3 |P|            Reserved               |M| Record Count  |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |                              ... Nonce                        |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |                         Nonce ...                             |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |            Key ID             |  Authentication Data Length   |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     ~                     Authentication Data                       ~
 * +-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |   |                          Record TTL                           |
 * |   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * R   | Locator Count | EID mask-len  | ACT |A|      Reserved         |
 * e   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * c   | Rsvd  |  Map-Version Number   |       EID-Prefix-AFI          |
 * o   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * r   |                          EID-Prefix                           |
 * d   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  /|    Priority   |    Weight     |  M Priority   |   M Weight    |
 * | L +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | o |        Unused Flags     |L|p|R|           Loc-AFI             |
 * | c +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  \|                             Locator                           |
 * +-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
struct LISPMapRegister {
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
	uint32_t nonce1;
	uint32_t nonce2;
	unsigned int Key_Id : 16; // 16 bits but we use a mask here
	unsigned int Authentication_Data_Length : 16; // number of bytes for Authentication_Data field -- 16 bits but we use a mask here
	unsigned int Authentication_Data : 32;  // uncomment and modify if needed
	struct LISPRecordLocatorHeader recLoc;
#else
#error "Only little endian is supported"
#endif
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
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Type=2 |P|E|S|          Reserved               | Record Count  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
struct LISPMapReplyOuterHeader {
#if CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN
	/*
	 * The Reserved field is splitted to align the fields on 8bits.
	 */
	unsigned int Reserved_1 : 1; // max value 1
	unsigned int S : 1;
	unsigned int E : 1;
	unsigned int P : 1;
	unsigned int Type : 4;
#elif CLICK_BYTE_ORDER == CLICK_BIG_ENDIAN
	unsigned int Type : 4;
	unsigned int P : 1;
	unsigned int E : 1;
	unsigned int S : 1;
	unsigned int Reserved_1 : 1; // max value 1
#else
#error "Undefined Byte Order!"
#endif
	unsigned int Reserved_2 : 8; // max value 255
	unsigned int Reserved_3 : 8; // max value 255
	unsigned int Record_Count : 8;
} CLICK_SIZE_PACKED_ATTRIBUTE;


/* TODO remove
 *	struct Inner Header 
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
#elif CLICK_BYTE_ORDER == CLICK_BIG_ENDIAN
	unsigned int ACT:		3;
	unsigned int A:			1;
	unsigned int Reserved1:		4;
	unsigned int Reserved2:		8;
	unsigned int Reserved3:		4; // Rsvd
	unsigned int Map_Version1:	4;
#else
#error "Undefined Byte Order!"
#endif
	unsigned int Map_Version2:	8;
	unsigned int EID_Prefix_AFI:	8;
	unsigned int EID_Prefix:	16;
} CLICK_SIZE_PACKED_ATTRIBUTE;
 */

/* TODO remove
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
*/

struct LISPMapReply {
	struct LISPMapReplyOuterHeader oh;
	uint32_t nonce1;
	uint32_t nonce2;
	struct LISPMapReplyInnerHeader ih;
	struct LISPMapReplyLoc loc;
} CLICK_SIZE_PACKED_ATTRIBUTE;

#endif

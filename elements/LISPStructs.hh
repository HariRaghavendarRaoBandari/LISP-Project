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
#else
#error "Only little endian is supported"
#endif
} CLICK_SIZE_PACKED_ATTRIBUTE;

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

#endif

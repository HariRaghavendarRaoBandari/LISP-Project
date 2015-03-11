#ifndef LISPSTRUCTS_H
#define LISPSTRUCTS_H

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


struct LISPHeader {

#if CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN
	/*
     *  0                   1                   2                   3
     *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
     *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     *  |N|L|E|V|I|flags|            Nonce/Map-Version                  |
     *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     *  |                 Instance ID/Locator-Status-Bits               |
     *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 */

    unsigned int flags : 3; // Les 3 bits du champs flags sont réservé à un usage futur
    unsigned int I : 1; /* Instance ID bit : Quand = 1, les 'Locator-Status-Bits' sont réduit à 8 bits et 24 bits de poid fort
    					 * sont un Instance ID. Si L = 0, alors 8 bits de poid faible sont des 0 et ignorés à la reception */
    
    unsigned int V : 1; // Map-Version present bit : Quand = 1 vérif Map-Version et si V = 1 alors N = 0
    unsigned int E : 1; // Echo-nonce-request bit : Quand = 1 ET N = 1, un ITR demande à l'ETR de retourner le Nonce pour verif chemin. 
    unsigned int L : 1; // Locator-Status-Bits bit : Quand = 1, les Locator-Status-Bits des seconds 32 bits sont utilisés.
    unsigned int N : 1; // Nonce-bit : Quand = 1 les 24 bits de poids faible des 32 premiers bits contiennent un Nonce
 

	unsigned int firstLine_1 : 8; // 24 bits de poids faible des 32 premiers bits du header LISP. Peuvent être Nonce ou Map-Version
	unsigned int firstLine_2 : 8;
	unsigned int firstLine_3 : 8;
	unsigned int secondLine : 32; // 32 seconds bits du header LISP. Peuvent être Instance ID ou Locator-Status-Bits

#else
#error "Only little endian is supported"
#endif
} CLICK_SIZE_PACKED_ATTRIBUTE;

#endif






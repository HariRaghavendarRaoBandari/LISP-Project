#ifndef LISPSTRUCTS_H
#define LISPSTRUCTS_H

/*
 * A collection of handy types.
 */
typedef struct { unsigned value : 4; } uint4_t;
typedef struct { unsigned value : 18; } uint18_t;

/*
 * Values for Type field.
 */
#define LISP_H_TYPE_0 00000	// Reserved
#define LISP_H_TYPE_1 00001	// Map-Request
#define LISP_H_TYPE_2 00010	// Map-Reply
#define LISP_H_TYPE_3 00011	// Map-Register
#define LISP_H_TYPE_4 00100	// Map-Notify
#define LISP_H_TYPE_8 01000	// Encapsulated Control Message

/*
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Type=3 |P|            Reserved               |M| Record Count  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
struct LISPMapRegisterOuterHeader {
	uint4_t Type,
	bool P,
	uint18_t Reserved,
	bool M,
	uint8_t Record_Count
};

#endif

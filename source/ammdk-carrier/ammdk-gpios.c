struct gpiotest {
	GPIO_Type *base;
	uint8_t pin_ix;
	PORT_Type *port;
	clock_ip_name_t clock;
};
#define NO_GPIO_LINE {0}
#define GPIO_LINE_OLD(port, ix) {GPIO ## port , ix , PORT ## port, kCLOCK_Port ## port }

//I numbered them starting a
#define GPIO_ARRAY_ELT(LETTER, NUMBER, IX) [IX-1] = GPIO_LINE_OLD(LETTER, NUMBER),
#define GPIO_VAR_DECL(LETTER, NUMBER, IX) int gpio_ ## LETTER ## NUMBER = IX-1;
#define AMMDK_GPIOS(X)\
	/*J16*/\
	X(D, 6, 1)\
	X(D, 4, 3)\
	X(D, 5, 4)\
	X(C, 6, 5)\
	X(C, 9, 6)\
	X(A, 4, 7)\
	X(C, 0, 8)\
	X(B, 1, 9)\
	X(C, 1, 10)\
	X(A, 8, 12)\
	X(B, 4, 13)\
	X(B, 3, 14)\
	X(B, 11, 15)\
	X(B, 10, 16)\
	X(B, 6, 17)\
	X(B, 7, 18)\
	X(B, 2, 19)\
	X(B, 5, 20)\
	X(B, 0, 21)\
	X(A, 11, 22)\
	X(A, 13, 23)\
	X(A, 14, 24)\
	X(A, 17, 25)\
	X(A, 15, 26)\
	X(A, 10, 27)\
	X(E, 25, 29)\
	X(A, 12, 30)\
	X(A, 7, 32)\
	X(E, 24, 34)\
	/*J17*/\
	X(D, 11, 41)\
	X(D, 12, 43)\
	X(C, 14, 44)\
	X(D, 9, 45)\
	X(C, 4, 46)\
	X(D, 8, 47)\
	X(C, 11, 48)\
	X(D, 7, 49)\
	X(C, 10, 50)\
	X(C, 15, 51)\
	X(C, 3, 52)\
	X(C, 2, 54)\
	X(B, 21, 55)\
	X(B, 23, 56)\
	X(B, 20, 57)\
	X(B, 22, 58)\
	X(B, 18, 59)\
	X(B, 19, 60)\
	X(E, 2, 61)\
	X(D, 10, 62)\
	X(E, 0, 63)\
	X(E, 1, 64)\
	X(E, 4, 65)\
	X(E, 3, 66)\
	X(E, 6, 67)\
	X(E, 5, 68)\
	X(E, 8, 69)\
	X(E, 7, 70)\
	X(E, 11, 71)\
	X(E, 12, 72)\
	X(C, 17, 73)\
	X(C, 16, 74)\
	X(E, 26, 77)\
	X(E, 28, 78)

struct gpiotest gpios[] = {
	AMMDK_GPIOS(GPIO_ARRAY_ELT)
};

AMMDK_GPIOS(GPIO_VAR_DECL);



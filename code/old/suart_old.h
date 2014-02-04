#ifndef _SUART_H_
#define _SUART_H_

#include <stdint.h>
//http://www.mikrocontroller.net/topic/131795

//TODO: adapt this suart with fifo, because its code is much cleaner, and it should offer better performance
//http://www.mikrocontroller.net/topic/125651

// aktiviere TXD
#define SUART_TXD
// aktiviere RXD
#define SUART_RXD

	void suart_init();

#ifdef SUART_TXD
		void suart_putc(uint8_t data);
//void suart_putc (const char);
//void suart_puts (char *s);
#endif	// SUART_RXD

#ifdef SUART_RXD
// signalisieren, dass wieder platz im Puffer ist
	inline void suart_set_CTS()
	{
		// TODO: CTS auf 0 setzen;
		// XXX: Tjaaaa, mit dem Pollin RS232-TTL-Wandler hat weder CTS noch RTS einen Effekt, daher wird der Pin hier garnicht erst gesetzt.
		return;
	}

// lieber direkt auf den fifo zugreifen, wenn er verwendet wird, sonst bringt das inlining auch nix
//\\uint8_t suart_getc_wait ();
//uint8_t suart_getc();
#endif	// SUART_RXD
#endif /* _SUART_H_ */

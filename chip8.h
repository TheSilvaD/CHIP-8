#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>

class chip8 {
	public:
		void initialize();
		void emulateCycle();
		void loadGame(char *);

		bool drawFlag;
		unsigned char key[16];
		unsigned char gfx[64 * 32]; //2048 pixels
	private:
		//Chip 8 has two byte opcodes
		unsigned short opcode;
		//Chip 8 has 4k memory
		unsigned char memory[4096];
		//15 8 bit general purpose registers named V0,V1,..., VE
		//16th register VE is a carry flag
		unsigned char V[16];
		//Index register I and PC which can have a value from 0x000 to 0xFFF
		unsigned short I;
		unsigned short pc;

		unsigned char delay_timer;
		unsigned char sound_timer;

		//16 levels of stack
		unsigned short stack[16];
		unsigned short sp;
};
#endif /* defined(CHIP8_H) */
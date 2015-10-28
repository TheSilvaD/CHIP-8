#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned char chip8_fontset[80] = { 
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip8::initialize() {
	// Initialize registers and memory once

	pc 		= 0x200;	//PC starts at 0x200
	opcode 	= 0;		//Reset current opcode
	I 		= 0;		//Reset index register
	sp		= 0;		//Reset stack pointer

	//Clear display
	for (int i = 0; i < 2048; ++i) {
		gfx[i] = 0x0;
	}
		/*for(int i=0; i < 64; ++i){
		for(int j=0; j < 32; ++j) {
			gfx[i][j]=0x0;
			oldgfx[i][j]=0x0;
		}
	}*/

	drawFlag = true;
	//Clear stack, registers V0-VF, and key
	for (int i = 0; i < 16; ++i) {
		stack[i] = 0x0;
		V[i] = 0x0;
		key[i] = 0x0;
	}

	//Clear memory
	for(int i = 0; i < 4096; ++i)
		memory[i] = 0x0;

	//Load fontset

	for(int i = 0; i < 80; ++i)
		memory[i] = chip8_fontset[i];

	//Reset timers
	delay_timer = 0;
	sound_timer = 0;

	srand(time(NULL));
}

void chip8::emulateCycle() {
	// Fetch Opcode
	opcode = (memory[pc] << 8) | memory[pc + 1];

	// Decode Opcode
	switch(opcode & 0xF000) {
		#include "opcode.h"

		default:
		printf("Unknown opcode: 0x%X\n", opcode);
	}

	// Update timers
	if(delay_timer > 0)
		--delay_timer;

	if(sound_timer > 0){
		if(sound_timer == 1)
			printf("\a");
		--sound_timer;
	}	
}

void chip8::loadGame(char *filename) {
	initialize();
	if(filename == NULL){
		fputs("No file provided\n",stderr);
		exit(EXIT_FAILURE);
	}

	FILE * gameBinary;
	gameBinary = fopen (filename,"rb");
	if(gameBinary == NULL) {
		fputs("Not a valid file\n",stderr);
		exit(EXIT_FAILURE);
	}

	fseek(gameBinary, 0, SEEK_END);
	long fileSize = ftell(gameBinary);
	rewind(gameBinary);

	char *buffer = (char*)malloc(sizeof(char) * fileSize);

	fread(buffer, fileSize, 1, gameBinary);

	for(int i = 0; i < fileSize; ++i)
		memory[i + 512] = buffer[i];
	fclose(gameBinary);
	free(buffer);
}

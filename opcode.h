case 0x0000:
			switch(opcode & 0x000F){
				case 0x0000:
				//Clears screen
					for(int i = 0; i < 2048; ++i)
						gfx[i] = 0x0;
					drawFlag = true;
					pc += 2;
				break;

				case 0x000E:
				//returns from subroutine
					--sp;
					pc = stack[sp];
					pc += 2;
				break;

				default:
					printf("Uknown opcode [0x0000]: 0x%X\n", opcode);
			}
			break;
		
		case 0x1000:
			//drawFlag = true;
			//Jumps to address NNN
			pc = opcode & 0x0FFF;
		break;
		
		case 0x2000:
			//Calls subroutine at NNN
			stack[sp] = pc;
			++sp;
			pc = opcode & 0x0FFF;
		break;
		
		case 0x3000:
			//Skips the next instruction if VX equals NN
			if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
				pc += 4;
			else pc += 2;
		break;

		case 0x4000:
			//Skips the next instruction of VX does not equal NN
			if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
				pc += 4;
			else pc += 2;
		break;

		case 0x5000:
			//Skips the next instruction if VX equals VY
			if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
				pc += 4;
			else pc += 2;
		break;

		case 0x6000:
			//Sets VX to NN
			V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
			pc += 2;
		break;

		case 0x7000:
			//Adds NN to VX
			V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
			pc += 2;
		break;

		case 0x8000:
			//more switch case
			switch(opcode & 0x000F){
				case 0x0000: // VX = VY
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				case 0x0001: // VX = VX|VY
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				case 0x0002: // VX = VX & VY
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				case 0x0003: // VX = VX ^ VY
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				case 0x0004:
					//Adds VY to VX.  VF is set to 1 when there's a carry, 
					//and to 0 when there isn't
					if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
						V[0xF] = 1; //carry
					else
						V[0xF] = 0; //no carry
					V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
					pc +=2;
					break;
				case 0x0005:
					//VY is subtracted from VX. VF is set to 0 when there's a borrow, 
					//and to 1 when there isn't
					if(V[(opcode & 0x00F0) >> 4] > (V[(opcode & 0x0F00) >> 8]))
						V[0xF] = 0; //borrow
					else
						V[0xF] = 1; //no borrow
					V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
					pc +=2;
					break;
				case 0x0006:
					//Shifts VX right by one.  VF is set to the least significant bit of VX before the shift
					V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] >> 1;
					pc += 2;
					break;
				case 0x0007:
					//Sets VX to VY minus VX. VF is set to 0 when there's a borrow, 
					//and to 1 when there isn't
					if(V[(opcode & 0x00F0) >> 4] < (V[(opcode & 0x0F00) >> 8]))
						V[0xF] = 0; //borrow
					else
						V[0xF] = 1; //no borrow
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] -V[(opcode & 0x0F00) >> 8];
					pc +=2;
					break;
				case 0x000E:
					//Shifts VX left by one.  VF is set to the most significant bit of VX before the shift
					V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] << 1;
					pc += 2;
					break;

					default:
					printf("Uknown opcode [0x8000]: 0x%X\n:", opcode);
			}
			break;

		case 0x9000:
			//Skips the instruction if VX does not equal VY
			if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
				pc += 4;
			else pc += 2;
			break;

		case 0xA000:
			//Sets I to address NNN
			I = opcode & 0x0FFF;
			pc += 2;
			break;

		case 0xB000:
			//Jumps to the address NNN plus V0
			pc = (opcode & 0x0FFF) + V[0];
			break;

		case 0xC000:
			//Sets VX to the result of a bitwise and operation on a random number and NN
			V[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);
			pc += 2;
			break;

		case 0xD000: {
			//.........................
			unsigned short x = V[(opcode & 0x0F00) >> 8];
			unsigned short y = V[(opcode & 0x00F0) >> 4];
			unsigned short height = opcode & 0x000F;
			unsigned short pixel;

			V[0xF] = 0;
			for (int yline = 0; yline < height; yline++) {
				pixel = memory[I + yline];
				for(int xline = 0; xline < 8; xline++){
					if((pixel & (0x80 >> xline)) != 0) {
						if(gfx[(x + xline + ((y + yline) * 64))] == 1)
							V[0xF] = 1;
						gfx[x + xline + ((y + yline) * 64)] ^= 1;
					}
				}
			}
			drawFlag = true;
			pc += 2;
		}
			break;

		case 0xE000:
			//more switch
			switch(opcode & 0x00FF){
				case 0x009E:
					// Skips the next instruction if the key stored in VX is pressed
					if(key[V[(opcode & 0x0F00) >> 8]] != 0)
						pc += 4;
					else
						pc += 2;
					break;

				case 0x00A1:
					// Skips the next instruction if the key stored in VX is not pressed
					if(key[V[(opcode & 0x0F00) >> 8]] == 0)
						pc += 4;
					else
						pc += 2;
					break;

				default:
					printf("Uknown opcode [0xE000]: 0x%X\n", opcode);
			}
			break;

		case 0xF000:
			//and more switch....
			switch(opcode & 0x00FF){
				case 0x0007:
					//Sets VX to the value of the delay timer.
					V[(opcode & 0x0F00) >> 8] = delay_timer;
					pc += 2;
					break;
				case 0x000A: {
					//A key press is awaited, and then stored in VX.
					bool keyPress = false;

					for(int i = 0; i < 16; ++i)
						if(key[i] != 0){
							V[(opcode & 0x0F00) >> 8] = i;
							keyPress = true;
						}
					if(!keyPress)
						return;
					pc += 2;
				}
					break;
				case 0x0015:
					//Sets the delay timer to VX.
					delay_timer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;
				case 0x0018:
					//Sets the sound timer to VX.
					sound_timer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;
				case 0x001E:
					//Adds VX to I.
					if(I + V[(opcode & 0x0F00) >> 8] > 0xFFF) //check for overflow
						V[0xF] = 1; //set to 1 in case of overflow
					else
						V[0xF] = 0;
					I += V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;
				case 0x0029:
					//Sets I to the location of the sprite for the character in VX. Characters 0-F
					// (in hexadecimal) are represented by a 4x5 font.
					I = V[(opcode & 0x0F00) >> 8] * 0x5;
					pc += 2;
					break;
				case 0x0033:
					// Stores the Binary-coded decimal representation of VX, with the most significant
					// of three digits at the address in I, the middle digit at I plus 1, and the 
					// least significant digit at I plus 2. (In other words, take the decimal representation
					// of VX, place the hundreds digit in memory at location in I, the tens digit at 
					// location I+1, and the ones digit at location I+2.)
					memory[I]		= V[(opcode & 0x0F00) >> 8] /100;
					memory[I + 1]	= (V[(opcode & 0x0F00) >> 8] / 10) % 10;
					memory[I + 2]	= (V[(opcode & 0x0F00) >> 8] % 100) % 10;
					pc +=2;
					break;
				case 0x0055:
					//Stores V0 to VX in memory starting at address I.
					for(int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
						memory[I + i] = V[i];
					I += ((opcode & 0x0F00) >> 8) + 1;
					pc += 2;
					break;
				case 0x0065:
					//Fills V0 to VX with values from memory starting at address I.
					for(int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
						V[i] = memory[I + i];
					I += ((opcode & 0x0F00) >> 8) + 1;
					pc += 2;
					break;
				default:
					printf("Uknown opcode [0xF000]: 0x%X\n", opcode);
			}
			break;


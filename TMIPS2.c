// Lance Boza, Andy Garcia

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void OP_Code(int i);

typedef struct {
  int op, rx, ry, rz;

} Instruction;

int IM[500];

int main(int argc, char* argv[]){

	FILE *fp;
	char *filename;
	char ch;
	int num;

	if(argc < 2){
		return 1;
	}
	else{
		filename = argv[1];
	}

	fp = fopen(filename,"r");
	int n = 0;

	if(fp){
		// go through every character until end of file, subtract '0' from the character to get the respective int value
		// store it in IM and use the op_code function to print the instructions
		while((ch = fgetc(fp)) != EOF)
		{
			if(n == 0 ||n % 4 == 0)
				printf("\n");
			//printf("%c", ch);
			IM[n] = ch - '0';
			if(n == 0 || n % 4 == 0){
				OP_Code(n);
				if(IM[n] == 7){
          IM[n+1] = 0;
          IM[n+2] = 0;
          IM[n+3] = 0;
					break;
        }
			}
			else{
				printf("%d", IM[n]);
			}
			n++;

		}
	}
	else{
		printf("failed to open");
	}


	//-------------------------------------//
	// the assembler program

	// instruction Register
	Instruction IR;
	//memory address register
	int MAR1;
	int MAR2;

	//data memory
	int DM[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	//register file
	int RF[8] = {0,0,0,0,0,0,0,0};

	//buffers
	int idcu;
	int idx;
	int idy;
	int idz;
	int IFID[4] = {0,0,0,0};
	int excu;
	int exx;
	int exy;
	int exz;
	int IDEX[4] = {0,0,0,0};
	int dmcu;
	int dmx;
	int dmy;
	int dmz;
	int EXDM[4] = {0,0,0,0};
	int wbcu;
	int wbx;
	int wby;
	int wbz;
	int DMWB[4] = {0,0,0,0};

	//program counter
	int PC = 0;

	//halt flag
	int halt = 1;

  //multiply flag to handle the multiplication cases
  int mult = 0;
  if(IM[0] == 5 && IM[8] == 5 && IM[16] == 5){
    mult = 1;
  }

	printf("\nAssmebling Program...\n");
	printf("Program Assembled.\n");


	// executing instructions
	while(halt == 1){


		//display format of the assembler
		printf("\nPC = %d | RF = [%d,%d,%d,%d,%d,%d,%d,%d]", PC, RF[0],RF[1],RF[2],RF[3],RF[4],RF[5],RF[6],RF[7]);
		printf(" | DM =[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]\n", DM[0],DM[1],DM[2],DM[3],DM[4],DM[5],DM[6],DM[7],DM[8],DM[9],DM[10],DM[11],DM[12],DM[13],DM[14],DM[15]);
		printf("IFID = [%d,%d,%d,%d] | ", IFID[0],IFID[1],IFID[2],IFID[3]);
		printf("IDEX = [%d,%d,%d,%d] | ", IDEX[0],IDEX[1],IDEX[2],IDEX[3]);
		printf("EXDM = [%d,%d,%d,%d] | ", EXDM[0],EXDM[1],EXDM[2],EXDM[3]);
		printf("DMWB = [%d,%d,%d,%d]\n", DMWB[0],DMWB[1],DMWB[2],DMWB[3]);
		printf("-------------------------------------------------------------\n");


		//INSTRUCTION FETCH
		MAR1 = PC;

    PC += 4;

    //skip, skips the instructions and changes the buffers accordingly
    if(IM[MAR1] == 9 && mult == 0){
      IFID[0] = 9;
      IFID[1] = 0;
      IFID[2] = 0;
      IFID[3] = 9;

      IDEX[0] = 9;
      IDEX[1] = 0;
      IDEX[2] = 0;
      IDEX[3] = 9;

      EXDM[0] = 9;
      EXDM[1] = 0;
      EXDM[2] = 0;
      EXDM[3] = 9;

      DMWB[0] = 9;
      DMWB[1] = 0;
      DMWB[2] = 0;
      DMWB[3] = 9;

      printf("\nPC = %d | RF = [%d,%d,%d,%d,%d,%d,%d,%d]", PC, RF[0],RF[1],RF[2],RF[3],RF[4],RF[5],RF[6],RF[7]);
  		printf(" | DM =[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]\n", DM[0],DM[1],DM[2],DM[3],DM[4],DM[5],DM[6],DM[7],DM[8],DM[9],DM[10],DM[11],DM[12],DM[13],DM[14],DM[15]);
  		printf("IFID = [%d,%d,%d,%d] | ", IFID[0],IFID[1],IFID[2],IFID[3]);
  		printf("IDEX = [%d,%d,%d,%d] | ", IDEX[0],IDEX[1],IDEX[2],IDEX[3]);
  		printf("EXDM = [%d,%d,%d,%d] | ", EXDM[0],EXDM[1],EXDM[2],EXDM[3]);
  		printf("DMWB = [%d,%d,%d,%d]\n", DMWB[0],DMWB[1],DMWB[2],DMWB[3]);
  		printf("-------------------------------------------------------------\n");

      PC += 4;
      MAR1 = PC;
    }

		IR.op = IM[MAR1];
		IR.rx = IM[MAR1 + 1];
		IR.ry = IM[MAR1 + 2];
		IR.rz = IM[MAR1 + 3];

    // jump
    if(IM[MAR1] == 8){
      MAR1 = (IR.rx * 100) + (IR.ry * 10 )+ IR.rz;
      PC = MAR1;
      IR.op = IM[MAR1 - 4];
  		IR.rx = IM[MAR1 - 3];
  		IR.ry = IM[MAR1 - 2];
  		IR.rz = IM[MAR1 - 1];
    }




		IFID[0] = IR.op;
		IFID[1] = IR.rx;
		IFID[2] = IR.ry;
		IFID[3] = IR.rz;


		//INSTRUCTION DECODE/OPERAND FETCH
		idcu = IFID[0];
		idx = IFID[1];
		idy = IFID[2];
		idz = IFID[3];

		//instruction handling for the IDEX

		//load
		if(idcu == 1){
      IDEX[0] = idcu;
			IDEX[1] = idx;
			IDEX[2] = idy;
			IDEX[3] = idz;
		}
		//add
		if(idcu == 2 && mult == 0){
			int v1 = RF[idy];
			int v2 = RF[idz];
			IDEX[0] = idcu;
			IDEX[1] = idx;
			IDEX[2] = v1;
			IDEX[3] = v2;
		}
    else if(idcu == 2 && mult == 1){
      // if performing the multiplcation operation, we successfully add rather than adding the same values
      int v1 = RF[idx];
      int v2 = RF[idy];
      IDEX[0] = idcu;
			IDEX[1] = idx;
			IDEX[2] = v1;
			IDEX[3] = v2;
    }

		//store
		if(idcu == 3){
			IDEX[0] = idcu;
			IDEX[1] = idx;
			IDEX[2] = idy;
			IDEX[3] = idz;
		}
		//subtract
		if(idcu == 4){
			int v1 = RF[idy];
			int v2 = RF[idz];
			IDEX[0] = idcu;
			IDEX[1] = idx;
			IDEX[2] = v1;
			IDEX[3] = v2;
		}
		//input value from user to R0
		if(idcu == 5){

			IDEX[0] = idcu;
			IDEX[1] = idx;
			IDEX[2] = idy;
			IDEX[3] = 0;
		}
		//output value from R0
		if(idcu == 6){
			IDEX[0] = idcu;
			IDEX[1] = RF[0];
			IDEX[2] = idy;
			IDEX[3] = 0;
		}
		//halt
		if(idcu == 7){
      IDEX[0] = idcu;
			IDEX[1] = 0;
			IDEX[2] = 0;
			IDEX[3] = 0;
		}
		//------------------------

		//EXECUTE
		excu = IDEX[0];
		exx = IDEX[1];
		exy = IDEX[2];
		exz = IDEX[3];
		//load
		if(excu == 1){
      EXDM[0] = excu;
			EXDM[1] = exx;
			EXDM[2] = exy;
			EXDM[3] = exz;
		}
		//add
		if(excu == 2){
			int value3 = exy + exz;

			EXDM[0] = excu;
			EXDM[1] = exx;
			EXDM[2] = 0;
			EXDM[3] = value3;
		}
		//store
		if(excu == 3){
			EXDM[0] = excu;
			EXDM[1] = exx;
			EXDM[2] = exy;
			EXDM[3] = exz;
		}
		//subtract
		if(excu == 4){
			int value3 = exy - exz;

			EXDM[0] = excu;
			EXDM[1] = exx;
			EXDM[2] = 0;
			EXDM[3] = value3;
		}
		//input
		if(excu == 5){
			int input;
			printf("Enter a value: ");
			scanf("%d", &input);

			EXDM[0] = excu;
			EXDM[1] = exx;
			EXDM[2] = exy;
			EXDM[3] = input;
		}
		//output
		if(excu == 6){
			EXDM[0] = excu;
			EXDM[1] = exx;
			EXDM[2] = exy;
			EXDM[3] = exz;
      printf("Outputting R0 to the screen: %d", RF[0]);
		}
		//halt
		if(excu == 7){
      EXDM[0] = excu;
			EXDM[1] = exx;
			EXDM[2] = exy;
			EXDM[3] = exz;
		}
		//------------------------

		//DATA MEMORY
		dmcu = EXDM[0];
		dmx = EXDM[1];
		dmy = EXDM[2];
		dmz = EXDM[3];
		//load
		if(dmcu == 1){
			DMWB[0] = dmcu;
			DMWB[1] = dmx;
			DMWB[2] = dmy;
			DMWB[3] = dmz;
		}
		//add
		if(dmcu == 2){
			DMWB[0] = dmcu;
			DMWB[1] = dmx;
			DMWB[2] = dmy;
			DMWB[3] = dmz;
		}
		//store
		if(dmcu == 3){
			DM[dmz] = RF[dmx];
			DMWB[0] = dmcu;
			DMWB[1] = dmx;
			DMWB[2] = dmy;
			DMWB[3] = dmz;
		}
		//subtract
		if(dmcu == 4){
			DMWB[0] = dmcu;
			DMWB[1] = dmx;
			DMWB[2] = dmy;
			DMWB[3] = dmz;
		}
		//input
		if(dmcu == 5){
			DMWB[0] = dmcu;
			DMWB[1] = dmx;
			DMWB[2] = dmy;
			DMWB[3] = dmz;

		}
		//output
		if(dmcu == 6){
			DMWB[0] = dmcu;
			DMWB[1] = dmx;
			DMWB[2] = dmy;
			DMWB[3] = dmz;
		}
		//halt
		if(dmcu == 7){
      DMWB[0] = dmcu;
			DMWB[1] = dmx;
			DMWB[2] = dmy;
			DMWB[3] = dmz;
      halt = 0;
      printf("\nPC = %d | RF = [%d,%d,%d,%d,%d,%d,%d,%d]", PC, RF[0],RF[1],RF[2],RF[3],RF[4],RF[5],RF[6],RF[7]);
  		printf(" | DM =[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]\n", DM[0],DM[1],DM[2],DM[3],DM[4],DM[5],DM[6],DM[7],DM[8],DM[9],DM[10],DM[11],DM[12],DM[13],DM[14],DM[15]);
  		printf("IFID = [%d,%d,%d,%d] | ", IFID[0],IFID[1],IFID[2],IFID[3]);
  		printf("IDEX = [%d,%d,%d,%d] | ", IDEX[0],IDEX[1],IDEX[2],IDEX[3]);
  		printf("EXDM = [%d,%d,%d,%d] | ", EXDM[0],EXDM[1],EXDM[2],EXDM[3]);
  		printf("DMWB = [%d,%d,%d,%d]\n", DMWB[0],DMWB[1],DMWB[2],DMWB[3]);
  		printf("-------------------------------------------------------------\n");
		}
		//--------------------

		//WRITE BACK
		wbcu = DMWB[0];
		wbx = DMWB[1];
		wby = DMWB[2];
		wbz = DMWB[3];
		//load
		if(wbcu == 1){
      RF[wbx] = DM[wbz];
		}
		//add
		if(wbcu == 2){
			RF[wbx] = wbz;

      if(RF[4] == RF[2] * RF[3]){
        mult = 0;
      }
		}
		if(wbcu == 4){
			RF[wbx] = wbz;
		}
		//input
		if(wbcu == 5){
			RF[0] = wbz;
		}
		//output
	}
}


void OP_Code(int i)
{
  if(IM[i] == 1)
    printf("lw   ");
  else if(IM[i] == 2)
    printf("add  ");
  else if(IM[i] == 3)
    printf("sw   ");
  else if(IM[i] == 4)
    printf("sub  ");
  else if(IM[i] == 5)
    printf("sio1 ");
  else if(IM[i] == 6)
    printf("sio2 ");
  else if(IM[i] == 7)
    printf("halt\n");
  else if(IM[i] == 8)
    printf("jmp ");
  else if(IM[i] == 9)
    printf("beqz ");
  else
    printf("INVALID");

  //if(IM[i].opcode != 7)
    //printf("%d %d %d\n", IM[i].rx, IM[i].ry, IM[i].rz);
}

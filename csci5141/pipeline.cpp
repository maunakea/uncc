#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

bool isArithmeticInstruction(char *);
bool isMemAccessInstruction(char *);
bool isBranchInstruction(char *);
bool isJumpInstruction(char *);

class Register {
public:
   int value;
   bool validBit;
   Register() {
      value = 0;
      validBit = true;
   }
};

// declare the 32 registers
Register R0,  R1,  R2,  R3,  R4,  R5,  R6,  R7, 
		 R8,  R9,  R10, R11, R12, R13, R14, R15,
		 R16, R17, R18, R19, R20, R21, R22, R23,
		 R24, R25, R26, R27, R28, R29, R30, R31;

Register registerArray[] = {
		 R0,  R1,  R2,  R3,  R4,  R5,  R6,  R7, 
		 R8,  R9,  R10, R11, R12, R13, R14, R15,
		 R16, R17, R18, R19, R20, R21, R22, R23,
		 R24, R25, R26, R27, R28, R29, R30, R31
};

class RegisterFile {
public:

	Register *getRegister(char *str) {

		char *tempStr = (char*)malloc(10);
		strcpy (tempStr, str+1);
		return &registerArray[atoi(tempStr)];
	}
};

class Latch {
public:
	char *instruction;
	char *op1String, *op2String, *op3String;
	int   op1Value, op2Value, op3Value;
	bool isEmpty;
	char *name;
	int ic;

	Latch() {
		isEmpty = true;
		instruction = (char*)malloc(10);
		op1String = (char*)malloc(10);
		op2String = (char*)malloc(10);
		op3String = (char*)malloc(10);
	}
	void clear() {
		isEmpty = true;
		instruction[0] = op1String[0] = op2String[0] = op3String[0] = '\0';
		op1Value = op2Value = op3Value = 0;
		ic = 0;

	}
	void setName(char *s) {
		name = s;
	}
	void print() {
		cout << "latch" << name << ": ";
		if (isEmpty) {
			cout << "is empty";
		} else if (strcmp(name, "IFtoOF") == 0) {
			if (isArithmeticInstruction(instruction))
				cout << instruction << " " << op1String << " "
					 << op2String   << " " << op3String;
			else if (isMemAccessInstruction(instruction))
				cout << instruction << " " << op1String << " " << op2String;
			else if (isBranchInstruction(instruction))
				cout << instruction << " " << op1String;
			else if (isJumpInstruction(instruction))
				cout << instruction << " " << op1String;
		} else if (strcmp(name, "OFtoEX") == 0) {
			if (isArithmeticInstruction(instruction))
				cout << instruction << " " << op1String << " "
					 << op2Value   << " " << op3Value;
			else if (isMemAccessInstruction(instruction)) {
				if (strcmp(instruction, "LD") == 0)
					cout << instruction << " " << op1String << " " << op2Value;
				else
					cout << instruction << " " << op1Value << " " << op2Value;
			} else if (isBranchInstruction(instruction))
				cout << instruction << " " << op1Value;
			else if (isJumpInstruction(instruction))
				cout << instruction << " " << op1Value;
		} else if (strcmp(name, "EXtoMA") == 0) {
			if (isArithmeticInstruction(instruction))
				cout << op1String << " " << op2Value;
			else if (isMemAccessInstruction(instruction)) {
				if (strcmp(instruction, "LD") == 0)
					cout << instruction << " " << op1String << " " << op2Value;
				else
					cout << instruction << " " << op1Value << " " << op2Value;
			}
		} else if (strcmp(name, "MAtoOS") == 0) {
			cout << op1String << " " << op2Value;
		}
		cout << endl;
	}
};

class BranchTargetBuffer {
public:
	int instructionAddress[256];
	int targetAddress[256];
	bool validBit[256];
	int numOfElements;
	BranchTargetBuffer() {
		numOfElements = 0;
	}
	bool getValidBit(int addr) {
		int i;
		for (i=0; i<numOfElements; i++) {
			if (instructionAddress[i] == addr)
				return validBit[i];
		}
		numOfElements++;
		instructionAddress[i] = addr;
		validBit[i] = false;
		return false;
	}
	int getTargetAddress(int addr) {
		for (int i=0; i<numOfElements; i++) {
			if (instructionAddress[i] == addr)
				return targetAddress[i];
		}
		return -1;
	}
	void setTargetAddress(int addr, int target) {
		for (int i=0; i<numOfElements; i++) {
			if (instructionAddress[i] == addr) {
				targetAddress[i] = target;
				validBit[i] = true;
				return;
			}
		}
	}
	void resetValidBit(int addr) {
		for (int i=0; i<numOfElements; i++) {
			if (instructionAddress[i] == addr) {
				validBit[i] = false;
				return;
			}
		}
	}
};

// program counter
int pc = 0;

// define the latches between the stages in the pipeline
Latch latchIFtoOF, latchOFtoEX, latchEXtoMA, latchMAtoOS;

RegisterFile registerFile;
BranchTargetBuffer branchTargetBuffer;
char *instructionMemory[512];
int  dataMemory[512];
int  CCR;
bool atEnd = false;
bool noMoreInstructions = false;

class InstructionFetchStage {
public:
	int stage;
	void execute() {

	//	cout << "PC:" << pc << endl;

		if (stage == 1) {
			stage++;
			return;
		}

		if (!latchIFtoOF.isEmpty)
			return;

		stage = 1;
		char *tempStr = (char*)malloc(33);
		strcpy(tempStr, instructionMemory[pc]);
		if (strcmp(tempStr, "") == 0) {
			noMoreInstructions = true;
			return;
		}
		char *tok;
		int i = 0;
		tok = strtok (tempStr," ");
		while (tok != NULL) {
			switch (i) {
				case 0: strcpy(latchIFtoOF.instruction, tok); break;
				case 1: strcpy(latchIFtoOF.op1String, tok); break;
				case 2: strcpy(latchIFtoOF.op2String, tok); break;
				case 3: strcpy(latchIFtoOF.op3String, tok); break;
			}
			tok = strtok (NULL, " ");
			i++;
		}
		latchIFtoOF.ic = pc;
		latchIFtoOF.isEmpty = false;
		if (isBranchInstruction(latchIFtoOF.instruction)) {
			if (branchTargetBuffer.getValidBit(pc))
				pc = branchTargetBuffer.getTargetAddress(pc);
			else
				pc++;
		} else
			pc++;
	}
	InstructionFetchStage() {
		stage = 2;
	}
};

class OperandFetchStage {
public:

	Register *rd, *rs1, *rs2;

	void execute() {

		bool badInstruction = false;

		if (latchIFtoOF.isEmpty || !latchOFtoEX.isEmpty)
			return;

		if (isArithmeticInstruction(latchIFtoOF.instruction)) {
			rd  = registerFile.getRegister(latchIFtoOF.op1String);
			rs1 = registerFile.getRegister(latchIFtoOF.op2String);
			char *loc;
			bool directAddress = true;

			loc = strchr(latchIFtoOF.op3String,'R');

			if (loc-latchIFtoOF.op3String == 0)
				directAddress = false;
			else {
				directAddress = true;
				loc = strchr(latchIFtoOF.op3String,'R');
				if (loc-latchIFtoOF.op3String == 0)
					directAddress = false;
			}

			if (!directAddress)
				rs2 = registerFile.getRegister(latchIFtoOF.op3String);

			if (!rd->validBit || !rs1->validBit ||
				(!directAddress && !rs2->validBit)) 
				return;

			strcpy(latchOFtoEX.instruction, latchIFtoOF.instruction);
			strcpy(latchOFtoEX.op1String, latchIFtoOF.op1String);
			latchOFtoEX.op2Value = rs1->value;
			if (!directAddress)
				latchOFtoEX.op3Value = rs2->value;
			else {
				latchOFtoEX.op3Value = atoi(latchIFtoOF.op3String);
			}
			rd->validBit = false;

		} else if (isMemAccessInstruction(latchIFtoOF.instruction)) {
			bool loadOperation = false;
			if (strcmp(latchIFtoOF.instruction, "LD") == 0)
				loadOperation = true;
			char *tempStr = (char*)malloc(10);
			if (loadOperation) {
				strncpy(tempStr, latchIFtoOF.op2String + 1, (strlen(latchIFtoOF.op2String)-2));
				tempStr[strlen(latchIFtoOF.op2String)-2] = '\0';
				rd = registerFile.getRegister(latchIFtoOF.op1String);
			} else {
				strncpy(tempStr, latchIFtoOF.op1String + 1, (strlen(latchIFtoOF.op1String)-2));
				tempStr[strlen(latchIFtoOF.op1String)-2] = '\0';
				rs1 = registerFile.getRegister(latchIFtoOF.op2String);
			}
			rs2 = registerFile.getRegister(tempStr);

			if (!rs2->validBit ||
				(loadOperation && !rd->validBit) ||
				(!loadOperation && !rs1->validBit))
				return;

			strcpy(latchOFtoEX.instruction, latchIFtoOF.instruction);
			if (loadOperation) {
				strcpy(latchOFtoEX.op1String, latchIFtoOF.op1String);
				latchOFtoEX.op2Value = rs2->value;
				rd->validBit = false;
			} else {
				latchOFtoEX.op1Value = rs2->value;
				latchOFtoEX.op2Value = rs1->value;
			}

		} else if (isBranchInstruction(latchIFtoOF.instruction)) {

			strcpy(latchOFtoEX.instruction, latchIFtoOF.instruction);
			latchOFtoEX.op1Value = atoi(latchIFtoOF.op1String);

		} else if (isJumpInstruction(latchIFtoOF.instruction)) {

			char *tempStr = (char*)malloc(10);
			strncpy(tempStr, latchIFtoOF.op1String + 1, (strlen(latchIFtoOF.op1String)-2));
			tempStr[strlen(latchIFtoOF.op1String)-2] = '\0';
			rs1 = registerFile.getRegister(tempStr);
			if (!rs1->validBit)
				return;
			strcpy(latchOFtoEX.instruction, latchIFtoOF.instruction);
			latchOFtoEX.op1Value = rs1->value;


		} else {
			cout << "Bad instruction - clearing latch" << endl;
			badInstruction = true;
		}

		rd = rs1 = rs2 = NULL;
		latchOFtoEX.ic = latchIFtoOF.ic;
		latchIFtoOF.clear();
		latchOFtoEX.isEmpty = false;

		if (badInstruction)
			latchOFtoEX.clear();
	}
};

class ExecuteStage {
public:
	void execute() {

		if (latchOFtoEX.isEmpty || !latchEXtoMA.isEmpty)
			return;

		if (isArithmeticInstruction(latchOFtoEX.instruction)) {

			if (strcmp(latchOFtoEX.instruction, "ADD") == 0) {
				latchEXtoMA.op2Value = latchOFtoEX.op2Value + latchOFtoEX.op3Value;
			} else if (strcmp(latchOFtoEX.instruction, "SUB") == 0) {
				latchEXtoMA.op2Value = latchOFtoEX.op2Value - latchOFtoEX.op3Value;
			} else if (strcmp(latchOFtoEX.instruction, "AND") == 0) {
				latchEXtoMA.op2Value = latchOFtoEX.op2Value & latchOFtoEX.op3Value;
			} else if (strcmp(latchOFtoEX.instruction, "OR") == 0) {
				latchEXtoMA.op2Value = latchOFtoEX.op2Value | latchOFtoEX.op3Value;
			} else if (strcmp(latchOFtoEX.instruction, "EOR") == 0) {
				latchEXtoMA.op2Value = latchOFtoEX.op2Value ^ latchOFtoEX.op3Value;
			}

			CCR = latchEXtoMA.op2Value;

			strcpy(latchEXtoMA.instruction, latchOFtoEX.instruction);
			strcpy(latchEXtoMA.op1String, latchOFtoEX.op1String);

			latchEXtoMA.isEmpty = false;

		} else if (isMemAccessInstruction(latchOFtoEX.instruction)) {

			if (strcmp(latchOFtoEX.instruction, "LD") == 0)
				strcpy(latchEXtoMA.op1String, latchOFtoEX.op1String);
			else
				latchEXtoMA.op1Value = latchOFtoEX.op1Value;

			strcpy(latchEXtoMA.instruction, latchOFtoEX.instruction);
			latchEXtoMA.op2Value = latchOFtoEX.op2Value;

			latchEXtoMA.isEmpty = false;

		} else if (isBranchInstruction(latchOFtoEX.instruction)) {

			int offset = latchOFtoEX.op1Value;
			bool branchTaken = false;
			int newpc;
			int addr = latchOFtoEX.ic;

			cout << "CCR:" << CCR << endl;
		
			if (strcmp(latchOFtoEX.instruction, "BZ") == 0) {
				if (CCR == 0)
					{newpc = addr + offset; branchTaken = true;}
			} else if (strcmp(latchOFtoEX.instruction, "BNZ") == 0) {
				if (CCR != 0)
					{newpc = addr + offset; branchTaken = true;}
			} else if (strcmp(latchOFtoEX.instruction, "BL") == 0) {
				if (CCR < 0)
					{newpc = addr + offset; branchTaken = true;}
			} else if (strcmp(latchOFtoEX.instruction, "BG") == 0) {
				if (CCR > 0)
					{newpc = addr + offset; branchTaken = true;}
			}

			if (branchTaken) {
				if (!branchTargetBuffer.getValidBit(addr)) {
					latchIFtoOF.clear();
					pc = newpc;
					branchTargetBuffer.setTargetAddress(addr, pc); 
				}
			} else {
				if (branchTargetBuffer.getValidBit(addr)) {
					latchIFtoOF.clear();
					pc = latchOFtoEX.ic + 1;
					branchTargetBuffer.resetValidBit(addr); 
				}
			}

		} else if (isJumpInstruction(latchOFtoEX.instruction)) {
			if (strcmp(latchOFtoEX.instruction, "J") == 0) {
				pc = latchOFtoEX.op1Value;
				pc--;
				latchIFtoOF.clear();
			}
		}
		latchOFtoEX.clear();
	}
};

class MemoryAccessStage { // 1 & 2
public:
	int stage;
	void execute() {
		if (latchEXtoMA.isEmpty || !latchMAtoOS.isEmpty)
			return;

		if (isArithmeticInstruction(latchEXtoMA.instruction)) {

			strcpy(latchMAtoOS.op1String, latchEXtoMA.op1String);
			latchMAtoOS.op2Value = latchEXtoMA.op2Value;
			latchMAtoOS.isEmpty = false;

		} else if (isMemAccessInstruction(latchEXtoMA.instruction)) {

			if (stage == 1) {
				stage++;
				return;
			} else {
				stage = 1;
			}

			if (strcmp(latchEXtoMA.instruction, "ST") == 0) {

				dataMemory[latchEXtoMA.op1Value] = latchEXtoMA.op2Value;

			} else if (strcmp(latchEXtoMA.instruction, "LD") == 0) {

				strcpy(latchMAtoOS.op1String, latchEXtoMA.op1String);
				latchMAtoOS.op2Value = dataMemory[latchEXtoMA.op2Value];
				latchMAtoOS.isEmpty = false;

			}
		}
		latchEXtoMA.clear();
	}
	MemoryAccessStage() {
		stage = 1;
	}
};

class OperandStoreStage {
public:
	void execute() {
		if (latchMAtoOS.isEmpty)
			return;
		Register *reg = registerFile.getRegister(latchMAtoOS.op1String);
		reg->value = latchMAtoOS.op2Value;
		reg->validBit = true;
		latchMAtoOS.clear();
	}
};

InstructionFetchStage instructionFetchStage;
OperandFetchStage operandFetchStage;
ExecuteStage executeStage;
MemoryAccessStage memoryAccessStage;
OperandStoreStage operandStoreStage;

int main(int argc, char *argv[]) {
	if (argc != 2) {
		cout << "Usage: pipeline filename" << endl;
		return 1;
	}

	// initialize the instruction memory array
	for (int i=0; i<512; i++) {
		instructionMemory[i] = (char*)malloc(33);
		instructionMemory[i][0] = '\0';
	}
	
	// load instructions from input file
	ifstream inFile(argv[1], ios::in);
	int  k=0;
	while (!inFile.eof()) {
		char inputLine[32];
		inFile.getline(inputLine, 32);
		if (strlen(inputLine) == 0)
			continue;
		strcpy(instructionMemory[k++], inputLine);
	}
	inFile.close();
	strcpy(instructionMemory[k++], "");

	for (int j=0; j<k; j++)
		cout << instructionMemory[j] << endl;

	latchIFtoOF.setName("IFtoOF");
	latchOFtoEX.setName("OFtoEX");
	latchEXtoMA.setName("EXtoMA");
	latchMAtoOS.setName("MAtoOS");

	int kount = 0;

	// start program execution
	while (!atEnd) {
		operandStoreStage.execute();
		memoryAccessStage.execute();
		executeStage.execute();
		operandFetchStage.execute();
		instructionFetchStage.execute();

		cout << "--------Cycle " << kount++ << "--------" << endl;
		latchIFtoOF.print();
		latchOFtoEX.print();
		latchEXtoMA.print();
		latchMAtoOS.print();
		cout << "Program Counter:" << pc << endl;

		if (latchIFtoOF.isEmpty && latchOFtoEX.isEmpty &&
			latchEXtoMA.isEmpty && latchMAtoOS.isEmpty && 
			noMoreInstructions)
			atEnd = true;
	}
	return 0;
}

bool isArithmeticInstruction(char *str) {
	if (strcmp(str, "ADD") == 0 ||
		strcmp(str, "SUB") == 0 ||
		strcmp(str, "AND") == 0 ||
		strcmp(str, "OR")  == 0 ||
		strcmp(str, "EOR") == 0)
		return true;
	else
		return false;
}

bool isMemAccessInstruction(char *str) {
	if (strcmp(str, "LD") == 0 ||
		strcmp(str, "ST") == 0)
		return true;
	else
		return false;
}

bool isBranchInstruction(char *str) {
	if (strcmp(str, "BZ")  == 0 ||
		strcmp(str, "BNZ") == 0 ||
		strcmp(str, "BL")  == 0 ||
		strcmp(str, "BG")  == 0)
		return true;
	else
		return false;
}

bool isJumpInstruction(char *str) {
	if (strcmp(str, "J")  == 0)
		return true;
	else
		return false;
}

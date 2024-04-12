/*****************************************************************************
TITLE : ASSEMBLER
NAME: Rachit Panwar																																
AUTHOR:   2201CS56
NAME: Rachit Panwar
Declaration of Authorship
This cpp file is part of the mini project of Architecture lab
*****************************************************************************/
#include<bits/stdc++.h>
using namespace std;

#define OP_BITS 8
#define OPERAND_BITS 24
#define MEM_SIZE 16777126  //max size of memory possible 2^24

int SP, PC, A, B;   // declaring all 4 registers

int memory[MEM_SIZE]; 

void charfill(char c , int n){
    for(int i = 0 ; i<n ; i++){
        cout<<c;
    }
    cout<<endl;
}

int getOpcode(int instruction) {
    return ((1 << OP_BITS) - 1) & instruction;
}

int getOperand(int instruction) {
    return (instruction >> OP_BITS);
}


// bulding table for instruction set
void print_instruction_set() {
    // Defining instruction set 
    string instructions[][3] = {
        {"ldc", "0", "value"},
        {"adc", "1", "value"},
        {"ldl", "2", "offset"},
        {"stl", "3", "offset"},
        {"ldnl", "4", "offset"},
        {"stnl", "5", "offset"},
        {"add", "6", ""},
        {"sub", "7", ""},
        {"shl", "8", ""},
        {"shr", "9", ""},
        {"adj", "10", "value"},
        {"a2sp", "11", ""},
        {"sp2a", "12", ""},
        {"call", "13", "offset"},
        {"return", "14", ""},
        {"brz", "15", "offset"},
        {"brlz", "16", "offset"},
        {"br", "17", "offset"},
        {"HALT", "18", ""}
    };

    // Defining the widths of each column in the table
    int col_widths[] = {10, 7, 8};

    // top border
    charfill('-' , 35);

    // heading row
    cout<<"| "<<"Mnemonic"<<"   "<<"| "<<"Opcode  |"<<" Operand  |"<<endl;

    // 2nd border
    charfill('-' , 35);

    // Printing instruction row
    for (int i = 0; i < 19; ++i) {
        cout << setfill(' ') << "| " << setw(col_widths[0]) << left << instructions[i][0]
            << " | " << setw(col_widths[1]) << left << instructions[i][1]
            << " | " << setw(col_widths[2]) << left << instructions[i][2]
            << " |" << endl;
    }

    // bottom border
    charfill('-' , 35);
}

void printOptions() {

    cout << "usage: emulator [options] [binary file to emulate] " << endl;
    cout << "-disc\t-> to display the instruction-set" << endl;
    cout << "-before\t-> to generate dump file before execution" << endl;
    cout << "-after\t-> to generate dump after execution" << endl;
    cout << "-trace\t-> to view trace an the terminal" << endl;

}

// funtion execution for all opcodes
int execute_Ins(int opcode, int oprand) {

    if (opcode == 0) { // ldc
        B = A;
        A = oprand;

    } 
    else if (opcode == 1) {         // adc
        A += oprand;

    } 
    else if (opcode == 2) {         // ldl
        B = A;
        A = memory[SP + oprand];

    } 
    else if (opcode == 3) {         // stl
        memory[SP + oprand] = A;
        A = B;

    } 
    else if (opcode == 4) {         // ldnl
        A = memory[A + oprand];

    } 
    else if (opcode == 5) {         // stnl
        memory[A + oprand] = B;

    } 
    else if (opcode == 6) {         // add
        A = B + A;

    } 
    else if (opcode == 7) {         // sub
        A = B - A;

    } 
    else if (opcode == 8) {         // shl
        A = (B << A);

    } 
    else if (opcode == 9) {         // shr
        A = (B >> A);

    }
    else if (opcode == 10) {        // adj
        SP = SP + oprand;

    }
    else if (opcode == 11) {        // a2sp
        SP = A;
        A = B;

    } 
    else if (opcode == 12) {        // sp2a
        B = A;
        A = SP;

    } 
    else if (opcode == 13) {        // call
        B = A;
        A = PC;
        PC = PC + oprand;

    } 
    else if (opcode == 14) {        // return
        PC = A;
        A = B;

    } 
    else if (opcode == 15) {        // brz
        if (A == 0) {
            PC = PC + oprand;
        }

    } 
    else if (opcode == 16) {        // brlz
        if (A < 0) {
            PC = PC + oprand;
        }

    } 
    else if (opcode == 17) {        // br
        PC = PC + oprand;

    } 
    else if (opcode == 18) {        // HALT
        return 0;

    } 
    else {
        // Handle invalid opcode
        return -1;
    }
    return 1;
}



int execute(bool trace_on) {
    SP = 8388607 -1;      // 2^23 -1

    int oprand = 0, opcode = 0;
    int instruction_num = 0;

    while ((opcode = getOpcode(memory[PC])) <= 18) {
        
        oprand = getOperand(memory[PC]);

        if(trace_on){
            fprintf(stderr, "PC=%08X, SP=%08X, A=%08X, B=%08X\n", PC, SP, A, B);
        }
        if (opcode == 18) break;
        if (opcode > 18) return instruction_num;
        int del=PC;
        PC++;
        execute_Ins(opcode, oprand);
        if(del==PC){
        	fprintf(stderr, "INFINITE LOOP DETECTED!!\n");
        	return instruction_num;
        }
        instruction_num += 1;
    }
    return instruction_num;
}

void dump(int line_num, ofstream& file,bool before) {

    if(before){
        file << "Memory Dump before exectution" << endl;
    }
    
    else file << "Memory Dump after execution" << endl;
    for (int i = 0; i < line_num; i++) {
        if (i % 4 == 0) file << std::endl << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << i << "\t";
        file << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << memory[i] << "\t";
    }
    file << endl;
    return;
}



int main(int argc, char* argv[]) {
    if (argc < 3) {
        printOptions();
        return 1;
    }
    string binary_file = argv[argc-1];
    ifstream bin(binary_file,ios::binary);
    ofstream Dump("dump.txt");

    bool trace,before,disc,after;
    trace=before=disc=after=false;
    for(int i=1;i<argc-1;++i){
        trace |= !strcmp(argv[i], "-trace");        //show the trace
        before |= !strcmp(argv[i],"-before");       //give the memory dump before the execution of the program
        disc |= !strcmp(argv[i], "-disc");          //show the instrution set 
        after |= !(strcmp(argv[i], "-after"));      //give the memory dump after the execution of the program
    }

    if(disc) print_instruction_set();
    string line;
    int line_number = 0;
    while (getline(bin,line)) {
        line_number++; 
    }
    bin.clear();    //clear the error state flags of a file stream
    bin.seekg(0);   //move the file read pointer to file.begin
    line_number = 0;
    while (getline(bin,line)) {
        bitset<32> bitset(line.substr(0,32));
        int value = bitset.to_ulong();
        memory[line_number] = value;
        line_number++; 
    }
    bin.close();
    if(before) dump(line_number,Dump,true);
    int totalExecuted = execute(trace);
    cout << std::dec << "Total "<< totalExecuted << " instructions have been exectued" << endl; //swithed the cout back to decimals
    if(after) dump(line_number,Dump,false);
    Dump.close();
    return 0;
}
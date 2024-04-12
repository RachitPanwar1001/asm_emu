/*****************************************************************************
TITLE: ASSEMBLER																																
AUTHOR:   2201CS56
NAME: Rachit Panwar
Declaration of Authorship
This cpp file is part of the mini project of Architecture Lab
*****************************************************************************/

# include <bits/stdc++.h>
# define OP_BITS 8
# define OPR_BITS 24
# define give_size size

using namespace std;

map<string,int> opcodes;	//maps opcodes to respective numberical opcodes
map<string,int> labels;		//stores branch labels
map<string,int> values;		//stores the labels declared using data

vector<pair<int,string>> errors,warnings;

string substring(string s , int st , int en){

	string ans = "";
	for(int i = st ; i<en ; i++){
		ans.push_back(s[i]);
	}
	return ans;
}

bool find_msi(map<string ,int> m , string s){
	if(m.find(s) == m.end()){
		return 0;
	}
	else{
		return 1;
	}
}




vector<string> ins = {"ldc","adc","ldl","stl","ldnl","stnl","add","sub","shl","shr","adj",
								"a2sp","sp2a","call","return","brz","brlz","br","HALT"};


vector<string> Oprand_need = {"ldc","adc","ldl","stl","ldnl","stnl","adj","call","brz","brlz","br"};


//mapping all op codes to numerical opcodes
void initilize(){ 
	for(int i=0  ;  i<19  ;  ++i){
		opcodes[ins[i]] = i;
	}
}


vector<string> break_(const string& str) {		//function to break_ the sting managing white spaces
    vector<string> ans;

    istringstream str1(str);

    for (string s; str1 >> s; ) {
        ans.push_back(s);
    }
	
    return ans;
}

int string_2_int(string str) {			//converting string of hex/int to int

    char *end;
    long val;
    int chk = 0;
	int base;
    if ((str[1] == 'x' || str[1] == 'X')  && str[0] == '0' ) { // indicating the start of hex
        chk = 1;
        str += 2;
    }
	if(chk==0){
		base = 10;
	}
	else{
		base = 16;
	}

    val = strtol(str.c_str(), &end, base);
    return (int)val;
}



bool is_number(string s) {		//checks if the given string is int(hex) or not
    if (s.size()== 0) {
        return 0;
    }
    int chk = 0;

    if (s[chk] == '-' || s[chk] == '+') {
        chk = 1;
    }
    int ret = 0;

	for(int i = chk ; i<s.give_size() ; i++){
		if(isdigit(s[i])){
			ret = 1;
		}
		else{
			ret = 0;
		}
	}
	return ret;
}

int find_st( vector<string> v , string s){
	for(int i = 0 ; i<v.give_size() ; i++){
		if(v[i]== s){
			return 0;
		}
	}
	return 1;
}

int BinarytoDecimal(char binary[],int len, int i=0){
   if (i == len-1)

   return (binary[i] - '0');

   int temp=binary[i]-'0';
   temp = temp<<len-i-1;

   temp = temp + BinarytoDecimal(binary,len,i+1);
   return (temp);
}

int cntr,line_numm;	//cntr acts like Program counter and line_numm gives line number

string line;

int main(int argc, char* argv[]){

	initilize();// map op codes

	// less arguments
	if(argc <= 1){
		cout << "Usage [name of file to assemble]" << endl;
		return 1;
	}

	//Mannaging Files 

	string filename = argv[1];

	int posn = filename.find_last_of(".");   //posn of extension
    string name = filename.substr(0, posn);		
	
	ifstream in(filename);

  	ofstream bin(name  +  ".o",ios:: binary);   // binary file
  	ofstream listing(name + ".l");

  	cntr = 0;
  	line_numm = 0;

	/************************************************/
	/* 		First Pass for error detection          */
	/************************************************/


	while(getline(in,line)){
		line_numm++;

		string label = "";
		int j = 0;  // start of label
		int line_sz = line.give_size();

		//splitting of strings in accordance with labels ,whitespaces etc
		int lp = 0;
		while (lp < line_sz) {
			if (line[lp] == ';') {
				line_sz = lp;
				break;
			}
			++lp;
		}

		lp = 0;
		while (lp < line_sz) {
			if (line[lp] == ':') {
				label = line.substr(0, lp);
				j = lp + 1;
				break;
			}
			++lp;
		}


		vector<string> Ins_set = {};

		if(j<line_sz){
			Ins_set = break_(line.substr( j , line_sz-j ));
		}

		//label detected
		if(label != ""){

			if(  (label[0]>='0'  &&  label[0]<='9')  ||  (label[0]<'a'  ||  label[0]>'z')  ){ 			//checking correctness of label names
				errors.push_back({line_numm,"Invalid label name"});
				break;
			}

			bool data = (line.find("data")  !=  string::npos);   //npos  ->   value returned by various member functions when they fail
			bool SET = (line.find("SET") != string :: npos);

			if( find_msi(labels ,label) ||  find_msi(values , label) ){

				errors.push_back({line_numm,"Duplicate label found"});

			}
			else{
				if(data){
					if(Ins_set.give_size()>=0){

						values[label] = cntr;
						cntr++;

					}else{

						errors.push_back({line_numm,"Value to store not found."});

					}
					continue;
				}
				if(SET){
					if(Ins_set.give_size()>=0){
						labels[label] = stoi(Ins_set[1]);
					}
				}

				else labels[label] = cntr;
			}
			if(Ins_set.give_size()<=1) continue;

		}

		if(Ins_set.give_size()<=0) continue;


		string operation = Ins_set[0];

		if(  find_st(ins , operation) && operation != "data" && operation != "SET"){                // change

			errors.push_back({line_numm,"Invalid Mnemonic/Command"});

		}
		else if( !find_st(Oprand_need , operation)){

			if(Ins_set.give_size()<=1){
				errors.push_back({line_numm,"Operand not found!"});

			}
			else if(Ins_set.give_size()>=3){ 
				errors.push_back({line_numm,"Additional operand found!"});

			}
		}
		cntr++;
	}

	in.clear();		//clears the error state flags of a file stream
	in.seekg(0);	// TO move the file's read pointer to the starting of file


	cntr = 0;
	line_numm = 0;
	/****************************************************************/
	//Second Pass dectecting further errors and to generate machine code
	/*****************************************************************/

	while(getline(in,line)){
		line_numm++;

		//making the 8 bit cntr string
		stringstream ss;
    	ss <<hex << uppercase << setw(8)  << setfill('0') <<  cntr;
    	string cnt_width = ss.str();


		string label = "";
		int j = 0;
		int line_sz = line.give_size();

		//splitting of string and managing white spaces
		for(int i=0  ;  i<line.give_size()  ;  ++i){

			if(line[i]  == ';'){
				line_sz = i;
				break;
			}

		}
		for(int i = 0  ; i < line_sz  ;  ++i){

			if(line[i]  ==  ':'){
				label = line.substr(0,i);
				j = i+1;
				break;
			}

		}
		vector<string> Ins_set = {};
		if(j < line.give_size()){
			Ins_set = break_(line.substr(j,line_sz-j));
		}

		//check if line has a label (label/data)


		if(label!=""){
			bool data = (line.find("data")  !=  string::npos);
			

			if(data){
				
				bitset<32> tmp(stoi(Ins_set[1]));
				bin << tmp.to_string() << endl;
				
				listing << cnt_width << "\t" << hex << uppercase << setw(8) << setfill('0') << Ins_set[1]<< "\t" << label << ": data" << Ins_set[1] << endl;
				cntr++;
				continue;
			}

			else{
				listing << cnt_width << "\t\t\t\t" << label << ":" << endl;
			}
		}

		if(Ins_set.give_size()<1) continue;

		string operation = Ins_set[0];

		int opcode = opcodes[operation];

		string operand_str = "";

		if(Ins_set.give_size() > 1){
			operand_str = Ins_set[1];
		}
		int operand = 0;

		// Handling b and call 

		if(operation[0]  ==  'b'  ||  operation  ==  "call" ){
			if(  labels.find(operand_str)  ==  labels.end()  ){

				errors.push_back({line_numm,"\"" + operand_str+"\" is not yet defined"});
				continue;

			}
			int offset = labels[Ins_set[1]]-cntr-1;
			operand = offset;
			
			if(offset==0){
				warnings.push_back({line_numm,"Infinite Loop detected"});
			}
		}
		// handiling other operations
		else{
			if(operand_str != ""  &&  !is_number(operand_str)  ){

				if(find_msi(values , operand_str)){
					operand = values[operand_str];
				}
				
				else if(find_msi(labels , operand_str)){
					operand = values[operand_str];
				
				}
				else{
					errors.push_back({line_numm,"Wrong format of number -> " + operand_str});
					continue;

				}	
			}
			else{
				operand = string_2_int(operand_str);
			}
		}
		bool data = (line.find("data")  !=  string::npos);
		if(data){
			bitset<32> tmp(stoi(Ins_set[1]));
			bin << tmp.to_string() << endl;
			
			listing << cnt_width << "\t" << hex << uppercase << setw(8) << setfill('0') << Ins_set[1] << "\t" << label << " data " << Ins_set[1] << endl;
			cntr++;
			continue;
		}

		//Printing the machine code to the [name].o file
		unsigned int binary = ((operand<<OP_BITS)|(opcode));
		bitset<32> binary_bin(binary);
		listing << cnt_width << "\t" << hex << uppercase << setw(8) << setfill('0')  << binary << "\t" << Ins_set[0] << " " << operand_str << endl;
		bin << binary_bin << endl;
		cntr++;
	}

	in.close();
	bin.close();
	listing.close();

	//arranging errors in accordance with line number
	sort(errors.begin(), errors.end() );
	sort(warnings.begin() , warnings.end());


	ofstream err(name+".log");

	for(int i = 0 ; i<warnings.give_size() ; i++){
		err << "Warning at Line -> " << warnings[i].first << "\t" << warnings[i].second << endl;
	}

	err << "Total number of errors found -> " << errors.give_size() << endl;

	for(int i = 0 ; i<errors.give_size() ; i++){
		err << "Error at Line ->" << errors[i].first << "\t" << errors[i].second << endl;
	}

	err.close();

	//if program finds errors -> exits with err code 1
	if(errors.give_size() > 0){
		return 1;
	}
	else{
		return 0;
	}
}


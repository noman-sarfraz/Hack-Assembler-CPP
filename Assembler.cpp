#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
using namespace std;

// Declaring Custom Functions
// built for my COMFORT

string change_extension(string);				// changes extension from .asm to .hack
bool is_nmbr(string);							// returns true if input string consists of only digits
string toString(int);							// gets int and returns string
int str_to_int(string);							// gets string and returns int
bool is_char_in_string(char,string);			// returns true if the given character is in the given string
string dec_to_bin(int);							// gets decimal number and returns binary number
string remove_spaces_and_tabs(string);			// gets a string, removes spaces and tabs, and returns the clean string
string remove_comments(string);					// gets a string, removes comments, and returns the clean string
string remove_labels(string);					// empties the string if it's a label and returns the string
string asm_to_bin(string);						// gets a pure Assembly instruction and returns its binary
string resolve_symbols(string);					// converts user-defined symbols into their corresponding binary codes


// Declaring Data Structures
// for holding codes for symbols 
class dest_table
{
private:
	string symb[8];
	string code[8];
public:
	dest_table();
	string code_for(string);
};
class jump_table
{
private:
	string symb[8];
	string code[8];
public:
	jump_table();
	string code_for(string);
};
class comp_table
{
	private:
	string symb[28];
	string code[28];
public:
	comp_table();
	string code_for(string);
};
class symb_table
{
private:
	string symb[1000];
	string code[1000];
	int array_size;
	int var_address;
public:
	symb_table();
	void add_symb(string,int);
	void show_symbols();
	string code_for(string);
};

// creating global instances of Data-Structures
// (Because they are used in some custom functions)
comp_table compTable;
dest_table destTable;
jump_table jumpTable;
symb_table symbTable;


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------  M a i n   L o g i c  -------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


int main()
{
	cout << "Assembly Code --> Machine Code" << endl;
	cout << "Enter Assembly File: ";
	
	string ifile,ofile;
	getline(cin,ifile);
	ofile = change_extension(ifile);
	
	ifstream read;
	read.open(ifile.c_str());
	if(!read) 
	{
		cout << "File not found" << endl;
		throw -1;
	}
	ofstream write;
	write.open(ofile.c_str());
	
	int count=0;
	string line;
	string temp;
	
	cout << endl << "..." << endl;
	
	// pass-1
	// adding labels symbols to Table
	while(!read.eof())
	{
		getline(read, line);
		line = remove_spaces_and_tabs(line);
		line = remove_comments(line);
		if(line=="") continue;
		if(line[0]=='(' && line[line.length()-1]==')')
		{
			temp="";
			for(int i=1;i<line.length()-1;i++)
			temp += line[i];
			symbTable.add_symb(temp,count);
		}
		else 
		count++;
	}
	read.close();
	
	cout << endl << "..." << endl;
	
	// pass-2
	// handling variables and writing binary
	ifstream read2;
	read2.open(ifile.c_str());
	
	while(!read2.eof())
	{
		getline(read2, line);
		line = remove_spaces_and_tabs(line);
		line = remove_comments(line);
		line = remove_labels(line);
		line = resolve_symbols(line);
		if(line=="") continue;
		
		line = asm_to_bin(line);
		
		write << line << endl;
	}
	
	cout << endl << "..." << endl;
	cout << endl << "Check your Machine Code in " << ofile << endl;
	
	read2.close();
	write.close();
	system("pause");
	return 0;
}


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------ Definitions of Custom Functions  --------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


string change_extension(string str)
{
	int len = str.length();
	if(str[len-4] == '.' && str[len-3] == 'a' && str[len-2] == 's' && str[len-1] == 'm')
	{
		str[len-4] = '.';
		str[len-3] = 'h';
		str[len-2] = 'a';
		str[len-1] = 'c';
		str += 'k';
		
	}
	else
	{
		cout << "Not an asm file" << endl;
		throw -1;
	}
	return str;
}

string remove_spaces_and_tabs(string line)
{
	string stripped="";
	for(int i=0;i<line.length();i++)
	{
		if(line[i]!=' ' && line[i]!='\t')
		stripped += line[i];
	}
	
	return stripped;
}

string remove_comments(string line)
{
	string modified="";
	int i=0;
	while(i<line.length())
	{
		if(line[i]=='/' && line[i+1]=='/')
		break;
		i++;
	}
	
	int j=0;
	while(j<i)
	{
		modified += line[j];
		j++;
	}
	
	return modified;
}


int str_to_int(string str)
{
	int nmbr=0;
	int len = str.length();
	for(int i=0,j=len-1;i<len;i++,j--)
	{
		nmbr += (str[i]-'0') * pow(10,j);
	}
	return nmbr;
}

string dec_to_bin(int dec)
{
	string bin="";
	string temp = "";
	while(dec>=2)
	{
		temp += char(dec%2) + '0';
		dec = dec/2;
	}
	
	temp += char(dec) + '0';
	
	for(int i=temp.length();i<15;i++)
	temp += '0';
	for(int i=temp.length()-1;i>=0;i--)
	bin += temp[i];
	
	return bin;
}

bool is_char_in_string(char ch, string str)
{
	
	for(int i=0;i<str.length();i++)
		if(ch == str[i])
			return true;
			
	return false;
}

bool is_nmbr(string val)
{
	for(int i=0;i<val.length();i++)
		if(!(val[i] >= '0' && val[i] <= '9'))
			return false;
	return true;
}

string toString(int nmbr)
{
	string str="";
	string temp="";
	while(nmbr>=10)
	{
		temp += nmbr%10 + '0';
		nmbr /= 10;
	}
	temp += nmbr%10 + '0';
	for(int i=temp.length()-1;i>=0;i--)
	str += temp[i];
	
	return str;
}

string remove_labels(string line)
{
	if(line[0]=='(' && line[line.length()-1]==')')
	return "";
	return line;
}


string asm_to_bin(string line)
{
	string bin="";
	
	// if it's an A-instruction
	if(line[0]=='@')
	{
		bin += "0";
		string value="";
		for(int i=1;i<line.length();i++)
		value += line[i];
		bin += dec_to_bin(str_to_int(value));
		
	}
	
	// if it's a C-instruction
	else 
	{
		string comp;
		string dest;
		string jump;
				
		if(is_char_in_string('=',line))
		{
			string temp="";
			int i=0;
			while(line[i]!='=')
			{
				temp += line[i];
				i=i+1;
			}
			dest = temp;
			temp="";
			i=i+1;
			
			while(i<line.length())
			{
				temp += line[i];
				i=i+1;
			}
			comp = temp;
			jump = "null";
			
		}
		
		else if(is_char_in_string(';',line))
		{
			string temp="";
			int i=0;
			while(line[i]!=';')
			{
				temp += line[i];
				i=i+1;
			}
			comp = temp;
			temp="";
			i=i+1;
			
			while(i<line.length())
			{
				temp += line[i];
				i=i+1;
			}
			
			jump = temp;
			dest = "null";
			
		}
				
		bin += "111";
		bin += compTable.code_for(comp);
		bin += destTable.code_for(dest);
		bin += jumpTable.code_for(jump);
		
	}
	
	return bin;
}

string resolve_symbols(string val)
{
	if(val[0]!='@') return val;
	string main_string="";
	for(int i=1;i<val.length();i++)
	main_string += val[i];
	
	return val[0] + symbTable.code_for(main_string);
}


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------- Definitions of Functions of Data-Structures  -------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


// dest_table

dest_table::dest_table()
{
	symb[0] = "null";
	symb[1] = "M";
	symb[2] = "D";
	symb[3] = "MD";
	symb[4] = "A";
	symb[5] = "AM";
	symb[6] = "AD";
	symb[7] = "AMD";
	
	code[0] = "000";
	code[1] = "001";
	code[2] = "010";
	code[3] = "011";
	code[4] = "100";
	code[5] = "101";
	code[6] = "110";
	code[7] = "111";
	
}

string dest_table::code_for(string val)
{
	for(int i=0;i<8;i++)
		if(symb[i] == val)
			return code[i];
	return "invalid dest";
}


// jump_table function definitions

jump_table::jump_table()
{
	symb[0] = "null";
	symb[1] = "JGT";
	symb[2] = "JEQ";
	symb[3] = "JGE";
	symb[4] = "JLT";
	symb[5] = "JNE";
	symb[6] = "JLE";
	symb[7] = "JMP";
	
	code[0] = "000";
	code[1] = "001";
	code[2] = "010";
	code[3] = "011";
	code[4] = "100";
	code[5] = "101";
	code[6] = "110";
	code[7] = "111";
	
}

string jump_table::code_for(string val)
{
	for(int i=0;i<8;i++)
		if(symb[i] == val)
			return code[i];
	return "invalid jump";
}


// comp_table function definitions

comp_table::comp_table()
{
	symb[0] = "0";
	symb[1] = "1";
	symb[2] = "-1";
	symb[3] = "D";
	symb[4] = "A";
	symb[5] = "!D";
	symb[6] = "!A";
	symb[7] = "-D";
	symb[8] = "-A";
	symb[9] = "D+1";
	symb[10] = "A+1";
	symb[11] = "D-1";
	symb[12] = "A-1";
	symb[13] = "D+A";
	symb[14] = "D-A";
	symb[15] = "A-D";
	symb[16] = "D&A";
	symb[17] = "D|A";
	symb[18] = "M";
	symb[19] = "!M";
	symb[20] = "-M";
	symb[21] = "M+1";
	symb[22] = "M-1";
	symb[23] = "D+M";
	symb[24] = "D-M";
	symb[25] = "M-D";
	symb[26] = "D&M";
	symb[27] = "D|M";
	
	
	code[0] = "0101010";
	code[1] = "0111111";
	code[2] = "0111010";
	code[3] = "0001100";
	code[4] = "0110000";
	code[5] = "0001101";
	code[6] = "0110001";
	code[7] = "0001111";
	code[8] = "0110011";
	code[9] = "0011111";
	code[10] = "0110111";
	code[11] = "0001110";
	code[12] = "0110010";
	code[13] = "0000010";
	code[14] = "0010011";
	code[15] = "0000111";
	code[16] = "0000000";
	code[17] = "0010101";
	code[18] = "1110000";
	code[19] = "1110001";
	code[20] = "1110011";
	code[21] = "1110111";
	code[22] = "1110010";
	code[23] = "1000010";
	code[24] = "1010011";
	code[25] = "1000111";
	code[26] = "1000000";
	code[27] = "1010101";
	
}

string comp_table::code_for(string val)
{
	for(int i=0;i<28;i++)
		if(symb[i] == val)
			return code[i];
	return "invalid comp";
}


// symb_table function definitions

symb_table::symb_table()
{
	symb[0] = "R0";
	symb[1] = "R1";
	symb[2] = "R2";
	symb[3] = "R3";
	symb[4] = "R4";
	symb[5] = "R5";
	symb[6] = "R6";
	symb[7] = "R7";
	symb[8] = "R8";
	symb[9] = "R9";
	symb[10] = "R10";
	symb[11] = "R11";
	symb[12] = "R12";
	symb[13] = "R13";
	symb[14] = "R14";
	symb[15] = "R15";
	symb[16] = "SP";
	symb[17] = "LCL";
	symb[18] = "ARG";
	symb[19] = "THIS";
	symb[20] = "THAT";
	symb[21] = "SCREEN";
	symb[22] = "KBD";
	
	code[0] = "0";
	code[1] = "1";
	code[2] = "2";
	code[3] = "3";
	code[4] = "4";
	code[5] = "5";
	code[6] = "6";
	code[7] = "7";
	code[8] = "8";
	code[9] = "9";
	code[10] = "10";
	code[11] = "11";
	code[12] = "12";
	code[13] = "13";
	code[14] = "14";
	code[15] = "15";
	code[16] = "0";
	code[17] = "1";
	code[18] = "2";
	code[19] = "3";
	code[20] = "4";
	code[21] = "16384";
	code[22] = "24576";		
	
	array_size=23;
	var_address=16;
}

void symb_table::add_symb(string str, int val)
{
	symb[array_size] = str;
	code[array_size] = toString(val);
	array_size++;
	
}

void symb_table::show_symbols()
{
	for(int i=0;i<array_size;i++)
	{
		cout << i << "\t" <<  symb[i] << "\t" <<  code[i] << endl;
	}
}

string symb_table::code_for(string val)
{
	if(is_nmbr(val)) return val;
	
	for(int i=0;i<array_size;i++)
		if(symb[i] == val)
			return code[i];
	symb[array_size] = val;
	code[array_size] = toString(var_address);
	array_size++;
	var_address++;
	
	return code[array_size-1];
}




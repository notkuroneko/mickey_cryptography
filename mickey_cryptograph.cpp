#include <iostream>
#include <fstream>
#include <chrono> 

using namespace std;

bool rtaps[100] = {1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1};
bool comp0[100] = {0 /*comp0[0]=X*/, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 /*comp0[100]=X*/};
bool comp1[100] = {0 /*comp1[0]=X*/, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 /*comp1[100]=X*/};
bool fb0[100] = {0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0};
bool fb1[100] = {0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0};

void clock_r(bool (&x)[100], bool ctrl_bit, bool input_bit){
    bool fb_bit = x[99]^input_bit;
    bool r_prev[100];
    for (int i = 0; i<100; i++) r_prev[i]=x[i];
    for (int i = 1; i<100; i++) x[i]=r_prev[i-1]; 
    x[0]=0;
    for (int i = 0; i<100; i++)
    if (rtaps[i]) x[i]=x[i]^fb_bit;
    if (ctrl_bit) for (int i = 0; i<100; i++) x[i]=x[i]^r_prev[i];
}

void clock_s(bool (&x)[100], bool ctrl_bit, bool input_bit){
    bool fb_bit = x[99]^input_bit;
    bool s_prev[100];
    for (int i = 0; i<100; i++) s_prev[i]=x[i];
    for (int i = 1; i<99; i++) x[i]=s_prev[i-1]^((s_prev[i]^comp0[i])&(s_prev[i+1]^comp1[i]));
    x[0]=0; x[99]=s_prev[98];
    if (!ctrl_bit) for (int i = 0; i<100; i++) x[i]=x[i]^(fb_bit&fb0[i]);
    else for (int i = 0; i<100; i++) x[i]=x[i]^(fb_bit&fb1[i]);
}

void clock_kg(bool (&x)[100], bool (&y)[100], bool mix, bool input_bit){
    bool ctrl_r = y[34]^x[67];
    bool ctrl_s = y[67]^x[33];
    bool input_r, input_s;
    if (mix) input_r=input_bit^y[50];
    else input_r=input_bit;
    input_s=input_bit;
    clock_r(x,ctrl_r,input_r);
    clock_s(y,ctrl_s,input_s);
}

void init(bool (&x)[100], bool (&y)[100], bool z[80], bool t[], int n){
    for (int i = 0; i<100; i++) {x[i]=0; y[i]=0;}
    for (int i = 0; i<n; i++) clock_kg(x,y,1,t[i]);
    for (int i = 0; i<80; i++) clock_kg(x,y,1,z[i]);
    for (int i = 0; i<100; i++) clock_kg(x,y,1,0);
}


int gcd(int a, int b){
    if (b==0) return a;
    else return gcd(b, a%b);
}

void read_input(bool (&x)[80], bool (&y)[80], int &n){ 
    string s; int count;
    ifstream f;
    f.open("mickey_key");
    getline(f, s);
    f.close();
    count = 0;
    for (int i=0; i<s.length(); i++) 
    switch (s[i]) {
        case '0': {x[count]=0; count++; break;} 
        case '1': {x[count]=1; count++; break;} 
        default: {break;}
    }
    if (count!=80) cout << "Key doesn't have sufficient length";
    f.open("mickey_iv");
    getline(f, s);
    f.close();
    count = 0;
    for (int i=0; i<s.length(); i++) 
    switch (s[i]) {
        case '0': {y[count]=0; count++; break;} 
        case '1': {y[count]=1; count++; break;} 
        default: {break;}
    }
    if (count>80) cout << "Init vector is too long";
    n = count;
}

void read_plaintext(string &x){
    ifstream f;
    f.open("mickey_plaintext");
    getline(f, x);
    f.close();
}

int main(){
    bool iv[80];
    bool k[80];
    bool r[100];
    bool s[100];
    int iv_length;
    read_input(k,iv,iv_length);
    //for (int i = 0; i<iv_length; i++) iv[i]=rand()%2;
    //for (int i = 0; i<80; i++) k[i]=rand()%2; 
    //for (int i = 0; i<100; i++) rtaps[i]=rand()%2; (predefined)
    //for (int i = 0; i<100; i++) comp0[i]=rand()%2; (predefined)
    //for (int i = 0; i<100; i++) comp1[i]=rand()%2; (predefined)
    //for (int i = 1; i<99; i++) fb0[i]=rand()%2; (predefined)
    //for (int i = 1; i<99; i++) fb1[i]=rand()%2; (predefined)

    cout << "iv array: " << endl;
    for (int i = 0; i<iv_length; i++) cout << iv[i] << ", "; 
    cout<<endl<<endl;
    cout << "k array: " << endl;
    for (int i = 0; i<80; i++) cout << k[i] << ", "; 
    cout<<endl<<endl;
    /*cout << "rtap array: " << endl;
    for (int i = 0; i<100; i++) cout << rtaps[i] << ", "; 
    cout<<endl<<endl;
    cout << "comp0 array: " << endl;
    for (int i = 0; i<100; i++) cout << comp0[i] << ", "; 
    cout<<endl<<endl;
    cout << "comp1 array: " << endl;
    for (int i = 0; i<100; i++) cout << comp1[i] << ", "; 
    cout<<endl<<endl;
    cout << "fb0 array: " << endl;
    for (int i = 1; i<99; i++) cout << fb0[i] << ", "; 
    cout<<endl<<endl;
    cout << "fb1 array: " << endl;
    for (int i = 1; i<99; i++) cout << fb1[i] << ", "; 
    cout<<endl<<endl;*/
    
    string input_str;
    read_plaintext(input_str);
    int temp, count;
    bool bit_stream[8*input_str.length()];
    for (int i=0; i<input_str.length(); i++) {
        temp = (int)input_str[i];
        count = 8;
        while (count) {
            count--;
            bit_stream[8*i+count]=temp%2;
            temp/=2;
        }
    }

    /*cout << "iv array: " << endl;
    for (int i = 0; i<iv_length; i++) cout << iv[i] << ", "; 
    cout<<endl<<endl;
    cout << "k array: " << endl;
    for (int i = 0; i<80; i++) cout << k[i] << ", "; 
    cout<<endl<<endl;
    cout << "rtap array: " << endl;
    for (int i = 0; i<100; i++) cout << rtaps[i] << ", "; 
    cout<<endl<<endl;
    cout << "comp0 array: " << endl;
    for (int i = 0; i<100; i++) cout << comp0[i] << ", "; 
    cout<<endl<<endl;
    cout << "comp1 array: " << endl;
    for (int i = 0; i<100; i++) cout << comp1[i] << ", "; 
    cout<<endl<<endl;
    cout << "fb0 array: " << endl;
    for (int i = 1; i<99; i++) cout << fb0[i] << ", "; 
    cout<<endl<<endl;
    cout << "fb1 array: " << endl;
    for (int i = 1; i<99; i++) cout << fb1[i] << ", "; 
    cout<<endl<<endl;
    cout << "r array: " << endl;
    for (int i = 0; i<100; i++) cout << r[i] << ", ";
    cout<<endl<<endl;
    cout << "s array: " << endl;
    for (int i = 0; i<100; i++) cout << s[i] << ", ";
    cout<<endl<<endl;
    cout << endl;*/ 
    bool cyphertext[8*(input_str.length()+1)];
    bool keystream[8*(input_str.length()+1)];

    //place chrono here
    std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
    init(r,s,k,iv,iv_length);
    for (int i = 0; i<8*(input_str.length()+1); i++) {
        keystream[i]=r[0]^s[0];
        cyphertext[i]=bit_stream[i]^keystream[i];
        clock_kg(r,s,0,0);
    }
    end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

    /*cout << "Cyphertext: ";
    for (int i = 0; i<8*(input_str.length()+1); i++) {if (!(i%4)) cout << " "; cout << cyphertext[i];}
    cout << endl;*/
    return 0;
}
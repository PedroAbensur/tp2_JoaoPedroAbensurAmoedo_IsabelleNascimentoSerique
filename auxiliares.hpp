#include <iostream>
#include <string>
#include <fstream>

using namespace std;

/*Struct para receber os artigos a partir do arquivo*/
typedef struct Artigo{
    int id;
    string titulo;
    int ano;
    string autores;
    int citacoes;
    string atualizacao;
    string snippet;
} Artigo;

/*Função para converter uma string do arquivo para uma estrutura de artigo*/
Artigo stringArtigo(string line, ifstream& finput);

/*Função para converter um inteiro para vetor de bytes*/
unsigned char* intBytes(unsigned int n);

/*Função para converter short para vetor de bytes*/
unsigned char* shortBytes(short int n);

/*Função para converter uma string para vetor de bytes*/
unsigned char* stringBytes(string str);

/*Função para converter uma estrutura de artigo para vetor de bytes*/
unsigned char* ArtigoBytes(Artigo registro);

/*Função para converter decimal para hexadecimal*/
char decHex(int n);

/*Função para converter vetor de bytes para hexadecimal*/
string bytesHex(unsigned char byte);

/*Função para hexadecimal para inteiro*/
int hexInt(string hex);

/*Função para obter o valor char de um valor hexadecimal*/
unsigned char hexVal(unsigned char c);

/*Função para converter hexadecimal para string*/
string hexString(const string& in);

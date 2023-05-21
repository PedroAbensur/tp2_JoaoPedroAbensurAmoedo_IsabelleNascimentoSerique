#include "auxiliares.hpp"

using namespace std;

char decHex(int n) {
    if(n == 0) return '0';
    else if(n == 1) return '1';
    else if(n == 2) return '2';
    else if(n == 3) return '3';
    else if(n == 4) return '4';
    else if(n == 5) return '5';
    else if(n == 6) return '6';
    else if(n == 7) return '7';
    else if(n == 8) return '8';
    else if(n == 9) return '9';
    else if(n == 10) return 'A';
    else if(n == 11) return 'B';
    else if(n == 12) return 'C';
    else if(n == 13) return 'D';
    else if(n == 14) return 'E';
    else return 'F';   
}

string bytesHex(unsigned char byte){
    string str1(1, decHex((byte >> 4) & 0xF));
    string str2(1, decHex(byte & 0xF));
    str1.append(str2);
    return str1;
}

int hexInt(string hex){
    int i;
    sscanf(hex.c_str(), "%x", &i);
    return i;
}

unsigned char hexVal(unsigned char c){
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    else if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    else abort();
}

string hexString(const string& in){
    string out;
    out.reserve(in.length() / 2);
    for (string::const_iterator p = in.begin(); p != in.end(); p++) {
       unsigned char c = hexVal(*p);
       p++;
       if (p == in.end()) break;
       c = (c << 4) + hexVal(*p);
       out.push_back(c);
    }
    return out;
}

unsigned char* intBytes(unsigned int n){
    unsigned char* bytes = (unsigned char*) malloc(sizeof(unsigned char)*sizeof(unsigned int));
    bytes[0] = (n >> 24) & 0xFF;
    bytes[1] = (n >> 16) & 0xFF;
    bytes[2] = (n >> 8) & 0xFF;
    bytes[3] = (n >> 0) & 0xFF;
    return bytes;
}

unsigned char* shortBytes(short int n){
    unsigned char* bytes = (unsigned char*) malloc(sizeof(unsigned char)*sizeof(short int));
    bytes[0] = (n >> 8) & 0xFF;
    bytes[1] = (n >> 0) & 0xFF;
    return bytes;
}

unsigned char* stringBytes(string str){
    int size = str.size();
    unsigned char* bytes = (unsigned char*) malloc(sizeof(unsigned char)*(size));
    for(int i=0; i < size; i++){
        bytes[i] = (unsigned char) str.at(i);
    }
    return bytes;
}

unsigned char* ArtigoBytes(Artigo artigo){
    unsigned char *tam_bytes,*bytes,*id, *tam_titulo, *titulo, *ano, *tam_autores, 
    *autores, *citacoes, *tam_atualizacao, *atualizacao, *tam_snippet, *snippet;  
    int n_bytes, pos = 0;

    id = intBytes((unsigned int) artigo.id); 
    tam_titulo = shortBytes((short int) artigo.titulo.size()); 
    titulo = stringBytes(artigo.titulo);
    ano = intBytes((unsigned int) artigo.ano);
    tam_autores = shortBytes((short int) artigo.autores.size());
    autores = stringBytes(artigo.autores);
    citacoes = intBytes((unsigned int) artigo.citacoes);
    tam_atualizacao = shortBytes((short int) artigo.atualizacao.size());
    atualizacao = stringBytes(artigo.atualizacao);
    tam_snippet = shortBytes((short int) artigo.snippet.size());
    snippet = stringBytes(artigo.snippet);
    n_bytes = sizeof(int)*3 + sizeof(short int)*4 + artigo.titulo.size() + artigo.autores.size() + artigo.atualizacao.size() + artigo.snippet.size();
    bytes = (unsigned char*) malloc(sizeof(unsigned char*)*(n_bytes + 2));
    tam_bytes = shortBytes((short int) n_bytes);

    for(int i = 0; i < sizeof(short int); i++){
        bytes[pos++] = tam_bytes[i];
    }

    for(int i = 0; i < sizeof(int); i++){
        bytes[pos++] = id[i];
    }

    for(int i = 0; i < sizeof(short int); i++){
        bytes[pos++] = tam_titulo[i];
    }

    for(int i = 0; i < artigo.titulo.size(); i++){
        bytes[pos++] = titulo[i];
    }

    for(int i = 0; i < sizeof(int); i++){
        bytes[pos++] = ano[i];
    }

    for(int i = 0; i < sizeof(short int); i++){
        bytes[pos++] = tam_autores[i];
    }

     for(int i = 0; i < artigo.autores.size(); i++){
        bytes[pos++] = autores[i];
    }

    for(int i = 0; i < sizeof(int); i++){
        bytes[pos++] = citacoes[i];
    }

    for(int i = 0; i < sizeof(short int); i++){
        bytes[pos++] = tam_atualizacao[i];
    }

    for(int i = 0; i < artigo.atualizacao.size(); i++){
        bytes[pos++] = atualizacao[i];
    }

    for(int i = 0; i < sizeof(short int); i++){
        bytes[pos++] = tam_snippet[i];
    }

    for(int i = 0; i < artigo.snippet.size(); i++){
        bytes[pos++] = snippet[i];
    }

    free(tam_bytes);
    free(id); 
    free(tam_titulo);
    free(titulo);
    free(ano); 
    free(tam_autores);
    free(autores);
    free(citacoes);
    free(tam_atualizacao);
    free(atualizacao);
    free(tam_snippet);
    free(snippet);
    return bytes;
}

Artigo stringArtigo(string line, ifstream& finput){
    int n_campos_restantes = 6, pos;
    string token, delimiter = "\";", cell, aux;
    Artigo artigo;

    while(n_campos_restantes >= 0) {
        if((pos = line.find(delimiter)) != string::npos) {
            cell = line.substr(0, (pos));
            if(n_campos_restantes == 6) {
                artigo.id = stoi(cell.substr(1, cell.length()-1));
            }
            else if(n_campos_restantes == 5) {
                if(cell[0] == ';') {
                    artigo.titulo = "";
                    cell = cell.substr(2, cell.length()-1);
                    artigo.ano = stoi(cell);
                    n_campos_restantes--;
                }
                else {
                    artigo.titulo = cell.substr(1, cell.length()-1);
                }
            }
            else if(n_campos_restantes == 4) {
                if(cell[0] == ';') {
                    artigo.ano = 0;
                    cell = cell.substr(2, cell.length()-1);
                    artigo.autores = cell;
                    n_campos_restantes--;
                }
                else{
                    artigo.ano = stoi(cell.substr(1, cell.length()-1));
                }
            }
            else if(n_campos_restantes == 3) {
                if(cell[0] == ';') {
                    artigo.autores = "";
                    cell = cell.substr(2, cell.length()-1);
                    artigo.citacoes = stoi(cell);
                    n_campos_restantes--;
                }
                else {
                    artigo.autores = cell.substr(1, cell.length()-1);
                } 
            }
            else if(n_campos_restantes == 2) {
                if(cell[0] == ';') {
                    artigo.citacoes = 0;
                    cell = cell.substr(2, cell.length()-1);
                    artigo.atualizacao = cell;
                    n_campos_restantes--;
                }
                else {
                    artigo.citacoes = stoi(cell.substr(1, cell.length()-1));
                }
            }
            else if(n_campos_restantes == 1) {
                if(cell[0] == ';') {
                    artigo.atualizacao = "";
                    cell = cell.substr(2, cell.length()-1);
                    artigo.snippet = cell;
                    n_campos_restantes--;
                }
                else {
                    artigo.atualizacao = cell.substr(1, cell.length()-1);
                }  
            }
            n_campos_restantes--;
            line.erase(0, pos + delimiter.length());
        }
        else {
            if(n_campos_restantes == 0) {
            artigo.snippet = line;
            n_campos_restantes --;
            }
            else {
                getline(finput, aux);
                line = line.substr(0,line.length()-1);
                line.append(aux);
            }
        }
    }
    return artigo;
}

#include "auxiliares.hpp"

#define BUCKETS 300000
#define TAM_BUCKET 4100
#define TAM_BLOCK 4096
#define hash(x) (x % BUCKETS)

using namespace std;

const char BUCKET_INIT[8] = {'F','F','F','F','F','F','F','F'};

/*Função para printar o resultado da consulta do registro (artigo)*/
void ShowArtigoFind(int id, FILE *f) {
    string stringInt, string;
    int tam;
    cout << "ID: " << id << endl;

    stringInt.resize(4);
    fread(&stringInt[0], 2, 2, f);
    tam = hexInt(stringInt);
    string.resize(tam*2);
    fread(&string[0], 2, tam, f);
    cout << "Titulo: " << hexString(string) << endl;

    stringInt.resize(8);
    fread(&stringInt[0], 2, 4, f);
    cout << "Ano: " << hexInt(stringInt) << endl;

    stringInt.resize(4);
    fread(&stringInt[0], 2, 2, f);
    tam = hexInt(stringInt);
    string.resize(tam*2);
    fread(&string[0], 2, tam, f);
    cout << "Autores: " << hexString(string) << endl;

    stringInt.resize(8);
    fread(&stringInt[0], 2, 4, f);
    cout << "Citações: " << hexInt(stringInt) << endl;

    stringInt.resize(4);
    fread(&stringInt[0], 2, 2, f);
    tam = hexInt(stringInt);
    string.resize(tam*2);
    fread(&string[0], 2, tam, f);
    cout << "Atualização: " << hexString(string) << endl;

    stringInt.resize(4);
    fread(&stringInt[0], 2, 2, f);
    tam = hexInt(stringInt);
    string.resize(tam*2);
    fread(&string[0], 2, tam, f);
    cout << "Snippet: " << hexString(string) << endl;
}

/*Função para encontrar o registro (artigo), realiza busca no arquivo de dados*/
void buscaArtigo(int id){
    FILE *f = fopen("hashfile", "r");
    unsigned int bucket = hash(id), idDec, artigoTam, ptrEndBucket, current;
    string idHex, artigoTamHex, bucketOverflow, bytesLivresBloco;
    bytesLivresBloco.resize(4);
    bool foundFlag = false;
    idHex.resize(8); artigoTamHex.resize(4);
    current = (unsigned int) bucket*TAM_BUCKET*2;
    fseek(f, current, SEEK_SET);
    fread(&bytesLivresBloco[0], 2, 2, f);
    ptrEndBucket = (unsigned int) (((bucket * TAM_BUCKET) + (TAM_BLOCK - (unsigned int) hexInt(bytesLivresBloco)))*2);

    while(ftell(f) < ptrEndBucket){
        fread(&artigoTamHex[0], 2, 2, f);
        artigoTam = (unsigned int) hexInt(artigoTamHex);
        fread(&idHex[0], 2, 4, f);
        idDec = (unsigned int) hexInt(idHex);

        if(idDec == id) {
            ShowArtigoFind(id, f); 
            cout << "Blocos lidos: 1" << endl;
            fseek(f,0,SEEK_END);
            cout << "Blocos totais no arquivo de dados: " << (ftell(f)/(long)(TAM_BUCKET*2)) << endl;
            foundFlag = true;
            break;
        }
        else {
            current = (unsigned int) (artigoTam - 4)*2;
            fseek(f, current, SEEK_CUR);
        }
    }
    if(foundFlag == false) {
        bucketOverflow.resize(8);
        current = (unsigned int) (((bucket*TAM_BUCKET)+TAM_BUCKET-(4))*2);
        fseek(f, ((bucket*TAM_BUCKET)+TAM_BUCKET-(4))*2, SEEK_SET); 
        fread(&bucketOverflow[0], 2, 4, f);
        bucket =  (unsigned int) hexInt(bucketOverflow);

        if(bucket != (unsigned int)(-1)) { 
            fseek(f, bucket, SEEK_SET);
            fread(&bytesLivresBloco[0], 2, 2, f);
            ptrEndBucket = (bucket)+(unsigned int)(((unsigned int) hexInt(bytesLivresBloco)) * 2);

            while(ftell(f) < ptrEndBucket) {
                fread(&artigoTamHex[0], 2, 2, f);
                artigoTam = (unsigned int) hexInt(artigoTamHex);
                fread(&idHex[0], 2, 4, f);
                idDec = (unsigned int) hexInt(idHex);
                if(idDec == id) {
                    ShowArtigoFind(id, f);
                    cout << "Blocos lidos: 2" << endl;
                    fseek(f,0,SEEK_END);
                    cout << "Blocos totais no arquivo de dados: " << (ftell(f)/(long)(TAM_BUCKET*2)) << endl;
                    foundFlag = true;
                    break;
                }
                else {
                    current = (unsigned int) (artigoTam - 4)*2;
                    fseek(f, current, SEEK_CUR);
                }
            }
        }
    }
    if(!foundFlag) cout << "Nao foi encontrado o ID";
}

int main(int argc, char** argv) {
    if(argc != 2){
        cout << "Erro: ID nao especificado" << endl;
        cout << "Use: ./findrec <ID>" << endl;
        exit(1);
    }
    string arg1(argv[1]);
    buscaArtigo(stoi(arg1));

    return 0;
} 
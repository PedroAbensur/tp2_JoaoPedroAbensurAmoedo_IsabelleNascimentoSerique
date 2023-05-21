#include "auxiliares.hpp"

#define BUCKETS 300000
#define TAM_BUCKET 4100
#define TAM_BLOCK 4096
#define hash(x) (x % BUCKETS)

using namespace std;

const char BUCKET_INIT[8] = {'F','F','F','F','F','F','F','F'};

void insereArtigo(FILE *f, unsigned int bucket, string hexArtigo){
    string tamBloco, newBucket;
    unsigned char* aux;
    unsigned int n_bytes, artigo, current, ptr, ptrBucketAnterior;
    short int bytesBloco;

    n_bytes = (unsigned int) (hexArtigo.size()/2);
    current = (unsigned int) (bucket*TAM_BUCKET*2);
    fseek(f, current, SEEK_SET);
    tamBloco.resize(4); newBucket.resize(8);
    fread(&tamBloco[0], 2, 2, f);
    bytesBloco = hexInt(tamBloco);

    if(bytesBloco >= n_bytes) {
        current = (unsigned int) (((TAM_BLOCK - 2) - bytesBloco)*2);
        fseek(f, current, SEEK_CUR);
        fwrite(hexArtigo.c_str(), sizeof(char), hexArtigo.length(), f);

        current = (unsigned int) ((bucket*TAM_BUCKET)*2);
        fseek(f, current, SEEK_SET);

        bytesBloco = bytesBloco - n_bytes;
        aux = shortBytes(bytesBloco);
        tamBloco = bytesHex(aux[0]);
        tamBloco.append(bytesHex(aux[1]));

        free(aux);
        fwrite(tamBloco.c_str(), 2, 2, f);

    }

    else {
        current = (unsigned int) ((TAM_BLOCK-2)*2);
        fseek(f, current, SEEK_CUR);
        fread(&newBucket[0], 2, 4, f);
        bucket = (unsigned int) hexInt(newBucket);

        if(bucket != (unsigned int)(-1)) {
            fseek(f, bucket, SEEK_SET);
            fread(&tamBloco[0], 2, 2, f);
            bytesBloco = hexInt(tamBloco);

            current = (unsigned int) (((TAM_BLOCK - 2) - bytesBloco)*2);
            fseek(f, current, SEEK_CUR);
            fwrite(hexArtigo.c_str(), sizeof(char), hexArtigo.length(), f);

            fseek(f, bucket, SEEK_SET);
            bytesBloco = bytesBloco - n_bytes;
            aux = shortBytes(bytesBloco);
            tamBloco = bytesHex(aux[0]);
            tamBloco.append(bytesHex(aux[1]));

            free(aux);
            fwrite(tamBloco.c_str(), 2, 2, f);

        }

        else {
            fseek(f, -(4*2), SEEK_CUR);
            ptrBucketAnterior = (unsigned int) ftell(f);
            fseek(f, 0, SEEK_END);

            ptr = (unsigned int) ftell(f);
            bytesBloco = (TAM_BLOCK - 2) - n_bytes;
            aux = shortBytes(bytesBloco);
            tamBloco = bytesHex(aux[0]);
            tamBloco.append(bytesHex(aux[1]));

            free(aux);

            fwrite(tamBloco.c_str(), 2, 2, f);
            fseek(f, (TAM_BLOCK-2)*2, SEEK_CUR);
            fwrite(BUCKET_INIT, 2, 4, f); 

            current = (unsigned int) (ptr+(2*2));
            fseek(f, current, SEEK_SET);
            fwrite(hexArtigo.c_str(), sizeof(char), hexArtigo.length(), f);
            fseek(f, ptrBucketAnterior, SEEK_SET);

            aux = intBytes(ptr);
            newBucket = bytesHex(aux[0]);
            newBucket.append(bytesHex(aux[1]));
            newBucket.append(bytesHex(aux[2]));
            newBucket.append(bytesHex(aux[3]));
            
            free(aux);
            fwrite(newBucket.c_str(), 2, 4, f);
        }
    }
}

void upload(string file){
    Artigo artigo;
    int n_bytes;
    ifstream finput(file);
    FILE *fout = fopen("hashfile", "w+");
    string line, hexBytes = "";
    unsigned char* bytesArtigo;
    unsigned char *tamBlocoLivre = shortBytes(TAM_BLOCK-2);
    string bytesBloco = bytesHex(tamBlocoLivre[0]);
    bytesBloco.append(bytesHex(tamBlocoLivre[1]));

    for(int i = 0; i < BUCKETS; i++){
        fwrite(bytesBloco.c_str(), 2, 2, fout);
        fseek(fout, (TAM_BLOCK-2)*2, SEEK_CUR); 
        fwrite(BUCKET_INIT, 2, 4, fout); 
    }

    while(getline(finput, line)){
        artigo = stringArtigo(line, finput);
        bytesArtigo = ArtigoBytes(artigo);
        n_bytes = (((short int) bytesArtigo[0]) << 8) + ((short int) bytesArtigo[1]) + 2;
        hexBytes.clear();
        for(int i=0; i < n_bytes; i++){
            hexBytes.append(bytesHex(bytesArtigo[i]));
        }
        insereArtigo(fout, hash(artigo.id), hexBytes);
    }

    free(tamBlocoLivre);
    fclose(fout);
}

int main(int argc, char** argv) {
    if(argc != 2){
        cout << "Erro: Arquivo nao especificado" << endl;
        cout << "Use: ./upload <arquivo>" << endl;
        exit(1);
    }
    
    string arg1(argv[1]);
    upload(arg1);
    return 0;
} 
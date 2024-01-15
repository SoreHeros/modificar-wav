#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char * ChunkId;
    unsigned int ChunkSize;
    char * Format;
    char * Subchunk1ID;
    unsigned int Subchunk1Size;
    unsigned short int AudioFormat;
    unsigned short int NumChanels;
    unsigned int SampleRate;
    unsigned int ByteRate;
    unsigned short int BlockAlign;
    unsigned short int BitsPerSample;
    char * Subchunk2ID;
    unsigned int Subchunk2Size;
    unsigned int * Data;
}* wav;


unsigned int read_int(FILE * file){
    unsigned int num = 0;
    unsigned char c;
    for(int i = 0; i < 4; i++){
        fscanf(file, "%c", &c);
        num /= 256;
        num += c * 256 * 256 * 256;
    }
    return num;
}

unsigned short int read_short(FILE * file){
    unsigned int num = 0;
    unsigned char c;
    for(int i = 0; i < 2; i++){
        fscanf(file, "%c", &c);
        num /= 256;
        num += c * 256;
    }
    return num;
}

char * read_string(FILE * file, int size){
    char c;
    char * string = malloc(size + 1);
    for(int i = 0; i < size; i++){
        fscanf(file, "%c", &c);
        string[i] = c;
    }
    string[size] = '\0';
    return string;
}

int main(){

    FILE * archivo;
    archivo = fopen("test.wav", "r");

    wav audio = malloc(sizeof(wav));


    char * string;

    printf("\n\n\n\n");

    printf("\nChunkID:\t");
    printf("%s", audio->ChunkId = read_string(archivo, 4));


    printf("\nChunkSize:\t");
    printf("%u", audio->ChunkSize = read_int(archivo));

    printf("\nFormat: \t");
    printf("%s", audio->Format = read_string(archivo, 4));

    printf("\n");

    printf("\nSubchunk1Id:\t");
    printf("%s", audio->Subchunk1ID = read_string(archivo, 4));

    printf("\nSubchunk1size:\t");
    printf("%u", audio->Subchunk1Size = read_int(archivo));

    printf("\nAudioFormat:\t");
    printf("%u", audio->AudioFormat = read_short(archivo));

    printf("\nNumChanels:\t");
    printf("%u", audio->NumChanels = read_short(archivo));

    printf("\nSampleRate:\t");
    printf("%u", audio->SampleRate = read_int(archivo));

    printf("\nByteRate:\t");
    printf("%u", audio->ByteRate = read_int(archivo));

    printf("\nBlockAlign:\t");
    printf("%u", audio->BlockAlign = read_short(archivo));

    printf("\nBitsPerSample:\t");
    printf("%u", audio->BitsPerSample = read_short(archivo));

    printf("\n");

    printf("\nSubchunk2Id:\t");
    printf("%s", audio->Subchunk2ID = read_string(archivo, 4));

    printf("\nSubchunk2size:\t");
    printf("%u", audio->Subchunk2Size = read_int(archivo));

    printf("\nData:\n");
    audio->Data = malloc(audio->Subchunk2Size);

    //todo leer stereo

    if(audio->BitsPerSample == 16){
        for(int i = 0; i < audio->Subchunk2Size / 4; i++){
            printf("%u", audio->Data[i] = read_int(archivo));
        }
    }else{
        for(int i = 0; i < audio->Subchunk2Size / 4; i++){
            printf("%u", audio->Data[i] = read_short(archivo));
        }
    }

    //todo liberar memoria del wav
    free(audio->Data);
    fclose(archivo);
    printf("\n\n\n\n\n");
    getchar();

}
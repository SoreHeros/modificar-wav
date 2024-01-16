#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int * * chanelData;
}wav;

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

int short_to_int_escalado(short int in){
    int num = 0;

    if(in < 0)
        num = -0x8000;

    num += in & 0x7fff;

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

void loading_bar(float value, int size){
    int i;
    printf("\r%5.1f%%[",  value * 100);
    for(i = 0; i < value * size; i++)
        printf("%c", 219);

    for(; i < size; i++)
        printf(" ");
    printf("]");
}

wav * cargar_wav(char * filename){

    FILE * archivo;
    archivo = fopen(filename, "rb");

    if(archivo == NULL){
        printf("ERROR: Archivo no encontrado");
        return NULL;
    }


    wav * audio = malloc(sizeof(wav));


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

    printf("\nData:      \t");
    audio->chanelData = malloc(audio->NumChanels * sizeof(int *));

    //reservar memoria para los canales
    for(int i = 0; i < audio->NumChanels; i++){
        audio->chanelData[i] = malloc(audio->Subchunk2Size / audio->NumChanels / (audio->BitsPerSample / 8) * sizeof(int));
    }

    //todo leer stereo
    for (int i = 0; i < audio->Subchunk2Size / (audio->BitsPerSample / 8); i++) {
        if (audio->BitsPerSample == 32) {
            audio->chanelData[i % audio->NumChanels][i / audio->NumChanels] = (int) read_int(archivo);
        } else {
            audio->chanelData[i % audio->NumChanels][i / audio->NumChanels] = short_to_int_escalado((short int) read_short(archivo));
        }
    }

    printf("cargada\n");

    fclose(archivo);

    return audio;
}

void guardar_wav(wav * audio){
    char * nombre = malloc(1000);
    FILE * archivo;

    printf("\nIntroduce el nombre del archivo guardado\n");
    scanf("%s", nombre);

    archivo = fopen(nombre, "w");

    fwrite(audio->ChunkId, 1, 4, archivo);
    fwrite(&audio->ChunkSize, 4, 1, archivo);
    fwrite(audio->Format, 1, 4, archivo);
    fwrite(audio->Subchunk1ID, 1, 4, archivo);
    fwrite(&audio->Subchunk1Size, 4, 1, archivo);
    fwrite(&audio->AudioFormat, 2, 1, archivo);
    fwrite(&audio->NumChanels, 2, 1, archivo);
    fwrite(&audio->SampleRate, 4, 1, archivo);
    fwrite(&audio->ByteRate, 4, 1, archivo);
    fwrite(&audio->BlockAlign, 2, 1, archivo);
    fwrite(&audio->BitsPerSample, 2, 1, archivo);
    fwrite(audio->Subchunk2ID, 1, 4, archivo);
    fwrite(&audio->Subchunk2Size, 4, 1, archivo);

    char * datos = malloc(audio->Subchunk2Size);

    //rellenar datos
    for(int i = 0; i < audio->Subchunk2Size / (audio->BitsPerSample / 8); i++)
        fwrite(&audio->chanelData[i % audio->NumChanels][i / audio->NumChanels], 1, (audio->BitsPerSample / 8), archivo); //por algún motivo hay que darle la vuelta a la orientación de los bits


    fclose(archivo);
    free(datos);
    free(nombre);
}

void cerrar_wav(wav * audio){
    printf("\ncerrando wav...\n");
    if(audio == NULL) {
        printf("El wav no existe\n");
        return;
    }


    for(int i = 0; i < audio->NumChanels; i++)
        free(audio->chanelData[i]);
    free(audio->chanelData);
    free(audio->Subchunk2ID);
    free(audio->Subchunk1ID);
    free(audio->Format);
    free(audio);

    printf("wav cerrado con exito!\n");
}

int main(){

    wav * audio = NULL;
    char * menu = malloc(100);

    do {
        if(audio == NULL) {
            printf("\nIntroduzca el nombre del archivo a cargar\n");
            scanf("%s", menu);
            audio = cargar_wav(menu);
        }else {

            printf("\nIntroduce lo que quieres hacer:\n");
            printf("cerrar: cerrar el archivo para abrir otro\n");
            printf("guardar: guarda un archivo\n");
            printf("exit: salir\n");

            scanf("%s", menu);

            if (!strcmp(menu, "cerrar")) {
                cerrar_wav(audio);
                audio = NULL;
            }else if(!strcmp(menu, "guardar"))
                guardar_wav(audio);
        }

    } while(strcmp(menu, "exit"));

    cerrar_wav(audio);
}
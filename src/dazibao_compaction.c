#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <errno.h>

#include "dazibao.h"
#include "dazibao_utilities.h"

unsigned char* convertIntSizeToCharSize(int size) { /* Convertie la taille d'un int sur 3 octets */

        int nombre = size;
        unsigned char *length = (unsigned char *) malloc (3);
        char *msg_error_size = "Erreur taille: TLV trop grande\n";

        if( nombre > 16843008 ) {
                write(STDIN_FILENO, msg_error_size, strlen(msg_error_size));
        }

        if( nombre > 65792 ) {
                length[0] = size / 256; /* Premier octect */
                size = size % 256;
        }
        else {
                length[0] = 0;
        }

        if( nombre > 256 ) {
                length[1] = size / 256; /* Deuxième octet */
                size = size % 256;
        } else {
                length[1] = 0;
        }
        
        length[2] = size; /* Troisième octet */

        return length;

}

void callDeletePad(int f, char *file) {

        int shift;

        if(flock(f, LOCK_EX) < 0) { /* Verrou sur le fichier */
                exit(EXIT_FAILURE);
        }

        lseek(f, 4, SEEK_SET); /* Ignore l'entête */
        shift = deletePad(f, get_file_size(file), 0); /* Supprime les pad */
        truncate(file, get_file_size(file) - shift); /* Tronque le fichier */
        
        if(flock(f, LOCK_UN) < 0) { /* Déverrouillage fichier */
                exit(EXIT_FAILURE);
        }

}

int deletePad(int f, int sizeLoop, int val) {

        off_t tmpPos;
        off_t curRead;
        int fr;
        int size;
        int tmpVal;
        int shift_start;
        int shift_end;
        int shift = val;
        char *buf_copy;
        unsigned char date[4];
        unsigned char buff;
        unsigned char length[3];
        unsigned char *newLength;

        while (((fr = read(f, &buff, 1)) > 0) && (lseek(f,0,SEEK_CUR) <= sizeLoop)) {

                if (buff == 0) { /* TLV PAD1 */
                        
                        shift = shift + 1; /* incrémentation de la valeur du décallage de 1 comme pad1 */

                } else {

                        read(f, &length, sizeof(length));
                        size = length[0]*256*256 + length[1]*256 + length[2];

                        if (buff == 1) { /* TLV PADN */
                                
                                lseek(f, ( lseek(f, 0, SEEK_CUR) + size) , SEEK_SET);
                                shift = shift + size + 4; /* incrémentation de la valeur du décallage de size + 4 (pour l'entête) comme c'est un padN */

                        } else if (buff == 5) { /* TLV COMPOUND */

                                tmpPos = lseek(f, 0, SEEK_CUR);
                                shift_start = shift;
                                tmpVal = deletePad(f, (tmpPos + size), shift); /* appel récursif */

                                if(tmpVal != 0) {

                                        lseek(f, tmpPos - (shift + 4), SEEK_SET);
                                        shift_end = tmpVal - shift_start; /* calcul du décalage dans un compound */
                                        newLength = convertIntSizeToCharSize(size - shift_end); /* nouvelle taille convertie */
                                        write(f, &buff, 1);
                                        write(f, &newLength[0], 1);
                                        write(f, &newLength[1], 1);
                                        write(f, &newLength[2], 1);
                                        free(newLength);

                                }

                                lseek(f, (tmpPos + size), SEEK_SET);
                                shift = tmpVal; /* affectation de la nouvelle valeur de décalage */

                        } else if (buff == 6) { /* TLV DATED */

                                tmpPos = lseek(f, 0, SEEK_CUR);
                                read(f, &date, 4);
                                shift_start = shift;
                                tmpVal = deletePad(f, (tmpPos + size), shift); /* appel récursif */

                                if(tmpVal != 0) {

                                        lseek(f, tmpPos - (shift + 4), SEEK_SET);
                                        shift_end = tmpVal - shift_start; /* calcul du décalage dans un dated */
                                        newLength = convertIntSizeToCharSize(size - shift_end); /* nouvelle taille convertie */
                                        write(f, &buff, 1);
                                        write(f, &newLength[0], 1);
                                        write(f, &newLength[1], 1);
                                        write(f, &newLength[2], 1);
                                        write(f, &date, 4);
                                        free(newLength);
                                        
                                }

                                lseek(f, (tmpPos + size), SEEK_SET);
                                shift = tmpVal; /* affectation de la nouvelle valeur de décalage */

                        } else { /* AUTRES TLV */

                                if( shift == 0 ) { /* Si la valeur du décallage est égale à zéro on saute le bloc */

                                        lseek(f, ( lseek(f, 0, SEEK_CUR) + size) , SEEK_SET);

                                } else { /* Sinon on recopie la TLV à la bonne place */

                                        buf_copy = malloc (size * sizeof(char)); /* allocation mémoire pour stocket le contenu de la TLV */
                                        read(f, buf_copy, size);
                                        curRead = lseek(f, 0, SEEK_CUR); /* sauvegarde de la position courante de lecture */
                                        lseek(f, (lseek(f, 0, SEEK_CUR)) - (shift + size + 4), SEEK_SET); /* repositionnement en avant */
                                        write(f, &buff, 1); /* type */
                                        write(f, &length, 3); /* taille */
                                        write(f, buf_copy, size/*strlen(buf_copy)*/); /* corps */
                                        lseek(f, curRead, SEEK_SET); /* repositionnement au bon emplacement pour lire les TLV suivant */
                                        free(buf_copy);

                                }

                        }

                }
                        
        }

        return(shift);

}

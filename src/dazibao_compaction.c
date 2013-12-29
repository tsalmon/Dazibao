#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/file.h>
#include <errno.h>

#include "dazibao.h"
#include "dazibao_utilities.h"
#include "dazibao_safe.h"

void callDeletePad(int f, char *file) {

	int shift;

	if (!safe_flock(f, LOCK_EX)) exit(7); /* Verrou sur le fichier */

	if (!safe_lseek(f, 4, SEEK_SET)) exit(4); /* Ignore l'entête */
	
	shift = deletePad(f, get_file_size(file), 0); /* Supprime les pad */
	
	if (truncate(file, get_file_size(file) - shift) == -1) {  /* Tronque le fichier */
		perror("Truncate Error:");
		exit(EXIT_FAILURE);
	}

	if (!safe_flock(f, LOCK_UN)) exit(7); /* Déverrouillage fichier */

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

			if (!safe_read(f, &length, sizeof(length))) exit(5);
			size = length[0] * 256 * 256 + length[1] * 256 + length[2];

			switch(buff) {

				case 1: /* TLV PADN */
				
					if (!safe_lseek(f, ( lseek(f, 0, SEEK_CUR) + size) , SEEK_SET)) exit(4);
					shift = shift + size + 4; /* incrémentation de la valeur du décallage de size + 4 (pour l'entête) comme c'est un padN */
					
					break;

				case 5: /* TLV COMPOUND ET TLV DATED */
				case 6:

					tmpPos = lseek(f, 0, SEEK_CUR);
					if (tmpPos == - 1) exit(4);
					
					if (buff == 6) {

						if (!safe_read(f, &date, 4)) exit(5);

					}

					shift_start = shift;
					tmpVal = deletePad(f, (tmpPos + size), shift); /* appel récursif */

					if(tmpVal != 0) {

						if (!safe_lseek(f, tmpPos - (shift + 4), SEEK_SET)) exit(4);
						shift_end = tmpVal - shift_start; /* calcul du décalage dans un compound */
						newLength = convert_int_to_char(size - shift_end); /* nouvelle taille convertie */
						if (!safe_write(f, &buff, 1)) exit(6);
						if (!safe_write(f, &newLength[0], 1)) exit(6);
						if (!safe_write(f, &newLength[1], 1)) exit(6);
						if (!safe_write(f, &newLength[2], 1)) exit(6);

						if (buff == 6) {
							
							if (!safe_write(f, &date, 4)) exit(6);

						}

						safe_free(newLength);

					}

					if (!safe_lseek(f, (tmpPos + size), SEEK_SET)) exit(4);
					shift = tmpVal; /* affectation de la nouvelle valeur de décalage */
					break;

				default: /* AUTRES TLV */

					if( shift == 0 ) { /* Si la valeur du décallage est égale à zéro on saute le bloc */

						if (!safe_lseek(f, ( lseek(f, 0, SEEK_CUR) + size) , SEEK_SET)) exit(4);

					} else {   /* Sinon on recopie la TLV à la bonne place */

						buf_copy = safe_malloc (size * sizeof(char)); /* allocation mémoire pour stocket le contenu de la TLV */
						if (buf_copy == NULL) exit(1);
						if (!safe_read(f, buf_copy, size)) exit(5);
						curRead = lseek(f, 0, SEEK_CUR); /* sauvegarde de la position courante de lecture */
						if (curRead < 0) exit(4);
						if (!lseek(f, (curRead) - (shift + size + 4), SEEK_SET)) exit(4); /* repositionnement en avant */ 
						if (!safe_write(f, &buff, 1)) exit(6);          /* type */
						if (!safe_write(f, &length, 3)) exit(6);        /* taille */
						if (!safe_write(f, buf_copy, size)) exit(6);    /* corps */
						if (!lseek(f, curRead, SEEK_SET)) exit(4); /* repositionnement au bon emplacement pour lire les TLV suivant */
						safe_free(buf_copy);

					}

					break;

				}

			}

		}

	return(shift);

}

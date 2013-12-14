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

char *testTexte = "Est-ce aussi simple d'ajouter une TLV texte?";

unsigned char* convertIntSizeToCharSize(int size) { // Convertie la taille en int sur 3 octets

	unsigned char *length = (unsigned char *) malloc (3);
	
	length[0] = size / 256; // Premier octect
	size = size % 256;
	length[1] = size / 256; // Deuxième  octet
	size = size % 256;
	length[2] = size;       // Troisième octet

	return length;

}

void writeType(int f, int typeTlv) { // Ecris le type de TLV

	int fw;
	int type = typeTlv;
	char *msg_error_write = "Erreur ecriture\n";

	if((fw = write(f, &type, sizeof(char))) == -1) {
		write(STDIN_FILENO, msg_error_write, strlen(msg_error_write));
	}

}

void writeLength(int f, int arg) { // Ecrit la taille du contenu

	int fw;
	int i = 0;
	unsigned char *length = convertIntSizeToCharSize(arg);
	char *msg_error_write = "Erreur ecriture\n";

	while(i < 3) { // Seulement 3 octects sont écris
		if((fw = write(f, &length[i], sizeof(char))) == -1) {
			write(STDIN_FILENO, msg_error_write, strlen(msg_error_write));
		}
		i++;
	}

	free(length); // Libère la mémoire allouée

}

void writeData(int f, char *arg) { // Ecrit les données

	int fw;
	int j = 0;
	char *msg_error_write = "Erreur ecriture\n";

	while( j < strlen(arg)) {
		if((fw = write(f, &arg[j], sizeof(char))) == -1) {
			write(STDIN_FILENO, msg_error_write, strlen(msg_error_write));
		}
		j++;
	}

}

void addText(int f, int typeTlv, char *arg) { // Ajouter une TLV texte

	writeType(f, typeTlv);       // Type TLV 
	writeLength(f, strlen(arg)); // Taille TLV
	writeData(f, arg);          // Données TLV

}

void addPad1(int f, int typeTlv) { // Ajouter une TLV Pad1

	writeType(f, typeTlv);

}

void addPadN(int f, int typeTlv, int size) { // Ajouter une TLV PadN
	
	int i = 0;

	writeType(f, typeTlv); // Type TLV
	writeLength(f, size);  // Taille TLV
	
	/*char *c = "a";
	while(i < size) {
		write(f, (char *)&c, sizeof(c));	   // Données TLV
		i++;
	}*/
	while(i < size) {
		writeData(f,"0"); // Données TLV 
		i++;
	}

}

int sizeFile(char *file) { // Obtenir la taille d'une image

	int fs;
	struct stat fileStat;
	char *msg_error_size  = "Erreur taille fichier\n";
	
	if(( fs = stat(file, &fileStat)) != 0 ) {
		write(STDIN_FILENO, msg_error_size, strlen(msg_error_size));
		exit(EXIT_FAILURE);
	}

	return fileStat.st_size;
	
}

void addPicture(int f, int typeTlv, char *arg) { // Ajouter une image

	int fw;
	int fdin;
 	char *src;
	int i = 0;
	unsigned char *length = convertIntSizeToCharSize(sizeFile(arg));
	char *msg_error = "Erreur ouverture\n";
	char *msg_lseek = "Erreur lseek\n";
	char *msg_mmap  = "Erreur mmap\n";
	char *msg_erwr  = "Erreur ecriture\n";

	writeType(f, typeTlv); // Type TLV

	while(i < 3) { // Seulement 3 octects sont écris
		if((fw = write(f, &length[i], sizeof(char))) == -1) {
			write(STDIN_FILENO, msg_error, strlen(msg_error));
		}
		i++;
	}

	free(length); // Libère la mémoire

	if ((fdin = open ("./flower.jpeg", O_RDONLY)) < 0) { // Ouverture du fichier à copier
   write(STDIN_FILENO, msg_error, strlen(msg_error));
	}

	if (lseek (f, 0, SEEK_END) == -1) { // Déplacement du curseur en fin de dazibao
   write(STDIN_FILENO, msg_lseek, strlen(msg_lseek));
	}

	if ((src = mmap (0, sizeFile(arg), PROT_READ, MAP_PRIVATE, fdin, 0)) == (caddr_t) -1) { // Charge en mémoire l'image
   write(STDIN_FILENO, msg_mmap, strlen(msg_mmap));
	}

	//printf("TAILLE IMAGE: %d", sizeFile(arg));
	if ((fw = write(f, src, sizeFile(arg))) != 0) { // Ecris l'image dans le dazibao // <
		write(STDIN_FILENO, msg_erwr, strlen(msg_erwr));	
	}

}

void addToDazibao(char *argv) { // Ecrire dans un dazibao

	int fd;
	char *msg_error = "Erreur du fichier dzb\n";
  char *msg_open  = "Ouverture du fichier dzb\n";
  char *msg_close = "Fermeture du fichier dzb\n";

	char *msg_error_lock   = "Erreur lock\n";
	char *msg_lock         = "Verrou acquis\n";
	char *msg_error_unlock = "Erreur unlock\n";
	char *msg_unlock       = "Verrou supprime\n";
	
	// Ouverture du fichier

	if(( fd = open(argv, O_RDWR)) < 0 ) {
    write(STDIN_FILENO, msg_error, strlen(msg_error));
    exit(EXIT_FAILURE);
  } else {
    write(STDIN_FILENO, msg_open, strlen(msg_open));
  }

	// Verrou sur le fichier

	if(flock(fd, LOCK_EX) < 0) {
		write(STDIN_FILENO, msg_error_lock, strlen(msg_error_lock));
		exit(EXIT_FAILURE);
	} else {
		write(STDIN_FILENO, msg_lock, strlen(msg_lock));
	}

	// Déplacement à la fin du fichier
	
	lseek(fd, 0, SEEK_END);

	// Ajout d'une TLV

	addText(fd, 2, testTexte);
	//addPicture(fd,4,"./flower.jpeg");
	//addText(fd, 2, testTexte);
	addPadN(fd,1,30);
	addPad1(fd, 0);

	// Déverrouillage fichier

	if(flock(fd, LOCK_UN) < 0) {
		write(STDIN_FILENO, msg_error_unlock, strlen(msg_error_unlock));
	} else {
		write(STDIN_FILENO, msg_unlock, strlen(msg_unlock));
	}	

	// Fermeture du fichier
	
	write(STDIN_FILENO, msg_close, strlen(msg_close));
  close(fd);

}

int main(int argc, char *argv[]) {

	char *msg_error_arg = "Erreur argument\n";

	if( argc != 2 ) {
		write(STDIN_FILENO, msg_error_arg, strlen(msg_error_arg));
		exit(EXIT_FAILURE);
	}

  addToDazibao(argv[1]);

	return(0);

}

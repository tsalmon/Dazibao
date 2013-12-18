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

struct tlv { // Structure d'une TLV

	int type;
	char *arg;	
	int length;
	struct tlv *arrayListTlv;
	
};

unsigned char* convertIntSizeToCharSize(int size) { // Convertie la taille d'un int sur 3 octets

	int nombre = size;
	unsigned char *length = (unsigned char *) malloc (3);
	char *msg_error_size = "Erreur taille: TLV trop grande\n";

	if( nombre > 16843008 ) {
		write(STDIN_FILENO, msg_error_size, strlen(msg_error_size));
	}

	if( nombre > 65792 ) {  
		length[0] = size / 256; // Premier octect
		size = size % 256;
	}
	else {
		length[0] = 0;
	}

	if( nombre > 256 ) {
		length[1] = size / 256; // Deuxième octet
		size = size % 256;
	} else {
		length[1] = 0; 
	}
	
	length[2] = size;       // Troisième octet

	return length;

}

int typePicture(char *arg) {

	char *strJpeg = ".jpeg";
	char *strPng  = ".png";

	if( strstr(arg, strJpeg) != NULL ) {
		return (4);
	} else if ( strstr(arg, strPng) != NULL ) {
		return (3);
	}

	return (-1);

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

void writeType(int f, int typeTlv) { // Ecris le type de TLV

	int fw;
	int type = typeTlv;
	char *msg_error_write = "Erreur ecriture type\n";

	if((fw = write(f, &type, sizeof(char))) == -1) {
		write(STDIN_FILENO, msg_error_write, strlen(msg_error_write));
	}

}

void writeLength(int f, int arg) { // Ecris la taille du contenu

	int fw;
	int i = 0;
	unsigned char *length = convertIntSizeToCharSize(arg);
	char *msg_error_write = "Erreur ecriture length\n";

	while(i < 3) { // Seulement 3 octects sont écris
		if((fw = write(f, &length[i], sizeof(char))) == -1) {
			write(STDIN_FILENO, msg_error_write, strlen(msg_error_write));
		}
		i++;
	}

	free(length); // Libère la mémoire allouée

}

void writeData(int f, char *arg) { // Ecris les données

	int fw;
	int j = 0;
	char *msg_error_write = "Erreur ecriture data\n";

	while( j < strlen(arg)) {
		if((fw = write(f, &arg[j], sizeof(char))) == -1) {
			write(STDIN_FILENO, msg_error_write, strlen(msg_error_write));
		}
		j++;
	}

}

void writeDate(int f) { // Ecris la date sur 4 octets

	int currentDate;
	int value;
	time_t t;

	time(&t);             // Récupère la date courante
	currentDate = (int)t; // Convertie en int la date courante

	value = (currentDate >> 24) & 0xFF; // Premier octet
	write(f, &value, 1);

	value = (currentDate >> 16) & 0xFF; // Deuxième octet
	write(f, &value, 1);

	value = (currentDate >> 8) & 0xFF;  // Troisième octet
	write(f, &value, 1);

	value = currentDate & 0xFF;         // Quatrième octet
	write(f, &value, 1);
	
}

void addPad1(int f) { // Ajouter une TLV Pad1

	writeType(f, 0);

}

void addPadN(int f, int size) { // Ajouter une TLV PadN
	
	int i = 0;

	writeType(f, 1); // Type TLV
	writeLength(f, size);  // Taille TLV
	
	while(i < size) {
		writeData(f,"0"); // Données TLV 
		i++;
	}

}

void addText(int f, char *arg) { // Ajouter une TLV texte

	writeType(f, 2);       // Type TLV
	writeLength(f, strlen(arg)); // Taille TLV
	writeData(f, arg);           // Données TLV

}

void addPicture(int f, char *arg) { // Ajouter une image PNG ou JPEG

	int fw;
	int fdin;
 	char *src;
	char *msg_error  = "Erreur ouverture\n";
	char *msg_lseek  = "Erreur lseek\n";
	char *msg_mmap   = "Erreur mmap\n";
	char *msg_erwr   = "Erreur ecriture image\n";
	char *msg_ertype = "Erreur type image\n";

	if( typePicture(arg) != -1) { // Teste si l'argument a bien une extension compatible image
		writeType(f, typePicture(arg)); // Type TLV
	} else {
		write(STDIN_FILENO, msg_ertype, strlen(msg_ertype));
		exit(1);
	}

	writeLength(f, sizeFile(arg)); // Taille TLV

	if ((fdin = open (arg, O_RDONLY)) < 0) { // Ouverture du fichier à copier
   write(STDIN_FILENO, msg_error, strlen(msg_error));
	}

	if (lseek (f, 0, SEEK_END) == -1) { // Déplacement du curseur en fin de dazibao
   write(STDIN_FILENO, msg_lseek, strlen(msg_lseek));
	}

	if ((src = mmap (0, sizeFile(arg), PROT_READ, MAP_PRIVATE, fdin, 0)) == (caddr_t) -1) { // Charge en mémoire l'image
   write(STDIN_FILENO, msg_mmap, strlen(msg_mmap));
	}

	if ((fw = write(f, src, sizeFile(arg))) == -1) { // Ecris l'image dans le dazibao
		write(STDIN_FILENO, msg_erwr, strlen(msg_erwr));	
	}

}

void addBloc(int f, int typeTlv, struct tlv *arrayTlv, int posTab, int countTlv) { // Ajouter une TLV Compund ou Dated

	int getPos;
	int diff;
	int i;
	char *msg_error_add = "TLV inconnue\n";

	writeType(f, typeTlv); // Type TLV

	getPos = (int)lseek(f, 0, SEEK_CUR); // Position du premier octet de la taille à écrire
	lseek(f, getPos+3, SEEK_SET);

	if( typeTlv == 6 ) {
		writeDate(f);
	}

	for(i = posTab; i < countTlv; i++) { // Parcours le tableau des TLV à ajouter

		switch (arrayTlv[i].type) {
		case 0:
			addPad1(f);   														                  // Pad1
			break;
		case 1:
			addPadN(f, arrayTlv[i].length);       		                  // PadN
			break;
		case 2:
			addText(f, arrayTlv[i].arg);        			                  // Texte
			break;
		case 3 ... 4:
			addPicture(f, arrayTlv[i].arg);      			                  // Image PNG et JPEG
			break;
		case 5 ... 6:
			addBloc(f, arrayTlv[i].type, arrayTlv, posTab, countTlv);   // Compound et Dated
			break;
		default:
			write(STDIN_FILENO, msg_error_add, strlen(msg_error_add));	// Exception
			break;
		}

	}

	diff = (int)lseek(f, 0, SEEK_CUR) - (getPos + 3); /* Taille du contenu du bloc qui
																									   * est égale à la position courante
                                                     * moins le position courante avant
                                                     * l'écriture des données moins 3 
                                                     * pour la taille de l'entête TLV du bloc */
	lseek(f, getPos, SEEK_SET);

	writeLength(f, diff); // Taille TLV
	
	lseek(f, 0, SEEK_END);

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

	//addText(fd, 2, testTexte);
	//addPicture(fd,4,"./flower.jpeg");
	//addText(fd, 2, testTexte);
	//addPicture(fd,4,"./flower.jpeg");
	//addPadN(fd,1,30);
	//addPad1(fd, 0);
	//addText(fd, 2, testTexte);

	int countTlv  = 1; // Nombre de TLV à ajouter
	struct tlv *tab = malloc (countTlv * sizeof (struct tlv)); // Tableau de TLV

	struct tlv tlv1 = { .type = 4, .arg = "./flower.jpeg", .length = sizeFile(tlv1.arg) };
	//struct tlv tlv2 = { .type = 2, .arg = testTexte, .length = strlen(testTexte) };
	//struct tlv tlv3 = { .type = 5, .arrayListTlv = tab, .visited = 0};
	//struct tlv tlv4 = { .type = 2, .arg = testTexte, .length = strlen(testTexte), .visited = 0 };
	tab[0] = tlv1;
	//tab[1] = tlv2;
	//tab[2] = tlv3;
	//tab[3] = tlv4;

	addBloc(fd, 6, tab, 0, countTlv);

	free(tab); // Libère la mémoire du tableau de TLV

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

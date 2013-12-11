#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/file.h>

void readTLV(int fd, char str); // Définition préalable

struct tlv{ // Structure TLV
	int type;
	int size;
	int *start;
};

void padN(int f) { // TLV PadN
  
  int fr;
	int size = 0;
  char length[3];
  char *msg_read_error = "Erreur de lecture\n";
    
  if(( fr = read(f, &length, sizeof(length))) < 0) {
    write(STDIN_FILENO, msg_read_error, strlen(msg_read_error));
  } else {
		size = length[0]*256*256 + length[1]*256 + length[2];
		lseek(f, size, SEEK_CUR); // Ignore le contenu
  }

}

void text(int f) { // TLV TEXTE

  int fr;
	int j = 0;
	int size = 0;
	unsigned char buff;
  unsigned char length[3];
  char *msg_read_error = "Erreur de lecture\n";

  if(( fr = read(f, &length, sizeof(length))) < 0) {
    write(STDIN_FILENO, msg_read_error, strlen(msg_read_error));
  } else {
		size = length[0]*256*256 + length[1]*256 + length[2];
    while((j < size) && (fr = read(f, &buff, 1)) >= 0) { 
      write(STDIN_FILENO, &buff, 1);
			j++;
    }
  }

}

void jpeg(int f) { // TLV JPEG

	int fd1;
	int fr;
	int j = 0;
	int size = 0;
	unsigned char buff;
	unsigned char length[3];
	char *msg_read_error  = "Erreur de lecture\n";
	char *msg_create_file = "Erreur creation du fichier image\n";

	if(( fr = read(f, &length, sizeof(length))) < 0) {
		write(STDIN_FILENO, msg_read_error, strlen(msg_read_error));
	} else {
		size = length[0]*256*256 + length[1]*256 + length[2];
		if(( fd1 = open("./image.jpeg", O_CREAT | O_WRONLY, S_IRUSR| S_IWUSR)) <= 0 ) {
			write(STDIN_FILENO, msg_create_file, strlen(msg_create_file));
		} 
		while((j < size) && (fr = read(f, &buff, 1)) >= 0) {
			write(fd1, &buff, 1);
			j++;
		}
		close(fd1);
	}
}

void png(int f) { // TLV PNG

	int fd1;
	int fr;
	int j = 0;
	int size = 0;
	unsigned char buff;
	unsigned char length[3];
	char *msg_read_error  = "Erreur de lecture\n";
	char *msg_create_file = "Erreur creation du fichier image\n";

	if(( fr = read(f, &length, sizeof(length))) < 0) {
		write(STDIN_FILENO, msg_read_error, strlen(msg_read_error));
	} else {
		size = length[0]*256*256 + length[1]*256 + length[2];
		if(( fd1 = open("./image.png", O_CREAT | O_WRONLY, S_IRUSR| S_IWUSR)) <= 0 ) {
			write(STDIN_FILENO, msg_create_file, strlen(msg_create_file));
		} 
		while((j < size) && (fr = read(f, &buff, 1)) >= 0) {
			write(fd1, &buff, 1);
			j++;
		}
		close(fd1);
	}
}

char* convertDate(time_t timestamp) { // Convertie timestamp en une date lisible
	
	struct tm ts;
	char buff[80];

	// Format de date "YYYY-MM-DD"
	ts = *localtime(&timestamp);
	strftime(buff, sizeof(buff), "%d-%m-%Y", &ts);

	// Alloue de la mémoire pour retourner la date
	char *date = (char *) malloc(strlen(buff) + 1);
	strncpy(date, buff, strlen(buff)+1);
	return date;

}

void dated(int f) { // TLV DATED

	int fr;
	int j = 0;
	int size = 0;
	int time = 0;
	char *dateConverted;
	char buffDate[80];
	unsigned char buff;
	unsigned char length[3];
	unsigned char date[4];
	char *msg_read_error  = "Erreur de lecture\n";

	if(( fr = read(f, &length, sizeof(length))) < 0) {
		write(STDIN_FILENO, msg_read_error, strlen(msg_read_error));
	} else {
		size = length[0]*256*256 + length[1]*256 + length[2];
		if(( fr = read(f, &date, sizeof(date))) < 0) {
			write(STDIN_FILENO, msg_read_error, strlen(msg_read_error));
		} else {
			time = date[0]*256*256*256 + date[1]*256*256 + date[2]*256 + date[3];
			dateConverted = convertDate(time); // Récupère date
			strcpy(buffDate,dateConverted); // Copie la date
			free(dateConverted); // Libère la mémoire
			write(STDIN_FILENO, &buffDate, strlen(buffDate));
		}
		int sizeParcourue = 0;
		while((j < size) && (fr = read(f, &buff, 1)) >= 0) {
			write(STDIN_FILENO, "\n\t*", 3);
			if(( fr = read(f, &length, sizeof(length))) >= 0) {
				sizeParcourue = length[0]*256*256 + length[1]*256 + length[2];
			}
			lseek(f, -sizeof(length), SEEK_CUR);
			readTLV(f, buff);
			j = j + sizeParcourue + 8;
		}
	}

}

void compound(int f) { // TLV COMPOUND
	
	int fr;
	int j = 0;
	int size = 0;
	int sizeParcourue = 0;
	unsigned char buff;
	unsigned char length[3];
	char *msg_read_error  = "Erreur de lecture\n";

	if(( fr = read(f, &length, sizeof(length))) < 0) {
		write(STDIN_FILENO, msg_read_error, strlen(msg_read_error));
	} else {
		size = length[0]*256*256 + length[1]*256 + length[2];
		while((j < size) && (fr = read(f, &buff, 1)) >= 0) {
			write(STDIN_FILENO, "\n\t*", 3);
			if(( fr = read(f, &length, sizeof(length))) >= 0) {
				sizeParcourue = length[0]*256*256 + length[1]*256 + length[2];
			}
			lseek(f, -sizeof(length), SEEK_CUR);
			readTLV(f, buff);
			j = j + sizeParcourue + 8;
		}
	}

}

void readDazibao(char *argv) { // Lire le dazibao
  
  int fd;
	struct stat fileStat;
  char *msg_error = "Erreur du fichier dzb\n";
  char *msg_open  = "Ouverture du fichier dzb\n";
  char *msg_close = "Fermeture du fichier dzb\n";

	int fs;
  int fr;
  int i = 0;
  char str;
	char *msg_error_size  = "Erreur taille fichier\n";
  char *msg_read_error  = "Erreur de lecture\n";
  char *msg_value_error = "Fichier non supporte\n";

	char *msg_error_lock   = "Erreur lock\n";
	char *msg_lock         = "Verrou acquis\n";
	char *msg_error_unlock = "Erreur unlock\n";
	char *msg_unlock       = "Verrou supprime\n";
  
  // Ouverture du fichier

  if(( fd = open("./exemple.dzb", O_RDONLY)) < 0 ) {
    write(STDIN_FILENO, msg_error, strlen(msg_error));
    exit(EXIT_FAILURE);
  } else {
    write(STDIN_FILENO, msg_open, strlen(msg_open));
  }

	// Verrou sur le fichier

	if(flock(fd, LOCK_SH) < 0) {
		write(STDIN_FILENO, msg_error_lock, strlen(msg_error_lock));
	} else {
		write(STDIN_FILENO, msg_lock, strlen(msg_lock));
	}

	// Taille du fichier passé en paramètre	

	if(( fs = stat(argv, &fileStat)) != 0 ) {
		write(STDIN_FILENO, msg_error_size, strlen(msg_error_size));
		exit(EXIT_FAILURE);
	}

  // Verification de l'entête

  while(i < 4) {
    if(( fr = read(fd, &str, 1)) < 0 ) {
      write(STDIN_FILENO, msg_read_error, strlen(msg_read_error));
      exit(EXIT_FAILURE);
    } else if( i == 0 && str != 53 ) {
				write(STDIN_FILENO, msg_value_error, strlen(msg_value_error));
				exit(EXIT_FAILURE);
    } else if( i == 1 && str != 0 ) {
				write(STDIN_FILENO, msg_value_error, strlen(msg_value_error));
				exit(EXIT_FAILURE);
    }
		i++;
  }

	// Lecture des TLV

	while((( fr = read(fd, &str, 1)) >= 0 ) && ( lseek(fd, 0, SEEK_CUR) != fileStat.st_size) ) {
		readTLV(fd, str);
	}

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

void readTLV(int fd, char str) { // Lecture des TLV
	
  switch(str) {
	case 0:
    write(STDIN_FILENO, "Pad1: \n", 7);
    break;
  case 1:
    write(STDIN_FILENO, "PADN: ", 6);
    padN(fd);
  	write(STDIN_FILENO, "\n", 1);
    break;
  case 2:
    write(STDIN_FILENO, "TEXT: ", 6);
    text(fd);
	  write(STDIN_FILENO, "\n", 1);
    break;
  case 3:
    write(STDIN_FILENO, "PNG: ", 5);
    png(fd);
	  write(STDIN_FILENO, "\n", 1);
    break;
  case 4:
    write(STDIN_FILENO, "JPEG: ", 6);
    jpeg(fd);
	  write(STDIN_FILENO, "Image cree\n", 11);
    break;
  case 5:
    write(STDIN_FILENO, "COMPOUND: ", 10);
		compound(fd);
	  write(STDIN_FILENO, "\n", 1);
    break;
  case 6:
    write(STDIN_FILENO, "Dated: ", 7);
    dated(fd);
		//write(STDIN_FILENO, "\n", 1);
    break;
  default:
    write(STDIN_FILENO, "Default \n", 9);
		padN(fd); // Ignore le TLV inconnu comme PADN sauf que la fonction connait le TLV
    break;
  }

}

int main(int argc, char *argv[]) {

	char *msg_error_arg = "Erreur argument";

	if( argc != 2 ) {
		write(STDIN_FILENO, msg_error_arg, strlen(msg_error_arg));
		exit(EXIT_FAILURE);
	}

  readDazibao(argv[1]);

  return(0);

}


                       DOCUMENTATION DE L'IMPLANTATION




            . Les structures
            . Les fonctions d'accès aux données
            . exemple.dzb en mémoire






LES STRUCTURES
==============

Dazibao_TLV_Type
----------------
Le type d'un TLV, dans une énumération, comme ça c'est pratique.

typedef enum {
    UNKNOW= -1,
    PADONE= 0,
    PADN= 1,
    TEXT= 2,
    PNG= 3,
    JPEG= 4,
    COMPOUND= 5,
    DATED= 6
} Dazibao_TLV_Type;

Dazibao_TLV
-----------
Un TLV : c'est des données stockés de la position n1 à n2 dans un fichier, et ça contient des données.

Dazibao_TLV_Type type : un type, comme juste en haut, peut etre utiliser comme un int.
off_t position : une position dans le fichier.
int length : une taille/longueur en octets.
void *value : un pointeur vers sa valeur, void parce que ça valeur peut etre des données brutes (text,png,...) ou bien une autre structure.

[!] Pour les type de tlv dont les valeurs se compose de plusieurs infos on utilise des structures adaptés.

Dazibao_TLV_Dated_Value
-----------------------
La valeur d'un TLV Dated.

long timestamp : le timestamp !
Dazibao_TLV *element : un pointeur vers le tlv attaché à ce Dated.

Dazibao_TLV_Compound_Value
--------------------------
La valeur d'un TLV Compound.

int count : le nombre d'éléments qui le compose
Dazibao_TLV **elements : un tableau de pointeurs vers les elements contenues.

Dazibao
-------
Un dazibao est lié à un fichier et contient un array de pointeur vers ses tlvs.

char *file_path : chemin vers le fichier.
int file_descriptor : pointeur vers le fichier.
int file_size : taille du fichier.
int tlv_count : nombre de tlv (à sa racine).
Dazibao_TLV **elements : ses tlvs.

Et aussi...
-----------

Les structures suivantes sont toutes définies comme type. (#typedef)






FONCTIONS D'ACCÈS AUX DONNÉES
=============================

Presque toutes les fonctions on _Dazibao *dazibao_ en entrée, c'est pour pouvoir récuperer le fd du fichier.

find_*
------

Les fonctions find* servent à trouver les tlv dans le fichier et crée de nouvelles structures pour les stocké.
Remplissent aussi les champs type, offset, length.

find_next_tlv(Dazibao *dazibao, int offset);
La position dans le fichier où la tlv commence.

find_next_tlv_array
offset_start : position de départ dans le fichier pour la recherche
offset_max : position max dans le fichier
int *tlv_count : nombre de tlv trouvé
retourne un tableau de pointeur vers ce qui a été trouvé.

load_*
------

pour charger la valeur du tlv en memoire on utilise les fonctions load,
les fonctions prennent en parametre le tlv à charger, allou la mémoire et link au tlv->value

quand c'est txt, ou image :
load_tlv_value_raw(Dazibao *dazibao, Dazibao_TLV *tlv)

dated :
load_tlv_value_dated(Dazibao *dazibao, Dazibao_TLV *tlv)
la fonction allou et rempli le Dazibao_TLV_Dated_Value
et find_next_tlv_array() le prochain tlv

compound:
load_tlv_value_compound(Dazibao *dazibao, Dazibao_TLV *tlv)
allou de la place et find_next_tlv_array() ses elements + linkage






EXEMPLE.DZB EN MÉMOIRE
======================

Voici comment est stocké en mémoire le fichier exemple.dzb après que le programme ait parcourue l'arbre.

[ Dazibao
| file_path = "exemple.dzb"
| file size = 146270
| int tlv count = 4
| elements ->
    [0] -> [ Dazibao_TLV
            | type = 2 (TEXT)
            | position = 4
            | lenght = 265
            | *value = "All parts..."
            
    [1] -> [ Dazibao_TLV
            | type = 4 (JPEG)
            | position = 273
            | lenght = 56993
            | *value = binary image
            
    [2] -> [ Dazibao_TLV
            | type = 6 (DATED)
            | position = 57270
            | lenght = 36998
            | *value -> [ Dazibao_Dated_Value
                        | timestamp = 1383709705
                        | element -> [ Dazibao_TLV
                                     | type = 4 (JPEG)
                                     | position = 57278
                                     | lenght = 36990
                                     | *value = binary image

    [3] -> [ Dazibao_TLV
            | type = 6 (DATED)
            | position = 94272
            | lenght = 51994
            | *value -> [ Dazibao_Dated_Value
                        | timestamp =
                        | element -> [ Dazibao_TLV
                                     | type = 5 (COMPOUND)
                                     | position = 94280
                                     | lenght =
                                     | *value -> [ Dazbiao_TLV_Compound_Value
                                                 | count = 2
                                                 | **elements ->
                                                     [0] -> [ Dazibao_TLV
                                                             | type = 2 (TEXT)
                                                             | position = 94284
                                                             | lenght = 31
                                                             | *value = "..."
                                                             
                                                     [1] -> [ Dazibao_TLV
                                                             | type = 4 (JPEG)
                                                             | position = 94319
                                                             | lenght = 51947
                                                             | *value = binary_image

--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

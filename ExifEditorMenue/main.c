//
//  main.c
//  EXIF_start_menü
//
//  Created by Gabor Seliger on 15.05.22.
//


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libexif/exif-data.h>


int start_menue(void);
int paramter_menue(int counter);
int funktion_menue(void);
void clearScreen(void);

static void trim_spaces(char* buf)
#
{
    char* s = buf - 1;
    for (; *buf; ++buf) {
        if (*buf != ' ')
            s = buf;
    }
    *++s = 0; /* nul terminate the string on the first of the final spaces */
}

/* Show the tag name and contents if the tag exists */
const char* show_tag(ExifData* d, ExifIfd ifd, ExifTag tag)
{
    /* See if this tag exists */
    ExifEntry* entry = exif_content_get_entry(d->ifd[ifd], tag);
    if (entry) {
        char buf[1024];

        /* Get the contents of the tag in human-readable form */
        exif_entry_get_value(entry, buf, sizeof(buf));

        /* Don't bother printing it if it's entirely blank */
        //trim_spaces(buf);
        if (*buf)
        {
            return(buf);
        }
    }

}

/* Show the given MakerNote tag if it exists */
static void show_mnote_tag(ExifData* d, unsigned tag)
{
    ExifMnoteData* mn = exif_data_get_mnote_data(d);
    if (mn) {
        int num = exif_mnote_data_count(mn);
        int i;

        /* Loop through all MakerNote tags, searching for the desired one */
        for (i = 0; i < num; ++i) {
            char buf[1024];
            if (exif_mnote_data_get_id(mn, i) == tag) {
                if (exif_mnote_data_get_value(mn, i, buf, sizeof(buf))) {
                    /* Don't bother printing it if it's entirely blank */
                    trim_spaces(buf);
                    if (*buf) {
                        printf("%s: %s\n", exif_mnote_data_get_title(mn, i),
                            buf);
                    }
                }
            }
        }
    }
}

int main(int argc, const char* argv[]) {

    int aktion = 0;
    int auswahl_paramter = 0;
    int auswahl_funktion = 0;
    int counter = 0;

    ExifData* ed;
    ExifEntry* entry;

    /* Load an ExifData object from an EXIF file */
    ed = exif_data_new_from_file(argv[1]);
    if (!ed) {
        printf("File not readable or no EXIF data in file %s\n", argv[1]);
        return 2;
    }

    do {
        aktion = start_menue();
        /* Ausgeben unsere Relevanten Exif Tags*/
        printf("\n");
        printf("Die Exif Daten des Bildes:\n\n");
        printf("[1]Aufnahmedatum & Uhrzeit:    %s \n", show_tag(ed, EXIF_IFD_EXIF, EXIF_TAG_DATE_TIME_ORIGINAL));
        printf("[2]Kamerahersteller:           %s \n", show_tag(ed, EXIF_IFD_0, EXIF_TAG_MAKE));
        printf("[3]Kameramodell:               %s \n", show_tag(ed, EXIF_IFD_0, EXIF_TAG_MODEL));
        printf("[4]GPS Breitengrad:            %s ", show_tag(ed, EXIF_IFD_GPS, EXIF_TAG_GPS_LATITUDE));
        printf("%s\n", show_tag(ed, EXIF_IFD_GPS, EXIF_TAG_GPS_LATITUDE_REF));
        printf("[5]GPS Laengengrad:            %s ", show_tag(ed, EXIF_IFD_GPS, EXIF_TAG_GPS_LONGITUDE));
        printf("%s\n", show_tag(ed, EXIF_IFD_GPS, EXIF_TAG_GPS_LONGITUDE_REF));

        printf("\n");
        //show_tag(ed, EXIF_IFD_EXIF, 0x9003);    //Zeigt das Aufnahmedatum an
        //show_tag(ed, EXIF_IFD_0, EXIF_TAG_MAKE);        //Zeigt den Kameraherrsteller an
        //show_tag(ed, EXIF_IFD_0, EXIF_TAG_MODEL);        //Zeigt das Kameramodell an
        //show_tag(ed, EXIF_IFD_GPS, EXIF_TAG_GPS_LATITUDE); //Zeigt GPS Koordinaten den Breitengrad in GRAD MINUTE SEKUNDE an
        //show_tag(ed, EXIF_IFD_GPS, EXIF_TAG_GPS_LATITUDE_REF); //Bezug des Breitengrads (Westen oder Osten)
        //show_tag(ed, EXIF_IFD_GPS, EXIF_TAG_GPS_LONGITUDE); //Zeigt GPS Koordinaten den Längengrad in GRAD MINUTE SEKUNDE an
        //show_tag(ed, EXIF_IFD_GPS, EXIF_TAG_GPS_LONGITUDE_REF); //Bezug des Längengrads (Norden oder Süden

        exif_data_unref(ed);
        setbuf(stdout, NULL);

        do {

            auswahl_paramter = paramter_menue(counter);

            if (auswahl_paramter == 0) { //0 = Abbrechen
                break;
            }
            if (auswahl_paramter == 8) { // 8 = Speichern
                //Speicherfunktion einfügen (teilweise schon fertig)
                printf("\nKopie wird gespeichert.\n");
                break;
            }

            auswahl_funktion = funktion_menue();


            switch (auswahl_funktion) {
            case 0:
                continue; //0 = Abbrechen

            case 1: //1 = Löschen
                break;//Löschen funktion hinzufügen

            case 2://2 = Hinzufügen
                break;//Hinzufügen funktion hinzufügen

            case 3://3 = Ändern
                break;//Ändernfunktion hinzufügen

                //9 = Programm beenden ist bereits in der menüfunktion enthalten
            }


            counter++; // wird benötigt um ein Speichen ohen änderung zu verhinden

        } while (1);

    } while (1);

    return (0);
}


//***************************************************************************************************
//*******************************+           FUNKTIONEN         *************************************
//***************************************************************************************************

//Menü das beim starten erscheint

int start_menue(void) {
    int aktion;
    int str_len = 0;
    char c_a[2];
    char dummy_zeichen = ' ';

    printf("\n* * * Willkommen im EXIF Editor * * *\n\nBitte waehlen Sie was Sie tun moechten:\n\n");
    printf("Beenden[0]     Starten[1]\n\n");

    do {
        printf("Eingabe:  ");
        scanf("%[^\r\n]", &c_a);         // es werden keine Leerzeichen oder anbätze eingelesen
        scanf("%c", &dummy_zeichen);     // keine Ahnung siehe 2. Semester
        if (dummy_zeichen == '\r\n') {    // keine Ahnung siehe 2. Semester
            scanf("%c", &dummy_zeichen); // keine Ahnung siehe 2. Semester
        }

        str_len = (int)strlen(c_a);    //länge der eingabe bestimmen

        //Wenn mehr als eie zeichen ein gegebne wird z.b. 11 geht es nicht

    } while (!(str_len == 1 && (c_a[0] == '1' || c_a[0] == '0')));

    if (c_a[0] == '0') {
        printf("\nProgram Beenden...\n");
        exit(1);
    }

    aktion = (int)c_a[0];  //Umandlung von chat in int
    aktion = aktion - 48;   //ASCII umrechnung zu zahlen von 0 bis 9

    return(aktion);
}

// Menü um die Daten auszuwählen

int paramter_menue(int counter) {


    int aktion = 99;
    int str_len = 0;
    char c_a[10];
    char dummy_zeichen = ' ';


    printf("\nBitte waehlen Sie den Parameter der bearbeitet werden soll: \n\n");
    printf("Abbrechen[0] \nDatum und Uhrzeit[1] \nKameraherrsteller[2] \nKameramodell[3] \nGPS Breitengrad Ref: Norden[4] \nGPS Laengengrad Ref: Osten[5] \nSpeichern[8] \nProgramm beenden[9]\n\n");


    do {
        printf("Eingabe:  ");
        scanf("%[^\r\n]", &c_a);
        scanf("%c", &dummy_zeichen);
        if (dummy_zeichen == '\r\n') {
            scanf("%c", &dummy_zeichen);
        }

        str_len = (int)strlen(c_a);


        if (counter == '0') {
            if (aktion == '8') {
                printf("\nOhne aenderung kann keine Kopie gespeichert werden.\n\n");
            }
        }

    } while (!(str_len == 1 && (c_a[0] == '1' || c_a[0] == '2' || c_a[0] == '3' || c_a[0] == '4' || c_a[0] == '0' || (c_a[0] == '8' && counter != '0') || c_a[0] == '9')));

    if (c_a[0] == '9') {
        printf("\nProgramm wird beendet.\n");
        exit(9);
    }

    aktion = (int)c_a[0];
    aktion = aktion - 48;

    return (aktion);
}

// Menü um die funktionen auszuwälen

int funktion_menue(void) {

    int aktion;
    int str_len = 0;
    char c_a[2];
    char dummy_zeichen = ' ';

    printf("\nWaehlen Sie die Aktion die Sie benutzen wollen:\n\n");
    printf(" Loeschen[1] Aendern[2] Abbrechen[8] Programm beenden[9]\n\n");

    do {
        printf("Eingabe:  ");
        scanf("%[^\r\n]", &c_a);
        scanf("%c", &dummy_zeichen);
        if (dummy_zeichen == '\r\n') {
            scanf("%c", &dummy_zeichen);
        }

        str_len = (int)strlen(c_a);

    } while (!(str_len == 1 && (c_a[0] == '1' || c_a[0] == '2' ||  c_a[0] == '9' || c_a[0] == '8')));

    if (c_a[0] == '9') {
        printf("\nProgramm wird beendet.\n");
        exit(9);
    }

    aktion = (int)c_a[0];
    aktion = aktion - 48;
    printf("%d", aktion);

    return (aktion);
}

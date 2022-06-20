#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <libexif/exif-data.h>
#include <stdlib.h>
#include <io.h>

//In der Funktion werden die Exif Daten eines Bildes eingelesen und unter der Funkion ShowTag gleichzeitig auch ausgegeben. 
//Um diesen Code lauffähig zu bekommen muss noch die LibExif bibliothek eingebunden werden. Am besten mit einem Package Manager machen wie vcpkg. Dann libexif dort suchen. Die Datei muss im selben Verzeichnis wie die Bibliothek seiin
//Momentan muss man noch die Eingabeparameter mit der Main funktion eingeben, also z.B mit der Powershell. 

/* Remove spaces on the right of the string */
static void trim_spaces(char* buf)
{
    char* s = buf - 1;
    for (; *buf; ++buf) {
        if (*buf != ' ')
            s = buf;
    }
    *++s = 0; /* nul terminate the string on the first of the final spaces */
}

/* Show the tag name and contents if the tag exists */
static ExifTag show_tag(ExifData* d, ExifIfd ifd, ExifTag tag)
{
    /* See if this tag exists */
    ExifEntry* entry = exif_content_get_entry(d->ifd[ifd], tag);
    if (entry) {
        char buf[1024];

        /* Get the contents of the tag in human-readable form */
        exif_entry_get_value(entry, buf, sizeof(buf));

        /* Don't bother printing it if it's entirely blank */
        trim_spaces(buf);
        if (*buf) {
            printf("%s: %s\n", exif_tag_get_name_in_ifd(tag, ifd), buf);
        }
    }
    return 1;
}
jpeg_data_set_exif_data(ExifData* d, ExifIfd ifd, ExifTag tag);




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

int main(int argc, char* argv[])
{
    ExifData* ed;
    ExifEntry* entry;

    setbuf(stdout, NULL);
    char filepath[1024] = { 0 };

    // If no filepath was given as an argument
    if (argc == 1)
    {
            printf("\nBitte noch als Argument das Verzeichnis des Bildes anhaengen.\n\n");
            /*fgets(filepath, 1024 - 1, stdin);
            printf("Pfad: %s\n\n", filepath);*/
            return 3;
        
    }
    // If a filepath was given as an argument
    else if (argc == 2) {
        strcpy(filepath, argv[1]);
    }
    // If too much arguments were given
    else {
        printf("Ungueltige Anzahl von Argumenten uebergeben.\n");
        // return with error
        return 1;
    }

    /* Load an ExifData object from an EXIF file */
    ed = exif_data_new_from_file(argv[1]);
    if (!ed) {
        printf("Datei nicht lesbar oder keine EXIF Daten in der Datei enthalten %s\n", argv[1]);
        return 2;
    }

    /* Ausgeben unsere Relevanten Exif Tags*/

    show_tag(ed, EXIF_IFD_0, EXIF_TAG_DATE_TIME_ORIGINAL);    //Zeigt das Aufnahmedatum an
    show_tag(ed, EXIF_IFD_0, EXIF_TAG_MAKE);        //Zeigt den Kameraherrsteller an
    show_tag(ed, EXIF_IFD_0, EXIF_TAG_MODEL);        //Zeigt das Kameramodell an
    show_tag(ed, EXIF_IFD_GPS, EXIF_TAG_GPS_LATITUDE); //Zeigt GPS Koordinaten den Breitengrad in GRAD MINUTE SEKUNDE an
    show_tag(ed, EXIF_IFD_GPS, EXIF_TAG_GPS_LATITUDE_REF); //Bezug des Breitengrads (Westen oder Osten)
    show_tag(ed, EXIF_IFD_GPS, EXIF_TAG_GPS_LONGITUDE); //Zeigt GPS Koordinaten den Längengrad in GRAD MINUTE SEKUNDE an
    show_tag(ed, EXIF_IFD_GPS, EXIF_TAG_GPS_LONGITUDE_REF); //Bezug des Längengrads (Norden oder Süden





    /* A couple of MakerNote tags can contain useful data.  Read the
     * manufacturer tag to see if this image could have one of the recognized
     * MakerNote tags.
     */
    entry = exif_content_get_entry(ed->ifd[EXIF_IFD_0], EXIF_TAG_MAKE);
    if (entry) {
        char buf[64];

        /* Get the contents of the manufacturer tag as a string */
        if (exif_entry_get_value(entry, buf, sizeof(buf))) {
            trim_spaces(buf);

            if (!strcmp(buf, "Canon")) {
                show_mnote_tag(ed, 9); /* MNOTE_CANON_TAG_OWNER */

            }
            else if (!strcmp(buf, "Asahi Optical Co.,Ltd.") ||
                !strcmp(buf, "PENTAX Corporation")) {
                show_mnote_tag(ed, 0x23); /* MNOTE_PENTAX2_TAG_HOMETOWN_CITY */
            }
        }
    }

    /* Free the EXIF data */
    exif_data_unref(ed);
    printf("\nEs ging bis ganz nach unten\n");
    return 0;
}
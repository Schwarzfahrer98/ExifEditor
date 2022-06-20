#include <stdio.h>
#include <string.h>
#include <libexif/exif-data.h>
#define _CRT_SECURE_NO_WARNINGS
//In der Funktion werden die Exif Daten eines Bildes eingelesen und unter der Funkion ShowTag gleichzeitig auch ausgegeben. 
//Um diesen Code lauffähig zu bekommen muss noch die LibExif bibliothek eingebunden werden. Am besten mit einem Package Manager machen wie vcpkg. Dann libexif dort suchen. Die Datei muss im selben Verzeichnis wie die Bibliothek seiin

/* Remove spaces on the right of the string */
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
const char* show_tag (ExifData* d, ExifIfd ifd, ExifTag tag)
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

int main(int argc, char** argv)
{
    ExifData* ed;
    ExifEntry* entry;

    /* Load an ExifData object from an EXIF file */
    ed = exif_data_new_from_file(argv[1]);
    if (!ed) {
        printf("File not readable or no EXIF data in file %s\n", argv[1]);
        return 2;
    }

    /* Ausgeben unsere Relevanten Exif Tags*/
    printf("\n");
    printf("Aufnahmedatum & Uhrzeit:    %s \n", show_tag(ed, EXIF_IFD_EXIF, EXIF_TAG_DATE_TIME_ORIGINAL));
    printf("Kamerahersteller:           %s \n", show_tag(ed, EXIF_IFD_0, EXIF_TAG_MAKE));
    printf("Kameramodell:               %s \n", show_tag(ed, EXIF_IFD_0, EXIF_TAG_MODEL));
    printf("GPS Breitengrad:            %s ",show_tag(ed, EXIF_IFD_GPS, EXIF_TAG_GPS_LATITUDE));
    printf("%s\n", show_tag(ed, EXIF_IFD_GPS, EXIF_TAG_GPS_LATITUDE_REF));
    printf("GPS Laengengrad:            %s ", show_tag(ed, EXIF_IFD_GPS, EXIF_TAG_GPS_LONGITUDE));
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
    printf("\nWas moechten Sie aendern?\n");
    return 0;
}
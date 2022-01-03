#define VERSION "0.05"

/*----------------------

Version 0.01

14.01.1999	Erste Version

Version 0.02

05.02.1999	énderungsdatum wird vierstellig ausgegeben. [Dietmar Belloff]
28.01.1999	dBASE-Datei wurde fÑlschlicherweise im Textmodus geîffnet.
						Dadurch konnten Zeichen verloren gehen. [Dietmar Belloff]
						Datum wurde nicht korrekt konvertiert. [Dietmar Belloff]
25.01.1999	Buttons integriert um alle Datenfelder auszuwÑhlen.
20.01.1999	Fehler in SysGem bei FileSelect behoben. Es wurde unter TOS
						versucht die MagiC-Dateiauswahl aufzurufen.
						Der Feldtyp logisch wird als String mit der LÑnge von 1
						Åbertragen.
15.01.1999	KEYTAB eingebaut.

Version 0.03

04.03.1999	Anleitung soweit fertig.

25.02.1999	Erste Anleitung mit UDO geschrieben.
						Beim betÑtigen der HELP-Taste wird ST-Guide aufgefrufen.

20.02.1999	Auswertung der dBASE Version verbessert. Erkennt nun auch
						dBASE V fÅr Windows.

Version 0.04

06.04.1999	Das Anlegen einer Datei funktioniert nicht korrekt bei einem 
						Datesystem mit langen	Dateinamen. [Hilmar Bîhm]
07.04.1999	Ausgabe der Tabellenstruktur der Phoenixdatenbank in eine
						Datei. [Hilmar Bîhm]
08.09.1999	Ausgabe der dBASE Tabellestruktur in eine Datei.
						[Hilmar Bîhm]
09.04.1999	Maximale Anzahl der dBASE Felder auf 100 erhîht.
12.04.1999	Keytab besitzt nun einen eigenen Dilaog, indem man den
						Importfilter einstellen kann.
						Die Dialoge der Strukturen wurden nicht geschlossen, wenn
						ein MenÅpunkt"..Schliessen" betÑtigt wurde.
						Fehler in SysGem behoben, jetzt zeigt die Dateiauswahl den
						korrekten Pfad an.
						dB2Ph speichert den Importfilter in seiner Konfig ab.
21.05.1999	Im Dialog Phoenixstrukture MenÅzeile fÅr die Textausgabe
						erzeugt.
22.05.1999	Durch eine Erweiterung von SysGem kann nun auch mit Clr/Home und
            Shift Clr/Home gescrollt werden.

Version 0.05

06.06.1999	Durch neu compalieren kann jetzt das Fenster der dBASE-
						Daten auch mit Clr/Home bewegt werden.
						Beim erzeugen einer neuen Phoenixtabelle wird ShowStatus
						angezeigt.

Version 0.05

05.02.2000	War Keytab nicht installiert stÅrzte db2Ph ab (2 Bomben).
						[Bernd MÑdicke]
						Bei der Ausgabe der Phoenixdatenfelder wird beim Typ 'Text'
						berÅcksichtig, daû der Designer immer ein Byte mehr
						resverviert. Somit wird bei der Anzeige eins abgezogen.

----------------*/

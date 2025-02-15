# Project-bjump

Das Spiel der Gruppe Project BJump 

Erstellt von 

(owos02)[https://github.com/owos02]

(majue001)[https://github.com/majue001]

(BlackSoulKnight)[https://github.com/BlackSoulKnight]

## Kurzinfo

Bullet Hell, Jump King, Celeste mashup.

# Spiel

## Controlls:

| Keys    | Controlls|
| -------- | ------- |
| <kbd>A</kbd> <kbd>D</kbd> <kbd>&#8592;</kbd> <kbd>&#8594;</kbd>  | Bewegen    |
| <kbd>Space</kbd> <kbd>W</kbd> <kbd>&#8593;</kbd>| Springen |
| <kbd>L-Shift</kbd> <kbd>R-Shift</kbd> | Dashen   |
| <kbd>L-Ctrl</kbd> | Langsam Laufen |

## Levels

Zurzeit gibt es 2 fertige Level.

Standardmäßig ist das schwerere Level ausgewählt.

Um dies zu ändern muss im Game-Code, der Main funktion `53: newWorld->loadmap(1);` auf 1 oder 2 gesetzt werden.

# Level Editor

Der Level Editor kann gestartet werden indem die Projektmappe `bjump-map-editor/bjump-map-editor.sln` gestartet wird.

## Level Editor Requirements:

Der Level Editor kann nur von Windows aus gestartet werden, da die Windows API direkt benutzt wird zum öffnen und speichern von Maps.

Verwendung von WINE oder ähnlicher Software zur Ausführung auf anderen Platformen wurde nicht auf funktionalität überprüft.

## Steuerung des Level Editors

Die Steuerung wird als Windows Message beim Start des Level Editor gezeigt.

Aus Komplettheit hier nochmal die Steuerung:

<kbd>F1</kbd> Tastenbelegung zeigen

<kbd>LClick</kbd> Tile platzieren

<kbd>RClick + Drag</kbd> Kamera bewegen

<kbd>S</kbd> Speichern

<kbd>G</kbd> Toggle grid mode

<kbd>X</kbd> Toggle delete mode

Ebene wechseln 

- <kbd>1</kbd> Vordergrund
- <kbd>2</kbd> Hintergrund
- <kbd>3</kbd> Zeige alle Ebenen
- <kbd>4</kbd> Gegner Ebene

<kbd>&#8592;</kbd> <kbd>&#8594;</kbd> Wechsel Gegner Typ

<kbd>M</kbd> Kamera auf Ursprung zurücksetzen

<kbd>H</kbd> Horizontal Lock

<kbd>V</kbd> Vertikal Lock

<kbd>Maus Rad</kbd> Zoom

<kbd>&#8593;</kbd> <kbd>&#8595;</kbd> Tileset kleiner/größer

## UI des Level Editor

Links Oben: Tileset

Rechts Oben: Ebenen makierung & Ausgewähler Gegnertyp

	- FG: Vordergrund
	- BG: Hintergrund
	- ALL: Zeige ohne Alpha
	- PE: Place Enemy Mode

# Credits

Tileset

	- License: (Creative Commons CC0)[https://creativecommons.org/publicdomain/zero/1.0/]
	- Source: (Kenney.nl)[https://kenney.nl/assets/1-bit-platformer-pack]
	- Size: 16x16
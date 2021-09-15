# TIN_testbed

## Jak to skonfigurować?

Projekt jest ustawiony w taki sposób że powinien działać    \
"z biegu", wystarczy skopiować folder wxWidgets do folderu  \
w do którego jest sklonowane repozytorium.

Przykład: \
Sklonowano repo do folderu

```batch
D:\mujFolderek\he\he
```

Czyli znajduje się w lokalizacji:

```batch
D:\mujFolderek\he\he\TIN_testbed
```

Wtedy folder ```wxWidgets``` powinien być w:

```batch
D:\mujFolderek\he\he
```

Folder można pobrać tu -> [wxWidgets](https://drive.google.com/file/d/19QC6fGoTbvylOnT7RSkwvBJ8GiaywVZF/view?usp=sharing)

Jeżeli nie chcesz pobierać tych ~160MB to można zawsze pobrać źródła \
wxWidgets i skompilować samodzielnie (trwa to zależnie od \
komputera ~10 - 20 min), następnie podać w VS2019 ścieżkę do katalogu  ```vc_lib``` (dołączenie statycznej biblioteki)
```include``` oraz ```include\msvc```

### WAŻNA SPRAWA

Żeby projekt się skompilował w VS2019 target musi być ustawiony na ```x86```
Jest tak dlatego że statyczne biblioteki wxWidgets są turbo ciężkie (wszystko razem waży ok. 2-3GB) dlatego
zostawiłem tylko 1 konfiguracjię :)

## Konfiguracja projektów na uC

W tym przypadku nie korzystamy z żadnych dodatkowych bibliotek, otwarcie plików projektu odpowiednim programem
(CubeIDE lub MicrochipStudio) powinno wystarczyć w kwestii konfiguracji

## Działanie projektu

Opisy działania poszczególnych elementów projeku znajdują sie w folderze `Schematics` w pliku `JakToDziala.md`

## Wygląd GUI

![Obrazek](https://github.com/Commentsareuseless/TIN_testbed/blob/main/Schematics/GUI.PNG)

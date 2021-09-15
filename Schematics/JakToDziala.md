# JakToDziała.jpg

## Definicje żeby się nie zgubić

STM - najprawdopodobniej nucleo64 (może być inne ale trzeba to skonfigurować w projekcie CubeIDE)

AVR - Moduł Xplained

## Apka na PC

Aby uruchomić test należy:
* podłączyć zarówno STM jak i AVR do komputera przez USB
* umieścić oba moduły w przygotowanej płytce
* wybrać porty COM odpowiadające poszczególnym urządzeniom
* Zaznaczyć interesujący nas test case
* Kliknąć najpierw `Select` potem `Test`

W rezultacie powinien pojawić się log opisujący przebieg testu

### GUI

GUI jest tak intuicyjne jak to możliwe ;)
Wszystko działa na podstawie biblioteki wxWidgets, jeżeli jesteś z nią obeznany/a to jak najbardziej można modyfikować
do woli (Lepiej nie zmieniać linijek odpowiedzialnych za sizery w kostruktorze MainWIndow ;) )

### Tworzenie TC

Kolejne przypadki testowe można tworzyć w pliku `TCtable.cpp`, wystarczy napisać funkcje o sygnaturze 
`void funkcja(void)`, następnie umieścic ją w mapie wszystkich TC na końću pliku w taki sposób jak poprzednie TC

Dostępne jest "API" które ułatwia komunikacje z uC i samym GUI. Warto zauwarzyć że program jest uruchamiany bez skojażonej konsoli,
wszystkie "logi" powinny być zpisywane jako Output testu przez odpowiednie funkcje GUIManagera

## Komunikacja z uC

Komunikacja z oboma uC zachodzi przez interfejs UART (z poziomu systemu, przez port COM). Klasa SerialCom jest 
helperem w komunikacji. Wystarczy stworzyć nową jej instancję podając odpowiedni port COM, potem można już bez problemu
wywoływać funkcje Read* i Write*, bezpośrednio komunikujące się z uC.

Dostępne komendy dla AVR jak i STM są ściśle określone (można je podejżeć w programach na odpowiednie uC)
Dla STM
* `RDPX` - gdzie X może być dużą literą od A do D, wysłanie tej komendy do STM spowoduje że odeśle on łańcuch 
    znaków przedstawiający stany poszczególnych pinów na odpowiednim porcie (łańcuch składa się z '0' i '1')
* `RDAL` - To samo co wcześniej ale czytane są wszystkie porty.

Dla AVR
(Komunikacja zachodzi przez 1 bajt, dlatego wymienione zostaną poszczególne "pola bitowe")
Wszystko przedstawia obrazek `UARTmsgAVR.png`

### UWAGA

Schematy w folderze `CommunicationSch` są nieaktualne, zostają w razie gdyby w przyszłości ktoś chciał je poprawić :)

## Dalsze rozszerzenia

Program na PC jest dość uniwersalny, nie powinno być potrzeby zmieniać go w inny sposób niż dopisywanie nowych Test casów.

Programy na uC są na razie dość "statyczne", odpowiadają tylko na kilka instrukcji z PC, dlatego aby
przetestować inne aspekty badanego uC należy zacząć od przebudowy właśnie jego programu


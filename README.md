# IPK_projek1

## Pouziti

konzole:<br>
> ./ipkcpc -h [HOSTNAME] -p [PORT] -m [MODE]<br><br>
Pripoji se na server zadany HOSTNAME a port PORT a posle sadu dotazu na server s bezicim programem ipkpd a pomoci vybraneho modu pomoci protokolu IPKCP a pro zadane dotazy zakoncene kodem 'EOF' tiskne prijate odpovedi.<br>
Pri jakekoli chybe v parametrech nebo pri behu tiskne chybovou hlasku na stderr a konci kodem 1.<br>
<br>
    &emsp;HOSTNAME - musi byt validni domenove jmeno serveru s bezicim programem ipkpd v odpovidajicim modu MODE<br>
    &emsp;PORT - cislo portu, na kterem program ipkpd ma otevreny soket<br>
    &emsp;MODE - ipkcp podporuje dva mody udp a tcp<br>
<br>

Protokol IPKCP:<br>
```
    UDP
    -prijma dotazy ve forme ([+-*/] [0-9]+ [0-9]+)
    -vysledek program tiskne formou OK:[0-9]+
    -v pripade ze server nerozezna dotaz, nebo neni schopen vypocitat vysledek program vytiskne ERR:<errmsg>

    TCP
    -je potreba otevrit spojeni zpravou HELLO
    -dale nasleduje libovolny pocet zprav SOLVE formou SOLVE ([+-*/] [0-9]+ [0-9]+)
    -spojeni je treba ukoncit zpravou BYE
    -v pripade ze server nerozezna zaslanou zpravu zasila zpravu BYE a ukonci spojeni, timto konci i beh programu
```
V pripade zachyceni signalu SIGINT program zasle zpravu BYE a spojeni timto ukonci. Tyto zpravy uz netiskne.

## Zakladni popis

Program nacte argumenty zadane pres konzoli s vyuzitim funkce getopt (knihovna `<unistd.h>` a `<getopt.h>`). V pripade neznamych argumentu, nebo spatne zadanych parametru se vola exit(1) a program konci chybou.<br>

Cyklus s funkci getopt() v podmince zkontroluje, jestli index posledniho zpracovaneho argumentu neni mensi nez celkovy pocet argumentu. Tim se vyradi pripadne prebytecne nekontrolni argumenty. Pokud tomu tak neni program opet konci chybou 1.<br>

Po splneni  "vstupnich podminek" program vola knihovni funkci gethostbyname() (knihovna `<netdb.h>`), ktera vyuziva sluzby DNS(dle [dokumentace](https://man7.org/linux/man-pages/man3/gethostbyname.3.html) obcas neni sluzba vyuzita, nicmene pro predstavu rekneme, ze se vyuzije vzdy), s jejiz pomoci ziska z argumentu [HOSTNAME] IP adresu zadaneho serveru serveru. Ve chvili, kdy funkce vraci **NULL** znamena to, ze domenove jmeno neni zname a program konci chybou 1.<br>

Nasledne se nastavi komunikacni protokol na IPv4 a cislo portu. Tyto informace se ulozi do struktury **sockaddr_in**.<br>

S temito daty je program schopen vytvorit soket pro odesilani a prijmani dat. K tomuto ucelu je pouzita funkce soket() (knihovna `<sys/soket.h>`) teto funkci program predava strukturu **sockaddr_in** vyse nastavenou a promennou sock_type, ktera nese typ prenosu dat. (but **SOCK_DGRAM** nebo **SOCK_STREAM** na zaklade vstupnich parametru). Pokud tato funkce vrati zapornou hodnotu, nebo nulu soket se nepodarilo vytvorit a program konci chybou 1.<br>

Nakonec program spusti jednu z funkci `send_udp()` a `send_tcp()` pro finalni zprostredkovani komunikace.<br>

## Funkce

## Testovani

## Odkazy

### Poznamky pod carou

nebojte prijde 'fix: interpunkce'<br>
program vraci pro vsechny chyby stejnou hodnotu (1) s ohledem na vypis chybovych zprav mi prislo zbytecne definovat rozdilne navratove hodnoty

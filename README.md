# IPK_projek1

## Pouziti

#### Konzole:

```
./ipkcpc -h [HOSTNAME] -p [PORT] -m [MODE]
```

Pripoji se na server zadany HOSTNAME a port PORT a posle sadu dotazu na server s bezicim programem ipkpd a pomoci vybraneho modu pomoci protokolu IPKCP a pro zadane dotazy zakoncene kodem 'EOF' tiskne prijate odpovedi.<br>
Pri jakekoli chybe v parametrech nebo pri behu tiskne chybovou hlasku na stderr a konci kodem 1.<br>

<br>
    &emsp;HOSTNAME - musi byt validni domenove jmeno serveru s bezicim programem ipkpd v odpovidajicim modu MODE<br>
    &emsp;PORT - cislo portu, na kterem program ipkpd ma otevreny soket<br>
    &emsp;MODE - ipkcp podporuje dva mody udp a tcp<br>
<br>

#### Protokol IPKCP:
```
    UDP
    -prijma dotazy ve forme ([+-*/] [0-9]+ [0-9]+)
    -vysledek program tiskne formou OK:[0-9]+
    -v pripade ze server nerozezna dotaz, nebo neni schopen vypocitat vysledek program vytiskne ERR:<errmsg>

    TCP
    -je potreba otevrit spojeni zpravou HELLO
    -dale nasleduje libovolny pocet zprav SOLVE formou SOLVE ([+-*/] [0-9]+ [0-9]+)
    -spojeni je treba ukoncit zpravou BYE
    -v pripade ze server nerozezna zaslanou zpravu zasila zpravu BYE a ukonci spojeni, timto konci i beh programu<br>
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

##### print_help()

Vytiskne pomoc pri spatnemu zadani argumentu prikazove radky.

##### send_udp()

Zajistuje odeslani a prijem datagramu pro binarni verzi protokolu IPKCP opakovanym volanim funkci sendto() a recvfrom() (knihovna `<sys/socket.h>`.

##### handle_sigint()

Spusti se pri tcp modu a pri zachyceni signalu **SIGINT** zasle serveru zpravu BYE a konci spojeni.

##### send_tcp()

Vyrizuje textovou komunikaci se serverem vyuzitim funkci connect() pro vytvoreni spojeni, send() a recv() pro odeslani a prijeti zprav (knihovna `<sys/socket.h>`) a close() pro uzavreni spojeni (knihovna `<unistd.h>`).

## Testovani

1. Na virtualce ipk NixOs spustte server ipkpd v shellu nasledovne:

```
ipkpd -h localhost -p 2023 -m udp & ipkpd -h localhost -p 2024 -m tcp &
```

2. Pustte skript

```
./test.sh
```

3. Po ukonceni testovani je treba najit procesy serverovych stran a pomoci zaslani signalu SIGKILL (9) je ukoncit

```
kill -9 idp_ipkpd_udp
kill -9 idp_ipkpd_tcp
```

### Odkazy

Rysavy, Ondrej, Ph. D. “IPK-DemoUdp.” *FIT - VUT Brno - Git*, git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs/cpp/DemoUdp/client.c.
Rysavy, Ondrej, Ph. D. “IPK-DemoTcp.” *FIT - VUT Brno - Git*, git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs/cpp/DemoTcp/client.c.
Kadam Patel. “Signals in C Language.” *GeeksforGeeks*, 8 Feb. 2018, www.geeksforgeeks.org/signals-c-language
www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html

### Poznamky pod carou

nebojte prijde 'fix: interpunkce'
program vraci pro vsechny chyby stejnou hodnotu (1) s ohledem na vypis chybovych zprav mi prislo zbytecne definovat rozdilne navratove hodnoty

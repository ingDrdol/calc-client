# IPK_projek1
## Pouziti
konzole:<br>
    &emsp;./ipkcpc -h [HOSTNAME] -p [PORT] -m [MODE]<br>
Pripoji se na server zadany HOSTNAME a port PORT a posle sadu dotazu na server s bezicim programem ipkpd a pomoci vybraneho modu pomoci protokolu IPKCP a pro zadane dotazy zakoncene kodem 'EOF' tiskne prijate odpovedi.<br>
Pri jakekoli chybe v parametrech nebo pri behu tiskne chybovou hlasku na stderr a konci kodem 1.<br>
<br>
    &emsp;HOSTNAME - musi byt validni domenove jmeno serveru s bezicim programem ipkpd v odpovidajicim modu MODE<br>
    &emsp;PORT - cislo portu, na kterem program ipkpd ma otevreny soket<br>
    &emsp;MODE - ipkcp podporuje dva mody udp a tcp<br>
<br>

Protokol IPKCP:<br>
```
    &emsp;UDP<br>
    &emsp;-prijma dotazy ve forme ([+-*/] [0-9]+ [0-9]+)<br>
    &emsp;-vysledek program tiskne formou OK:[0-9]+<br>
    &emsp;-v pripade ze server nerozezna dotaz, nebo neni schopen vypocitat vysledek program vytiskne ERR:<errmsg><br>
<br>
    &emsp;TCP<br>
    &emsp;-je potreba otevrit spojeni zpravou HELLO<br>
    &emsp;-dale nasleduje libovolny pocet zprav SOLVE formou SOLVE ([+-*/] [0-9]+ [0-9]+)<br>
    &emsp;-spojeni je treba ukoncit zpravou BYE<br>
    &emsp;-v pripade ze server nerozezna zaslanou zpravu zasila zpravu BYE a ukonci spojeni, timto konci i beh programu<br>
<br>
```
V pripade zachyceni signalu SIGINT program zasle zpravu BYE a spojeni timto ukonci. Tyto zpravy uz netiskne.
## Zakladni popis
## Funkce
## Testovani
### Poznamky pod carou
nebojte prijde 'fix: interpunkce'

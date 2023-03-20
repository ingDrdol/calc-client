# IPK_projek1
## Pouziti
konzole:
	./ipkcpc -h [HOSTNAME] -p [PORT] -m [MODE]
Pripoji se na server zadany HOSTNAME a port PORT a posle sadu dotazu na server s bezicim programem ipkpd a pomoci vybraneho modu pomoci protokolu IPKCP a pro zadane dotazy zakoncene kodem 'EOF' tiskne prijate odpovedi.
Pri jakekoli chybe v parametrech nebo pri behu tiskne chybovou hlasku na stderr a konci kodem 1.

	HOSTNAME - musi byt validni domenove jmeno serveru s bezicim programem ipkpd v odpovidajicim modu MODE
	PORT - cislo portu, na kterem program ipkpd ma otevreny soket
	MODE - ipkcp podporuje dva mody udp a tcp

Protokol IPKCP:
	UDP
	-prijma dotazy ve forme ([+-*/] [0-9]+ [0-9]+)
	-vysledek program tiskne formou OK:[0-9]+
	-v pripade ze server nerozezna dotaz, nebo neni schopen vypocitat vysledek program vytiskne ERR:<errmsg>

	TCP
	-je potreba otevrit spojeni zpravou HELLO
	-dale nasleduje libovolny pocet zprav SOLVE formou SOLVE ([+-*/] [0-9]+ [0-9]+)
	-spojeni je treba ukoncit zpravou BYE
	-v pripade ze server nerozezna zaslanou zpravu zasila zpravu BYE a ukonci spojeni, timto konci i beh programu

V pripade zachyceni signalu SIGINT program zasle zpravu BYE a spojeni timto ukonci. Tyto zpravy uz netiskne.
## Zakladni popis
## Funkce
## Testovani
### Poznamky pod carou
nebojte prijde 'fix: interpunkce'

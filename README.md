# IPK_projek1

## Použití

#### Konzole:

```
./ipkcpc -h [HOSTNAME] -p [PORT] -m [MODE]
```

Připojí se na server zadaný HOSTNAME a port PORT a pošle sadu dotazů na server s běžícím programem ipkpd pomocí vybraného módu protokolem IPKCP a pro zadané dotazy zakončené kódem 'EOF' tiskne přijaté odpovědi.<br>
Při jakékoliv chybě v parametrech tiskne nápovědu. Při chybě za běhu tiskne chybovou hlášku na stderr a končí kódem 1.<br>

<br>
    &emsp;HOSTNAME - musí být validní doménové jméno serveru s bežícím programem ipkpd v odpovídajícím módu MODE<br>
    &emsp;PORT - číslo portu, na kterém program ipkpd má otevřený soket<br>
    &emsp;MODE - ipkcp podporuje dva módy udp a tcp<br>
<br>

#### Protokol IPKCP:
```
    UDP
    -příjmá dotazy ve formě ([+-*/] [0-9]+ [0-9]+)
    -výsledek program tiskne formou OK:[0-9]+
    -v případě že server nerozezná dotaz, nebo není schopen vypočítat výsledek program vytiskne ERR:<errmsg>

    TCP
    -je potřeba otevřít spojení zprávou HELLO
    -dále následuje libovolný počet zpráv SOLVE formou SOLVE ([+-*/] [0-9]+ [0-9]+)
    -spojení je třeba ukončit zprávou BYE
    -v případě že server nerozezná zaslanou zprávu zasíla BYE a ukončí spojení, tímto končí i běh programu<br>
```

V případe zachycení signalu SIGINT program zašle zpravu BYE a spojení ukončí. Tyto zprávy už netiskne.

## Základní popis

Program načte argumenty zadané přes konzoli s využitím funkce getopt (knihovna `<unistd.h>` a `<getopt.h>`). V případě neznámých argumentů, nebo špatně zadaných parametrů se tiskne nápověda a program končí chybou.<br>

Dále se zkontroluje, jestli index posledního zpracovaného argumentu není menší než celkový počet argumentů. Tím se vyřadí případné přebytečné nekontrolní argumenty. Pokud tomu tak není program opět končí chybou 1.<br>

Po splnění  "vstupních podmínek" program volá knihovní funkci gethostbyname() (knihovna `<netdb.h>`), která využívá služby DNS(dle [dokumentace](https://man7.org/linux/man-pages/man3/gethostbyname.3.html) občas není služba využita, nicméně pro představu řekněme, že se využije vždy), s jejíž pomocí získá z argumentu [HOSTNAME] IP adresu zadaného serveru. Ve chvíli, kdy funkce vrací **NULL** znamená to, že domenové jméno není známé a program končí chybou 1.<br>

Následně se nastaví komunikační protokol na IPv4 a číslo portu. Tyto informace se uloží do struktury **sockaddr_in**.<br>

S těmito daty je program schopen vytvořit soket pro odesílání a příjmání dat. K tomuto účelu je použita funkce soket() (knihovna `<sys/soket.h>`) této funkci program předává strukturu **sockaddr_in** výse nastavenou a proměnnou sock_type, která nese typ přenosu dat. (buť **SOCK_DGRAM** nebo **SOCK_STREAM** na základe vstupního parametru MODE). Pokud tato funkce vratí zápornou hodnotu, nebo nulu soket se nepodařilo vytvořit a program končí chybou 1.<br>

Nakonec program spustí jednu z funkcí `send_udp()` a `send_tcp()` pro finalní zprostředkovaní komunikace.<br>

## Funkce

##### print_help()

Vytiskne pomoc při špatnému zadání argumentů příkazové řádky.

##### send_udp()

Zajišťuje odeslání a příjem datagramů pro binarní verzi protokolu IPKCP opakovaným volaním funkcí sendto() a recvfrom() (knihovna `<sys/socket.h>`.

##### handle_sigint()

Spustí se při tcp módu a při zachycení signálu **SIGINT** zašle serveru zprávu BYE a končí spojení.

##### send_tcp()

Vyřizuje textovou komunikaci se serverem vyuzitím funkce connect() pro vytvoření spojení, send() a recv() pro odeslání a přijetí zpráv (knihovna `<sys/socket.h>`) a close() pro uzavření spojení (knihovna `<unistd.h>`).

## Testování

1. Na virtuálce ipk NixOs spusťte server ipkpd v shellu následovně:

```
ipkpd -h localhost -p 2023 -m udp & ipkpd -h localhost -p 2024 -m tcp &
```

2. Pusťte skript

```
./test.sh
```

3. Po ukončení testování je třeba najít procesy serverových stran a zasláním signálu SIGKILL (9) je ukončit

```
kill -9 idp_ipkpd_udp
kill -9 idp_ipkpd_tcp
```

### Odkazy

Ryšavý, Ondřej, Ph. D. “IPK-DemoUdp.” *FIT - VUT Brno - Git*, git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs/cpp/DemoUdp/client.c.
Ryšavý, Ondřej, Ph. D. “IPK-DemoTcp.” *FIT - VUT Brno - Git*, git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs/cpp/DemoTcp/client.c.
Kadam Patel. “Signals in C Language.” *GeeksforGeeks*, 8 Feb. 2018, www.geeksforgeeks.org/signals-c-language
www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html

### Poznámky pod čarou

nebojte už přišel 'fix: interpunkce'
program vrací pro věechny chyby stejnou hodnotu (1) s ohledem na výpis chybových zpráv mi přislo zbytečné definovat rozdílné návratové hodnoty

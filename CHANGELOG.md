## Popis funkcionality (stejná jako sekce základní popis v README)

Program načte argumenty zadané přes konzoli s využitím funkce getopt (knihovna `<unistd.h>` a `<getopt.h>`). V případě neznámých argumentů, nebo špatně zadaných parametrů se tiskne nápověda a program končí chybou.<br>

Dále se zkontroluje, jestli index posledního zpracovaného argumentu není menší než celkový počet argumentů. Tím se vyřadí případné přebytečné nekontrolní argumenty. Pokud tomu tak není program opět končí chybou 1.<br>

Po splnění  "vstupních podmínek" program volá knihovní funkci gethostbyname() (knihovna `<netdb.h>`), která využívá služby DNS(dle [dokumentace](https://man7.org/linux/man-pages/man3/gethostbyname.3.html) občas není služba využita, nicméně pro představu řekněme, že se využije vždy), s jejíž pomocí získá z argumentu [HOSTNAME] IP adresu zadaného serveru. Ve chvíli, kdy funkce vrací **NULL** znamená to, že domenové jméno není známé a program končí chybou 1.<br>

Následně se nastaví komunikační protokol na IPv4 a číslo portu. Tyto informace se uloží do struktury **sockaddr_in**.<br>

S těmito daty je program schopen vytvořit soket pro odesílání a příjmání dat. K tomuto účelu je použita funkce soket() (knihovna `<sys/soket.h>`) této funkci program předává strukturu **sockaddr_in** výse nastavenou a proměnnou sock_type, která nese typ přenosu dat. (buť **SOCK_DGRAM** nebo **SOCK_STREAM** na základe vstupního parametru MODE). Pokud tato funkce vratí zápornou hodnotu, nebo nulu soket se nepodařilo vytvořit a program končí chybou 1.<br>

Nakonec program spustí jednu z funkcí `send_udp()` a `send_tcp()` pro finalní zprostředkovaní komunikace.<br>

## Známé nedostatky

1. Co jsem našel, to jsem vyřešil. Nicméně chyba tam určitě někde je
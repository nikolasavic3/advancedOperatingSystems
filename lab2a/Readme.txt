Example device driver + timers + workqueues

Based mostly on instructions in book
	"Linux Device Drivers, Third Edition" by
	Jonathan Corbet, Alessandro Rubini, and Greg Kroah-Hartman


Copyright (C) 2021 Leonardo Jelenkovic

The source code in this file can be freely used, adapted,
and redistributed in source or binary form.
No warranty is attached.


----------------------------------------------------------------------------------------


IMPLEMENTACIJA:

Kernel modul (shofer.ko):

Kreira 6 character device naprava (/dev/shofer0 do /dev/shofer5)
Svaka naprava ima svoj međuspremnik
Omogućuje read/write operacije nad napravama


Program citac:

Otvara svih 6 naprava u read-only modu
Koristi poll() za praćenje novih podataka
Kad stigne novi podatak, čita ga i ispisuje na ekran
Simultano prati sve naprave


Program pisac:

Otvara svih 6 naprava u write-only modu
Svakih 5 sekundi provjerava dostupnost naprava za pisanje
Nasumično odabire jednu dostupnu napravu
Šalje nasumično generirano slovo (A-Z)



KORIŠTENJE:

Učitavanje modula:
sudo ./load_shofer
Kompilacija programa:
gcc -o citac test/citac.c
gcc -o pisac test/pisac.c
Pokretanje (u odvojenim terminalima):
./citac    # čita znakove sa svih naprava
./pisac    # šalje nasumična slova svakih 5 sekundi
Uklanjanje modula:
sudo rmmod shofer

TOK RADA:

Modul kreira naprave pri učitavanju
Pisac periodički šalje podatke na nasumične naprave
Citac konstantno osluškuje sve naprave
Kad pisac zapiše znak, citac ga odmah detektira i ispisuje


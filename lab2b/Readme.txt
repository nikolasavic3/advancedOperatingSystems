Example device driver

Based mostly on instructions in book
	"Linux Device Drivers, Third Edition" by
	Jonathan Corbet, Alessandro Rubini, and Greg Kroah-Hartman


Copyright (C) 2021 Leonardo Jelenkovic

The source code in this file can be freely used, adapted,
and redistributed in source or binary form.
No warranty is attached.

-------------------------------------------

Implementirane funkcije:
1. shofer_open_write
   - Implementira otvaranje ulaznog uređaja
   - Provjerava dozvole za pisanje
   - Postavlja privatne podatke za uređaj

2. shofer_write
   - Implementira pisanje u ulazni međuspremnik
   - Koristi kfifo za sigurno spremanje podataka
   - Uključuje zaključavanje za sigurnost kod višestrukog pristupa

3. control_ioctl
   - Implementira upravljanje prijenosom podataka
   - Prebacuje zadani broj znakova iz ulaznog u izlazni međuspremnik
   - Koristi zaključavanje oba međuspremnika

Način rada:
- Ulazni uređaj (/dev/shofer_in) prima podatke kroz write operaciju
- Upravljački uređaj (/dev/shofer_control) premješta podatke kroz ioctl
- Izlazni uređaj (/dev/shofer_out) šalje podatke kroz read operaciju
- Timer automatski premješta jedan znak svakih 5 sekundi

Testiranje:
1. Kompilacija:
   make

2. Učitavanje modula:
   sudo ./load_shofer

3. Primjer korištenja:
   echo "test" > /dev/shofer_in
   cd test && sudo ./ioctl /dev/shofer_control <broj>
   cat /dev/shofer_out

4. Uklanjanje modula:
   sudo ./unload_shofer

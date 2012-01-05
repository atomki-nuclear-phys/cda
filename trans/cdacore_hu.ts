<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="hu_HU">
<context>
    <name>Address</name>
    <message>
        <location filename="../core/common/Address.cxx" line="39"/>
        <source>Constructing address from string: &quot;%1&quot;</source>
        <translatorcomment>Using &quot;ö&quot; instead of &quot;ő&quot; as the encoding doesn&apos;t like the latter.</translatorcomment>
        <translation>Cím létrehozása a(z) &quot;%1&quot; névböl</translation>
    </message>
    <message>
        <location filename="../core/common/Address.cxx" line="47"/>
        <source>Couldn&apos;t decode address: &quot;%1&quot;
Setting to 127.0.0.1:50000</source>
        <translation>Nem lehetett értelmezni a(z) &quot;%1&quot; címet
Beállítva 127.0.0.1:50000 címre</translation>
    </message>
    <message>
        <location filename="../core/common/Address.cxx" line="57"/>
        <source>Decoded host name: %1, port: %2</source>
        <translation>A dekódolt hoszt név: %1, port: %2</translation>
    </message>
    <message>
        <location filename="../core/common/Address.cxx" line="68"/>
        <source>Couldn&apos;t decode port: %1
Setting it to 50000</source>
        <translation>Nem lehetett a(z) &quot;%1&quot; portot értelmezni
Beállítva 50000-re</translation>
    </message>
</context>
<context>
    <name>CHECK</name>
    <message>
        <location filename="../core/common/errorcheck.h" line="33"/>
        <source>Failed executing &quot;%1&quot;</source>
        <translation>Hiba a következö futtatásakor: &quot;%1&quot;</translation>
    </message>
</context>
<context>
    <name>Digitizer_CHECK</name>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="388"/>
        <source>Failed executing &quot;%1&quot;, Return value: %2</source>
        <translation>Hiba a következö futtatásakor: &quot;%1&quot;, visszatérési érték: %2</translation>
    </message>
</context>
<context>
    <name>Fifo</name>
    <message>
        <location filename="../core/fifo/Fifo.cxx" line="37"/>
        <source>%1 exists</source>
        <translation>%1 létezik</translation>
    </message>
    <message>
        <location filename="../core/fifo/Fifo.cxx" line="41"/>
        <source>%1 does not exist</source>
        <translation>%1 nem létezik</translation>
    </message>
</context>
<context>
    <name>caen::Digitizer</name>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="571"/>
        <source>Opening connection type %1 on link number %2,  CONET node %3, VME address %4</source>
        <translation>Kapcsolat megnyitása %1 típussal a(z) %2 link számon, %3 CONET egységgel, %4 VME címmel</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="579"/>
        <source>Opened connection to digitizer</source>
        <translation>A kapcsolat a digitalizálóval megnyitva</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="597"/>
        <source>No need to close the device, it&apos;s not open</source>
        <translation>Nem szükséges lezárni az eszközt, nincsen megnyitva</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="612"/>
        <source>Closed connection to digitizer</source>
        <translation>A kapcsolat a digitalizálóval lezárva</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="624"/>
        <source>writeRegister( address: %1, data: %2 )</source>
        <translation>writeRegister( cím: %1, adat: %2 )</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="638"/>
        <source>readRegister( address: %1, data: to be set )</source>
        <translation>readRegister( cím: %1, adat: beállítás alatt )</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="663"/>
        <source>Digitizer board information:
  model name : %1
  model      : %2
  channels   : %3
  form factor: %4
  family code: %5
  ROC firmwate release: %6
  AMC firmware release: %7
  serial num.: %8
  PCB rev.   : %9
  ADC NBits  : %10</source>
        <translation>Digitalizáló eszköz információ:
  modell név  : %1
  modell      : %2
  csatornák   : %3
  forma faktor: %4
  család-kód  : %5
  ROC firmware kiadás: %6
  AMC firmware kiadás: %7
  sorozat szám: %8
  PCB verzió  : %9
  ADC bit-szám: %10</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="684"/>
        <location filename="../core/caen/Digitizer.cxx" line="845"/>
        <source>No CAEN library available</source>
        <translation>A CAEN könyvtár nem elérhetö</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="779"/>
        <source>Setting acquisition mode to %1</source>
        <translation>Az adatgyüjtési mód beállítása %1-ra/re</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="811"/>
        <source>Setting interrupt to happen after %1 events</source>
        <translation>A megszakítás beállítása %1 esemény begyüjtése után</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="833"/>
        <source>Interrupt configuration:
  state       : %1
  level       : %2
  status ID   : %3
  event number: %4
  mode        : %5</source>
        <translation>Megszakítási beállítások:
  állapot          : %1
  szint            : %2
  státusz azonosító: %3
  esemény szám     : %4
  mód              : %5</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="870"/>
        <source>Setting SW trigger mode to %1</source>
        <translation>A szoftveres trigger beállítása %1 módba</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="898"/>
        <source>Setting Ext trigger mode to %1</source>
        <translation>A külsö trigger beállítása %1 módba</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="927"/>
        <source>Setting channel %1 self-trigger mode to %2</source>
        <translation>A(z) %1. csatorna ön-trigger módja %2-ra/re állítva</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="944"/>
        <source>Requesting channel %1&apos;s self-trigger mode</source>
        <translation>A(z) %1. csatorna ön-trigger módjának lekérdezése</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="960"/>
        <source>Setting group %1 self-trigger mode to %2</source>
        <translation>A(z) %1. csoport ön trigger módja %2-ra/re állítva</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="977"/>
        <source>Requesting group %1&apos;s self-trigger mode</source>
        <translation>A(z) %1. csoport ön-trigger módjának lekérdezése</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="991"/>
        <source>Sent a SW trigger</source>
        <translation>Szoftveres trigger elküldve</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1005"/>
        <source>Setting group %1&apos;s channel mask to %2</source>
        <translation>A(z) %1. csoport csatorna maszkja %2-ra/re állítva</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1020"/>
        <source>Requestuing group %1&apos;s channel mask</source>
        <translation>A(z) %1. csoport csatorna maszkjának lekérdezése</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1036"/>
        <source>Setting channel %1&apos;s trigger threshold to %2</source>
        <translation>A(z) %1. csatorna trigger küszöbe beállítva %2-ra/re</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1051"/>
        <source>Retrieving trigger threshold for channel %1</source>
        <translation>A(z) %1. csatorna trigger küszöbének lekérdezése</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1067"/>
        <source>Setting group %1&apos;s trigger threshold to %2</source>
        <translation>A(z) %1. csoport trigger küszöbe beállítva %2-ra/re</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1082"/>
        <source>Retrieving trigger threshold for group %1</source>
        <translation>A(z) %1. csoport trigger küszöbének lekérdezése</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1096"/>
        <source>Channel enable mask set to %1</source>
        <translation>A csatorna engedélyezési maszk %1-ra/re állítva</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1121"/>
        <source>Group enable mask set to %1</source>
        <translation>A csoport engedélyezési maszk %1-ra/re állítva</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1146"/>
        <source>Record length set to %1</source>
        <translation>A minta szám %1-ra/re állítva</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1172"/>
        <source>Post trigger size set to %1%</source>
        <translation>A trigger utáni minta méret %1%-ra állítva</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1199"/>
        <source>Setting channel %1&apos;s DC offset to %2</source>
        <translation>A(z) %1. csatorna DC eltolása %2-ra/re állítva</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1214"/>
        <source>Retrieving channel %1&apos;s DC offset</source>
        <translation>A(z) %1. csatorna DC eltolásának lekérdezése</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1230"/>
        <source>Setting group %1&apos;s DC offset to %2</source>
        <translation>A(z) %1. csoport DC eltolása %2-ra/re állítva</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1245"/>
        <source>Retrieving group %1&apos;s DC offset</source>
        <translation>A(z) %1. csoport DC eltolásának lekérdezése</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1259"/>
        <source>Setting maximum number of events to read out to %1</source>
        <translation>Az egyszerre kiolvasható események száma %1-ra/re állítva</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1272"/>
        <source>Retrieving maximum number of events to read out</source>
        <translation>Az egyszerre kiolvasható események számának lekérdezése</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1285"/>
        <source>Allocating readout buffer</source>
        <translation>Kiolvasási buffer lefoglalása</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1299"/>
        <source>Freeing readout buffer</source>
        <translation>Kiolvasási buffer felszabadítása</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1314"/>
        <source>Reading data in mode &quot;%1&quot;</source>
        <translation>Adatok olvasása &quot;%1&quot; módban</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1331"/>
        <source>Extracting number of events from buffer of size %1</source>
        <translation>Az események számának megállapítása egy %1 méretü bufferböl</translation>
    </message>
    <message>
        <location filename="../core/caen/Digitizer.cxx" line="1373"/>
        <location filename="../core/caen/Digitizer.cxx" line="1430"/>
        <source>Reading event %1 from buffer with size %2</source>
        <translation>A(z) %1. esemény kiolvasása egy %2 méretü bufferböl</translation>
    </message>
</context>
<context>
    <name>camac::Crate</name>
    <message>
        <location filename="../core/camac/Crate.cxx" line="26"/>
        <source>Device constructed</source>
        <translation>Eszköz létrehozva</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="36"/>
        <source>Device destructed</source>
        <translation>Eszköz törölve</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="61"/>
        <source>CAMAC crate could not be opened
with message: %1</source>
        <translation>A CAMAC keretet nem lehetett megnyitni.
A kapott üzenet: %1</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="68"/>
        <source>CAMAC crate opened on device: %1</source>
        <translation>CAMAC keret megnyitva a(z) %1 eszközön</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="91"/>
        <source>CAMAC crate closing failed
with message: %1</source>
        <translation>A CAMAC keret lezárása sikertelen.
A kapott üzenet: %1</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="98"/>
        <source>CAMAC crate closed on device: %1</source>
        <translation>CAMAC keret lezárva a(z) %1 eszközön</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="122"/>
        <source>readWord(...) called with: N = %1, A = %2, F = %3</source>
        <translation>readWord(...) meghívva N = %1, A = %2, F = %3 paraméterekkel</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="146"/>
        <source>readLong(...) called with: N = %1, A = %2, F = %3</source>
        <translation>readLong(...) meghívva N = %1, A = %2, F = %3 paraméterekkel</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="176"/>
        <source>readLong(...) called with: N = %1, A = %2, F = %3, Q, X</source>
        <translation>readLong(...) meghívva N = %1, A = %2, F = %3, Q, X paraméterekkel</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="204"/>
        <source>writeWord(...) called with: N = %1, A = %2, F = %3, data = %4</source>
        <translation>writeWord(...) meghívva N = %1, A = %2, F = %3, adat = %4 paraméterekkel</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="230"/>
        <source>writeLong(...) called with: N = %1, A = %2, F = %3, data = %4</source>
        <translation>writeLong(...) meghívva N = %1, A = %2, F = %3, adat = %4 paraméterekkel</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="253"/>
        <source>There was a problem enabling interrupts
on device: %1</source>
        <translation>Hiba történt a megszakítások engedélyezésével
a(z) %1 eszközön</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="259"/>
        <source>Interrupts enabled on device: %1</source>
        <translation>Megszakítások engedélyezve a(z) %1 eszközön</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="280"/>
        <source>There was a problem disabling interrupts
on device: %1</source>
        <translation>Hiba történt a megszakítások kikapcsolásával
a(z) %1 eszközön</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="286"/>
        <source>Interrupts disabled on device: %1</source>
        <translation>Megszakítások kikapcsolva a(z) %1 eszközön</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="300"/>
        <source>Inhibit set on device: %1</source>
        <translation>Gátlás beállítva a(z) %1 eszközön</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="314"/>
        <source>Inhibit reset on device: %1</source>
        <translation>Gátlás felfüggesztve a(z) %1 eszközön</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="329"/>
        <source>Initialize called on device: %1</source>
        <translation>Inicializáció meghívva a(z) %1 eszközre</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="344"/>
        <source>Clear called on device: %1</source>
        <translation>Törlés meghívva a(z) %1 eszközre</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="359"/>
        <source>LAM mask set to: 0x%1</source>
        <translation>A LAM maszk beállítva 0x%1-ra/re</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="384"/>
        <source>There was a problem waiting for a LAM interrupt.
Cause: </source>
        <translation>Hiba történt egy LAM megszakításra várakozás közben.
Magyarázat: </translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="390"/>
        <source>Waiting for LAM</source>
        <translation>Várakozás egy LAM-ra</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="406"/>
        <source>LAM cleared on device: %1</source>
        <translation>LAM jel törölve a(z) %1 eszközön</translation>
    </message>
    <message>
        <location filename="../core/camac/Crate.cxx" line="424"/>
        <source>Trying to use the CAMAC crate without opening it first
Use the Crate::open(...) function!</source>
        <translation>Megpróbálta a CAMAC keretet hasznáni annak megnyitása nélkül
Használja a Crate::open(...) függvényt!</translation>
    </message>
</context>
<context>
    <name>cernlib::HistMgr</name>
    <message>
        <location filename="../core/cernlib/HistMgr.cxx" line="27"/>
        <source>Object constructed</source>
        <translation>Objektum létrehozva</translation>
    </message>
    <message>
        <location filename="../core/cernlib/HistMgr.cxx" line="43"/>
        <source>Booking global memory block: %1</source>
        <translation>A(z) %1 globális memória blokk lefoglalása</translation>
    </message>
    <message>
        <location filename="../core/cernlib/HistMgr.cxx" line="56"/>
        <source>Couldn&apos;t const_cast specified global memory name</source>
        <translation>Nem lehetett const_cast-olni a megadott globális memória nevet</translation>
    </message>
    <message>
        <location filename="../core/cernlib/HistMgr.cxx" line="64"/>
        <source>Created PAW global memory with name: &quot;%1&quot;</source>
        <translation>PAW globális memória létrehozva &quot;%1&quot; névvel</translation>
    </message>
    <message>
        <location filename="../core/cernlib/HistMgr.cxx" line="88"/>
        <source>Booking 1D histogram with name &quot;%1&quot;, channels: %2, lower bound: %3, upper bound: %4</source>
        <translation>1 dimenziós hisztogram lefoglalva &quot;%1&quot; névvel, %2 csatornával, %3 alsó és %4 felsö korláttal</translation>
    </message>
    <message>
        <location filename="../core/cernlib/HistMgr.cxx" line="101"/>
        <location filename="../core/cernlib/HistMgr.cxx" line="149"/>
        <source>Couldn&apos;t const_cast specified histogram name</source>
        <translation>Nem lehetett const_cast-olni a megadott hisztogram nevet</translation>
    </message>
    <message>
        <location filename="../core/cernlib/HistMgr.cxx" line="136"/>
        <source>Booking 2D histogram with name &quot;%1&quot;, x-channels: %2, x-low: %3, x-up: %4, y-channels: %5, y-low: %6, y-up: %7</source>
        <translation>2 dimenziós histogram lefoglalva &quot;%1&quot; névvel, %2 x-csatornával, %3 x alsó-, %4 felsö korláttal, %5 y-csatornával, %6 y-alsó és %7 felsö-korláttal</translation>
    </message>
    <message>
        <location filename="../core/cernlib/HistMgr.cxx" line="179"/>
        <source>Filling histogram %1 with data: %2, weight: %3</source>
        <translation>A(z) %1 hisztogram feltöltése %2 helyen %3 súllyal</translation>
    </message>
    <message>
        <location filename="../core/cernlib/HistMgr.cxx" line="210"/>
        <source>Filling histogram %1 with xdata: %2, ydata: %3, weight: %4</source>
        <translation>A(z) %1 hisztogram feltöltése %2 X- ées %3 Y-koordinátákkal és %4 súllyal</translation>
    </message>
    <message>
        <location filename="../core/cernlib/HistMgr.cxx" line="237"/>
        <source>Trying to create/access histograms without
initializing the global memory first</source>
        <translatorcomment>Using &quot;ö&quot; instead of &quot;ő&quot; because the latter is not supported by the text encoding</translatorcomment>
        <translation>Hisztogramok készítésének/elérésének megkísérlése a
globális memória inicializációja elött</translation>
    </message>
</context>
<context>
    <name>cernlib::NTupleMgr</name>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="44"/>
        <source>Object constructed</source>
        <translation>Objektum létrehozva</translation>
    </message>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="54"/>
        <source>Object destructed</source>
        <translation>Objektum törölve</translation>
    </message>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="73"/>
        <source>Adding variable with name: %1</source>
        <translation>Új változó hozzáadása &quot;%1&quot; névvel</translation>
    </message>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="79"/>
        <source>You can&apos;t add new variables when the output file
is already open!</source>
        <translation>Nem lehet új változókat megadni amikor a kimeneti fájl
már meg van nyitva!</translation>
    </message>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="104"/>
        <source>Opening output file: %1</source>
        <translation>A(z) &quot;%1&quot; kimeneti fájl megnyitása</translation>
    </message>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="112"/>
        <source>The output file is already open</source>
        <translation>A kimeneti fájl már meg van nyitva</translation>
    </message>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="126"/>
        <source>Variable name &quot;%1&quot; too long. It was shortened to &quot;%2&quot;</source>
        <translation>A(z) &quot;%1&quot; változó név túl hosszú. Le lett rövidítve &quot;%2&quot;-ra/re</translation>
    </message>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="140"/>
        <source>Output file name too long. File not opened!</source>
        <translation>A kimeneti fájlnév túl hosszú. A fájl nem lett megnyitva!</translation>
    </message>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="154"/>
        <source>There was a problem opening: %1</source>
        <translation>Hiba történt a(z) %1 megnyitásával</translation>
    </message>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="157"/>
        <source>&quot;%1&quot; opened</source>
        <translation>&quot;%1&quot; megnyitva</translation>
    </message>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="166"/>
        <source>NTuple created</source>
        <translation>NTuple létrehozva</translation>
    </message>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="192"/>
        <source>No output file is open</source>
        <translation>Nincsen kimeneti fájl megnyitva</translation>
    </message>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="210"/>
        <source>A total of %1 events written to the ntuple</source>
        <translation>Összesen %1 esemény kiírva az ntuple-ba</translation>
    </message>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="217"/>
        <source>Output file closed</source>
        <translation>Kimeneti fájl lezárva</translation>
    </message>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="243"/>
        <source>Setting variable %1 to: %2</source>
        <translation>A(z) &quot;%1&quot; változó %2-re/ra állítva</translation>
    </message>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="250"/>
        <source>You should not be setting variables before opening
an output file!</source>
        <translation>Nem lehet a változó értékeket beállítani egy kimeneti
fájl megnyitása előtt!</translation>
    </message>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="257"/>
        <source>Index %1 is not defined in the ntuple!</source>
        <translation>A(z) %1 index nincs definiálva az ntuple-ban!</translation>
    </message>
    <message>
        <location filename="../core/cernlib/NTupleMgr.cxx" line="282"/>
        <source>saveEvent() should only be called when an output
file is already open!</source>
        <translation>A saveEvent() függvényt csak akkor lehet meghívni amikor egy
kimeneti fájl már meg van nyitva!</translation>
    </message>
</context>
<context>
    <name>dev::Loader</name>
    <message>
        <location filename="../core/device/Loader.cxx" line="318"/>
        <source>Setting device plugin directory to: %1</source>
        <translation>Az eszköz plugin könyvtár beállítva %1-ra/re</translation>
    </message>
    <message>
        <location filename="../core/device/Loader.cxx" line="95"/>
        <source>There was a problem loading the static plugins</source>
        <translation>Hiba történt a statikus plugin-ek betöltése közben</translation>
    </message>
    <message>
        <location filename="../core/device/Loader.cxx" line="98"/>
        <source>Loaded the static plugins</source>
        <translation>A statikus plugin-ek betöltve</translation>
    </message>
    <message>
        <location filename="../core/device/Loader.cxx" line="115"/>
        <source>Evaluating: %1</source>
        <translation>%1 elbírálása</translation>
    </message>
    <message>
        <location filename="../core/device/Loader.cxx" line="122"/>
        <source>Trying to load plugin: %1</source>
        <translation>Megpróbálkozás a(z) %1 plugin betöltésével</translation>
    </message>
    <message>
        <location filename="../core/device/Loader.cxx" line="125"/>
        <source>%1 loaded</source>
        <translation>%1 betöltve</translation>
    </message>
    <message>
        <location filename="../core/device/Loader.cxx" line="128"/>
        <source>%1 could not be loaded</source>
        <translation>%1-t nem lehetett betölteni</translation>
    </message>
    <message>
        <location filename="../core/device/Loader.cxx" line="159"/>
        <source>Couldn&apos;t load plugin with name: %1</source>
        <translatorcomment>Using &quot;ü&quot; instead of &quot;ű&quot;, as the latter is not supported by the current encoding.</translatorcomment>
        <translation>Nem lehetett a(z) %1 nevü plugin-t betölteni</translation>
    </message>
    <message>
        <location filename="../core/device/Loader.cxx" line="163"/>
        <source>Loaded plugin with name: %1</source>
        <translatorcomment>Using &quot;ü&quot; instead of &quot;ű&quot;, as the latter is not supported by the current encoding.</translatorcomment>
        <translation>A(z) %1 nevü plugin betöltve</translation>
    </message>
    <message>
        <location filename="../core/device/Loader.cxx" line="172"/>
        <source>%1 doesn&apos;t provide the dev::Factory interface!</source>
        <translation>%1 nem biztosítja a dev::Factory interfészt!</translation>
    </message>
    <message>
        <location filename="../core/device/Loader.cxx" line="178"/>
        <source>Accessed dev::Factory interface in plugin: %1</source>
        <translation>Sikerült a dev::Factory interfészt elérni a(z) %1 plugin-ben</translation>
    </message>
    <message>
        <location filename="../core/device/Loader.cxx" line="195"/>
        <source>Loaded device &quot;%1&quot; from plugin: %2</source>
        <translation>&quot;%1&quot; eszköz betöltve a(z) %2 plugin-ból</translation>
    </message>
    <message>
        <location filename="../core/device/Loader.cxx" line="231"/>
        <source>Found a static plugin not providing the needed interface</source>
        <translation>Találtam egy statikus plugin-t ami nem biztosítja az elvárt interfészt</translation>
    </message>
    <message>
        <location filename="../core/device/Loader.cxx" line="235"/>
        <source>Accessed dev::Factory interface in static plugin</source>
        <translation>Sikerült a dev::Factory interfészt elérni egy statikus plugin-ban</translation>
    </message>
    <message>
        <location filename="../core/device/Loader.cxx" line="245"/>
        <source>Loaded device &quot;%1&quot; from a static plugin</source>
        <translation>&quot;%1&quot; eszköz betöltve egy statikus plugin-ból</translation>
    </message>
    <message>
        <location filename="../core/device/Loader.cxx" line="277"/>
        <source>No device with name &quot;%1&quot; loaded currently</source>
        <translation>Nincs eszköz &quot;%1&quot; névvel betöltve jelenleg</translation>
    </message>
</context>
<context>
    <name>ev::BinaryStream</name>
    <message>
        <location filename="../core/event/BinaryStream_ev.cxx" line="67"/>
        <location filename="../core/event/BinaryStream_ev.cxx" line="133"/>
        <location filename="../core/event/BinaryStream_ev.cxx" line="147"/>
        <source>Incomplete event received</source>
        <translation>Részleges esemény fogadva</translation>
    </message>
    <message>
        <location filename="../core/event/BinaryStream_ev.cxx" line="104"/>
        <source>Couldn&apos;t send data</source>
        <translation>Nem lehetett az adatot elküldeni</translation>
    </message>
</context>
<context>
    <name>ev::EventServer</name>
    <message>
        <location filename="../core/event/EventServer.cxx" line="44"/>
        <source>Restarting the event reading thread.
This could be dangerous...</source>
        <translation>Az esemény-olvasó programszál újraindítása.
Ez elég veszélyes lehet...</translation>
    </message>
    <message>
        <location filename="../core/event/EventServer.cxx" line="53"/>
        <source>Listening for incoming events on %1:%2</source>
        <translation>Bejövö események várása a(z) %1:%2 címen</translation>
    </message>
    <message>
        <location filename="../core/event/EventServer.cxx" line="108"/>
        <source>Server is listening on host &quot;%1&quot; and port %2</source>
        <translation>A szerver a(z) &quot;%1&quot; néven és %2 porton figyel</translation>
    </message>
    <message>
        <location filename="../core/event/EventServer.cxx" line="112"/>
        <source>Server could not be started on host &quot;%1&quot; and port &quot;%2&quot;</source>
        <translation>A szervert nem lehetett elindítani a(z) &quot;%1&quot; címen és &quot;%2&quot; porton</translation>
    </message>
    <message>
        <source>Received new incoming connection</source>
        <translation type="obsolete">Új beérkezö kapcsolat érzékelése</translation>
    </message>
    <message>
        <location filename="../core/event/EventServer.cxx" line="130"/>
        <source>There was a problem while waiting for an incoming connection</source>
        <translation>Hiba történt bejövö kapcsolatra várakozás közben</translation>
    </message>
    <message>
        <location filename="../core/event/EventServer.cxx" line="141"/>
        <source>There was a problem while accepting an incoming connection</source>
        <translation>Hiba történt egy bejövö kapcsolat felépítése közben</translation>
    </message>
    <message>
        <source>Data is ready for readout</source>
        <translation type="obsolete">Adatok állnak készen kiolvasásra</translation>
    </message>
    <message>
        <location filename="../core/event/EventServer.cxx" line="162"/>
        <source>Connection to event sender lost.</source>
        <translation>Kapcsolat megszakadt az esemény-küldövel.</translation>
    </message>
    <message>
        <source>Bytes available: %1</source>
        <translation type="obsolete">%1 bájt elérhetö</translation>
    </message>
    <message>
        <location filename="../core/event/EventServer.cxx" line="191"/>
        <source>Can&apos;t store any more events in internal buffer
Throwing away incoming event!</source>
        <translation>Nem lehet több eseményt rögzíteni a belsö bufferben
Eldobom a bejövö eseményt!</translation>
    </message>
</context>
<context>
    <name>ev::Sender</name>
    <message>
        <location filename="../core/event/Sender_ev.cxx" line="22"/>
        <source>Object created</source>
        <translation>Objektum létrehozva</translation>
    </message>
    <message>
        <location filename="../core/event/Sender_ev.cxx" line="39"/>
        <source>Object deleted</source>
        <translation>Objektum törölve</translation>
    </message>
    <message>
        <location filename="../core/event/Sender_ev.cxx" line="56"/>
        <source>Connecting to %1:%2</source>
        <translation>Kapcsolódás a(z) %1:%2 címhez</translation>
    </message>
    <message>
        <location filename="../core/event/Sender_ev.cxx" line="80"/>
        <source>Connected to event receiver on %1:%2</source>
        <translation>Kapcsolódás sikeres egy esemény fogadóval a(z) %1:%2 címen</translation>
    </message>
    <message>
        <source>Bytes to write: %1</source>
        <translation type="obsolete">Írandó bájtok: %1</translation>
    </message>
    <message>
        <location filename="../core/event/Sender_ev.cxx" line="136"/>
        <source>Could not connect to event receiver on address &quot;%1&quot;, port &quot;%2&quot;</source>
        <translation>Nem sikerült egy esemény-olvasóhoz kapcsolódni a(z) &quot;%1&quot; címen, &quot;%2&quot; porton</translation>
    </message>
</context>
<context>
    <name>ev::TextStream</name>
    <message>
        <location filename="../core/event/TextStream_ev.cxx" line="47"/>
        <source>-------- Event record --------
</source>
        <translation>-------- Esemény feljegyzés --------
</translation>
    </message>
    <message>
        <location filename="../core/event/TextStream_ev.cxx" line="49"/>
        <source> Number of fragments: %1

</source>
        <translation> Fragmentumok száma: %1

</translation>
    </message>
    <message>
        <location filename="../core/event/TextStream_ev.cxx" line="62"/>
        <source> No event fragments!
</source>
        <translation> Nincs esemény fragmentum!
</translation>
    </message>
    <message>
        <location filename="../core/event/TextStream_ev.cxx" line="81"/>
        <source>  ---- Fragment record ----
</source>
        <translation>  ---- Fragmentum feljegyzés ----
</translation>
    </message>
    <message>
        <location filename="../core/event/TextStream_ev.cxx" line="83"/>
        <source>   Module ID   : %1

</source>
        <translation>   Modul azonosítója: %1</translation>
    </message>
    <message>
        <source>   Crate number : %1
</source>
        <translation type="obsolete">   Keret száma: %1
</translation>
    </message>
    <message>
        <source>   Module number: %1

</source>
        <translation type="obsolete">   Modul száma: %1
</translation>
    </message>
    <message>
        <location filename="../core/event/TextStream_ev.cxx" line="93"/>
        <source>   Data word: 0x%1
</source>
        <translation>   Adat: 0x%1
</translation>
    </message>
    <message>
        <location filename="../core/event/TextStream_ev.cxx" line="98"/>
        <source>   No data words!
</source>
        <translation>   Nem tartalmaz adatot!
</translation>
    </message>
</context>
<context>
    <name>i18n::Loader</name>
    <message>
        <location filename="../core/i18n/Loader_i18n.cxx" line="51"/>
        <source>Setting translation directory to: %1</source>
        <translation>Fordítások kövtára beállítva %1-nak/nek</translation>
    </message>
    <message>
        <location filename="../core/i18n/Loader_i18n.cxx" line="98"/>
        <source>Couldn&apos;t load translation file: %1</source>
        <translation>Nem sikerült betölteni a(z) %1 fordítási fájlt</translation>
    </message>
    <message>
        <location filename="../core/i18n/Loader_i18n.cxx" line="103"/>
        <source>Loaded translation file: %1</source>
        <translation>A(z) %1 fordítási fájl betöltve</translation>
    </message>
    <message>
        <location filename="../core/i18n/Loader_i18n.cxx" line="111"/>
        <source>Loaded all translations</source>
        <translation>Minden fordítás betöltve</translation>
    </message>
</context>
<context>
    <name>msg::Sender</name>
    <message>
        <location filename="../core/msg/Sender_msg.cxx" line="129"/>
        <source>* No message server set up yet.
* Messages that could not be sent will be
* printed to the console.</source>
        <translation>* Nincsen még üzenet szerver beállítva.
* Az üzenetek amiket nem lehetett elküldeni,
* a konzolra lesznek kiírva.</translation>
    </message>
    <message>
        <location filename="../core/msg/Sender_msg.cxx" line="163"/>
        <source>* A Message Server could not be contacted!
* Messages that could not be sent will be
* printed to the console.</source>
        <translation>* Nem lehetett elérni az egyik üzenet szervert!
* Az üzenetek amiket nem lehetett elküldeni,
* a konzolra lesznek kiírva.</translation>
    </message>
</context>
<context>
    <name>root::NTupleMgr</name>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="43"/>
        <source>Opening output file with name &quot;%1&quot; and tree name &quot;%2&quot;</source>
        <translation>A(z) &quot;%1&quot; kimeneti fájl megnyitása &quot;%2&quot; &quot;fa&quot; névvel</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="50"/>
        <source>An output file is already open. Not doing anything.</source>
        <translation>Egy kimeneti fájl már nyitva van. Nem csinálok semmit.</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="59"/>
        <source>Failed creating output file with name: %1</source>
        <translation>Nem sikerült megnyitni a kimeneti fájlt &quot;%1&quot; névvel</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="63"/>
        <source>&quot;%1&quot; opened</source>
        <translation>&quot;%1&quot; megnyitva</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="93"/>
        <source>No output file is open at the moment</source>
        <translation>Nincs kimeneti fájl megnyitva jelenleg</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="104"/>
        <source>The object is in an inconsistent state</source>
        <translation>Az objektum inknonzisztens állapotban van</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="110"/>
        <source>An error happened while writing out the final events</source>
        <translation>Hiba történt az utolsó események kiírása közben</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="129"/>
        <source>Closed output file after writing %1 events</source>
        <translation>Kimeneti fájl lezárva %1 esemény kiírása után</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="148"/>
        <source>A file is not open at the moment</source>
        <translation>Nincsen fájl megnyitva jelenleg</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="155"/>
        <source>A TTree is not available at the moment</source>
        <translation>A TTree nem elérhetö jelenleg</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="161"/>
        <source>An error happened while writing the current event</source>
        <translation>Hiba történt a jelenlegi esemény kiírása közben</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="173"/>
        <source>Received a complex object description</source>
        <translation>Egy komplex objektum leírását kaptam</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="214"/>
        <location filename="../core/root/NTupleMgr_root.cxx" line="220"/>
        <source>Unknown primitive type encountered: %1</source>
        <translation>Ismeretlen egyszerü típust találtam: %1</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="230"/>
        <source>You have to open an output file before declaring variables</source>
        <translation>Meg kell nyitni egy kimeneti fájlt a változók deklarálása elött</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="237"/>
        <source>A branch with name &quot;%1&quot; already exists</source>
        <translation>Egy &quot;%1&quot; nevü ág már létezik</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="242"/>
        <source>Checking if branch with name &quot;%1&quot; can be added</source>
        <translation>Annak ellenörzése, hogy egy &quot;%1&quot; nevü ágat létre lehet-e hozni</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="256"/>
        <location filename="../core/root/NTupleMgr_root.cxx" line="277"/>
        <source>Failed creating branch with name: %1</source>
        <translation>Nem sikerült a(z) &quot;%1&quot; nevü ágat létrehozni</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="263"/>
        <source>Creating primitive branch with name &quot;%1&quot; and leaf-list &quot;%2&quot;</source>
        <translation>A(z) &quot;%1&quot; primitív ág létrehozása &quot;%2&quot; levél-listával</translation>
    </message>
    <message>
        <location filename="../core/root/NTupleMgr_root.cxx" line="284"/>
        <source>Creating object branch with name &quot;%1&quot; and classname &quot;%2&quot;</source>
        <translation>A(z) &quot;%1&quot; objektum ág létrehozása &quot;%2&quot; osztály-névvel</translation>
    </message>
</context>
<context>
    <name>vme::VmeBus</name>
    <message>
        <location filename="../core/vme/VmeBus.cxx" line="34"/>
        <source>Creating object</source>
        <translation>Objektum létrehozása</translation>
    </message>
    <message>
        <location filename="../core/vme/VmeBus.cxx" line="64"/>
        <source>The VME bus is already open</source>
        <translation>A VME busz már meg van nyitva</translation>
    </message>
    <message>
        <location filename="../core/vme/VmeBus.cxx" line="74"/>
        <source>The PCIVME driver could not be opened</source>
        <translation>A PCIVME meghajtót nem lehetett megnyitni</translation>
    </message>
    <message>
        <location filename="../core/vme/VmeBus.cxx" line="102"/>
        <source>Failed to close the PCIVME driver</source>
        <translation>Nem sikerült lezárni a PCIVME meghajtót</translation>
    </message>
    <message>
        <location filename="../core/vme/VmeBus.cxx" line="142"/>
        <location filename="../core/vme/VmeBus.cxx" line="167"/>
        <location filename="../core/vme/VmeBus.cxx" line="192"/>
        <location filename="../core/vme/VmeBus.cxx" line="220"/>
        <location filename="../core/vme/VmeBus.cxx" line="245"/>
        <location filename="../core/vme/VmeBus.cxx" line="271"/>
        <location filename="../core/vme/VmeBus.cxx" line="297"/>
        <location filename="../core/vme/VmeBus.cxx" line="336"/>
        <source>VME bus not open yet</source>
        <translation>A VME busz nincsen még megnyitva</translation>
    </message>
    <message>
        <location filename="../core/vme/VmeBus.cxx" line="149"/>
        <source>8bit write &gt; Address: 0x%1; Data: %2</source>
        <translation>8 bites írás &gt; Cím: 0x%1, Adat: %2</translation>
    </message>
    <message>
        <location filename="../core/vme/VmeBus.cxx" line="174"/>
        <source>16bit write &gt; Address: 0x%1; Data: %2</source>
        <translation>16 bites írás &gt; Cím: 0x%1, Adat: %2</translation>
    </message>
    <message>
        <location filename="../core/vme/VmeBus.cxx" line="199"/>
        <source>32bit write &gt; Address: 0x%1; Data: %2</source>
        <translation>32 bites írás &gt; Cím: 0x%1, Adat: %2</translation>
    </message>
    <message>
        <location filename="../core/vme/VmeBus.cxx" line="227"/>
        <source>Unknown length write &gt; Address: 0x%1; length: %3</source>
        <translation>Ismeretlen hosszúságú írás &gt; Cím: 0x%1, hossz: %3</translation>
    </message>
    <message>
        <location filename="../core/vme/VmeBus.cxx" line="252"/>
        <source>8-bit read request from address: %1</source>
        <translation>8 bites olvasás a %1 címröl</translation>
    </message>
    <message>
        <location filename="../core/vme/VmeBus.cxx" line="278"/>
        <source>16-bit read request from address: %1</source>
        <translation>16 bites olvasás a %1 címröl</translation>
    </message>
    <message>
        <location filename="../core/vme/VmeBus.cxx" line="304"/>
        <source>32-bit read request from address: %1</source>
        <translation>32 bites olvasás a %1 címröl</translation>
    </message>
    <message>
        <location filename="../core/vme/VmeBus.cxx" line="343"/>
        <source>Unknown size read request from Address: %1; length: %2</source>
        <translation>Ismeretlen hosszúságú olvasás a %1 címröl, %2 hosszal</translation>
    </message>
</context>
</TS>

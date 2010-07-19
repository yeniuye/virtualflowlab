/*

TODO LIST (In Turkish)

- Giris-cikis seklinde bir akis cozulurken cikista basinc sabitlenmedigi icin basincsurekli dusuyor Basinc ve kutle residual'lari
  sabitleniyor, dusmuyor.

- Visualization yapmak icin acilan DAT dosyasinin zaten acik olan DAT dosyasi ile ayni oldugunun kontrolu yapilmali. En basitinden dosya
  isimlerine bakilabilir.

- DAT dosyasinin (sonuc dosyasi) okunup okunmadigini gosteren bir degiskene ihtiyac var. isDATread gibi birsey.

- DAT dosyalari isTecplot parametresi TRUE ise yaratiliyor. Ama bu DAT dosyalari VFL ile post processing yaparken de kullaniliyor.
  O zaman bu degiskenin adi degismeli, createDAT gibi birsey olmali.

- Yukaridakine bagli olarak DAT dosyalari zaten OUT dosyalarindaki tum bilgileri icermiyor mu? O zaman OUT'lar hic yaratilmasin
  ve DAT'lar restart icin kullanilabilir olsun. Boyle olursa DAT'lar yaratilsin mi diye de sorulmaz ve hep yaratilir, is basitlesir.

- Convergence ve control point grafikleri icin alinan screenshot'larda eksenler gorunmuyor.

- Problem isminde bosluk olunca calismiyor. Problem isimlerinde bosluk karakterine izin verilip verilmeyecegini dusun. Bosluk
  karakterli bir isim verilecekse boslugu kullaniciya hic sormadan kadirmak mi gerek?

- isMeshGenerated degiskeninin kullanimi sorunlu. Bir kere TRUE olduktan sonra hic FALSE yapilmiyor.

- Kontrol noktasi ve blocked cell sayisi 0 olunca deallocation problemi oluyor. Bu yuzden onlarin deallocation'lari
  tamamen iptal edildi. Buna care bul.

- Visualize sayfasindaki dugmelerin autoExclusive ozelligi tam calismiyor. Hicbiri secili hale gelebiliyor. Mesela geometry sayfasindakiler
  gibi degiller. Bu mainWindow::visualizeButtonsClicked() fonksiyonun sonundaki else {} ile halledildi.

- Eleman sayisi cok olan aglarda pan yaparken atlamalar oluyor. Eleman sayisina bakarak belli bir degerden cok oldugunda, fareyi kac
  piksel oynattigimizda pan yapilacak bilgisini degistirebiliriz.

- Bir problem acildiginda onunla ilgili bilgiler ver. Mesela su kadar item/blocked cell/kontrol noktasi var, ag yaratildi/yaratilmadi.
  Ag yaratildi ise su kadar eleman/nokta var, vesaire...

- Bir cozume baslayinca Mesaj penceresine baslama gunu ve zamani gibi detaylar yazilmali. Bitince de benzer data yazilmali. Ne kadar
  surdugu de yazilmali. Bu tip bilgiler Tecplot dosyalarina da konmali. Sonra okurken o satirlar gozardi edilir. Ama Tecplot sasirabilir.

- scheme, strategy ve discretization parametreleri icin secenekler arayuzde sifirdan basliyor, ama cozucude 1'den basliyor. O yuzden
  su anda .inp dosyasi yazdirilirken 1'er artiliyorlar, ama esasinda esitlenmeliler.

- Kismen birbirinin ustunde olan nesneleri secerken alttakinin rengi degisiyor, ama ustundekinden dolayi bu degisiklik gorunmuyor.
  Birden fazla nesneyi secme imkani getirmek gerekebilir. Periodic, symmetric BC icin de gerekli bu.

- delete ve deleteAll tuslarina basinca olup biteni toparlamak gerek, fazla daginik.

- Initial Conditions bilgisi hicbir degiskende tutulmuyor. Textbox'tan okunup, textbox'a yaziliyor. Bu multiblock olunca calismaz.

- Programdan cikarken kaydedip etmemek ile ilgili Scribble ornegine bak. Kolay cozum olarak her cikista kaydedip kaydetmemeyi sor.

- Bir hata ve uyari mesaji yazilinca Messages/Output sayfasi aktif yapilmali.

- Arayuzde ve cozucude kullanilan parametre isimlerinden bazilari farkli. Ayni yapilmali.

- .cfd dosyasi yazdirilirken en basa hangi state'de kalindigi da yazilmali. Mesela "sadece primitive'ler yaratildi", veya
  "Block/face setting'ler yapildi", veya "mesh yaratildi". Uzerinde dusunmek gerek.

- Her solver parametresinin ontanimli degeri olmali mi? Mesela ilgili kutucuga time step girilmez ise, cfd ve inp dosyasi yaratilirken
  yazilacak bir deger olmuyor. cfd dosyasi okunurken ilgili kutucuk bosluk kaliyor, sorun olmuyor, ama inp dosyasi okunurken cozucu
  sacma sapan bir time step degeri okuyor. ReadWriteCfdFile.cpp'de hic kutucuktan okuyup kutucuga yazdirma olmamali mi?

- getBCstring() fonksiyonu pointer return etmek yerine iki parametre alsin ve tek bir string return etsin.

- Face'ler icin S, E, N, W gibi karakterler yerine hep integer kullanmak isleri cok kolaylastiracak.

- Pan, zoom gibi olaylar icin uzun uzun IF'ler kullanmak yerine GUI'de cesitli sayfalara gecildiginde enablePan, enableZoom gibi
  bool'larla oynayarak bu is halledilebilir.

- Arayuzden float olarak okunan degiskenler cozucuye double olarak gidiyor. Bunlar ayni yapilmali. Ornek: density, kinvis, koordinatlar

- Fazla zoom out yapilinca sacmaliyordu, yeni bir zoom fonksiyonu yazildi. Bunun da response'u biraz garip oldu,
  daha gelistirilebilir.

- Blocks/Faces'daki Set dugmesine basmadan once herseyin kabul edilebilir olup olmadigini kontrol et.
  Radio button'lardan biri secili olmali, node sayisi integer ve en az 2 olmali. Boyle olunca node
  sayisi ontanimli olarak sifir gozukmez, iyi olur.
  
- readCfdFile'da okunmaya calisilan dosyanin ilk satirinda yazilanlara bakarak bunun dogru dosya olup olmadiginin basitce kontrolunu yap.
  Bunu yapacaksak cfd dosyalarinin ilk satirina "Do not modify this line!" diye bir uyari da koymamiz gerek.

- Designer ile uretilen formlara mainWindowBase gibi isimler verilirse daha iyi olur.

- "... Block/Face settings are not correct" mesaji mumkun oldugunca acik hale getirilmeli.

- calculateMinMaxXY fonksiyonunu elden gecir. Su anda kullanilmiyor.

- glMouseState glwidget'e mi ait olmali? Hep orada kullaniliyor.

- TFI'dan baska mesh yaratma algoritmasi yazilinca min iter, max iter, tolerance ve weight factor icin degisken yarat.
  Bunlari da cfd dosyasina yaz ve oradan oku.

- Yaratilan agdaki noktalar tek tek fare ile tutulup cekilebilmeli.

- Her primitive eklendiginde showAll() yapiyor. Acaba sadece ilk primitive'de mi yapsa bunu? Sonrakiler sasirtici oluyor.

- Blocks/Faces ve BC/IC sayfalarindakileri sadece bir primitive secili oldugunda aktif hale getir.

- Ag yaratildiktan sonra "Su kadar elemanli, su kadar noktali bir ag yaratildi" gibi bir uyari verilebilir.
  Birden fazla blok varsa bu mesaj her blok icin ayri ayri verilir.

- Solver parametrelerine "inner iteration" la ilgili birseyler girilebilir. Mesela sabit bir inner iteration sayisi.

- Bazi degiskenlere float yetmeyebilir. Bunlar double yapilmali.

- Butun dosya acma kapama islemlerinde hata kontrolu yapilmasi gerek.

- openGL penceresi icin klavye kisayollari konabilir. Mesela +/- zoom yapar. P'ye basinca pan secilmis olur, vs.
  Mandelbrot ornegindeki keyPressEvent'e bakilabilir.

- mainWindow fonksiyonlarinin public/private olma durumlarini incele.

- setupLineEditValidators()'in eksiklerini tamamla.

- .cfd dosyasindaki MESH ve MESHPOINT kisimlari birlestirilebilir.



NOTES

- To determine which OS the code is running on

  filter = "*"              // Unix  
  #ifdef Q_OS_WIN32         // Windows
    filter = "*.exe"
  #elseif Q_OS_MACX         // MAC
    filter = "*.something"
  #endif 

*/

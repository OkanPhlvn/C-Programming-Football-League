#include <stdlib.h>
#include <stdio.h>
#include <locale.h>

#define TAKIMADSINIR 25


/* Takim sayisi , galibiyet sayisi , beraberlik sayisi , maglubiyet sayisi , mac_sayisi struct yapisinda tanimlanmistir ve dosyadan okunmustur.  */
struct ayarlar
{
	int takim_sayisi;
	int galibiyet;
	int beraberlik;
	int maglubiyet;
	int mac_sayisi;	
}ayar;

/* Takimlar struct yapisi ile tanimlandi. */
struct takimlar
{
	char takim_ad[TAKIMADSINIR];
	char takma_kisaAd;
	int puan;
	int oynadigimacsayisi;
	int galibiyetsayisi;
	int beraberliksayisi;
	int maglubiyetsayisi;
	int attigigolsayisi;
	int yedigigolsayisi;
	int averaj;

}tkm[10]={
	{"",'A',0,0,0,0,0,0,0,0},
	{"",'B',0,0,0,0,0,0,0,0},
	{"",'C',0,0,0,0,0,0,0,0},
	{"",'D',0,0,0,0,0,0,0,0},
	{"",'E',0,0,0,0,0,0,0,0},
	{"",'F',0,0,0,0,0,0,0,0},
	{"",'G',0,0,0,0,0,0,0,0},
	{"",'H',0,0,0,0,0,0,0,0},
	{"",'I',0,0,0,0,0,0,0,0},
	{"",'J',0,0,0,0,0,0,0,0},
},temp;  /* temp degiskeni takim struct icerisine kaydedildi. */


/* Maclar struct yapisi ile tanimlandi. */
struct mac
{
	char evsahibi;
	char misafir;
	int evSkor;
	int misSkor;
}maclar[100];


/* Takimlarin uzun ismi dosyadan okundu */
void TakimGirisi()
{
	FILE *file;

	if((file=fopen("takimlar.txt", "r")) == NULL){
		printf("Dosya acilamadi!\n");
      	exit(1);
	}

	int i = 0;
	while(!feof(file))
	{
		fscanf(file, "%s", &tkm[i].takim_ad);
		//printf("Takim %d: %s\n",i+1, tkm[i].takim_ad);
		i++;
	}

	fclose(file);
}


/* Takim ,Galibiyet ,Beraberlik ,Maglubiyet degerleri ayarlar.txt dosyasindan okundu */
void AyarGirisi(){

	FILE *file;

	if((file=fopen("ayarlar.txt", "r")) == NULL){
		printf("Dosya acilamadi!\n");
      	exit(1);
	}

	fscanf(file, "%d %d %d %d", &ayar.takim_sayisi, &ayar.galibiyet, &ayar.beraberlik, &ayar.maglubiyet);

	fclose(file);
}


/* Struct yapisinda Takimlarin kisa adlarinin sayisal degerini bulmak icin for dongusu ile icerisinde gezindik ve her takimin numarasini eslestirdik. */
int TakimBul(char takim_Char){
	int i;
	for(i=0;i<ayar.takim_sayisi;i++){
		if(takim_Char == tkm[i].takma_kisaAd){
			return i;
		}
	}
	return -1;
}


/* Ev sahibi ve misafir takim bir kez daha mac yapar ise uyari veren fonksiyon tanimlandi. */
int MacKontrol(char takim1, int skor1, char takim2, int skor2){
	int i;		    
	for(i=0;i<ayar.mac_sayisi;i++){
		if(maclar[i].evsahibi == takim1 & maclar[i].misafir == takim2){
			return 0;  /* False durumu */
		}
	}
	maclar[ayar.mac_sayisi].evsahibi = takim1;
	maclar[ayar.mac_sayisi].misafir = takim2;
	maclar[ayar.mac_sayisi].evSkor = skor1;
	maclar[ayar.mac_sayisi].misSkor = skor2;
	ayar.mac_sayisi++;
	return 1;   /* True durumu */
}

/* Takimlar bu fonksiyonda tanimlandiktan sonra ilgili hesaplamalar yapildi. */
void TekMacGirisi(char takim1, int skor1, char takim2, int skor2){ // A 3 B 0
	int takim1Index = TakimBul(takim1); /* Girilen ev sahibi takimin sayisal degerini bulmak icin TakimBul fonksiyonuna yolladik. */
	int takim2Index = TakimBul(takim2); /* Girilen misafir takimin sayisal degerini bulmak icin TakimBul fonksiyonuna yolladik. */
	int i;

	/* Mevcut olmayan takim girisi yapildiginda uyari verilir. */
	if(takim1Index == -1 | takim2Index == -1){
		printf("Takim Kisa Ad Girisi Hatali!\n");
		return;
	}

	/* MacKontrol fonksiyonuna mac girisini yolladik daha onceden ayni ev sahibinde , ayni macin oynanip oynanmadigi kontrolu yapildi. */
	if(MacKontrol(takim1, skor1, takim2, skor2) == 0){
		printf("Bu mac daha onceden eklenmis!: Takim 1: %s - Takim 2: %s\n",tkm[takim1Index].takim_ad,tkm[takim2Index].takim_ad);
		return;
	}
	
	/* Puan tablosundaki degerlerin hesaplanmalari yapildi */
	int averaji;
	if(skor1>skor2)
	{
		averaji = skor1-skor2; /* Maçların averajlarının hesaplanması */
		tkm[takim1Index].puan +=ayar.galibiyet; /* Kazanan takıma galibiyet puanı eklendi */
		tkm[takim2Index].puan +=ayar.maglubiyet; /* Kaybeden takıma mağlubiyet puanï¿½ eklendi */
		tkm[takim1Index].averaj += averaji; /* Kazanan takıma averaj puanı eklendi */
		tkm[takim2Index].averaj -= averaji; /* Kaybeden takımdan averaj puanı çıkartıldı */
		tkm[takim1Index].oynadigimacsayisi++; /* Ev sahibi takımın oynadığı maç sayısı arttırıldı */
		tkm[takim2Index].oynadigimacsayisi++; /*Misafir takımın oynadığı maç sayısı arttırıldı */
		tkm[takim1Index].galibiyetsayisi++; /* Ev Sahibi takımın galibiyet sayısı arttırıldı*/
		tkm[takim2Index].maglubiyetsayisi++; /* Misafir takımın mağlubiyet sayısı arttırıldı */
		tkm[takim1Index].attigigolsayisi += skor1; /* Ev sahibi takımın attığı gol sayısı*/
		tkm[takim2Index].attigigolsayisi += skor2; /* Misafir takımın attığı gol sayısı */
		tkm[takim1Index].yedigigolsayisi += skor2; /* Ev sahibi takımın yediği gol sayısı*/
		tkm[takim2Index].yedigigolsayisi += skor1;	/* Misafir takımın yediği gol sayısı */

	}
	else if (skor2>skor1)
	{
		averaji = skor2-skor1;  /* Maçların averajlarının hesaplanması */
		tkm[takim2Index].puan +=ayar.galibiyet; /* Kazanan takï¿½ma galibiyet puanï¿½ eklendi */
		tkm[takim1Index].puan +=ayar.maglubiyet; /* Kaybeden takï¿½ma maï¿½lubiyet puanï¿½ eklendi */
		tkm[takim2Index].averaj += averaji;  /* Kazanan takï¿½ma averaj puanï¿½ eklendi */
		tkm[takim1Index].averaj -=averaji;  /* Kaybeden takï¿½mdan averaj puanï¿½ ï¿½ï¿½kartï¿½ldï¿½ */
		tkm[takim2Index].oynadigimacsayisi++; /* Ev sahibi takï¿½mï¿½n oynadï¿½ï¿½ï¿½ maï¿½ sayï¿½sï¿½ arttï¿½rï¿½ldï¿½ */
		tkm[takim1Index].oynadigimacsayisi++; /*Misafir takï¿½mï¿½n oynadï¿½ï¿½ï¿½ maï¿½ sayï¿½sï¿½ arttï¿½rï¿½ldï¿½ */
		tkm[takim2Index].galibiyetsayisi++; /* Misafir takï¿½mï¿½n galibiyet sayï¿½sï¿½ arttï¿½rï¿½ldï¿½ */
		tkm[takim1Index].maglubiyetsayisi++; /* Ev sahibi takï¿½mï¿½n maï¿½lubiyet sayï¿½sï¿½ arttï¿½rï¿½ldï¿½ */
		tkm[takim2Index].attigigolsayisi += skor2; /* Misafir takï¿½mï¿½n attï¿½ï¿½ï¿½ gol sayï¿½sï¿½ */
		tkm[takim1Index].attigigolsayisi += skor1; /* Ev sahibi takï¿½mï¿½n attï¿½ï¿½ï¿½ gol sayï¿½sï¿½ */
		tkm[takim2Index].yedigigolsayisi += skor1; /* Misafir takï¿½mï¿½n yediï¿½i gol sayï¿½sï¿½ */
		tkm[takim1Index].yedigigolsayisi += skor2;	/* Ev sahibi takï¿½mï¿½n yediï¿½i gol sayï¿½sï¿½ */
	}
	else if(skor1 == skor2)
	{
		tkm[takim1Index].puan += ayar.beraberlik; /* Ev sahibi takï¿½ma beraberlik puanï¿½ eklendi */
		tkm[takim2Index].puan += ayar.beraberlik; /* Misafir takï¿½ma beraberlik puanï¿½ eklendi */
		tkm[takim1Index].oynadigimacsayisi++; /* Ev sahibi takï¿½mï¿½n oynadï¿½ï¿½ï¿½ maï¿½ sayï¿½sï¿½ arttï¿½rï¿½ldï¿½ */
		tkm[takim2Index].oynadigimacsayisi++; /* Misafir takï¿½mï¿½n oynadï¿½ï¿½ï¿½ maï¿½ sayï¿½sï¿½ arttï¿½rï¿½ldï¿½ */
		tkm[takim1Index].beraberliksayisi++;  /* Ev sahibi takï¿½mï¿½n beraberlik sayï¿½sï¿½ arttï¿½rï¿½ldï¿½ */
		tkm[takim2Index].beraberliksayisi++;  /* Misafir takï¿½mï¿½n beraberlik sayï¿½sï¿½ arttï¿½rï¿½ldï¿½ */
		tkm[takim1Index].attigigolsayisi += skor1; /* Ev sahibi takï¿½mï¿½n attï¿½ï¿½ï¿½ gol sayï¿½sï¿½ arttï¿½rï¿½ldï¿½ */
		tkm[takim2Index].attigigolsayisi += skor2; /* Misafir takï¿½mï¿½n attï¿½ï¿½ï¿½ gol sayï¿½sï¿½ arttï¿½rï¿½ldï¿½ */
		tkm[takim1Index].yedigigolsayisi += skor2; /* Ev sahibi takï¿½mï¿½n yedigi gol sayï¿½sï¿½ arttï¿½rï¿½dlï¿½ */
		tkm[takim2Index].yedigigolsayisi += skor1; /* Misafir takï¿½mï¿½n yediï¿½i gol sayï¿½sï¿½ arttï¿½rï¿½ldï¿½ */
	}

	printf("Bilgi Mesaji: %s %d - %d %s Verisi Kaydedildi.\n",tkm[takim1Index].takim_ad, skor1, skor2, tkm[takim2Index].takim_ad);

}

/* Ev sahibi takim ve skoru , Misafir takim ve skoru klavyeden elle girilmesi */
void MacGirisleriElle()
{
	int i;
	int macsayisi;
	int skor1;
	int skor2;
	char takim1,takim2;

	printf("Yapilacak mac sayisini giriniz: "); scanf("%d",&macsayisi);

	for(i=0; i<macsayisi; i++)
	{
		printf("%d. Mac skoru: ", i+1); scanf(" %c %d %c %d",&takim1,&skor1,&takim2,&skor2);
		TekMacGirisi(takim1, skor1, takim2, skor2); /* Mac degerleri TekMacGirisi fonksiyonuna aktarildi. */
	}

}

/* Bir yazinin buyuk harflere cevrilmesi */
char* buyukYaz(char* yazi){
	
	char* yazi2;
	int i = 0;
	int offset = 'a' - 'A';
	 
	while(yazi[i] != '\0')
    {
    	if(yazi[i] >= 'a' || yazi[i] < 0)
    	{
    		yazi2[i] = yazi[i] - offset;
		}
		i++;
	}
	
	return yazi2;
}


/* Puan tablosu puantablosu.txt dosyasina yazildi. */
void DosyayaPuanTablosuYaz()
{
	/*puantablosu.txt dosyasi olusturulup yeni bir dosya acildi ve icerisine puan tablosunun guncellenmis hali geldi.*/
	FILE *file;
	int i;
	
	if ((file = fopen ("puantablosu.txt", "w")) == NULL) 
	{
   		printf("Dosya acma hatasi!\n");
      	exit(1);
  	}
  	
	fprintf(file ," |KisaAd| |Ad| |OM| |G| |B| |M| |AG| |YG| |A|  |P|\n");
	
  	for(i=0; i<ayar.takim_sayisi; i++)
  	{
  		fprintf(file , "%c  %s  %d   %d   %d   %d    %d      %d      %d      %d\n",tkm[i].takma_kisaAd,tkm[i].takim_ad,tkm[i].oynadigimacsayisi,
		  tkm[i].galibiyetsayisi,tkm[i].beraberliksayisi,	tkm[i].maglubiyetsayisi,tkm[i].attigigolsayisi,tkm[i].yedigigolsayisi,tkm[i].averaj,tkm[i].puan);
  		
	}
	fclose(file);
}


/* Mac sonuclari dosyadan okunup puan tablosuna yansitildi. */
void MacGirisleriOku(){

	char takim1, takim2;
	int skor1, skor2;
		
	FILE *file;
	
	if((file=fopen("maclar1.txt", "r")) == NULL){
		printf("Dosya aï¿½ï¿½lamadï¿½!\n");
      	exit(1);
	}

	while(!feof(file))
	{
		fscanf(file, " %c %d %c %d", &takim1,&skor1,&takim2,&skor2);
		TekMacGirisi(takim1,skor1,takim2,skor2);
	}

	fclose(file);
}


/* Puana sirasina gore takimlarin siralamasi yapildi */
void PuanSirala(){

	/* Bubble sort siralama algoritmasi kullanildi */
	char takim_kisaAd[20] = "|TakimKisaAd|";
	char takim_adi[15] = "|Takim adi|";
	int i;
	int j;
		
		for(i=0;i<ayar.takim_sayisi-1;i++){
		 for(j=0;j<ayar.takim_sayisi-i-1;j++){
				if(tkm[j].puan<tkm[j+1].puan || tkm[j].puan==tkm[j+1].puan && tkm[j].averaj<tkm[j+1].averaj) /* Puanlar esit ise averaji yuksek olan takim ust siraya yukselir */
				{
				 	temp = tkm[j+1];  /* NOT : temp degiskeni struct yapisinda tanimlandi. */
				 	tkm[j+1] = tkm[j];
				 	tkm[j] = temp;
				} 
			}
		}
		 
	/* Takimlarin puan sirasina gore siralanmis hali puan tablosuna yazildi. */
 	printf("%15s %15s |OM|  |G| |B| |M| |AG| |YG| |A| |Puan|\n",takim_kisaAd, takim_adi);
 	
	for(i=0;i<ayar.takim_sayisi;i++)
	{
		printf("%13c  %15s   %d     %d   %d   %d   %d     %d   %d    %d\n",tkm[i].takma_kisaAd,tkm[i].takim_ad,tkm[i].oynadigimacsayisi,tkm[i].galibiyetsayisi,tkm[i].beraberliksayisi,
		tkm[i].maglubiyetsayisi,tkm[i].attigigolsayisi,tkm[i].yedigigolsayisi,tkm[i].averaj,tkm[i].puan);
	}

}

/* Takimlarin uzun isimleri alfabetik olarak siralandi. */
void HarfSirasi()
{
	char takim_kisaAd[20] = "|TakimKisaAd|";
	char takim_adi[15] = "|Takim adi|";
	int i;
	int j;
	int k;
		
		/* Bubble sort siralama algoritmasi kullanildi */
		 for(i=0;i<ayar.takim_sayisi;i++){
		 for(j=i+1;j<ayar.takim_sayisi;j++){
				 for(k=0; k<TAKIMADSINIR; k++){ 
				 	if(tkm[i].takim_ad[k] > tkm[j].takim_ad[k])   /* Takimlarin uzun isimlerinde gezinip harflerin ASCII degerlerine gore buyukluk kucukluk kontrolu yapildi. */ 
					 {
					 	temp = tkm[j];  /* NOT : temp degiskeni struct yapisinda tanimlandi. */
					 	tkm[j] = tkm[i];
					 	tkm[i] = temp;
					 	break;   /*  */
					 }
					 else if(tkm[i].takim_ad[k] < tkm[j].takim_ad[k]) break;  /* Deger kucuk ise zaten siralanmistir bu durumda break komutu gonderildi. */
				 }
			}
      
		 }		 
	
	/* Takimlarin alfabetik sirasina gore siralanmis hali puan tablosuna yazildi. */
 	printf("%15s %15s |OM|  |G| |B| |M| |AG| |YG| |A| |Puan|\n",takim_kisaAd, takim_adi);
 	
	for(i=0;i<ayar.takim_sayisi;i++)
	{
		printf("%13c  %15s   %d     %d   %d   %d   %d     %d   %d    %d\n",tkm[i].takma_kisaAd,tkm[i].takim_ad,tkm[i].oynadigimacsayisi,tkm[i].galibiyetsayisi,tkm[i].beraberliksayisi,
		tkm[i].maglubiyetsayisi,tkm[i].attigigolsayisi,tkm[i].yedigigolsayisi,tkm[i].averaj,tkm[i].puan);
	}
		
}

/* Puan tablosunun guncellenmis hali tabloya yansitildi. */
void PuanTablosuGoster(){

	char takim_kisaAd[20] = "|TakimKisaAd|";
	char takim_adi[15] = "|Takim adi|";
	
	printf("%15s %15s |OM|  |G| |B| |M| |AG| |YG| |A| |Puan|\n",takim_kisaAd, takim_adi);
	int i;
	for(i=0; i<ayar.takim_sayisi; i++)
	{	
		printf("%13c  %15s   %d     %d   %d   %d   %d     %d   %d    %d\n",tkm[i].takma_kisaAd,tkm[i].takim_ad,tkm[i].oynadigimacsayisi,tkm[i].galibiyetsayisi,tkm[i].beraberliksayisi,
		tkm[i].maglubiyetsayisi,tkm[i].attigigolsayisi,tkm[i].yedigigolsayisi,tkm[i].averaj,tkm[i].puan);
	}
}


/* Menuden yapilacak islemi secmek icin switch-case yapisi kullanildi */
void Menu(){

	 int secim;
	 while(secim!=8)
	 {
	 	printf("------------------------\n");
 		printf("[1] Skor Gir\n");
		printf("[2] Skor Dosyadan Oku\n");
 		printf("[3] Puan Durumu Tablosunu Takma isime Gore Sirala\n");
 		printf("[4] Puan Durumu Tablosunu Puan Sirasina Gore Sirala\n");
 		printf("[5] Puan Durumu Tablosunu Dosyaya Yaz\n");
 		printf("[6] Puan Durumu Tablosunu Harf Sirasina Gore Sirala Yaz\n");
 		printf("[7] Puan Durumu Tablosunu Buyuk Harflerle Yaz\n");
 		printf("[8] Programi kapat\n\n");
   	 	printf("Islemi seciniz: "); scanf("%d",&secim);

   	 	switch(secim)
   		{
   			case 1:
			   	printf("1. islemi sectiniz \n") ;
			   	MacGirisleriElle();
				break;

   			case 2:
			   printf("2. islemi sectiniz \n");
			   MacGirisleriOku();
			   break;

   			case 3:
			   printf("3. islemi sectiniz \n");
			   PuanTablosuGoster();
			   break;

   			case 4:
			   printf("4. islemi sectiniz \n");
			   PuanSirala();
			   break;
			   
			case 5:
			   printf("5. islemi sectiniz \n");
			   DosyayaPuanTablosuYaz();
			   break;
			   
			case 6:
			   printf("6. islemi sectiniz \n");
			   HarfSirasi();
			   break;
			
			// Takimlarin uzun adlari cevirilmedi fakat kucuk hafli yazilan kelimenin buyuk harflere cevirilmesi icin algoritmasi anlasildi ve yapildi.
			case 7:
			   printf("7. islemi sectiniz \n");
			   char yazi[11] = "asidasd";
			   printf(buyukYaz(yazi));
			   break;

   			case 8:
   			   printf("\n******EN BUYUK TRABZONSPOR****** \n");
			   exit(1);
			   break;
			
			default:
			   printf("\n******EN BUYUK TRABZONSPOR****** \n");
			   exit(1);
			   break;		   
		}
	}
}


int main()
{
	setlocale(LC_ALL, "Turkish"); // Turkce karakter tanimlamasi yapildi.

	ayar.mac_sayisi = 0;	
	AyarGirisi();
	TakimGirisi();
	Menu();

	return 0;
}

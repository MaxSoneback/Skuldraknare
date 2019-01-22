/*
================================================================================
Datorer och programmering, DoP, 10 hsp, termin/år:ST18
 
Inlämningsuppgift nr 6b
 
Namn: Max Sonebäck
 
Personummer: 9302205472
 
Fyll i (alt stryk det som ej är relevant) av nedanstående:
 
Den fil som jag lämnar in går att kompilera och
programmet fungerar för alla de testdata som finns för uppgiften: Ja
 
Jag har använt kompilator/editor (namn/version) Netbeans 8.2 med kompilator Cygwin 2.10.0
 
Jag har använt följande dator (PC/Mac/Annat): PC
      med operativsystemet: Windows
 
Jag har arbetat ungefär  5 timmar med denna uppgift
================================================================================
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <cmath>
#include <math.h>
#include <valarray>

using namespace std;

class Person {
private:
    string namn;
    double betalat_andras; // ligger ute med totalt
    double skyldig; // skyldig totalt

public:
    Person();
    Person(string n, double b, double s);
    Person& operator=(const Person& p);
    double haemta_betalat();
    double haemta_skyldig();
    string haemta_namn();
    double raekna_differans(double a, double b);
    void skrivUt();
};

class PersonLista {
private:
    int antal_pers;
    Person *pers;

public:
    PersonLista();
    PersonLista(const PersonLista& pl);
    ~PersonLista();
    PersonLista& operator=(const PersonLista& pl);
    void laggTillEn(Person pny);
    void skrivUtOchFixa();
    double summaSkyldig();
    double summaBetalat();
    bool finnsPerson(const string& namn);
};

class Transaktion {
private:
    string datum;
    string typ;
    string namn;
    double belopp;
    int ant_kompisar;
    string *kompisar;

public:
    Transaktion();
    ~Transaktion();
    Transaktion& operator=(const Transaktion& t);
    string haemta_namn();
    double haemta_belopp();
    int haemta_ant_kompisar();
    bool finnsKompis(string namnet);
    bool laesEnTrans(istream &is);
    void skrivEnTrans(ostream &os);
};

class TransaktionsLista {
private:
    Transaktion *trans;
    int antalTrans;

public:
    TransaktionsLista();
    ~TransaktionsLista();
    void laesin(istream &is);
    void skrivut(ostream &os);
    void laggTill(Transaktion &t);
    double totalkostnad();
    double liggerUteMed(string namnet);
    double aerSkyldig(string namnet);
    PersonLista FixaPersoner();
};


int meny();

int main() {
    int val;
    TransaktionsLista minLista;

    ifstream fin("resa_in.dat");
    ofstream fout("resa_in.dat", ios::app);
    minLista.laesin(fin);
    //minLista.FixaPersoner();
    val = meny();
    string namn;

    while (val != 0) {
        switch (val) {
            case 0:
                fin.close();
                fout.close();
                exit(EXIT_SUCCESS);
                break;

            case 1:
                minLista.laesin(cin);
                minLista.skrivut(fout);


                break;

            case 2:
                minLista.laesin(fin);
                minLista.skrivut(cout);

                break;

            case 3:

                cout << "Den totala kostnaden är: " << minLista.totalkostnad() << endl;

                break;

            case 4:
                cout << "Ange vems skuld du vill få reda på: " << endl;
                cin >> namn;

                cout << namn << " har en skuld på " << minLista.aerSkyldig(namn) << endl;

                break;

            case 5:

                cout << "Ange namnet på någon för att se vad denne ligger ute med: " << endl;
                cin >> namn;

                cout << namn << " ligger ute med " << minLista.liggerUteMed(namn) << endl;

                break;

            case 6:
                minLista.FixaPersoner().skrivUtOchFixa();

                break;

        }
        val = meny();
    }

    return 0;
}
// <editor-fold desc="main-funktioner">

int meny() {
    int val = -1;
    do {
        cout << "Välj i menyn nedan" << endl;
        cout << "0) Avsluta. Alla transaktioner sparas på fil." << endl;
        cout << "1) Läs in en transaktion från tangentbordet." << endl;
        cout << "2) Skriv ut information om alla transaktioner." << endl;
        cout << "3) Beräkna totala kostnaden." << endl;
        cout << "4) Hur mycket är en viss person skyldig?" << endl;
        cout << "5) Hur mycket ligger en viss person ute med?" << endl;
        cout << "6) Lista alla personer mm och FIXA!!!" << endl;
        cin >> val;
    } while (val < 0 || val > 6);
    return val;
}




// <editor-fold desc="Person">
//-----------------------------------------------------------------------------
//Klassen Person
//-----------------------------------------------------------------------------
//Konstruktorer & Destruktorer, operator

Person::Person() {

}

Person::Person(string n, double b, double s) {
    namn = n;
    betalat_andras = b;
    skyldig = s;
}

Person& Person::operator=(const Person& p) {
    if (this != &p) {
        namn = p.namn;
        betalat_andras = p.betalat_andras;
        skyldig = p.skyldig;

    }
    return *this;
}

//-----------------------------------------------------------------------------
//Selektorer
//-----------------------------------------------------------------------------

double Person::haemta_betalat() {
    return betalat_andras;
}

double Person::haemta_skyldig() {
    return skyldig;
}

string Person::haemta_namn() {
    return namn;
}

//-----------------------------------------------------------------------------
//Metoder
//-----------------------------------------------------------------------------

void Person::skrivUt() {
    cout << namn << " ligger ute med " << betalat_andras << " kr och är skyldig " << skyldig << " kr. ";
    if (betalat_andras > skyldig) {
        cout << "Skall ha " << raekna_differans(betalat_andras, skyldig) << " kr från potten." << endl;
    } else {
        cout << "Skall betala " << raekna_differans(skyldig, betalat_andras) << " kr till potten." << endl;
    }
}

//Räknar ut differansen mellan skuld/utlagt

double Person::raekna_differans(double a, double b) {
    return (a - b);
}


//</editor-fold>

// <editor-fold desc="PersonLista">
//-----------------------------------------------------------------------------
//Klassen PersonLista
//-----------------------------------------------------------------------------
//Konstruktorer & Destruktorer, operator

PersonLista::PersonLista() {
    antal_pers = 0;
    pers = 0;

}

PersonLista::PersonLista(const PersonLista &pl) {
    antal_pers = pl.antal_pers;
    pers = new Person[antal_pers];

    for (int i = 0; i < antal_pers; i++) {
        pers[i] = pl.pers[i];
    }
}

PersonLista::~PersonLista() {
    delete[] pers;
}

PersonLista& PersonLista::operator=(const PersonLista& pl) {
    if (this != &pl) {
        delete[] pers;
        antal_pers = pl.antal_pers;
        pers = new Person[antal_pers];

        for (int i = 0; i < antal_pers; i++) {
            pers[i] = pl.pers[i];
        }

    }
    return *this;
}
//-----------------------------------------------------------------------------
//Metoder
//-----------------------------------------------------------------------------

//Lägger till en ny person sist i person-arrayen

void PersonLista::laggTillEn(Person pny) {

    Person *p = 0; // Lokal variabel.
    p = new Person[antal_pers + 1]; // Skapa array med (antal_pers + 1) elem.

    for (int i = 0; i < antal_pers; i++) {
        p[i] = pers[i];
    }
    p[antal_pers] = pny;

    delete [] pers; // Avallokera minne.
    pers = p; // Sätt pers till nya arrayen p.
    antal_pers++;

}

void PersonLista::skrivUtOchFixa() {
    for (int i = 0; i < antal_pers; i++) {
        pers[i].skrivUt();
    }

    double a = summaSkyldig();
    double b = summaBetalat();

    if (fabs(a - b) <= 0, 000001 * fabs(a)) {
        cout << " Skulder och utlägg går jämt upp" << endl;
    } else {
        cout << "Någonting gick snett..." << endl;
    }

}

//Räknar ut totalen på allas skuld, borde gå jämt ut med summaBetalt

double PersonLista::summaSkyldig() {
    double summaSkyldig = 0;

    for (int i = 0; i < antal_pers; i++) {
        summaSkyldig += pers[i].haemta_skyldig();
    }

    cout << "Skyldig: " << summaSkyldig << endl;
    return summaSkyldig;

}

//Räknar ut totalen på vad alla betalat in, borde gå jämt ut med summaSkyldig

double PersonLista::summaBetalat() {
    double summaBetalat = 0;

    for (int i = 0; i < antal_pers; i++) {
        summaBetalat += pers[i].haemta_betalat();
    }

    cout << "Betalat: " << summaBetalat << endl;
    return summaBetalat;

}

//Går igenom arrayen med alla personer och kollar efter ett visst namn, om namnet finns hos någon av personerna, returnera true

bool PersonLista::finnsPerson(const string& namn) {

    for (int i = 0; i < antal_pers; i++) {
        if (namn == pers[i].haemta_namn()) {
            return true;
        }
    }

    return false;
}


// </editor-fold>

// <editor-fold desc="Transaktion">
//-----------------------------------------------------------------------------
//Klassen Transaktion
//-----------------------------------------------------------------------------
//Konstruktorer & Destruktor, operator

Transaktion::Transaktion() {
    datum = "0";
    typ = "odef";
    namn = "";
    belopp = 0;
    ant_kompisar = 0;
    kompisar = 0;

}

Transaktion::~Transaktion() {
    delete[] kompisar;
}

Transaktion& Transaktion::operator=(const Transaktion& t) {
    if (this != &t) {
        delete[] kompisar;
        datum = t.datum;
        typ = t.typ;
        namn = t.namn;
        belopp = t.belopp;
        ant_kompisar = t.ant_kompisar;
        kompisar = new string[ant_kompisar];
        for (int i = 0; i < ant_kompisar; i++)
            kompisar[i] = t.kompisar[i];
    }
    return *this;
}
//-----------------------------------------------------------------------------
//Selektorer

string Transaktion::haemta_namn() {

    return namn;
}

double Transaktion::haemta_belopp() {

    return belopp;
}

int Transaktion::haemta_ant_kompisar() {

    return ant_kompisar;
}
//-----------------------------------------------------------------------------
//Metoder
//-----------------------------------------------------------------------------

//Letar igenom en array av kompisar och returnerar true om det givna namnet finns med

bool Transaktion::finnsKompis(string namnet) {

    for (int i = 0; i < ant_kompisar; i++) {

        if (namnet == kompisar[i]) {
            return true;
        }
    }

    return false;
}

//Läser in från en istream till instansvariablerna i en transaktion, returnerar true så länge eof inte är nådd

bool Transaktion::laesEnTrans(istream &is) {

    //cout << "Nu är jag i laesEnTrans" << endl;
    is >> datum >> typ >> namn >> belopp >> ant_kompisar;
    delete[] kompisar;
    kompisar = new string[ant_kompisar];
    for (int i = 0; i < ant_kompisar; i++) {

        is >> kompisar[i];
    }

    return !is.eof();

}

//Skriver över informationen om en transaktion ut på en ostream

void Transaktion::skrivEnTrans(ostream &os) {
    os << endl << datum << " " << typ << " " << namn << " " << belopp << " " << ant_kompisar << " ";
    for (int i = 0; i < ant_kompisar; i++) {
        os << kompisar[i] << " ";
    }
    os << "\n";

}
//-----------------------------------------------------------------------------
// </editor-fold>

// <editor-fold desc="TransaktionsLista">
//-----------------------------------------------------------------------------
//Klassen TransaktionsLista
//-----------------------------------------------------------------------------
//Konstruktorer & Destruktorer

TransaktionsLista::TransaktionsLista() {
    antalTrans = 0;
    trans = 0;
}

TransaktionsLista::~TransaktionsLista() {
    delete[] trans;
}
//-----------------------------------------------------------------------------
//Metoder
//-----------------------------------------------------------------------------

//Läser in en transaktion/lista av transaktioner från en istream

void TransaktionsLista::laesin(istream &is) {
    Transaktion enTrans;

    if (&is == &cin) { //Här kollar jag om jag läser från cin eller från fil

        cout << "Läs in datum, kategori, namn, belopp, antal personer, vilka personer" << endl;
        enTrans.laesEnTrans(is);
        laggTill(enTrans);
        //cout << " Kom jag hit?" << endl;
    } else {

        //cout << " hit?" << endl;
        while (enTrans.laesEnTrans(is)) {
            //cout << " Eller hit??" << endl;
            laggTill(enTrans);
        }
    }
}

//Skriver ut alla transaktioner på en ostream

void TransaktionsLista::skrivut(ostream &os) {
    //cout << "Nu är jag i skrivut" << endl;

    if (&os == &cout) {
        for (int i = 0; i < antalTrans; i++) {
            trans[i].skrivEnTrans(os);
        }
    } else {
        trans[antalTrans - 1].skrivEnTrans(os);
    }

}

//Lägger till en transaktion sist i en dynamisk transaktionsarray

void TransaktionsLista::laggTill(Transaktion &t) {
    Transaktion *temp; //En temporar array som är en kopia av trans fast med +1 element
    temp = new Transaktion[antalTrans + 1];

    for (int i = 0; i < antalTrans; i++) {
        temp[i] = trans[i];
    }

    temp[antalTrans] = t;

    delete[] trans;
    trans = temp;
    antalTrans++;

}

//Räknar ut den totala kostnaden för alla transaktioner

double TransaktionsLista::totalkostnad() {

    double totKostnad = 0;

    for (int i = 0; i < antalTrans; i++) {
        totKostnad += trans[i].haemta_belopp();
    }

    return totKostnad;

}

double TransaktionsLista::liggerUteMed(string namnet) {
    double utlagt = 0;
    double temp;

    //Går igenom alla transaktioner, jämför det angivna namnet med transaktionens loggade namn
    // Om de är samma, lägg till beloppet till summan
    for (int i = 0; i < antalTrans; i++) {
        if (namnet == trans[i].haemta_namn()) {
            // ta beloppet, dela med alla inblandade (betalare + kompisar) och multiplicera med antalet kompisar
            utlagt += (trans[i].haemta_belopp() / (trans[i].haemta_ant_kompisar() + 1)) * trans[i].haemta_ant_kompisar();
        }
    }
    return utlagt;
}

double TransaktionsLista::aerSkyldig(string namnet) {
    double skyldig = 0;

    for (int i = 0; i < antalTrans; i++) { //går igenom alla transaktioner

        if (trans[i].finnsKompis(namnet)) {

            //Om namnet finns i listan för kompisar som var med i en given transaktion,
            //dela transaktionens totalkostnad med antalet personer som var med 
            // och lägg till i skulden

            skyldig += trans[i].haemta_belopp() / (trans[i].haemta_ant_kompisar() + 1);
        }

    }

    return skyldig;

}
//Skapar en personlista från de namn som finns med på transaktionerna, förutsätter att alla betalat/blivit betalade för minst 1 gång

PersonLista TransaktionsLista::FixaPersoner() {
    string tempNamn;
    double tempBetalt;
    double tempSkyldig;
    PersonLista pLista;
    Person tempPerson;
    int antalP = 0;
    //cout << "Nu är jag i FixaPersoner" << endl;

    //Gå igenom alla transaktioner
    for (int i = 0; i < antalTrans; i++) {

        //Ta ut betalarens namn och se om betalaren finns i Personlistan än, annars: lägg till personen
        //Vi antar att alla någon gång har betalat för sig själv och för andra

        tempNamn = trans[i].haemta_namn();
        if (!pLista.finnsPerson(tempNamn)) {

            tempBetalt = liggerUteMed(tempNamn);
            tempSkyldig = aerSkyldig(tempNamn);
            tempPerson = Person(tempNamn, tempBetalt, tempSkyldig);

            pLista.laggTillEn(tempPerson);
            antalP++;
        }

    }
    return pLista;

}
// </editor-fold>

/*
================================================================================
Här följer programkörningar för alla testdata:
================================================================================
 * 
 * Testkörning 1:
 
Välj i menyn nedan
0) Avsluta. Alla transaktioner sparas på fil.
1) Läs in en transaktion från tangentbordet.
2) Skriv ut information om alla transaktioner.
3) Beräkna totala kostnaden.
4) Hur mycket är en viss person skyldig?
5) Hur mycket ligger en viss person ute med?
6) Lista alla personer mm och FIXA!!!
2

050615 transp Eva 6000 5 Bosse Kristin Paul Torsten Stina 

050721 mat Eva 300 2 Bosse Kristin 

050721 mat Paul 400 2 Torsten Stina 

050721 transp Bosse 5000 3 Eva Kristin Paul 

050721 transp Stina 5000 1 Torsten 

050722 boende Kristin 200 1 Eva 

050722 mat Eva 300 2 Kristin Bosse 

050723 mat Torsten 300 2 Paul Stina 

050724 mat Paul 200 1 Stina 

050725 transp Eva 600 3 Bosse Kristin Paul 
Välj i menyn nedan
0) Avsluta. Alla transaktioner sparas på fil.
1) Läs in en transaktion från tangentbordet.
2) Skriv ut information om alla transaktioner.
3) Beräkna totala kostnaden.
4) Hur mycket är en viss person skyldig?
5) Hur mycket ligger en viss person ute med?
6) Lista alla personer mm och FIXA!!!
6
Eva ligger ute med 5850 kr och är skyldig 1350 kr. Skall ha 4500 kr från potten.
Paul ligger ute med 366.667 kr och är skyldig 2500 kr. Skall betala 2133.33 kr till potten.
Bosse ligger ute med 3750 kr och är skyldig 1350 kr. Skall ha 2400 kr från potten.
Stina ligger ute med 2500 kr och är skyldig 1333.33 kr. Skall ha 1166.67 kr från potten.
Kristin ligger ute med 100 kr och är skyldig 2600 kr. Skall betala 2500 kr till potten.
Torsten ligger ute med 200 kr och är skyldig 3633.33 kr. Skall betala 3433.33 kr till potten.
Skyldig: 12766.7
Betalat: 12766.7
 Skulder och utlägg går jämt upp
Välj i menyn nedan
0) Avsluta. Alla transaktioner sparas på fil.
1) Läs in en transaktion från tangentbordet.
2) Skriv ut information om alla transaktioner.
3) Beräkna totala kostnaden.
4) Hur mycket är en viss person skyldig?
5) Hur mycket ligger en viss person ute med?
6) Lista alla personer mm och FIXA!!!


 * Testkörning 2:
 * 
 Välj i menyn nedan
0) Avsluta. Alla transaktioner sparas på fil.
1) Läs in en transaktion från tangentbordet.
2) Skriv ut information om alla transaktioner.
3) Beräkna totala kostnaden.
4) Hur mycket är en viss person skyldig?
5) Hur mycket ligger en viss person ute med?
6) Lista alla personer mm och FIXA!!!
6
Eva ligger ute med 600 kr och är skyldig 100 kr. Skall ha 500 kr från potten.
Bosse ligger ute med 100 kr och är skyldig 600 kr. Skall betala 500 kr till potten.
Skyldig: 700
Betalat: 700
 Skulder och utlägg går jämt upp
Välj i menyn nedan
0) Avsluta. Alla transaktioner sparas på fil.
1) Läs in en transaktion från tangentbordet.
2) Skriv ut information om alla transaktioner.
3) Beräkna totala kostnaden.
4) Hur mycket är en viss person skyldig?
5) Hur mycket ligger en viss person ute med?
6) Lista alla personer mm och FIXA!!!

 
 * 
 * 
 * 
 * 
 * Här skriver du en kort rapport om uppgiften:
 * 
 * * Uppgiften har gått ut på att göra om uppgift 6a så att alla statiska arrayer istället blir dynamiska.
 * 
 * Mitt största hinder har tveklöst varit att greppa vad begreppen ens har inneburit och vilket syfte de fyllt. Efter att ha lusläst litteraturen
 * hoppas jag nu äntligen förstått vad det hela går ut på. Jag vill tro att poletten trillat ner och, förutsatt att jag har tänkt rätt,
 * har det inte varit så svårt att kopiera över de gamla arrayerna till nya +1 större arrayer.
 * 
 * Svar på frågorna:
 * 
 * 1) Denna behövs för att, när transaktioner läggs över i den nya arrayen i funktionen laggTill() i TransaktionsLista, vill vi att dessa ska bli just KOPIOR
 * och inte ALIAS till de transaktioner som kopieras över.
 * 2) if(this != &t) kollar så att this-pekaren har en annan address än den transaktion som blivit passerad som inparameter till tilldelningsoperatorn
 * 
 

 */
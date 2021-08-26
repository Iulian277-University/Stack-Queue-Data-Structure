# Stack & Queue data structures in C

// Taiatu Iulian Marius - 312CB

Pentru implementarea temei am folosit: 
    -> Liste simplu inlantuite fara santinela
    -> Stive si cozi implementate cu ajutorul listelor mentionate anterior

ADD - Functia care adauga un serial intr-una dintre cele 3 liste (tendinte, documentare, tutoriale):
    Am citit informatiile serialului din fisierul de input
    Am setat campurile structurii serial cu informatiile primite
        Sezoanele serialului alocat anterior le-am tinut intr-o coada
            Episoadele fiecarui sezon alocat anterior le-am tinut intr-o coada
    In functie de ID-ul serialului (1, 2 sau 3), am introdus serialul (la care sunt legate 
    sezoanele si episoadele) in lista corespunzatoare, tinand cont de regula 
    de inserare in lista: descrescator dupa rating: rating egal => crescator dupa nume

ADD_SEZ - Functia care adauga sezoane la un serial deja existent
    Am iterat prin cele 4 liste (tendinte, documentare, tutoriale, top10)
    Daca am gasit serialul intr-una dintre cele 4 liste
        Returnez un pointer catre adresa serialului gasit in liste si introduc 
        noul sezon aferent serialului gasit
    Daca serialul nu se afla in liste
        Caut in coada 'watch_later'
        Daca l-am gasit in coada 'watch_later'
            Apelez functia 'AdaugaSezoaneSerialQ' - insereaza tinand cont de politica de accesare
            a elementelor dintr-o coada
        Daca nu l-am gasit in coada 'watch_later'
            Caut si in stiva 'currently_watching'
            Daca l-am gasit in stiva 'currently_watching'
                Apelez functia 'AdaugaSezoaneSerialS' - insereaza tinand cont de politica de accesare
            a elementelor dintr-o stiva
            Daca nu l-am gasit in stiva 'currently_watching'
                Ies din functie

ADD_TOP10 - Functia care adauga un serial in lista 'top10'
    Ca si la functiile anterioare, citesc informatiile de la input si aloc un serial
    La final apelez functia 'InsListaTop10' - insereaza in lista 'top10', tinand cont de regula
    de inserare specificata pentru top10 (modificand si pozitia in top a serialelor de mai jos)
    Tin cont si pastrez doar primele 10 pozitii din lista 'top10'

LATER - Functia care muta un serial din cele 4 categorii (liste) in coada 'watch_later'
    Cauta serialul intr-una dintre cele 4 categorii:
    Daca l-am gasit
        Introduce serialul in coada 'watch_later'
        Rupere legaturi pentru serial din lista

WATCH - Functia care muta un serial in stiva 'currently_watching' daca nu exista deja
    Cauta serialul in liste
    Daca l-am gasit in liste
        Introduce in stiva 'currently_watching'
        Daca s-a vizionat tot serialul din prima, il adaug in stiva 'history'
    Daca nu l-am gasit in liste
    Cauta in stiva 'currently_watching'
    Daca l-am gasit in stiva
        Daca a fost vizionat intergral, il adaug in stiva 'history'
    Daca nu l-am gasit in stiva
        Cauta si in coada 'watch_later'
        Daca l-am gasit in coada
            Daca a fost vizionat intergral, il adaug in stiva 'history'
        Daca nu l-am gasit in coada
            Ies din functie

SHOW - Functia care afiseaza la output lista / coada / stiva dorita
    In functie de input, parcurg listele, cozile sau stivele (tinand cont de politica
    de accesare) si afisez ceea ce se doreste

Punctajul obtinut la teste (checker local): 135

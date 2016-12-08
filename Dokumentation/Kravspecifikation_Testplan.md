#**Kravspecifikation**

1. Den ska visa dag binärt med led lampor
2. Den ska uppdatera lamporna klockan 00.01 varje ny dag
3. Man ska kunna ställa in dag och tid genom 3 knappar (mode, +, \-)
4. Ha en display där vi visar tiden
5. Hålla tiden/inte drifta


#Testplan

##Verktyg
Vi kommer använda oss utav Proteus för att simulera koden med MCU 328p. 
Mätning med oscilloskåp kommer ske med virituellt instrument inuti Proteus.

##Unittest

##### Tid
5.1 Testa att mikrokontrollern ökar sekunder med 1, varje tids sekund och sparar det. \- Med en räkne function 
5.2 Testa att den håller tiden genom att mäta tidsintervall. \- Mäta med oscilloskåp

##### Display
4.1 Testa att skriva ut ett tecken till display. \- Okulärt test  
4.2 Testa att skriva ut datum. \- Okulärt test
4.3 Testa att displayen klarar av att uppdatera en gång i sekunden utan flimmer. \- Okulärt test

##### Knappar
3.1 Testa och se att MCU registrerar att vi trycker på mode knapp. \- Tryck på knapp och tänd lampa 
3.2 Testa och se att MCU registrerar att vi trycker på + \- knapp. \- Tryck på knapp och tänd lampa 
3.3 Testa och se att MCU registrerar att vi trycker på + \- knapp. \- Tryck på knapp och tänd lampa 
3.4 Testa att rätt funktion körs vid knapptryck. \- funktion som kollar ett register
3.5 Testa och se att MCU enbart reagerar vid knapptryck (ingen ghosting). \- Kolla störning med oscilloskåp och se koden

##### LED
1.1 Testa att lampa 1\-5 fungerar \- Okulärt test
1.2 Testa och se till så att MCU kan kontrollera dem \- Okulärt test


##### Komponenttest

3.6 Testa och se om vi kan ställa in datum - Okulärt test, se display
3.7 Testa och se om vi kan ställa in tid - Okulärt test, se display
4.8 Testa och se om vi kan skriva ut tiden på display - Okulärt test, se display
1.3 Testa och se om vi kan ändra vilka lampor som är tända, beroendes på dag och tid - Okulärt test, se display


## Systemtest

1.4 Se så att rätt lampa är tänd när vi ändrat datum och MCU kör och att de ändras när datum ändras - Okulärt test



MitsakeCorrectru
================

Spell Checker in C++,
tightly packed tries implementation and adaptation:
    http://aclweb.org/anthology-new/W/W09/W09-1505.pdf


Description
-----------

Two Tries implementation:
  * the dummy one: C++, no optimization, used by the compiler
  * the packed one: using a tight C representation,
                    mmaped from the compiler version

moulinette:
  generator.py dic1.txt dic2.txt...

1. compile each dict with the reference and our implementation as long as the
  ref binaries are in the test folder.
2. Then build a test file by scrambling the dictionnary words
3. Generate a yaml test file and the dict test folder

  moulinette.py

Run the test suite

* -t to time the executions
* -v to activate verbose mode

Build & Use
-----------

    # generate the TextMiningCompiler and TextMiningApp
    make

    # generate the documentation
    make doc

    # generate the test suite (using the txt files in the tests folder)
    make gen

    # run the test suite
    make check

    # bench the test suite
    make time

    # basic test to check that the chain is working
    make test

TODO
----

* precomupte the offset shifting when starting the application
* compress the tree (not simply the trie representation) using the LinkLetter
* add more precise tests
* optimize more

From the subject (French)
-------------------------

 1. Decrivez les choix de design de votre programme
Le compiler construit une structure de Trie basique,
on profite du C++ pour construire simplement l'arbre,
ensuite ont génère une version compressée (C-style).

l'application applique l'algorithme de distance en
parcourant la version compressée.

Cette double implémentation nous permet de modifier la structure
de l'arbre facilement tant que la fonction de dump vers la structure
C reste à jour.

 2.	Listez l’ensemble des tests effectués sur votre programme (en plus des units tests)
A partir de n'importe quel dictionnaire, le generator.py construit une suite de test
en mélangeant les mots du dictionnaire ensuite le script moulette.py vérifie cette suite de test.
C'est pratique pour génèrer des tests minimaux (un seul mot par exemple) pour corriger un cas précis.

Le dossier scripts contient aussi quelques outils pour vérifier la présence de charactères unicode
et le taux de compression qu'on pourrait atteindre avec un Patricia (savoir si c'est rentable) sur le
dictionnaire de référence. Ce qui nous a poussé à choisir une structure de Trie Compressé en gèrant les charactères
sur 7 bits permettant d'utiliser le bit libre pour l'itération.

 3.	Avez-vous détecté des cas où la correction par distance ne fonctionnait pas (même avec une distance  élevée) ?
TODO

 4.	Quelle est la structure de données que vous avez implémentée dans votre projet, pourquoi ?
On utilise un tihgt,... titghl... un Trie compressé, en mmapant la structure directement,
on espère profiter des optimisations de la mmu et du cache.

La structure est un peu modifiée pour compresser l'arbre (cf les tests), on peut mettre plusieurs charactères sur un lien:
On fait rentrer 4 TrieLink (char:7 pour le charactère et char:1 pour savoir si le charactère est utilisé) dans un lien
(taille des int, 64 bits,...). Cette méthode nous ermet de compresser les noeuds avec un seul fils sur 4 niveaux.

 5.	Proposez un réglage automatique de la distance pour un programme qui prend juste une chaîne de caractères en entrée, donner le processus d’évaluation ainsi que les résultats
On utiliserais des fonctions de statistique plus ou moins compliquées,
TODO

 6.	Comment comptez vous améliorer les performances de votre programme
Compresser le Trie en utilisant les link letter,
faire de vrai optimisations sur le parcours de l'arbre,
Précalculer les offsets au démarrage de l'application.

 7.	Que manque-t-il à votre correcteur orthographique pour qu’il soit à l’état de l’art ?
Le dictionnaire de Google :) L'utilisation des mots alentours pour pondèrer correctement les résultats
permettrait d'améliorer facilement les résultats du correcteur.
L'implémentation du Trie elle même pourrait être largement améliorée.


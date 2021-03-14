# AISE-Projet

Notre projet est un outil de monitoring permettant de récupérer certaines métriques de performance de plusieurs machines.
Il y a 2 parties : la première mesure l'activité et la seconde est l'interface utilisateur.

Les deux parties communiquent par socket.

# Compilation

Tout un systeme de build a ete fait.
Notre projet est compilable avec make.

Pour compiler tout le projet:
	% make

# Execution

Il faut d'abord lancer la partie server, interface, qui va attendre des clients.
Il prend comme seul argument le port sur lequel la socket va s'ouvrir.
Par exemple :
	% ./bin/server 8080

La partie client quand a elle prend deux arguments.
Le premier etant l'adresse ip et le second le port de la socket a laquelle on veux se connecter.
Exemple :
	% ./bin/client localhost 8080

# Liste des fonctionnalites

### Partie 1
* A1, Systeme de build
* A2, Communication entre capteur et interface
* A3, Premier capteur (CPU)
* A4, Premiere interface
* A5, Rapport

### Partie 2
* B2, Multi client
* B5, Interactivité, redimensionnement de la fenêtre
* B6, GUI enrichi, ncurses


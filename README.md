# iOT


############ TELECHARGER LE PROJET ################



 - si vous avez git faire sur ligne de commande : 

 	$ git clone https://github.com/karineould/iOT

 - sinon télécharger le contenu en zip depuis la page : https://github.com/karineould/iOT



Ensuite ouvrir un terminal par répertoire, c'est à dire un pour le serveur et un autre pour le client.





############ LANCER LE PROJET #############





##### Partie Serveur Linux via la Raspberry #####



Depuis le répertoire serveur/ , les commandes à exécuter sont :



$ make clean

$ make 

$ ./stcp



A ce moment là, le serveur sera lancé et affichera le port sur lequel il se trouve. Il attendra qu'un client se connecte en TCP avec lui.







##### Partie Client avec Node Js #####



Au préalable, il faut installer node sur votre ordinateur pour pouvoir lancer le client



Depuis le répertoire client/, les commandes à exécuter sont :

$ npm install     /* installation sur votre machine de toutes les libraires nécessaires pour notre projet */

$ npm start /* lancement du client */



Se rendre ensuite sur un navigateur et taper dans la barre d'URL : localhost:3000







A VOUS DE JOUER !

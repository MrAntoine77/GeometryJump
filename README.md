# Geometry Jump

Un mini-jeu inspiré du jeu Geometry Dash de RobTop, développé en **C++ avec SDL2**, où une **IA génétique** apprend à jouer toute seule. Ce projet explore l’apprentissage évolutif dans un environnement simple.

## Fonctionnalités

- Gameplay de type "runner" : sauts, obstacles, vitesse constante
- IA génétique qui évolue génération après génération
- Visualisation en temps réel de l’apprentissage
- Mode manuel pour jouer soi-même
- Editeur de niveau simple

## IA Génétique

L’intelligence artificielle joue le niveau des centaines de fois à travers une population d’agents. Chaque agent est équipé de **plusieurs ensembles de capteurs virtuels** répartis autour de lui. Ces capteurs peuvent détecter différents types d’informations, la **présence d'un obstacle**, un block, un pique, une orbe..., ou l'**absence d'un obstacle**.

Chaque ensemble de capteurs fonctionne comme une unité de décision. 👉 **Le joueur saute si l’un de ses ensembles de capteurs a tous ses capteurs activés**. Cela permet une logique flexible et adaptative : un agent peut apprendre à sauter dans des configurations très différentes selon l’ensemble activé.

À chaque génération :
- Les agents sont évalués selon leur performance (distance parcourue, nombre de sauts…)
- Les meilleurs ensembles de capteurs sont conservés
- Une nouvelle génération est créée par **croisement** et **mutation**, permettant à l’IA d’explorer de nouvelles stratégies

Ce système favorise l’émergence de comportements intelligents sans avoir à coder explicitement les règles du jeu.

## Technologies utilisées

- **C++**
- **SDL2** pour le rendu graphique et les entrées clavier
- Algorithme génétique maison

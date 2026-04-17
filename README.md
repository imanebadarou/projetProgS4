# ♟️ Jeu d'échecs

Un jeu d'échecs 2D et 3D développé Pauline COSSON et Imane BADAROU avec **C++**, **OpenGL** et **ImGui**.

## Fonctionnalités Principales
- **Interface Graphique** :
    - Menu de démarrage avec choix du mode de jeu et définition de l'elo des joueurs.
    - Vue 2D et 3D.
    - Menu informations et options du jeu.


- **Rendu 3D Avancé** :
    - Chargement de modèles 3D (format OBJ).
    - Rendu instancié pour optimiser les performances.
    - Intégration de la vue 3D dans une interface ImGui via Framebuffer (FBO).
- **2 modes de caméra** :
    - **Mode Orbite (Trackball)** : Rotation libre autour de l'échiquier.
    - **Mode Première Personne** : Vue POV depuis la perspective d'une pièce.
- **Animations Fluides** :
    - Déplacements de pièces animés par des courbes de Bézier/Sinus (effet de "saut").
    - Rotation fluide des pièces lors des déplacements.

## Spécificités du Projet


- **Mode Aléatoire**:
    - **Permutation des rangs arrière** : Les pièces majeures sont disposées aléatoirement au début.
    - **Promotions aléatoires** : Des pions peuvent être promus en Reines selon une distribution géométrique de probabilité.
    - **Météorites** : Une météorite peut détruire aléatoirement une case et la pièce qui l'occupe à tout moment.
    - **Vitesse d'animation variable** : La vitesse des animations des pièces change dynamiquement à chaque coup.
- **Environnement dynamique** :
    - L'ambiance visuelle (couleur du ciel, éclairage) change selon le tour (Blanc vs Noir).
    - Utilisation d'une **Skybox**.
    - Lumière ponctuelle dynamique selon le tour.
- **Matériaux procéduraux** :
    - Shader de **marbre procédural** calculé via *Fractal Brownian Motion* (FBM) et du noise.
- **Feedback visuel intelligent** :
    - Mise en évidence des cases survolées, sélectionnées et des mouvements possibles.
